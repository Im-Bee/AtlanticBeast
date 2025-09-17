#ifdef __linux__

#include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"

namespace Core
{

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicLinuxWindowPolicy::CreateImpl(WindowDesc* pWd)
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
void BasicLinuxWindowPolicy::ShowImpl(WindowDesc* wd)
{
    XMapWindow(wd->DisplayHandle, wd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HideImpl(WindowDesc* pWd)
{
    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::DestroyImpl(WindowDesc* pWd)
{
    if (!pWd->IsAlive || !pWd->DisplayHandle || !pWd->WindowHandle)
        return;

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);

    AbAskToCloseDisplayLinux(NULL);
    m_pWindowDesc->WindowHandle = 0;
    m_pWindowDesc->DisplayHandle = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::UpdateImpl(WindowDesc* pWd)
{
    XEvent      event;
    Display* display = pWd->DisplayHandle;
    Window      window = pWd->WindowHandle;
    int         screen = pWd->Screen;


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

            XIDeviceEvent* rawev = (XIDeviceEvent*)event.xcookie.data;

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

} // !Core


#endif // !__linux__