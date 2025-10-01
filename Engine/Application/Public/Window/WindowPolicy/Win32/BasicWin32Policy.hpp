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
    virtual void OnPreWcex()
    { }

    /**
     * @brief Called later on Create. Use it to register window in custom way.
     */
    virtual void OnPreRegister()
    { }

    /**
     * @brief Called on every Update. Can capture the event or pass it to base class implementation.
     */
    virtual void OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:

    template<class Policy = BasicWin32WindowPolicy>
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Policy* pPolicy = NULL;

        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pPolicy = reinterpret_cast<Policy*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pPolicy));

            pPolicy->m_pWindowDesc->Hwnd = hwnd;
        }
        else
            pPolicy = reinterpret_cast<Policy*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


        if (pPolicy) {
            pPolicy->OnUpdate(uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

protected:

    WindowDesc* GetWindowDesc() const
	{ return m_pWindowDesc; }

private:

	WindowDesc* m_pWindowDesc = nullptr;

};

} // !App
#endif // !AB_WINDOW_POLICY_H
#endif // _WIN32
