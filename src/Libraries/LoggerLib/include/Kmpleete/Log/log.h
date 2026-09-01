#pragma once

#if defined (KMP_CONFIG_TYPE_PRODUCTION)

#define KMP_LOG_TRACE(...)
#define KMP_LOG_DEBUG(...)
#define KMP_LOG_INFO(...)
#define KMP_LOG_WARN(...)
#define KMP_LOG_ERROR(...)
#define KMP_LOG_CRITICAL(...)

#define KMP_LOG_TRACE_FN(...)
#define KMP_LOG_DEBUG_FN(...)
#define KMP_LOG_INFO_FN(...)
#define KMP_LOG_WARN_FN(...)
#define KMP_LOG_ERROR_FN(...)
#define KMP_LOG_CRITICAL_FN(...)

#else
#include "Kmpleete/Log/log_api.h"
#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/platform.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/macro.h"

#if defined (KMP_COMPILER_MSVC)
    #pragma warning(push)
    #pragma warning(disable : 4996)
#endif
#include <spdlog/spdlog.h>
#if defined (KMP_COMPILER_MSVC)
    #pragma warning(pop)
#endif


namespace Kmpleete
{
    //! Global logging controller backed by the spdlog library, available for logging
    //! to a console or a file. Initially the logger is instantiated with a
    //! NullObject sink. Initialization is split to two parts:
    //! first by a "Boot" function that stores all messages in a temporary storage, then
    //! when an application loads its settings (including logger settings) all messages
    //! from that storage moved to sinks according to those settings.
    //! Filtering of messages is done by a levels mechanism (from 0 to 5): level 0 keeps
    //! all the messages while level 5 keeps only critical ones.
    class KMP_LOG_API Log
    {
    public:
        static constexpr auto SettingsEntryName = "Log";
        static constexpr auto FilenameStr = "Filename";
        static constexpr auto EnabledStr = "Enabled";
        static constexpr auto TruncateStr = "Truncate";
        static constexpr auto OutputConsoleStr = "OutputConsole";
        static constexpr auto OutputFileStr = "OutputFile";
        static constexpr auto LevelStr = "Level";
        static constexpr auto LevelFlushStr = "LevelFlush";

        struct LogSettings
        {
            String filename = "Kmpleete_log.txt";
            bool enabled = true;
            bool truncate = false;
            bool outputConsole = true;
            bool outputFile = true;
            int level = spdlog::level::trace;
            int levelFlush = spdlog::level::trace;
        };

    public:
        static void Boot(const String& programName);
        static void Initialize(const String& programName, const Filepath& logsDirectory);
        static void Finalize();

        static void SetSettings(const LogSettings& settings);
        static const LogSettings& GetSettings();

        template <typename... Args>
        static void Trace(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->trace(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->debug(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->info(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->warn(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->error(fmt, std::forward<Args>(args)...); }

        template <typename... Args>
        static void Critical(spdlog::format_string_t<Args...> fmt, Args&&... args) { _logger->critical(fmt, std::forward<Args>(args)...); }

    private:
        static Ptr<spdlog::logger> _logger;
    };
    //--------------------------------------------------------------------------
}

//! Utility struct that enables logging Filepath without explicitly converting it to a string
template<>
struct fmt::formatter<Kmpleete::Filepath> : fmt::formatter<Kmpleete::String>
{
    template <typename FormatContext>
    auto format(const Kmpleete::Filepath& filepath, FormatContext& ctx)
    {
        return fmt::formatter<Kmpleete::String>::format(filepath.generic_u8string(), ctx);
    }
};
//--------------------------------------------------------------------------


//! Two sets of logging macros for all levels: the first set is used for a class functions (with class name printed first),
//! while the second one is used for any other parts of code
//! @see log_class_macro.h

#define KMP_LOG_TRACE(...)          ::Kmpleete::Log::Trace("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_DEBUG(...)          ::Kmpleete::Log::Debug("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_INFO(...)           ::Kmpleete::Log::Info("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_WARN(...)           ::Kmpleete::Log::Warn("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_ERROR(...)          ::Kmpleete::Log::Error("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__))
#define KMP_LOG_CRITICAL(...)       ::Kmpleete::Log::Critical("{}: {}", GetLogClassName(), fmt::format(__VA_ARGS__)); KMP_DEBUGBREAK

#define KMP_LOG_TRACE_FN(...)       ::Kmpleete::Log::Trace(__VA_ARGS__)
#define KMP_LOG_DEBUG_FN(...)       ::Kmpleete::Log::Debug(__VA_ARGS__)
#define KMP_LOG_INFO_FN(...)        ::Kmpleete::Log::Info(__VA_ARGS__)
#define KMP_LOG_WARN_FN(...)        ::Kmpleete::Log::Warn(__VA_ARGS__)
#define KMP_LOG_ERROR_FN(...)       ::Kmpleete::Log::Error(__VA_ARGS__)
#define KMP_LOG_CRITICAL_FN(...)    ::Kmpleete::Log::Critical(__VA_ARGS__); KMP_DEBUGBREAK

#endif
