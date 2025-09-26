#ifdef _WIN32

#ifndef AB_WINDOW_POLICY_H
#define AB_WINDOW_POLICY_H

#include "Window/WindowPolicy/IWindowPolicy.hpp"

namespace App
{

/**
 * @brief Basic implementation of window that uses WinApi.
 *
 * If pwszClassName is provided, we assume that WCEX is already filled,
 * otherwise we fall back to basic window class.
 * */
class BEAST_API BasicWin32WindowPolicy : public IWindowPolicy<BasicWin32WindowPolicy>
{
public:

    BasicWin32WindowPolicy() = default;
    ~BasicWin32WindowPolicy() = default;
     
    BasicWin32WindowPolicy(const BasicWin32WindowPolicy&) = default;
    BasicWin32WindowPolicy(BasicWin32WindowPolicy&&) noexcept = default;

public:

    uint32_t CreateImpl(WindowDesc* pWd);
    
    void ShowImpl(WindowDesc* pWd);
    
    void HideImpl(WindowDesc* pWd);
    
    void DestroyImpl(WindowDesc* pWd);

    void UpdateImpl(WindowDesc* pWd);

public:

    /**
     * @brief Called first on Create. Use it to create WCEX.
     */
    virtual void OnCreate(WindowDesc* pWd)
    { }

    /**
     * @brief Called on every Update. Can capture the event or pass it to base class implementation.
     */
    virtual uint32_t OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:

    template<class Policy = BasicWin32WindowPolicy>
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        WindowDesc* pWd = NULL;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pWd = reinterpret_cast<WindowDesc*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWd));

            pWd->Hwnd = hwnd;
        }
        else
            pWd = reinterpret_cast<WindowDesc*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


        if (pWd) {
            Policy p;
            p.OnUpdate(pWd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

} // !App
#endif // !AB_WINDOW_POLICY_H
#endif // _WIN32
