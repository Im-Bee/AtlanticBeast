#ifndef AB_IBASEWINDOW_H
#define AB_IBASEWINDOW_H

#include "Core.h"
#include "Window/WindowDesc.hpp"
#include "Window/WindowEvents.h"
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

    IBaseWindow() 
        : m_Policy(::std::make_unique<WindowPolicy>())
        , m_pWindowDesc(::std::make_shared<WindowDesc>(CreateWindowDesc(L"IBaseWindow", 1280, 720)))
    { }

    template<class U>
    explicit IBaseWindow(U&& windowDesc = WindowDesc())
		: m_Policy(::std::make_unique<WindowPolicy>())
        , m_pWindowDesc(::std::make_shared<WindowDesc>(::std::forward<U>(windowDesc)))
    { }

    ~IBaseWindow()
    { 
        this->Destroy(); 
    }

    IBaseWindow(const IBaseWindow& other) = delete;
    
    IBaseWindow(IBaseWindow&& other) noexcept
		: m_Policy(::std::move(other.m_Policy))
        , m_pWindowDesc(::std::move(other.m_pWindowDesc))
    { }

public:

    template<class NewPolicy>
    void ChangePolicy()
    {
        AB_ASSERT(m_pWindowDesc != nullptr);

        bool bWasAlive = this->m_pWindowDesc->IsAlive;
        ::std::unique_ptr<DefaultSystemWindowPolicy> pNewPolicy = ::std::make_unique<NewPolicy>();
        m_Policy.swap(pNewPolicy);
        if (!bWasAlive) {
            return;
        }

        WindowDesc oldDesc = *(this->m_pWindowDesc.get());
        WindowDesc newDesc = CreateWindowDesc(oldDesc.Name, 
                                              oldDesc.Width, 
                                              oldDesc.Height);
        *(this->m_pWindowDesc.get()) = newDesc;

        try {
            AB_LOG(Core::Debug::Warning, L"Creating new window!");
            this->Create();
        }
        catch (...) {
            *(this->m_pWindowDesc.get()) = oldDesc;
            m_Policy.swap(pNewPolicy);
            AB_LOG(Core::Debug::Error, L"We couldn't change this window policy!");
            return;
        }

        newDesc = *(this->m_pWindowDesc.get());
        *(this->m_pWindowDesc.get()) = oldDesc;
        m_Policy.swap(pNewPolicy);
        try {
            AB_LOG(Core::Debug::Warning, L"Destroying the old window!");
            this->Destroy();
        }
        catch (...) { 
            AB_LOG(Core::Debug::Error, L"Old verison of window wasn't properly closed!");
        }

        *(this->m_pWindowDesc.get()) = newDesc;
        m_Policy.swap(pNewPolicy);
        m_pWindowDesc->LastEvent = EAbWindowEvents::ChangedBehavior;
    }

public:

    void Create()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
		AB_ASSERT(m_Policy != nullptr);

        if (m_pWindowDesc->IsAlive) {
            AB_LOG(Core::Debug::Warning, L"Cannot create alive window");
            return;
        }
        
        App::AppStatus::Get().SendOpenWindowSignal(m_pWindowDesc);

        if (m_Policy->WindowPolicyCreate(m_pWindowDesc.get()) != 0) {
            throw AB_EXCEPT("Couldn't create the window");
        }

        m_pWindowDesc->IsAlive = true;
    }

    void Show()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        AB_ASSERT(m_Policy != nullptr);

        m_Policy->WindowPolicyShow(m_pWindowDesc.get()); 
    }

    void Hide()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        AB_ASSERT(m_Policy != nullptr);

        m_Policy->WindowPolicyHide(m_pWindowDesc.get()); 
    }

    void Destroy()
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        AB_ASSERT(m_Policy != nullptr);

        if (!m_pWindowDesc->IsAlive) {
            AB_LOG(Core::Debug::Warning, L"Cannot destroy dead window");
            return;
        }
        
        App::AppStatus::Get().SendCloseWindowSignal(m_pWindowDesc);
        
        m_Policy->WindowPolicyDestroy(m_pWindowDesc.get());
        
        m_pWindowDesc->IsAlive = false;
    }

    void Update(const float fDelta)
    { 
        AB_ASSERT(m_pWindowDesc != nullptr);
        AB_ASSERT(m_Policy != nullptr);

        if (!m_pWindowDesc->IsAlive) {
            return;
        }

        // Don't reset the last event flags if the only flag that we have set is EAbWindowEvents::ChangedBehavior
        if (this->m_pWindowDesc->LastEvent & ~EAbWindowEvents::ChangedBehavior)
            m_pWindowDesc->LastEvent &= 0;

        // Make sure that after EAbWindowEvents::ChangedBehavior propagation, 
        // we are going to reset the events by setting EAbWindowEvents::NothingNew flag
        this->m_pWindowDesc->LastEvent |= EAbWindowEvents::NothingNew;
        m_Policy->WindowPolicyUpdate(m_pWindowDesc.get());
        
        if (m_pWindowDesc->LastEvent & EAbWindowEvents::Destroy) {
            AB_LOG(Core::Debug::Info, L"Window is being closed by user");
            this->Destroy();
        }
        
        static_cast<Derived*>(this)->HandleMessageImpl(fDelta, m_pWindowDesc->LastEvent);
    }

public:

    const ::std::shared_ptr<WindowDesc>& GetWindowDesc() const
    { return m_pWindowDesc; }

private:

    void HandleMessage(const float fDelta, EAbWindowEventsFlags events)
    { static_cast<Derived*>(this)->HandleMessageImpl(fDelta, events); }

private:
    
    ::std::unique_ptr<DefaultSystemWindowPolicy> m_Policy;

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

};

} // !App
#endif // !AB_IBASEWINDOW_H
