#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/functional.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Localization/localization_library.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log_class_macro.h"

#include <boost/locale.hpp>


namespace Kmpleete
{
    class SettingsDocument;
    class WindowApplication;


    //! Manager for localization of the application, responsible for handling locale settings,
    //! creating locales, adding/removing translations from files (domains). Uses Boost.Locale library as
    //! the localization backend. Each time a locale change occur this manager will emit a LocaleChangeEvent.
    //! "Translate[...]" functions names SHOULD MATCH parameters of gettext utility
    //! programs in order to successfully update and compile translations - thus if any renaming occurs, make sure
    //! to rename those parameters as well.
    //! Translations may either be fetched from pre-filled library object or can be "directly" translated using
    //! free functions from localization_translator.h
    //! @see localization_translator.h
    //! @see localization_base.h
    //! @see LocalizationLibrary
    class KMP_API LocalizationManager
    {
        KMP_LOG_CLASSNAME(LocalizationManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(LocalizationManager)

    public:
        //! Passkey for ProcessLocaleChangeRequest invocation
        //! to limit function availability to certain classes 
        //! without "friend"-ing them
        class RequestPasskey
        {
            KMP_DISABLE_COPY_MOVE(RequestPasskey)

        private:
            friend class ::Kmpleete::WindowApplication;
            RequestPasskey() {}
        };

    public:
        explicit LocalizationManager(const String& initialMessagesPath = "") noexcept;
        ~LocalizationManager() = default;

        void SetLocaleChangeRequest(const LocaleStr& localeString);
        void ProcessLocaleChangeRequest(RequestPasskey);

        bool SetLocale(const LocaleStr& localeString);
        KMP_NODISCARD const LocaleStr& GetLocale() const noexcept;

        void AddMessagesPath(const String& path);

        bool AddMessagesDomain(const DomainStr& domain);
        bool RemoveMessagesDomain(const DomainStr& domain);

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

        TranslationStr Translate(const DomainStr& domain, const SourceStr& source);
        TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count);
        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context);
        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context);

        KMP_NODISCARD const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const;
        KMP_NODISCARD const TranslationStr& Translation(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                        const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD TranslationStr TranslationFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                          const SourceStrSID& sourceSidPlural, int count) const;
        KMP_NODISCARD const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                           const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD TranslationStr TranslationCtxFormatted(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                             const SourceStrSID& sourceSidPlural, int count, const ContextStrSID& contextSid) const;
        KMP_NODISCARD const TranslationStr& TranslationCtx(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const;
        KMP_NODISCARD const TranslationStr& TranslationOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& fallback) const;
        KMP_NODISCARD const TranslationStr& TranslationCtxOrFallback(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, 
                                                                     const ContextStrSID& contextSid, const TranslationStr& fallback) const;

        KMP_NODISCARD TranslationStr Translation(const DomainStr& domain, const SourceStr& source, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr Translation(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const LocaleStr& localeString);
        KMP_NODISCARD TranslationStr TranslationCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const LocaleStr& localeString);

    private:
        //! Helper struct to keep track on future localization changes requests
        struct LocaleChangeRequest
        {
            bool pending;
            String newLocale;
        };

    private:
        void _ImbueLocale() const;
        void _FillDictionary();
        bool _SetLocale(const LocaleStr& localeString);

    private:
        boost::locale::generator _localeGenerator;
        UPtr<LocalizationLibrary> _library;
        LocaleStr _currentLocale;
        LocaleChangeRequest _localeChangeRequest;
    };
    //--------------------------------------------------------------------------
}