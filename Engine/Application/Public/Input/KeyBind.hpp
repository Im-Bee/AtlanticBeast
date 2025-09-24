#ifndef AB_KEY_BIND_H
#define AB_KEY_BIND_H

#include "CSystem.hpp"

namespace App
{

/**
 * Empty struct to force biding type safety. Like a rock for your shoe.
 * */
struct ActionType 
{ };

using Action = ActionType(*)(void* pThis);
using MouseAction = ActionType(*)(void* pThis, float fX, float fY);

enum EKeyState
{
    IsPressed = 1,
    IsReleased = IsPressed << 1,
};

enum EOnKeyState
{
    Release = 1,
    Press = Release << 1,
    Continuous = Press << 1,
};

enum EBindType
{
    Keyboard = 1,
    Mouse = Keyboard << 1,
};

struct KeyboardBind
{
    EOnKeyState KeyState;
    int8_t KeyCode;
};

struct InputBind 
{
    EBindType Type;

    union {
        KeyboardBind keyboard;
    };
};

} // !App
  
#endif // !AB_KEY_BIND_H
