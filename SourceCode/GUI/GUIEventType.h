#ifndef GUI_EVENT_TYPE_H__INCLUDE
#define GUI_EVENT_TYPE_H__INCLUDE

#include "Event\EventType.h"

namespace FCGUI
{
    enum EGUIEventType
    {
        eGUIET_EVENT_COMMON = eEVT_EVENT_GUI + (1 << 16),
        eGUIET_EVENT_WINDOW = eEVT_EVENT_GUI + (2 << 16),
        eGUIET_EVENT_BUTTON = eEVT_EVENT_GUI + (3 << 16),
        eGUIET_EVENT_CHECKBOX = eEVT_EVENT_GUI + (4 << 16),
        eGUIET_EVENT_SLIDER = eEVT_EVENT_GUI + (5 << 16),
        eGUIET_EVENT_TEXTEDIT = eEVT_EVENT_GUI + (6 << 16),
        eGUIET_EVENT_TEXTLABEL = eEVT_EVENT_GUI + (7 << 16),
        eGUIET_EVENT_LISTBOX = eEVT_EVENT_GUI + (8 << 16),
        eGUIET_EVENT_COMBOBOX = eEVT_EVENT_GUI + (9 << 16),
        eGUIET_EVENT_LISTCONTROL = eEVT_EVENT_GUI + (10 << 16),
    };
}

#endif