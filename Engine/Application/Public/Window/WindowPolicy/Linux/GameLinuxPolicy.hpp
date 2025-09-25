#ifdef __linux__

#ifndef AB_GAME_WINDOW_POLICY_H
#define AB_GAME_WINDOW_POLICY_H

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

    uint32_t OnUpdate(WindowDesc* pWd, XEvent& event) override;

};

} // !App
#endif // !AB_GAME_WINDOW_POLICY_H
#endif // __linux__


