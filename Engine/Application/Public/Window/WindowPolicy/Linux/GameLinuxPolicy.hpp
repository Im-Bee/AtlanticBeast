#ifndef AB_GAME_WINDOW_POLICY_H
#define AB_GAME_WINDOW_POLICY_H

#ifdef __linux__

#include "Window/WindowPolicy/Linux/BasicLinuxPolicy.hpp"

namespace App
{

class BEAST_API GameLinuxWindowPolicy : public BasicLinuxWindowPolicy
{
public:

	GameLinuxWindowPolicy() = default;
	~GameLinuxWindowPolicy() = default;

	GameLinuxWindowPolicy(const GameLinuxWindowPolicy&) = default;
	GameLinuxWindowPolicy(GameLinuxWindowPolicy&&) noexcept = default;

public:

    uint32_t UpdateEvent(WindowDesc* pWd, XEvent& event) override;

};

} // !App
#endif // __linux__
#endif // !AB_GAME_WINDOW_POLICY_H


