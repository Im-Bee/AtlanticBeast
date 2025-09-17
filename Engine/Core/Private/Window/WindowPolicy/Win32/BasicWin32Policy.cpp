#ifdef _WIN32

#include "Window/BaseWindowDetails.h"
#include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

namespace Core
{

// Statics // ----------------------------------------------------------------------------------------------------------
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowDesc* pWd = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pWd = (WindowDesc*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWd);

        pWd->Hwnd = hwnd;
    }
    else
    {
        pWd = (WindowDesc*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pWd)
    {
        switch (uMsg) {
            case WM_KEYDOWN:
            {
                uint32_t scanCode = (lParam >> 16) & 0xFF;
                pWd->InputStruct.Handled = 0;
                pWd->InputStruct.Event = EAbInputEvents::AbButtonPress;
                pWd->InputStruct.KeyId = scanCode;
                break;
            }

            case WM_KEYUP:
            {
                uint32_t scanCode = (lParam >> 16) & 0xFF;
				pWd->InputStruct.Handled = 0;
                pWd->InputStruct.Event = EAbInputEvents::AbKeyRelease;
                pWd->InputStruct.KeyId = scanCode;
                break;
            }

			case WM_MOUSEMOVE:
                pWd->InputStruct.Handled = 0;
                pWd->InputStruct.Event = EAbInputEvents::AbMotion;
                pWd->InputStruct.MouseX = GET_X_LPARAM(lParam);
                pWd->InputStruct.MouseY = GET_Y_LPARAM(lParam);
                break;

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

        // AB_LOG(Core::Debug::Info, L"pwd->uLastMessage = %u", pWd->uLastMessage);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------
uint32_t BasicWin32WindowPolicy::CreateImpl(WindowDesc* pWd)
{
	AB_ASSERT(pWd != NULL);
    AB_ASSERT(pWd->Hwnd == NULL);
    AB_ASSERT(!pWd->IsAlive);

    // Fallback to default class name if none is provided,
    // if pwszClassName is provided, we assume that WCEX is already filled
    if (pWd->pwszClassName == NULL || wcscmp(pWd->pwszClassName, L"") == 0) {
        pWd->pwszClassName = L"AtlanticClass";

        memset(&pWd->Wcex, 0, sizeof(WNDCLASSEX));

        pWd->Wcex.cbSize = sizeof(WNDCLASSEX);
        pWd->Wcex.style = CS_HREDRAW | CS_VREDRAW;
        pWd->Wcex.hInstance = GetModuleHandle(NULL);
        pWd->Wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        pWd->Wcex.lpszClassName = pWd->pwszClassName;
        pWd->Wcex.lpfnWndProc = WindowProc;
    }

    if (!AbAskForWindowClass(pWd->pwszClassName)) {
        AbAskToRegisterWindowClass(pWd->Wcex);
    }

    HWND hwnd = CreateWindow(pWd->pwszClassName,
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

    if (!hwnd) {
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

    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, 1) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

} // !Core
#endif // !_WIN32