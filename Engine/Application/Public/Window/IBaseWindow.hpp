#ifndef AB_IBASEWINDOW_H
#define AB_IBASEWINDOW_H

#include "Core.h"
#include "Debug/Assert.h"
#include "Window/WindowDesc.hpp"
#include "Input/UserInput.hpp"
#include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#include "AppStatus.hpp"

namespace App
{

/**
 * @brief Basic wrapper for window class.
 *
 * It's possible to create your own implementation of WindowPolicy
 * by overriding virtual methods of the base class or creating your new
 * policy from IWindowPolicy class, to handle different and custom os level logic.
 * */
template<typename Derived, typename WindowPolicy = DefaultSystemWindowPolicy>
class IBaseWindow
{
public:

    IBaseWindow() = default;

    template<class U>
    explicit IBaseWindow(U&& windowDesc = WindowDesc())
        : m_pWindowDesc(::std::make_shared<WindowDesc>(::std::forward<U>(windowDesc)))
    { }

    ~IBaseWindow()
    { 
        this->Destroy(); 
    }

    IBaseWindow(const IBaseWindow& other) = delete;
    
    IBaseWindow(IBaseWindow&& other) noexcept
        : m_pWindowDesc(::std::move(other.m_pWindowDesc))
        , m_pInput(::std::move(other.m_pInput))
    { }

public:

    template<class NewPolicy>
    void ChangePolicy()
    {
		bool bWasAlive = m_pWindowDesc->IsAlive;

        m_Policy.WindowPolicyDestroy(m_pWindowDesc.get());

		m_Policy = NewPolicy();
        
        if (bWasAlive) 
            this->Create();
    }

public:

    void Create()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);

        if (m_pWindowDesc->IsAlive) {
            return;
        }
        
        m_pWindowDesc->uUinqueIndex = App::AppStatus::Get().SendOpenedWindowSignal();

        if (m_Policy.WindowPolicyCreate(m_pWindowDesc.get()) != 0) {
            throw AB_EXCEPT("Couldn't create the window");
        }

        m_pWindowDesc->IsAlive = true;
    }

    void Show()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        m_Policy.WindowPolicyShow(m_pWindowDesc.get()); 
    }

    void Hide()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        m_Policy.WindowPolicyHide(m_pWindowDesc.get()); 
    }

    void Destroy()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);

        if (!m_pWindowDesc->IsAlive) {
            return;
        }
        
        App::AppStatus::Get().SendClosedWindowSignal();
        
        m_Policy.WindowPolicyDestroy(m_pWindowDesc.get());
        
        m_pWindowDesc->IsAlive = false;
    }

    void Update()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        if (!m_pWindowDesc->IsAlive) {
            return;
        }

        m_pWindowDesc->LastEvent &= 0;
        m_Policy.WindowPolicyUpdate(m_pWindowDesc.get());
        
        if (m_pWindowDesc->LastEvent & EAbWindowEvents::Destroy) {
            AB_LOG(Core::Debug::Info, L"Window is being closed by user");
            this->Destroy();
        }
        
        static_cast<Derived*>(this)->HandleMessageImpl(m_pWindowDesc->LastEvent);
    }

public:

    const ::std::shared_ptr<WindowDesc>& GetWindowDesc() const
    { return m_pWindowDesc; }

private:

    void HandleMessage(EAbWindowEventsFlags events)
    {
        static_cast<Derived*>(this)->HandleMessageImpl(events);
    }

private:
    
    WindowPolicy m_Policy;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

};

} // !App
#endif // !AB_IBASEWINDOW_H
