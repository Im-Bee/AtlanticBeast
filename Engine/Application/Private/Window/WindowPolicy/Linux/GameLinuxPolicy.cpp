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
            int rootX, rootY, posX, posY;
            Window dummyWindow;
            unsigned int dummy;

            XQueryPointer(display, 
                          window,
                          &dummyWindow,
                          &dummyWindow,
                          &rootX,
                          &rootY,
                          &posX,
                          &posY,
                          reinterpret_cast<unsigned int*>(&dummy));

            if (posX == static_cast<int>(pWd->Width * 0.5f) && 
                posY == static_cast<int>(pWd->Height * 0.5f)) 
            {
                pWd->LastEvent = NothingNew;
                return 1;
            }

            pWd->LastEvent = Input;
            pWd->InputStruct.Event = AbMotion;
            pWd->InputStruct.LastMouseX = 0;
            pWd->InputStruct.LastMouseY = 0;
            pWd->InputStruct.MouseX = posX - event.xmotion.x;
            pWd->InputStruct.MouseY = posY - event.xmotion.y;

            XWarpPointer(display, 
                         None, 
                         window,
                         0, 0, 
                         0, 0, 
                         pWd->Width * 0.5f, pWd->Height * 0.5f);

            XFlush(display);
            return 1;
    }

    return BasicLinuxWindowPolicy::UpdateEvent(pWd, event);
}

} // !App
#endif // !__linux__
