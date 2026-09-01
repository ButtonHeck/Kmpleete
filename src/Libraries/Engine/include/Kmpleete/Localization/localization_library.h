#pragma once

#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Localization/localization_dictionary.h"
#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Profile/profiler_fwd.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    //! A wrapper for multiple translation dictionaries, responsible for managing lifetime of
    //! contained dictionary objects, delegating adding translations to them, adding or removing
    //! dictionaries and universally updating dictionaries' locales.
    //! As with LocalizationDictionary used as a delegate for this class, the LocalizationLibrary itself
    //! supposed to be used as a delegate for LocalizationManager class.
    //! @see LocalizationManager
    //! @see LocalizationDictionary
    class KMP_API LocalizationLibrary
    {
        KMP_LOG_CLASSNAME(LocalizationLibrary)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(LocalizationLibrary)

    public:
        LocalizationLibrary() noexcept;
        ~LocalizationLibrary() = default;

        void SetLocale(const LocaleStrSID& localeSid) noexcept;
        void SetLocale(const LocaleStr& locale) noexcept;

        KMP_NODISCARD bool AddDictionary(const DomainStrSID& domainSid);
        KMP_NODISCARD bool AddDictionary(const DomainStr& domain);
        KMP_NODISCARD bool RemoveDictionary(const DomainStrSID& domainSid) noexcept;
        KMP_NODISCARD bool RemoveDictionary(const DomainStr& domain) noexcept;

        void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& translation);
        void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                 PluralityForm pluralityForm, const TranslationStr& translation);
        void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation);
        void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                 PluralityForm pluralityForm, const ContextStrSID& contextSid, const TranslationStr& translation);

        KMP_NODISCARD const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const;
        KMP_NODISCARD const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm) const;
        KMP_NODISCARD const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const;
        KMP_NODISCARD const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm, const ContextStrSID& contextSid) const;

    private:
        LocaleStrSID _currentLocaleSid;
        HashMap<DomainStrSID, UPtr<LocalizationDictionary>> _dictionaryMap;
    };
    //--------------------------------------------------------------------------
}