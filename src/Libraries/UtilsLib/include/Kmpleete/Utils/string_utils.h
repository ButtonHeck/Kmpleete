#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Profile/profiler.h"

#include <sstream>


namespace Kmpleete
{
    //! Helper functions for common string operations
    namespace Utils
    {
        template<typename... Ts>
        KMP_NODISCARD String Concatenate(Ts&&... args) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            std::ostringstream oss;
            (oss << ... << std::forward<Ts>(args));
            return oss.str();
        }}
        //--------------------------------------------------------------------------

        template<typename... Ts>
        std::ostringstream& ToSStream(std::ostringstream& oss, Ts&&... args) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            (oss << ... << std::forward<Ts>(args));
            return oss;
        }}
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API String RegexReplace(const String& source, const char* regexp, const char* replacement);

        KMP_NODISCARD KMP_API String StringVectorToString(const StringVector& stringVector, char delimiter, bool startWithDelimiter = true);
        KMP_NODISCARD KMP_API String StringVectorToString(const StringVector& stringVector, const String& delimiter, bool startWithDelimiter = true);

        KMP_NODISCARD KMP_API WString NarrowToWide(const String& str);
        KMP_NODISCARD KMP_API String WideToNarrow(const WString& wstr);
        KMP_NODISCARD KMP_API String NarrowToUtf8(const String& str);
        KMP_NODISCARD KMP_API String Utf8ToNarrow(const String& str);
        KMP_NODISCARD KMP_API Filepath::string_type NarrowToFilepath(const String& str);

        KMP_NODISCARD KMP_API String PrettifyFunctionName(const String& rawName);
    }
}