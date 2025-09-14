#include "Core.h"
#include "BaseWindowDetails.h"

#ifdef __linux__

// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbCreateImpl(WindowDesc* pWd)
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
void AbShowImpl(WindowDesc* wd)
{
    XMapWindow(wd->DisplayHandle, wd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbHideImpl(WindowDesc* pWd)
{
    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDestroyImpl(WindowDesc* pWd)
{ 
    if (!pWd->IsAlive || !pWd->DisplayHandle || !pWd->WindowHandle) {
        return;
    }

    XUnmapWindow(pWd->DisplayHandle, pWd->WindowHandle);
}
 
// ---------------------------------------------------------------------------------------------------------------------
void AbUpdateImpl(WindowDesc* pWd)
{
    XEvent event;
    Display*  display     = pWd->DisplayHandle;
    Window    window      = pWd->WindowHandle;
    int       screen      = pWd->Screen;

    while (display && XPending(display)) {

        XNextEvent(display, &event);
        if (event.type == Expose)
        {
            pWd->Width = event.xexpose.width;
            pWd->Height = event.xexpose.height;
        }
        else if (event.type == ConfigureNotify)
        {
            pWd->Height = event.xconfigure.height;
            pWd->Width = event.xconfigure.width;
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
}

#elif _WIN32

// ---------------------------------------------------------------------------------------------------------------------
uint32_t AbCreateImpl(WindowDesc* wd)
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
void AbShowImpl(WindowDesc* wd)
{
    ShowWindow(wd->Hwnd, SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbHideImpl(WindowDesc* wd)
{
    ShowWindow(wd->Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void AbDestroyImpl(WindowDesc* wd)
{
    if (!wd->IsAlive || wd->Hwnd == NULL) {
        return;
    }

    if (DestroyWindow(wd->Hwnd)) {
        wd->Hwnd = NULL;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
void AbUpdateImpl(WindowDesc* wd)
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
