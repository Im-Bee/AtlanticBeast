#include "Core.h"
#include "EmptyCanvas.hpp"

namespace Core
{

#ifdef __linux__

// EmptyCanvas // ------------------------------------------------------------------------------------------------------
void IBaseWindow::CreateImpl()
{
    if (m_WindowDesc.Display) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to create an already created window for EmptyCanvas");
        return;
    }

    AB_LOG(Debug::ESeverity::Info, L"Creating canvas");

    m_WindowDesc.Display = AskForDisplayLinux(NULL);
    if (m_WindowDesc.Display == NULL) {
        throw AB_EXCEPT("Couldn't open a connection to X server");
    }

    int screen = DefaultScreen(m_WindowDesc.Display);
    auto* display = m_WindowDesc.Display;
    Window window = XCreateSimpleWindow(m_WindowDesc.Display,
                                        RootWindow(display, screen),
                                        100, 100, 
                                        m_WindowDesc.Width, m_WindowDesc.Height, 
                                        1,
                                        BlackPixel(display, screen),
                                        WhitePixel(display, screen));

    XTextProperty windowName;

    char* szWindowName = new char[MaxSmallString];

    if (m_WindowDesc.Name.length() >= MaxSmallString) {
        m_WindowDesc.Name = m_WindowDesc.Name.substr(0, 63);
    }

    size_t uWriten = wcstombs(szWindowName, m_WindowDesc.Name.c_str(), m_WindowDesc.Name.length());
    szWindowName[uWriten] = '\0';

    XStringListToTextProperty(&szWindowName, 1, &windowName);
    XSetWMName(display, window, &windowName);
    delete[] szWindowName;
    XFree(windowName.value);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    m_WindowDesc.Screen = DefaultScreen(display);
    m_WindowDesc.Window = window;

    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);
}

// ---------------------------------------------------------------------------------------------------------------------
void IBaseWindow::ShowImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Show canvas");

    XMapWindow(m_WindowDesc.Display, m_WindowDesc.Window);
}

// ---------------------------------------------------------------------------------------------------------------------
void IBaseWindow::HideImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Hide canvas");

    XUnmapWindow(m_WindowDesc.Display, m_WindowDesc.Window);
}

// ---------------------------------------------------------------------------------------------------------------------
void IBaseWindow::DestroyImpl()
{ 
    if (!m_WindowDesc.Display) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to destroy an already destroyed window for EmptyCanvas");
        return;
    }

    AB_LOG(Debug::ESeverity::Info, L"Destroy canvas");

    XUnmapWindow(m_WindowDesc.Display, m_WindowDesc.Window);

    AskToCloseDisplayLinux(NULL);
    m_WindowDesc.Display = NULL;
}
 
// ---------------------------------------------------------------------------------------------------------------------
void IBaseWindow::UpdateImpl()
{
    XEvent event;
    ::Display* display = m_WindowDesc.Display;
    ::Window window = m_WindowDesc.Window;
    int screen = m_WindowDesc.Screen;

    if (!display) {
        return;
    }

    XNextEvent(display, &event);
    if (event.type == Expose)
    {
        XDrawString(display, 
                    window,
                    DefaultGC(display, screen),
                    10,
                    20,
                    "Hello, explicit display!", 
                    24);
    }
    else if (event.type == ClientMessage)
    {
        Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
        if ((Atom)event.xclient.data.l[0] == wmDeleteMessage)
        {
            this->Destroy();
            return;
        }
    }
}

#elif _WIN32


// Statics // ----------------------------------------------------------------------------------------------------------
template<class T>
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    T* pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (T*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_WindowDesc.Hwnd = hwnd;
    }
    else
    {
        pThis = (T*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (uMsg)
        {
        case WM_CLOSE:
        {
            pThis->Destroy();
            break;
        }
        default:
            break;
        }

        pThis->HandleMessage(uMsg);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------
static const wchar_t wszClassName[] = L"EmptyCanvasClass";

// EmptyCanvas // ------------------------------------------------------------------------------------------------------
void EmptyCanvas::CreateImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Creating canvas");

    if (m_WindowDesc.Hwnd) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to create an already created window for EmptyCanvas");
        return;
    }

    if (!AskForWindowClass(wszClassName)) {
        WNDCLASSEX wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc<EmptyCanvas>;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = wszClassName;

        AskToRegisterWindowClass(wcex);
    }

    auto hwnd = CreateWindow(wszClassName,
                             m_WindowDesc.Name,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             m_WindowDesc.Width,
                             m_WindowDesc.Height,
                             nullptr,
                             nullptr,
                             GetModuleHandle(NULL),
                             this);

    if (!hwnd) {
        throw AB_EXCEPT("Couldn't create a window for EmptyCanvas");
    }

    ShowWindow(hwnd, SW_SHOW);

    m_WindowDesc.Hwnd = hwnd;
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::ShowImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Show canvas");

    ShowWindow(m_WindowDesc.Hwnd, SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::HideImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Hide canvas");

    ShowWindow(m_WindowDesc.Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::DestroyImpl()
{
    AB_LOG(Debug::ESeverity::Info, L"Destroy canvas");

    if (m_WindowDesc.Hwnd == NULL) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to destroy an already destroyed window for EmptyCanvas");
        return;
    }

    if (DestroyWindow(m_WindowDesc.Hwnd)) {
        m_WindowDesc.Hwnd = NULL;
    }
    else {
        AB_LOG(Debug::ESeverity::Error, L"Couldn't destroy a window for EmptyCanvas");
    }

    AskToCloseWindowClass(wszClassName);
}

// ---------------------------------------------------------------------------------------------------------------------
void EmptyCanvas::UpdateImpl()
{
    if (m_WindowDesc.Hwnd == NULL) {
        return;
    }

    MSG msg = { };
    if (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#endif // !__linux__
       
} // !Core
