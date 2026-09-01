#include "Kmpleete/Core/stacktrace.h"
#include "Kmpleete/Application/application_context.h"
#include "Kmpleete/Time/helper_functions.h"
#include "Kmpleete/Utils/string_utils.h"
#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Base/named_bool.h"
#include "Kmpleete/Log/log.h"

#include <cpptrace/cpptrace.hpp>


namespace Kmpleete
{
    void DumpStacktrace()
    {
        const auto stacktrace = cpptrace::generate_trace();
        stacktrace.print();

        auto dumpName = Utils::Concatenate(ApplicationContext::GetApplicationName(), " stacktrace ", Time::GetCurrentTimeString(), ".txt");
        dumpName = Utils::RegexReplace(dumpName, ":", "_");
        const auto dumpFile = ApplicationContext::GetApplicationLogPath() / dumpName;

        if (Filesystem::WriteFile(dumpFile, stacktrace.to_string(), "append"_false))
        {
            KMP_LOG_ERROR_FN("stacktrace written to a file '{}'", dumpFile);
        }
    }
    //--------------------------------------------------------------------------
}