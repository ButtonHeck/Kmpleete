#include "Kmpleete/Localization/localization_dictionary.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    LocalizationDictionary::LocalizationDictionary(const DomainStrSID& domain) noexcept
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _domain(domain)
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    const DomainStrSID& LocalizationDictionary::GetDomain() const noexcept
    {
        return _domain;
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Clear() noexcept
    {
        _translationMap.clear();
        _translationCtxMap.clear();
        _translationPluralMap.clear();
        _translationCtxPluralMap.clear();
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const TranslationStr& translation) KMP_PROFILING(ProfileLevelImportant)
    {
        if (_translationMap.contains(sourceSid))
        {
            if (_translationMap[sourceSid] != translation)
            {
                KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"", _domain, sourceSid);
            }
        }
        else
        {
            _translationMap.insert(std::make_pair(sourceSid, translation));
        }
    }}
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm, const TranslationStr& translation) KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(pluralityForm >= PluralityFormSingular && pluralityForm < PluralityFormCount);

        const PluralSource&& pluralSource = PluralSource{ sourceSidSingular, sourceSidPlural };
        const auto isPluralSourceAdded = _translationPluralMap.contains(pluralSource);
        if (isPluralSourceAdded && 
            not _translationPluralMap[pluralSource][pluralityForm].empty() &&
            _translationPluralMap[pluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"/\"{}\" (plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, static_cast<int>(pluralityForm));
            return;
        }

        if (not isPluralSourceAdded)
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            _translationPluralMap.insert(std::make_pair(pluralSource, pluralTranslations));
        }
        else
        {
            _translationPluralMap[pluralSource][pluralityForm] = translation;
        }
    }}
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation) KMP_PROFILING(ProfileLevelImportant)
    {
        const ContextedSource&& contextedSource = ContextedSource{ sourceSid, contextSid };
        if (_translationCtxMap.contains(contextedSource))
        {
            if (_translationCtxMap[contextedSource] != translation)
            {
                KMP_LOG_WARN("\"{}\" possible duplicate \"{}\" (context \"{}\")", _domain, sourceSid, contextSid);
            }
        }
        else
        {
            _translationCtxMap.insert(std::make_pair(contextedSource, translation));
        }
    }}
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm,
                                     const ContextStrSID& contextSid, const TranslationStr& translation) KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(pluralityForm >= PluralityFormSingular && pluralityForm < PluralityFormCount);

        const ContextedPluralSource&& contextedPluralSource = ContextedPluralSource{ sourceSidSingular, sourceSidPlural, contextSid };
        const auto isContextedPluralSourceAdded = _translationCtxPluralMap.contains(contextedPluralSource);
        if (isContextedPluralSourceAdded &&
            not _translationCtxPluralMap[contextedPluralSource][pluralityForm].empty() &&
            _translationCtxPluralMap[contextedPluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"/\"{}\" (context \"{}\" plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, contextSid, static_cast<int>(pluralityForm));
            return;
        }

        if (not isContextedPluralSourceAdded)
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            _translationCtxPluralMap.insert(std::make_pair(contextedPluralSource, pluralTranslations));
        }
        else
        {
            _translationCtxPluralMap[contextedPluralSource][pluralityForm] = translation;
        }
    }}
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid) KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        return _translationMap[sourceSid];
    }}
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm) KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        KMP_ASSERT(pluralityForm >= PluralityFormSingular && pluralityForm < PluralityFormCount);

        return _translationPluralMap[PluralSource{ sourceSidSingular, sourceSidPlural }][pluralityForm];
    }}
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid) KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        return _translationCtxMap[ContextedSource{ sourceSid, contextSid }];
    }}
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural,
                                                      PluralityForm pluralityForm, const ContextStrSID& contextSid) KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        KMP_ASSERT(pluralityForm >= PluralityFormSingular && pluralityForm < PluralityFormCount);

        return _translationCtxPluralMap[ContextedPluralSource{ sourceSidSingular, sourceSidPlural, contextSid }][pluralityForm];
    }}
    //--------------------------------------------------------------------------
}