#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"

#include <ostream>


namespace Kmpleete
{
    namespace Events
    {
        using EventTypeID = StringID;


    //! Helper macro wrapper for derived events info getters
    #define EVENT_CLASS_TYPE(eventTypeStr) \
        static constexpr EventTypeID TypeID = ToStringID(eventTypeStr); \
        static constexpr const char* TypeName = eventTypeStr; \
        KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept override { return TypeID; } \
        KMP_NODISCARD virtual const char* GetName() const noexcept override { return TypeName; }


        //! Base class for all the events of the application
        struct Event
        {
            virtual ~Event() = default;

            KMP_NODISCARD virtual EventTypeID GetTypeID() const noexcept = 0;
            KMP_NODISCARD virtual const char* GetName() const noexcept = 0;
            KMP_NODISCARD virtual String ToString() const
            {
                return GetName();
            }

            bool handled = false;
        };
        //--------------------------------------------------------------------------

        inline std::ostream& operator<<(std::ostream& os, const Event& event)
        {
            return os << event.ToString();
        }
        //--------------------------------------------------------------------------
    }
}
