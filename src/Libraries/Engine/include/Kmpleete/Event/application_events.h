#pragma once

#include "Kmpleete/Event/event.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Utils/string_utils.h"


namespace Kmpleete
{
    //! Definitions of application events supported by engine

    namespace Events
    {
        static constexpr auto LocaleChangeEventTypeStr = "LocaleChangeEvent";

        static constexpr auto LocaleChangeEventTypeID = ToStringID(LocaleChangeEventTypeStr);


        struct LocaleChangeEvent : public Event
        {
            EVENT_CLASS_TYPE(LocaleChangeEventTypeStr)

            explicit LocaleChangeEvent(const Localization::LocaleStr& locale) noexcept
                : _locale(locale)
            {}

            KMP_NODISCARD const Localization::LocaleStr& GetLocale() const noexcept
            {
                return _locale;
            }

            KMP_NODISCARD String ToString() const override
            {
                return Utils::Concatenate(GetName(), ": ", _locale);
            }

        private:
            const Localization::LocaleStr _locale;
        };
        //--------------------------------------------------------------------------
    }
}