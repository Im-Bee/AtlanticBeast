#ifndef AB_KEY_BIND_H
#define AB_KEY_BIND_H

namespace Core
{

struct Action 
{ };

using InputAction = Action(*)(void* pThis);

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

struct InputBind 
{
    EBindType Type;

    union Data {
        struct Keyboard {
            EKeyState KeyState;
            int8_t KeyCode;
        };
    };
};

} // !Core
  
#endif // !AB_KEY_BIND_H
