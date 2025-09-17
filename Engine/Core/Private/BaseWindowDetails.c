#include "Core.h"
#include "BaseWindowDetails.h"

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbDetailsCreateImpl(WindowDesc* pWd)
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
    if (!pWd->IsAlive || !pWd->DisplayHandle || !pWd->WindowHandle) {
        return;
    }

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
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
                    break;
                case XI_KeyRelease:
                    pWd->InputStruct.Event = AbKeyRelease;
                    break;
                case XI_ButtonPress:
                    pWd->InputStruct.Event = AbButtonPress;
                    break;
                case XI_ButtonRelease:
                    pWd->InputStruct.Event = AbButtonRelease;
                    break;
                case XI_Motion:
                    pWd->InputStruct.Event = AbMotion;
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

                if ((Atom)event.xclient.data.l[0] == wmDeleteMessage)
                    pWd->LastEvent = Destroy;

                break;
        }
    }
}

#elif _WIN32

// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbDetailsCreateImpl(WindowDesc* wd)
{
    if (wd->Hwnd) {
        return -2;
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
