#ifndef AB_IBASEWINDOW_H
#define AB_IBASEWINDOW_H

#include "Core.h"
#include "Debug/Assert.h"
#include "Window/WindowDesc.h"
#include "Input/UserInput.hpp"
#include "Window/WindowPolicy/BasicSystemPolicy.hpp"
#include "AppStatus.hpp"

namespace App
{

/**
 * Basic wrapper for window class.
 * It's possible to create your own implementation of WindowPolicy
 * by hidding the base implementations of BasicSystemPolicy or creating your new
 * policy from IWindowPolicy class, to create whole custom os level logic.
 * */
template<typename Derived, typename WindowPolicy = DefaultSystemWindowPolicy>
class IBaseWindow : public WindowPolicy
{
public:

    IBaseWindow() = default;

    template<class U>
    explicit IBaseWindow(U&& windowDesc = WindowDesc())
        : m_pWindowDesc(::std::make_shared<WindowDesc>(::std::forward<U>(windowDesc)))
        , m_Input(::std::make_shared<UserInput>(m_pWindowDesc))
    { }

    ~IBaseWindow()
    { 
        this->Destroy(); 
    }

    IBaseWindow(const IBaseWindow& other) = delete;
    
    IBaseWindow(IBaseWindow&& other) noexcept
        : m_pWindowDesc(::std::move(other.m_pWindowDesc))
        , m_Input(::std::move(other.m_Input))
    { }

public:

    void Create()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);

        if (m_pWindowDesc->IsAlive) {
            return;
        }
        
        m_pWindowDesc->uUinqueIndex = App::AppStatus::Get().SendOpenedWindowSignal();

        if (this->WindowPolicyCreate(m_pWindowDesc.get()) != 0) {
            throw AB_EXCEPT("Couldn't create the window");
        }

        m_pWindowDesc->IsAlive = true;
    }

    void Show()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        this->WindowPolicyShow(m_pWindowDesc.get()); 
    }

    void Hide()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        this->WindowPolicyHide(m_pWindowDesc.get()); 
    }

    void Destroy()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);

        if (!m_pWindowDesc->IsAlive) {
            return;
        }
        
        App::AppStatus::Get().SendClosedWindowSignal();
        
        this->WindowPolicyDestroy(m_pWindowDesc.get());
        
        m_pWindowDesc->IsAlive = false;
    }

    void Update()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        if (!m_pWindowDesc->IsAlive) {
            return;
        }

        this->WindowPolicyUpdate(m_pWindowDesc.get());
        
        if (m_pWindowDesc->LastEvent & EAbWindowEvents::Destroy) {
            AB_LOG(Core::Debug::Info, L"Window is being closed by user");
            this->Destroy();
        }
        
        static_cast<Derived*>(this)->HandleMessageImpl(m_pWindowDesc->LastEvent);

        // My decision is, that every client of this library, 
        // should be able to handle Input events by themselves
        // in HandleMessageImpl() or use the builtin UserInput class.
        if (m_pWindowDesc->LastEvent & EAbWindowEvents::Input) {
            m_pWindowDesc->InputStruct.Handled = false;
        }
        
        m_pWindowDesc->LastEvent = EAbWindowEvents::NothingNew;
    }

public:

    const ::std::shared_ptr<WindowDesc>& GetWindowDesc() const
    { return m_pWindowDesc; }

    ::std::shared_ptr<UserInput> GetInput()
    { return m_Input; }

private:

    void HandleMessage(EAbWindowEvents msg)
    {
        static_cast<Derived*>(this)->HandleMessageImpl(msg);
    }

private:

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;
    
    ::std::shared_ptr<UserInput> m_Input;

};

} // !App
#endif // !AB_IBASEWINDOW_H
