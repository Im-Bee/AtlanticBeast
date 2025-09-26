#ifdef _WIN32

#include "Window/BaseWindowDetails.h"
#include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

namespace App
{

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicWin32WindowPolicy::CreateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd == NULL);
    AB_ASSERT(!pWd->IsAlive);

    this->OnCreate(pWd);

    // Fallback to default class name if none is provided,
    // if pwszClassName is provided, we assume that WCEX is already filled
    if (pWd->pwszClassName == NULL || wcscmp(pWd->pwszClassName, L"") == 0)
    {
        pWd->pwszClassName = L"AtlanticClass";

        memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

        pWd->Wcex.cbSize            = sizeof(WNDCLASSEX);
        pWd->Wcex.style             = CS_HREDRAW | CS_VREDRAW;
        pWd->Wcex.hInstance         = GetModuleHandle(NULL);
        pWd->Wcex.hCursor           = LoadCursor(NULL, IDC_ARROW);
        pWd->Wcex.lpszClassName     = pWd->pwszClassName;
        pWd->Wcex.lpfnWndProc       = BasicWin32WindowPolicy::WindowProc;
    }

    AbAskToRegisterWindowClass(pWd->pwszClassName, pWd->Wcex);

    HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                               pWd->pwszClassName,
                               pWd->Name,
                               WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               pWd->Width,
                               pWd->Height,
                               NULL,
                               NULL,
                               GetModuleHandle(NULL),
                               pWd);

    if (hwnd == NULL) {
        AB_LOG(Core::Debug::Error, L"Couldn't CreateWindow(), last error %u", GetLastError());

        return -1;
    }

    ShowWindow(hwnd, SW_SHOW);

    pWd->Hwnd = hwnd;

    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::ShowImpl(WindowDesc* pWd)
{ 
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);

    ShowWindow(pWd->Hwnd, SW_SHOW);
} 

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::HideImpl(WindowDesc* pWd)
{ 
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);

    ShowWindow(pWd->Hwnd, SW_HIDE);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::DestroyImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd != NULL);
    AB_ASSERT(pWd->IsAlive);

    if (DestroyWindow(pWd->Hwnd)) {
        pWd->Hwnd = NULL;
    }

    AbAskToCloseWindowClass(pWd->pwszClassName);
}

// ---------------------------------------------------------------------------------------------------------------------
void BasicWin32WindowPolicy::UpdateImpl(WindowDesc* pWd)
{
    AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->IsAlive);

    MSG msg;
    while (PeekMessage(&msg, pWd->Hwnd, 0, 0, PM_REMOVE) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (pWd->LastEvent & Input && pWd->InputStruct.Event & ~AbMotion)
            return;
    }
}

void BasicWin32WindowPolicy::OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
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

        case WM_MOUSEMOVE:
            pWd->LastEvent = EAbWindowEvents::Input;
            pWd->InputStruct.Event = EAbInputEvents::AbMotion;
            pWd->InputStruct.MouseX = GET_X_LPARAM(lParam);
            pWd->InputStruct.MouseY = GET_Y_LPARAM(lParam);
            return;

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
}

} // !App
#endif // !_WIN32
