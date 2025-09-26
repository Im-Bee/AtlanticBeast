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
uint32_t GameWin32WindowPolicy::OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
        case WM_SETFOCUS:
        {
            RAWINPUTDEVICE rid;

            rid.usUsagePage  = 0x01;
            rid.usUsage      = 0x02;
            rid.dwFlags      = 0;
            rid.hwndTarget   = pWd->Hwnd;

            if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
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
            RAWINPUTDEVICE rid;

            rid.usUsagePage  = 0x01;
            rid.usUsage      = 0x02;
            rid.dwFlags      = RIDEV_REMOVE;
            rid.hwndTarget   = NULL;

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

            ::std::vector<BYTE> vRi(cbSize2);

            size_t uRiRead = GetRawInputBuffer(reinterpret_cast<PRAWINPUT>(&vRi[0]), &cbSize2, sizeof(RAWINPUTHEADER));
            if (uRiRead == (UINT)-1) {
                AB_LOG(Core::Debug::Error, L"GetRawInputBuffer error %d", GetLastError());
                break;
            }

            pWd->LastEvent = EAbWindowEvents::Input;
            pWd->InputStruct.Event = EAbInputEvents::AbMotion;
            RAWINPUT* pRi = reinterpret_cast<PRAWINPUT>(&vRi[0]);
            for (size_t i = 0; 
                 i < m_uRiRead; 
                 ++i, pRi = NEXTRAWINPUTBLOCK(pRi))
            {
                if (pRi->header.dwType != RIM_TYPEMOUSE ||
                    pRi->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
                    continue;

                pWd->InputStruct.MouseX += pRi->data.mouse.lLastX;
                pWd->InputStruct.MouseY += pRi->data.mouse.lLastY;
            }

            SetCursorPos(m_uCenterX, m_uCenterY);
            return 1;
        }

        case WM_SIZE:
            BasicWin32WindowPolicy::OnUpdate(pWd, uMsg, wParam, lParam);

            RECT rect;
            GetClientRect(pWd->Hwnd, &rect);
            m_uCenterX = (rect.right - rect.left) + 0.5f * pWd->Height;
            m_uCenterY = (rect.bottom - rect.top) + 0.5f * pWd->Width;

            return 1;

        case WM_MOUSEMOVE:
            return 1;
    }

    return BasicWin32WindowPolicy::OnUpdate(pWd, uMsg, wParam, lParam);
}


} // !App
#endif // !_WIN32
