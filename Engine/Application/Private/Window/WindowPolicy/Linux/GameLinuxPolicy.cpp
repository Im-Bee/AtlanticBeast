#ifdef __linux__
#include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t GameLinuxWindowPolicy::UpdateEvent(WindowDesc* pWd, XEvent& event)
{
    Display* display = pWd->DisplayHandle;
    Window   window  = pWd->WindowHandle;
    char     pEmptyData[8] = { 0 };
  
    switch (event.type) {
        case FocusIn:
            XGrabPointer(display, 
                         window,
                         True, 
                         PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         window,
                         None,
                         CurrentTime);

            Pixmap pixMap;
            Colormap colormap;
            XColor dummyColor;
            Cursor invisibleCursor;

            pixMap = XCreateBitmapFromData(display, window, pEmptyData, 8, 8);
            invisibleCursor = XCreatePixmapCursor(display, pixMap, pixMap, &dummyColor, &dummyColor, 0, 0);

            XDefineCursor(display, window, invisibleCursor);

            XWarpPointer(display, 
                         None, 
                         window, 
                         0, 0, 
                         0, 0, 
                         pWd->Width * 0.5f, pWd->Height * 0.5f);

            XFlush(display);
            return 1;

        case MotionNotify:
            int rootX, rootY, dummy;
            Window dummyWindow;

            XQueryPointer(display, 
                          window,
                          &dummyWindow,
                          &dummyWindow,
                          &rootX,
                          &rootY,
                          &dummy,
                          &dummy,
                          reinterpret_cast<unsigned int*>(&dummy));

            pWd->LastEvent = Input;
            pWd->InputStruct.Event = AbMotion;
            pWd->InputStruct.LastMouseX = pWd->InputStruct.MouseX;
            pWd->InputStruct.LastMouseY = pWd->InputStruct.MouseY;
            pWd->InputStruct.MouseX = rootX;
            pWd->InputStruct.MouseY = rootY;

            XWarpPointer(display, 
                         None, 
                         DefaultRootWindow(display), 
                         0, 0, 
                         0, 0, 
                         pWd->Width * 0.5f, pWd->Height * 0.5f);

            XFlush(display);

            AB_LOG(Core::Debug::Info, 
                    L"%d %d, %d %d",
                    pWd->InputStruct.MouseX - pWd->InputStruct.LastMouseX, 
                    pWd->InputStruct.MouseY - pWd->InputStruct.LastMouseY,
                    rootX, rootY);
            return 1;
    }

    return BasicLinuxWindowPolicy::UpdateEvent(pWd, event);
}

} // !App
#endif // !__linux__
