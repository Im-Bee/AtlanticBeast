#include "BaseWindowDetails.h"
#include "AbLimits.h"

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
uint32_t CreateImpl(WindowDesc* pWd)
{
    if (pWd->DisplayHandle == NULL) {
        return -1;
    }

    int screen = DefaultScreen(pWd->DisplayHandle);
    Display* display = pWd->DisplayHandle;
    Window window = XCreateSimpleWindow(pWd->DisplayHandle,
                                        RootWindow(display, screen),
                                        100, 100, 
                                        pWd->Width, pWd->Height, 
                                        1,
                                        BlackPixel(display, screen),
                                        WhitePixel(display, screen));

    XTextProperty windowName;

    char* szWindowName = (char*)malloc(sizeof(char) * AB_SMALLS_STRING);

    size_t uWriten = wcstombs(szWindowName, pWd->Name, pWd->uNameLen);
    szWindowName[uWriten] = '\0';

    XStringListToTextProperty(&szWindowName, 1, &windowName);
    XSetWMName(display, window, &windowName);
    free(szWindowName);
    XFree(windowName.value);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    pWd->Screen = DefaultScreen(display);
    pWd->WindowHandle = window;

    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void ShowImpl(WindowDesc* wd)
{
    XMapWindow(wd->DisplayHandle, wd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void HideImpl(WindowDesc* pWd)
{
    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void DestroyImpl(WindowDesc* pWd)
{ 
    if (!pWd->IsAlive || !pWd->DisplayHandle || !pWd->WindowHandle) {
        return;
    }

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}
 
// ---------------------------------------------------------------------------------------------------------------------
void UpdateImpl(WindowDesc* pWd)
{
    XEvent event;
    Display*  display     = pWd->DisplayHandle;
    Window    window      = pWd->WindowHandle;
    int       screen      = pWd->Screen;

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
            pWd->uLastMessage = -1;

            return;
        }
    }
}

#elif _WIN32

// Statics // ----------------------------------------------------------------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowDesc* pWd = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pWd = (WindowDesc*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWd);

        pWd->Hwnd = hwnd;
    }
    else
    {
        pWd = (WindowDesc*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis)
    {
        switch (uMsg)
        {
        case WM_CLOSE:
        {
            pWd->uLastMessage = -1;
            DestroyImpl(*pWd);
            break;
        }
        default:
            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------
static const wchar_t wszClassName[] = L"WindowClass";

// ---------------------------------------------------------------------------------------------------------------------
void CreateImpl(WindowDesc& wd)
{
    AB_LOG(Debug::ESeverity::Info, L"Creating canvas");

    if (wd.Hwnd) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to create an already created window");
        return;
    }

    if (!AskForWindowClass(wszClassName)) {
        WNDCLASSEX wcex = { 0 };

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.lpszClassName = wszClassName;

        AskToRegisterWindowClass(wcex);
    }

    auto hwnd = CreateWindow(wszClassName,
                             wd.Name.c_str(),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             wd.Width,
                             wd.Height,
                             nullptr,
                             nullptr,
                             GetModuleHandle(NULL),
                             &wd);

    if (!hwnd) {
        throw AB_EXCEPT("Couldn't create a window");
    }

    ShowWindow(hwnd, SW_SHOW);

    wd.Hwnd       = hwnd;
    wd.IsAlive    = true;
}

// ---------------------------------------------------------------------------------------------------------------------
void ShowImpl(WindowDesc& wd)
{
    AB_LOG(Debug::ESeverity::Info, L"Show canvas");

    ShowWindow(wd.Hwnd, SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------
void HideImpl(WindowDesc& wd)
{
    AB_LOG(Debug::ESeverity::Info, L"Hide canvas");

    ShowWindow(wd.Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void DestroyImpl(WindowDesc& wd)
{
    if (!wd.IsAlive) {
        return;
    }

    AB_LOG(Debug::ESeverity::Info, L"Destroy canvas");

    if (wd.Hwnd == NULL) {
        AB_LOG(Debug::ESeverity::Warning, L"Trying to destroy an already destroyed window");
        return;
    }

    if (DestroyWindow(wd.Hwnd)) {
        wd.Hwnd = NULL;
    }
    else {
        AB_LOG(Debug::ESeverity::Error, L"Couldn't destroy a window");
    }

    AskToCloseWindowClass(wszClassName);
    wd.IsAlive = false;
}

// ---------------------------------------------------------------------------------------------------------------------
void UpdateImpl(WindowDesc& wd)
{
    if (wd.Hwnd == NULL) {
        return;  
    }

    MSG msg = { };
    if (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#endif // !__linux__
