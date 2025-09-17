#include "Core.h"
#include "Window/BaseWindowDetails.h"

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbDetailsCreateImpl(WindowDesc* pWd)
{
    pWd->DisplayHandle = AbAskForDisplayLinux(NULL);

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
    char* szWindowName = (char*)malloc(sizeof(char) * AB_SMALL_STRNG);
    size_t uWriten = wcstombs(szWindowName, pWd->Name, pWd->uNameLen);
    szWindowName[uWriten] = '\0';

    XStringListToTextProperty(&szWindowName, 1, &windowName);
    XSetWMName(display, window, &windowName);
    free(szWindowName);
    XFree(windowName.value);

    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);

    pWd->Screen = DefaultScreen(display);
    pWd->WindowHandle = window;

    int firstEvent, firstError;
    int major = 2, minor = 0;
    if (XQueryExtension(display, "XInputExtension", &pWd->XiOpcode, &firstEvent, &firstError) &&
        XIQueryVersion(display, &major, &minor) == Success) 
    {
        pWd->bInputSupport = 1;
    }
    else 
    {
        pWd->bInputSupport = 0;
    }

    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsShowImpl(WindowDesc* wd)
{
    XMapWindow(wd->DisplayHandle, wd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsHideImpl(WindowDesc* pWd)
{
    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsDestroyImpl(WindowDesc* pWd)
{ 
    if (!pWd->IsAlive || !pWd->DisplayHandle || !pWd->WindowHandle)  
        return;

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);

    AbAskToCloseDisplayLinux(NULL);
    m_pWindowDesc->WindowHandle = 0;
    m_pWindowDesc->DisplayHandle = NULL;
}
 
// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsUpdateImpl(WindowDesc* pWd)
{
    XEvent      event;
    Display*    display     = pWd->DisplayHandle;
    Window      window      = pWd->WindowHandle;
    int         screen      = pWd->Screen;
                

    Atom wmDeleteMessage;


    while (display && XPending(display)) 
    {
        XPeekEvent(display, &event);
        if (event.xcookie.type != GenericEvent && event.xany.window != pWd->WindowHandle) {
            return;
        }

        XNextEvent(display, &event);

        if (event.xcookie.type == GenericEvent && 
            event.xcookie.extension == pWd->XiOpcode) 
        {
            if (!XGetEventData(display, &event.xcookie)) 
                continue;

            XIDeviceEvent *rawev = (XIDeviceEvent*)event.xcookie.data;

            if (rawev->event != window)
                continue;

            pWd->LastEvent = Input;
            
            switch (rawev->evtype) {
                case XI_KeyPress:
                    pWd->InputStruct.Event = AbKeyPress;
                    pWd->InputStruct.KeyId = rawev->detail;
                    break;

                case XI_KeyRelease:
                    pWd->InputStruct.Event = AbKeyRelease;
                    pWd->InputStruct.KeyId = rawev->detail;
                    break;

                case XI_ButtonPress:
                    pWd->InputStruct.Event = AbButtonPress;
                    pWd->InputStruct.KeyId = rawev->detail;
                    break;

                case XI_ButtonRelease:
                    pWd->InputStruct.Event = AbButtonRelease;
                    pWd->InputStruct.KeyId = rawev->detail;
                    break;

                case XI_Motion:
                    pWd->InputStruct.Event = AbMotion;
                    pWd->InputStruct.MouseX = rawev->event_x;
                    pWd->InputStruct.MouseY = rawev->event_y;
                    break;
            }

            XFreeEventData(display, &event.xcookie);
            return;
        }

        switch (event.type) {
            case Expose:
                pWd->Width = event.xexpose.width;
                pWd->Height = event.xexpose.height;
                pWd->LastEvent = Resize;
                break;

            case ConfigureNotify:
                pWd->Height = event.xconfigure.height;
                pWd->Width = event.xconfigure.width;
                pWd->LastEvent = Resize;
                break;

            case ClientMessage:
                if (event.xclient.window != pWd->WindowHandle) {
                    return;
                }

                if ((wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", 1)) == None)
                    break;

                if ((Atom)event.xclient.data.l[0] == wmDeleteMessage) {
                    pWd->LastEvent = Destroy;
                    return;
                }

                break;
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

    if (pWd)
    {
        switch (uMsg) {
        case WM_SIZE:
            pWd->Width = LOWORD(lParam);
            pWd->Height = HIWORD(lParam);
            pWd->LastEvent = EAbWindowEvents::Resize;
            break;

        case WM_CLOSE:
            pWd->LastEvent = EAbWindowEvents::Destroy;
            break;

        default:
            break;
        }

        // AB_LOG(Core::Debug::Info, L"pwd->uLastMessage = %u", pWd->uLastMessage);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbDetailsCreateImpl(WindowDesc* wd)
{
    if (wd->Hwnd) {
        return -2;
    }
    
	// Fallback to default class name if none is provided,
	// if pwszClassName is provided, we assume that WCEX is already filled
    if (wd->pwszClassName == NULL || wcscmp(wd->pwszClassName, L"") == 0) {
        wd->pwszClassName = L"AtlanticClass";

        memset(&wd->Wcex, 0, sizeof(WNDCLASSEX));

        wd->Wcex.cbSize         = sizeof(WNDCLASSEX);
        wd->Wcex.style          = CS_HREDRAW | CS_VREDRAW;
        wd->Wcex.hInstance      = GetModuleHandle(NULL);
        wd->Wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wd->Wcex.lpszClassName  = wd->pwszClassName;
        wd->Wcex.lpfnWndProc    = WindowProc;
    }

    if (!AbAskForWindowClass(wd->pwszClassName)) {
        AbAskToRegisterWindowClass(wd->Wcex);
    }

    HWND hwnd = CreateWindow(wd->pwszClassName,
                             wd->Name,
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             wd->Width,
                             wd->Height,
                             NULL,
                             NULL,
                             GetModuleHandle(NULL),
                             wd);

    if (!hwnd) {
        return -1;
    }

    ShowWindow(hwnd, SW_SHOW);

    wd->Hwnd = hwnd;

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsShowImpl(WindowDesc* wd)
{
    ShowWindow(wd->Hwnd, SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsHideImpl(WindowDesc* wd)
{
    ShowWindow(wd->Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsDestroyImpl(WindowDesc* wd)
{
    if (!wd->IsAlive || wd->Hwnd == NULL) {
        return;
    }

    if (DestroyWindow(wd->Hwnd)) {
        wd->Hwnd = NULL;
    }

    AbAskToCloseWindowClass(wd->pwszClassName);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDetailsUpdateImpl(WindowDesc* wd)
{
    if (wd->Hwnd == NULL) {
        return;  
    }

    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, 1) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#endif // !__linux__
