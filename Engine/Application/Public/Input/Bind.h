#ifndef AB_KEY_BIND_H
#define AB_KEY_BIND_H

#include "CSystem.h"

/**
 * Empty struct to force binding type safety. Like a rock for your shoe.
 * */
typedef struct AbActionType {
    char Reserved;
} AbActionType;

typedef uint8_t AbKeyId;

typedef enum EKeyState {
    IsPressed = 1,
    IsReleased = IsPressed << 1,
} EKeyState;

typedef enum EAbOnKeyState {
    Release = 1,
    Press = Release << 1,
    Continuous = Press << 1,
} EAbOnKeyState;
 
typedef enum EAbBindType {
    Keyboard = 1,
    Mouse = Keyboard << 1,
} EAbBindType;

typedef struct AbKeyboardBind {
    EAbOnKeyState KeyState;
    AbKeyId KeyCode;
} AbKeyboardBind;

typedef struct AbInputBind {
    EAbBindType Type;

    union {
        AbKeyboardBind keyboard;
    };
} AbInputBind;

typedef AbActionType (*AbAction)(void* pThis);
typedef AbActionType (*AbMouseAction)(void* pThis, int32_t X, int32_t y);
  
#endif // !AB_KEY_BIND_H
