#ifndef AB_WINDOW_EVENTS_H
#define AB_WINDOW_EVENTS_H

#include "Core.h"

typedef uint32_t EAbWindowEventsFlags;

typedef enum EAbWindowEvents
{
    NothingNew = 1,
    Destroy = NothingNew << 1,
    Resize = Destroy << 1,
    Input = Resize << 1,
    ChangedBehavior = Input << 1,
    EventsMask = 0xFFFFFFFF
} EAbWindowEvents;

#endif // !AB_WINDOW_EVENTS_H

