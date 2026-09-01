#include "Kmpleete/Time/helper_functions.h"
#include "Kmpleete/Base/platform.h"

#include <chrono>
#include <iomanip>
#include <sstream>


namespace Kmpleete
{
    namespace Time
    {
        String GetCurrentTimeString(const char* format /*= "%Y-%m-%d %T"*/)
        {
            if (format == nullptr)
            {
                return String();
            }

            const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::stringstream timeStream;

#if defined (KMP_COMPILER_MSVC)
            struct tm buf{};
            localtime_s(&buf, &now);
            timeStream << std::put_time(&buf, format);
#else
            timeStream << std::put_time(localtime(&now), format);
#endif

            return timeStream.str();
        }
        //--------------------------------------------------------------------------
    }
}