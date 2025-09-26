#ifdef _WIN32

#include "Window/WindowPolicy/Win32/GameWin32Policy.hpp"

namespace App
{

using namespace std;
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
void GameWin32WindowPolicy::OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_SETFOCUS:
        {
            RAWINPUTDEVICE rid;

            rid.usUsagePage = 0x01;
            rid.usUsage = 0x02;
            rid.dwFlags = 0;
            rid.hwndTarget = pWd->Hwnd;

            if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
                throw AB_EXCEPT("Couldn't register raw input");

            ShowCursor(FALSE);

            RECT rect;
            GetWindowRect(pWd->Hwnd, &rect);
            ClipCursor(&rect);
            SetCursorPos(rect.left + 0.5f * pWd->Width, rect.top + 0.5f * pWd->Height);


            break;
        }

        case WM_KILLFOCUS:
        case WM_DESTROY:
        {
            RAWINPUTDEVICE rid;

            rid.usUsagePage = 0x01;
            rid.usUsage = 0x02;
            rid.dwFlags = RIDEV_REMOVE;
            rid.hwndTarget = NULL;

            if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
                throw AB_EXCEPT("Couldn't register raw input");

            ShowCursor(TRUE);
            ClipCursor(NULL);
            break;
        }

        case WM_INPUT:
        {
            UINT cbSize;

            if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)) != 0) {
                AB_LOG(Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError());
                break;
            }

            if (cbSize == 0)
                break;

            UINT cbSize2 = cbSize * 16; 

            static ::std::vector<BYTE> vRi(cbSize2);
            if (vRi.size() < cbSize2)
                vRi.resize(cbSize2);

            size_t uRiRead = GetRawInputBuffer(reinterpret_cast<PRAWINPUT>(&vRi[0]), &cbSize2, sizeof(RAWINPUTHEADER));
            if (uRiRead == static_cast<UINT>(-1)) {
                AB_LOG(Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError());
                break;
            }

            pWd->LastEvent = EAbWindowEvents::Input;
            pWd->InputStruct.Event = EAbInputEvents::AbMotion;
            RAWINPUT* pRi = reinterpret_cast<PRAWINPUT>(&vRi[0]);
            for (size_t i = 0; 
                 i < uRiRead; 
                 ++i, pRi = NEXTRAWINPUTBLOCK(pRi))
            {
                auto& mouse = pRi->data.mouse;

                if (pRi->header.dwType != RIM_TYPEMOUSE || mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
                    continue;

                pWd->InputStruct.MouseX += mouse.lLastX;
                pWd->InputStruct.MouseY += mouse.lLastY;
            }

            return;
        }

        case WM_KEYDOWN: {
            bool bIsRepeated = (lParam & (static_cast<uint64_t>(1) << 30)) != 0;
            if (bIsRepeated) {
                pWd->LastEvent = NothingNew;
                return;
            }

            pWd->LastEvent = EAbWindowEvents::Input;
            uint32_t scanCode = (lParam >> 16) & 0xFF;
            pWd->InputStruct.Event = EAbInputEvents::AbKeyPress;
            pWd->InputStruct.KeyId = scanCode;
            return;
        }

        case WM_KEYUP: {
            pWd->LastEvent = EAbWindowEvents::Input;
            uint32_t scanCode = (lParam >> 16) & 0xFF;
            pWd->InputStruct.Event = EAbInputEvents::AbKeyRelease;
            pWd->InputStruct.KeyId = scanCode;
            return;
        }

        case WM_SIZE:
            BasicWin32WindowPolicy::OnUpdate(pWd, uMsg, wParam, lParam);
            return;

        case WM_MOUSEMOVE:
            return ;
    }

    return BasicWin32WindowPolicy::OnUpdate(pWd, uMsg, wParam, lParam);
}


} // !App
#endif // !_WIN32
