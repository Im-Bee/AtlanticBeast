#ifndef AB_IBASEWINDOW_H
#define AB_IBASEWINDOW_H

#include "Core.h"
#include "WindowDesc.h"
#include "BaseWindowDetails.h"


namespace Core
{

template<class Derived>
class IBaseWindow
{
#ifdef _WIN32

    template<class T>
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

#endif // _WIN32

public:

    template<class U>
    explicit IBaseWindow(U&& windowDesc = WindowDesc())
        : m_pWindowDesc(::std::make_shared<WindowDesc>(::std::forward<U>(windowDesc)))
    { }

    ~IBaseWindow() 
    {
        this->Destroy();
    }

    IBaseWindow(const IBaseWindow& other) = default;

    IBaseWindow(IBaseWindow&& other) noexcept
        : m_pWindowDesc(::std::move(other.m_pWindowDesc))
    { 
        other.m_pWindowDesc = nullptr;
    }

public:

    inline void Create()
    { 
        Core::AppStatus::Get().SendOpenedWindowSignal();

#ifdef __linux__
        m_pWindowDesc->DisplayHandle = AbDetailsAskForDisplayLinux(NULL);
#elif _WIN32
		m_pWindowDesc->pwszClassName = L"BaseClass";


        if (!ImplAskForWindowClass(m_pWindowDesc->pwszClassName)) {
            m_pWindowDesc->Wcex = { 0 };

			m_pWindowDesc->Wcex.cbSize = sizeof(WNDCLASSEX);
            m_pWindowDesc->Wcex.style = CS_HREDRAW | CS_VREDRAW;
            m_pWindowDesc->Wcex.hInstance = GetModuleHandle(NULL);
            m_pWindowDesc->Wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
            m_pWindowDesc->Wcex.lpszClassName = m_pWindowDesc->pwszClassName;

            ImplAskToRegisterWindowClass(m_pWindowDesc->Wcex);
		}
#endif // !__linux__

        if (AbDetailsCreateImpl(m_pWindowDesc.get()) != 0) {
            throw AB_EXCEPT("Couldn't create the window");
        }

        m_pWindowDesc->IsAlive = true;
    }

    inline void Show()
    { return AbDetailsShowImpl(m_pWindowDesc.get()); }

    inline void Hide()
    { return AbDetailsHideImpl(m_pWindowDesc.get()); }

    inline void Destroy()
    { 
        if (!m_pWindowDesc || !m_pWindowDesc->IsAlive) {
            return;
		}

        Core::AppStatus::Get().SendClosedWindowSignal();

        AbDetailsDestroyImpl(m_pWindowDesc.get()); 

#ifdef __linux__
        AbDetailsAskToCloseDisplayLinux(NULL);
        m_pWindowDesc->WindowHandle = 0;
        m_pWindowDesc->DisplayHandle = NULL;
#elif _WIN32
        ImplAskToCloseWindowClass(m_pWindowDesc->pwszClassName);
		m_pWindowDesc->Hwnd = NULL;
#endif // !__linux__

        m_pWindowDesc->IsAlive = false;
    }

    inline void Update()
    { 
        AbDetailsUpdateImpl(m_pWindowDesc.get());
		// AB_LOG(Core::Debug::Info, L"Window last message: %d", m_pWindowDesc->uLastMessage);

        if (m_pWindowDesc->uLastMessage == -1) {
			AB_LOG(Core::Debug::Info, L"Window is being closed by user");
            this->Destroy();
        }

        static_cast<Derived*>(this)->HandleMessageImpl(m_pWindowDesc->uLastMessage);
    }

    inline const ::std::shared_ptr<WindowDesc>& GetWindowDesc() const
    { return m_pWindowDesc; }

private:

    void HandleMessage(uint32_t msg)
    {
        static_cast<Derived*>(this)->HandleMessageImpl(msg);
    }

private:

    ::std::shared_ptr<WindowDesc> m_pWindowDesc;

};

} // !Core
#endif // !AB_IBASEWINDOW_H
