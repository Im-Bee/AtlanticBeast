#ifndef AB_GAME_SYSTEM_POLICY_H
#define AB_GAME_SYSTEM_POLICY_H

#if _WIN32
#	include "Window/WindowPolicy/Win32/BasicWin32Policy.hpp"
#elif __linux__
#   include "Window/WindowPolicy/Linux/GameLinuxPolicy.hpp"
#endif // !_WIN32

namespace App
{

typedef 
#if _WIN32
	BasicWin32WindowPolicy
#elif __linux__
	GameLinuxWindowPolicy
#endif // !_WIN32
	DefaultGameSystemWindowPolicy;

} // !App
#endif // !AB_GAME_SYSTEM_POLICY_H

