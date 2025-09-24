#ifdef __linux__

#include "AbLimits.h"
#include "Window/BaseWindowDetails.h"
#include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicLinuxWindowPolicy::CreateImpl(WindowDesc* pWd)
{
    pWd->DisplayHandle = AbAskForDisplayLinux(NULL);

    if (pWd->DisplayHandle == NULL) {
        throw AB_EXCEPT("DisplayHandle is null!");
    }

    int screen = DefaultScreen(pWd->DisplayHandle);
    Display* pDisplay = pWd->DisplayHandle;

    Window window = XCreateSimpleWindow(pWd->DisplayHandle,
                                        RootWindow(pDisplay, screen),
                                        100, 100,
                                        pWd->Width, pWd->Height,
                                        1,
                                        BlackPixel(pDisplay, screen),
                                        WhitePixel(pDisplay, screen));


    XTextProperty windowName;
    char* szWindowName = (char*)malloc(sizeof(char) * AB_SMALL_STRING);
    size_t uWriten = wcstombs(szWindowName, pWd->Name, pWd->uNameLen);
    szWindowName[uWriten] = '\0';

    XStringListToTextProperty(&szWindowName, 1, &windowName);
    XSetWMName(pDisplay, window, &windowName);
    free(szWindowName);
    XFree(windowName.value);

    int bSupported;
    XkbSetDetectableAutoRepeat(pDisplay, True, &bSupported);
    if (!bSupported) {
        AB_LOG(Debug::Error, L"Detectable auto repeat ISN`T SUPPORTED!");
    }

    XSelectInput(pDisplay, window, ButtonMotionMask |
                                  ButtonPressMask |
                                  ButtonReleaseMask |
                                  KeyPressMask |
                                  KeyReleaseMask |
                                  ExposureMask | 
                                  StructureNotifyMask | 
                                  SubstructureNotifyMask |
                                  SubstructureRedirectMask);
    XMapWindow(pDisplay, window);

    pWd->Screen = DefaultScreen(pDisplay);
    pWd->WindowHandle = window;

    Atom wmDeleteMessage = XInternAtom(pDisplay, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(pDisplay, window, &wmDeleteMessage, 1);

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::ShowImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->IsAlive);
    AB_ASSERT(pWd->DisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XMapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::HideImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->IsAlive);
    AB_ASSERT(pWd->DisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::DestroyImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->IsAlive);
    AB_ASSERT(pWd->DisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XDestroyWindow(pWd->DisplayHandle, pWd->WindowHandle);
    UpdateImpl(pWd);

    AbAskToCloseDisplayLinux(NULL);
    pWd->WindowHandle = 0;
    pWd->DisplayHandle = NULL;
    pWd->Screen = 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicLinuxWindowPolicy::UpdateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd);
    AB_ASSERT(pWd->IsAlive);
    AB_ASSERT(pWd->DisplayHandle);
    AB_ASSERT(pWd->WindowHandle);

    XEvent      event;
    Display*    display = pWd->DisplayHandle;
    Window      window  = pWd->WindowHandle;

    Atom wmDeleteMessage;

    while(XPending(display))
    {
        XPeekEvent(display, &event);

        if (event.xany.window != window && 
            event.type != UnmapNotify &&
            event.type != DestroyNotify) 
        {
            return;
        }

        XNextEvent(display, &event);

        switch (event.type) {
            case KeyPress:
                pWd->LastEvent = Input;
                pWd->InputStruct.Event = AbKeyPress;
                pWd->InputStruct.KeyId = event.xkey.keycode - 8;
                return;

            case KeyRelease:
                pWd->LastEvent = Input;
                pWd->InputStruct.Event = AbKeyRelease;
                pWd->InputStruct.KeyId = event.xkey.keycode - 8;
                return;

            case ButtonPress:
                pWd->LastEvent = Input;
                pWd->InputStruct.Event = AbButtonPress;
                // pWd->InputStruct.KeyId =  - 8;
                return;

            case ButtonRelease:
                pWd->LastEvent = Input;
                pWd->InputStruct.Event = AbButtonRelease;
                // pWd->InputStruct.KeyId = deviceEvent->detail - 8;
                return;

            case MotionNotify:
                pWd->LastEvent = Input;
                pWd->InputStruct.Event = AbMotion;
                pWd->InputStruct.LastMouseX = pWd->InputStruct.MouseX;
                pWd->InputStruct.LastMouseY = pWd->InputStruct.MouseY;
                pWd->InputStruct.MouseX = static_cast<int32_t>(event.xmotion.x_root);
                pWd->InputStruct.MouseY = static_cast<int32_t>(event.xmotion.y_root);
                return;

            case Expose:
                pWd->LastEvent = Resize;
                pWd->Width = event.xexpose.width;
                pWd->Height = event.xexpose.height;
                break;

            case ConfigureNotify:
                pWd->LastEvent = Resize;
                pWd->Height = event.xconfigure.height;
                pWd->Width = event.xconfigure.width;
                break;

            case ClientMessage:
                if (event.xclient.window != window)
                    return;

                if ((wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", 1)) == None)
                    break;

                if (static_cast<Atom>(event.xclient.data.l[0]) == wmDeleteMessage) {
                    pWd->LastEvent = Destroy;
                    return;
                }

                break;
        }

        pWd->LastEvent = EAbWindowEvents::NothingNew;
    }
}

} // !App
#endif // !__linux__
