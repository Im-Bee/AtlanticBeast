#ifdef _WIN32

#ifndef AB_GAME_POLICY_H
#define AB_GAME_POLICY_H

#include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"

namespace App
{

class BEAST_API GameWin32WindowPolicy : public BasicWin32WindowPolicy
{
public:

    GameWin32WindowPolicy()
        : m_uRiRead(0)
        , m_vRi()
    { }

    ~GameWin32WindowPolicy() = default;

    GameWin32WindowPolicy(const GameWin32WindowPolicy&) = default;
    GameWin32WindowPolicy(GameWin32WindowPolicy&&) noexcept = default;

public:

    void OnCreate(WindowDesc* pWd) override;

    uint32_t OnUpdate(WindowDesc* pWd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:

    uint32_t m_uCenterX;
    uint32_t m_uCenterY;

    UINT m_uRiRead;
    ::std::vector<BYTE> m_vRi;

};

} // !App
#endif // !AB_GAME_POLICY_H
#endif // _WIN32
