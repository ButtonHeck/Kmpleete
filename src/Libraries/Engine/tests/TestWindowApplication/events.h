#pragma once

#include "Kmpleete/Event/event.h"


namespace Kmpleete
{
    namespace Events
    {
        struct CustomEvent : public Event
        {
            EVENT_CLASS_TYPE("CustomEvent")

            CustomEvent() = default;
        };
        //--------------------------------------------------------------------------

        struct VSyncChangeEvent : public Event
        {
            EVENT_CLASS_TYPE("VSyncChangeEvent")

            VSyncChangeEvent(bool vSync)
                : vSync(vSync)
            {}

            bool vSync;
        };
        //--------------------------------------------------------------------------
    }
}