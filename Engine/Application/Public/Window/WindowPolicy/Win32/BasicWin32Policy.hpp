#ifndef AB_WINDOW_POLICY_H
#define AB_WINDOW_POLICY_H

#ifdef _WIN32

#include "Window/WindowPolicy/IWindowPolicy.hpp"

namespace App
{

/**
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

};

} // !App
#endif // _WIN32
#endif // !AB_WINDOW_POLICY_H
