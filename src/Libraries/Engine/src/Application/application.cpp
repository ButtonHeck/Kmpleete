#include "Kmpleete/Application/application.h"
#include "Kmpleete/Application/application_context.h"
#include "Kmpleete/Localization/localization_unicode_map.h"
#include "Kmpleete/Log/log.h"
#include "Kmpleete/Core/settings_document.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Utils/function_utils.h"
#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    static constexpr auto SettingsEntryName = "Application";


    Application::Application(const ApplicationParameters& parameters)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _running(false)
        , _systemMetricsManager(nullptr)
        , _localizationManager(nullptr)
        , _settingsManager(nullptr)
    {
        _Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    Application::~Application() KMP_PROFILING(ProfileLevelAlways)
    {
        _Finalize();
    }}
    //--------------------------------------------------------------------------

    void Application::_Initialize(const ApplicationParameters& parameters)
    {
        ApplicationContext::_Initialize(parameters);

        const auto& applicationPath = ApplicationContext::GetApplicationPath();
        const auto& applicationDataPath = ApplicationContext::GetApplicationDataPath();

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger boot", ProfileLevelAlways);
            Log::Boot(ApplicationContext::GetApplicationName());
        }
#endif

        if (not Filesystem::FilepathExists(applicationPath))
        {
            throw RuntimeError("Application filepath initialization failed");
        }
        if (not Filesystem::FilepathExists(applicationDataPath))
        {
            throw RuntimeError("Application data filepath initialization failed");
        }

        _systemMetricsManager = CreateUPtr<SystemMetricsManager>();
        KMP_ASSERT(_systemMetricsManager);

        const auto messagesPath = Filesystem::ToGenericU8String(applicationPath / LocalesDirectory);
        KMP_ASSERT(messagesPath != LocalesDirectory);
        _localizationManager = CreateUPtr<LocalizationManager>(messagesPath);
        KMP_ASSERT(_localizationManager);

        KMP_MB_UNUSED const auto unicodeMapsInitialized = Localization::UnicodeMap::Initialize();
        KMP_ASSERT(unicodeMapsInitialized);

        _settingsManager = CreateUPtr<SettingsManager>(parameters.settingsFilepath.empty()
            ? applicationPath / parameters.defaultSettingsFileName
            : parameters.settingsFilepath);
        KMP_ASSERT(_settingsManager);

        _LoadSettings();

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger initialization", ProfileLevelAlways);
            Log::Initialize(ApplicationContext::GetApplicationName(), ApplicationContext::GetApplicationLogPath());
        }
#endif
    }
    //--------------------------------------------------------------------------

    void Application::_Finalize()
    {
        KMP_ASSERT(_settingsManager && _localizationManager && _systemMetricsManager);

        _SaveSettings();

        _settingsManager.reset();
        _localizationManager.reset();
        _systemMetricsManager.reset();

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger finalization", ProfileLevelAlways);
            Log::Finalize();
        }
#endif
    }
    //--------------------------------------------------------------------------

    void Application::_SaveSettings() const KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(_settingsManager && _localizationManager);

        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (not settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        const auto& logSettings = Log::GetSettings();
        settings->get().StartSaveObject(Log::SettingsEntryName);
        settings->get().SaveString(Log::FilenameStr, Utils::NarrowToUtf8(logSettings.filename));
        settings->get().SaveBool(Log::EnabledStr, logSettings.enabled);
        settings->get().SaveBool(Log::TruncateStr, logSettings.truncate);
        settings->get().SaveBool(Log::OutputConsoleStr, logSettings.outputConsole);
        settings->get().SaveBool(Log::OutputFileStr, logSettings.outputFile);
        settings->get().SaveInt(Log::LevelStr, logSettings.level);
        settings->get().SaveInt(Log::LevelFlushStr, logSettings.levelFlush);
        settings->get().EndSaveObject();
#endif

        _localizationManager->SaveSettings(*settings);

        _settingsManager->SaveSettings();
    }}
    //--------------------------------------------------------------------------

    void Application::_LoadSettings() KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(_settingsManager && _localizationManager);

        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (not settings)
        {
            KMP_LOG_WARN("failed to get settings entry for loading");
            return;
        }

#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        settings->get().StartLoadObject(Log::SettingsEntryName);

        Log::LogSettings logSettings;
        logSettings.filename = Utils::Utf8ToNarrow(settings->get().GetString(Log::FilenameStr, "Kmpleete_log.txt"));
        logSettings.enabled = settings->get().GetBool(Log::EnabledStr, true);
        logSettings.truncate = settings->get().GetBool(Log::TruncateStr, false);
        logSettings.outputConsole = settings->get().GetBool(Log::OutputConsoleStr, true);
        logSettings.outputFile = settings->get().GetBool(Log::OutputFileStr, true);
        logSettings.level = settings->get().GetInt(Log::LevelStr, spdlog::level::trace);
        logSettings.levelFlush = settings->get().GetInt(Log::LevelFlushStr, spdlog::level::trace);
        Log::SetSettings(logSettings);

        settings->get().EndLoadObject();
#endif

        _localizationManager->LoadSettings(*settings);
    }}
    //--------------------------------------------------------------------------
}