#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Event/event.h"
#include "Kmpleete/Event/event_dispatcher.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    namespace Events
    {
        //! Utility helper class to automatically attach/detach its typed handler to/from an EventDispatcher object
        //! during creation/deletion
        //! @see Events::EventDispatcher
        //! @see Events::EventHandler
        template<typename EventClass> requires (IsBaseClass<Event, EventClass>::value)
        class EventHandlerGuard
        {
            KMP_DISABLE_COPY_MOVE(EventHandlerGuard)
            KMP_LOG_CLASSNAME(EventHandlerGuard)

        public:
            EventHandlerGuard(EventDispatcher& eventDispatcher, const EventHandler<EventClass>& handler, bool attachOnCreate = true) noexcept
                : _eventDispatcher(eventDispatcher)
                , _handler(handler)
            {
                if (attachOnCreate)
                {
                    Attach();
                }
            }

            ~EventHandlerGuard()
            {
                try
                {
                    Detach();
                }
                catch (...)
                {
                    KMP_LOG_ERROR("failed to detach event handler");
                }
            }

            bool Attach()
            {
                return _eventDispatcher.AddHandler(_handler);
            }

            bool Detach()
            {
                return _eventDispatcher.RemoveHandler(_handler);
            }

        private:
            EventDispatcher& _eventDispatcher;
            EventHandler<EventClass> _handler;
        };
        //--------------------------------------------------------------------------
    }
}