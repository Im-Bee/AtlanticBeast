#ifdef _WIN32

#include "Window/WindowPolicy/Win32/GameWin32Policy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
void GameWin32WindowPolicy::OnCreate(WindowDesc* pWd)
{
    pWd->pwszClassName = L"GameAtlanticClass";

    memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

    pWd->Wcex.cbSize        = sizeof(WNDCLASSEX);
    pWd->Wcex.style         = CS_HREDRAW | CS_VREDRAW;
    pWd->Wcex.hInstance     = GetModuleHandle(NULL);
    pWd->Wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    pWd->Wcex.lpszClassName = pWd->pwszClassName;
    pWd->Wcex.lpfnWndProc   = WindowProc<GameWin32WindowPolicy>;
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t GameWin32WindowPolicy::OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
        case WM_SETFOCUS:
        {
            RAWINPUTDEVICE rid[2];

            rid[0].usUsagePage  = 0x01;
            rid[0].usUsage      = 0x02;
            rid[0].dwFlags      = RIDEV_NOLEGACY;
            rid[0].hwndTarget   = pWd->Hwnd;

            rid[1].usUsagePage  = 0x01;
            rid[1].usUsage      = 0x06;
            rid[1].dwFlags      = RIDEV_NOLEGACY;
            rid[1].hwndTarget   = pWd->Hwnd;

            if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
                throw AB_EXCEPT("Couldn't register raw input");
        
            ShowCursor(FALSE);

            RECT rect;
            GetWindowRect(pWd->Hwnd, &rect);
            ClipCursor(&rect);

            break;
        }
        
        case WM_KILLFOCUS:
        case WM_DESTROY:
        {
            RAWINPUTDEVICE rid[2];

            rid[0].usUsagePage  = 0x01;
            rid[0].usUsage      = 0x02;
            rid[0].dwFlags      = RIDEV_REMOVE;
            rid[0].hwndTarget   = NULL;

            rid[1].usUsagePage  = 0x01;
            rid[1].usUsage      = 0x06;
            rid[1].dwFlags      = RIDEV_REMOVE;
            rid[1].hwndTarget   = NULL;

            if (!RegisterRawInputDevices(rid, 2, sizeof(rid[0])))
                throw AB_EXCEPT("Couldn't register raw input");

            ShowCursor(TRUE);
            ClipCursor(NULL);
            break;
        }

        case WM_INPUT:
        {
            UINT dwSize = 0;
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1))
                break;

            if (dwSize == 0)
                break;

            BYTE* buffer = new BYTE[dwSize];

            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
            {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    RAWMOUSE* rm = &raw->data.mouse;
                    if (rm->usFlags & MOUSE_MOVE_ABSOLUTE) {
                        delete[] buffer;
                        return 1;
                    }

                    LONG dx = rm->lLastX;
                    LONG dy = rm->lLastY;

                    pWd->LastEvent = EAbWindowEvents::Input;
                    pWd->InputStruct.Event = EAbInputEvents::AbMotion;
                    pWd->InputStruct.MouseX += dx;
                    pWd->InputStruct.MouseY += dy;
                }
                else if (raw->header.dwType == RIM_TYPEKEYBOARD)
                {
                    RAWKEYBOARD* rk = &raw->data.keyboard;

                    bool isPress = (rk->Flags & RI_KEY_BREAK) == 0;
                    uint32_t scanCode = rk->MakeCode;
                    USHORT flags = rk->Flags;

                    if (isPress)
                    {
                        pWd->LastEvent = EAbWindowEvents::Input;
                        pWd->InputStruct.Event = EAbInputEvents::AbKeyPress;
                        pWd->InputStruct.KeyId = scanCode;
                    }
                    else
                    {
                        pWd->LastEvent = EAbWindowEvents::Input;
                        pWd->InputStruct.Event = EAbInputEvents::AbKeyRelease;
                        pWd->InputStruct.KeyId = scanCode;
                    }
                }
            }

            delete[] buffer;
            return 1;
        }
    }

    return BasicWin32WindowPolicy::OnUpdate(pWd, uMsg, wParam, lParam);
}


} // !App
#endif // !_WIN32
