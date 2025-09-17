#ifndef AB_INPUT_EVENTS_H
#define AB_INPUT_EVENTS_H

#include "CSystem.h"

typedef enum EAbInputEvents
{
    AbKeyPress = 1,
    AbKeyRelease = AbKeyPress << 1,
    AbButtonPress = AbKeyRelease << 1,
    AbButtonRelease = AbButtonPress << 1,
    AbMotion = AbButtonRelease << 1,
} EAbInputEvents;

typedef struct AbInputStruct
{
    EAbInputEvents Event;
    uint8_t        Handled;
    int32_t        KeyId;
    double         MouseX;
    double         MouseY;
} AbInputStruct;

#endif // !AB_INPUT_EVENTS_H

