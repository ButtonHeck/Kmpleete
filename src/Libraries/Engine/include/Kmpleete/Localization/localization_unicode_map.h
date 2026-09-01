#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    namespace Localization
    {
        //TODO: comments
        class KMP_API UnicodeMap
        {
            KMP_LOG_CLASSNAME(UnicodeMap)
            KMP_DISABLE_COPY_MOVE(UnicodeMap)

        public:
            KMP_NODISCARD static bool Initialize();

            KMP_NODISCARD static const CodepointVector& GetCodepointVector(const LocaleStr& locale);

        private:
            KMP_NODISCARD static bool _InitializeEn();
            KMP_NODISCARD static bool _InitializeRu();
            
        private:
            static bool _initialized;
            static HashMap<LocaleStr, CodepointVector> _localeCodepointRanges;
        };
        //--------------------------------------------------------------------------
    }
}