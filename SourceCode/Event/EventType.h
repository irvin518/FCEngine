#ifndef EVENT_EVENT_TYPE_H__INCLUDE
#define EVENT_EVENT_TYPE_H__INCLUDE

enum EEventType
{
    eEVT_EVENT_MOUSE = 1 << 24,
    eEVT_EVENT_MOUSE_PRESSED,
    eEVT_EVENT_MOUSE_RELEASED,
    eEVT_EVENT_MOUSE_MOVED,
    eEVT_EVENT_MOUSE_SCROLLED,

    eEVT_EVENT_KEYBOARD = 2 << 24,
    eEVT_EVENT_KEY_PRESSED,
    eEVT_EVENT_KEY_RELEASED,
    eEVT_EVENT_KEY_REPEATED,
    eEVT_EVENT_CHAR,

    eEVT_EVENT_TOUCH = 3 << 24,
    eEVT_EVENT_TOUCH_BEGIN,
    eEVT_EVENT_TOUCH_MOVE,
    eEVT_EVENT_TOUCH_END,
    eEVT_EVENT_TOUCH_CANCEL,

    eEVT_EVENT_GUI = 4 << 24,
    eEVT_EVENT_ANIMATION = 5 << 24,
};

#endif