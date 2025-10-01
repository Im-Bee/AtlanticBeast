#ifndef AB_WINDOW_EVENTS_H
#define AB_WINDOW_EVENTS_H

typedef uint32_t EAbWindowEventsFlags;

typedef enum EAbWindowEvents
{
    NothingNew = 1,
    Destroy = NothingNew << 1,
    Resize = Destroy << 1,
    Input = Resize << 1,
} EAbWindowEvents;

#endif // !AB_WINDOW_EVENTS_H

