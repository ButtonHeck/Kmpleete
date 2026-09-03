#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Event/event.h"

#include <mutex>


namespace Kmpleete
{
    class FrameListenerManager;


    namespace Events
    {
        //! "Global" event queue class that may be asked to queue any event object,
        //! additionally FrameListenerManager class is allowed to get stored events for
        //! further processing (the queue itself doesn't do anything other than collecting events).
        //! This is a somewhat compromise between decoupling parts of the engine on the one hand
        //! and hiding possibility of accidental stealing events from any other part of code on the other hand.
        //! @see FrameListenerManager
        //! @see Events::Event
        class KMP_API EventQueue
        {
            KMP_DISABLE_COPY_MOVE(EventQueue)

        public:
            //! Passkey for GetEvents invocation
            //! to limit function availability to a FrameListenerManager 
            //! without "friend"-ing it
            class GetEventsPasskey
            {
                KMP_DISABLE_COPY_MOVE(GetEventsPasskey)

            private:
                friend class ::Kmpleete::FrameListenerManager;
                GetEventsPasskey() {}
            };

        public:
            KMP_NODISCARD static EventQueue& Get();
            KMP_NODISCARD Vector<UPtr<Event>> GetEvents(GetEventsPasskey);

        public:
            void QueueEvent(UPtr<Event>&& event);

        private:
            EventQueue() = default;
            ~EventQueue() = default;

        private:
            Vector<UPtr<Event>> _events;
            std::mutex _mutex;
        };
        //--------------------------------------------------------------------------


        //! Shortcut to queue an event
        inline void QueueEvent(UPtr<Event>&& event)
        {
            EventQueue::Get().QueueEvent(std::move(event));
        }
        //--------------------------------------------------------------------------
    }
}