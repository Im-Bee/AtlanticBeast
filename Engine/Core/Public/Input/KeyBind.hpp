#ifndef AB_KEY_BIND_H
#define AB_KEY_BIND_H

#include "CSystem.hpp"

namespace Core
{

// Empty struct to force biding type safety. Like a rock for your shoe.
struct ActionType 
{ };

using Action = ActionType(*)(void* pThis);

enum EKeyState
{
    Release = 1,
    Press = Release << 1,
    Continuous = Press << 1,
};

enum EBindType
{
    Keyboard = 1,
};

struct KeyboardBind
{
    EKeyState KeyState;
    int8_t KeyCode;
};

struct InputBind 
{
    EBindType Type;

    union {
        KeyboardBind keyboard;
    };
};

} // !Core
  
#endif // !AB_KEY_BIND_H
