#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Core/program_options.h"
#include "Kmpleete/Core/settings_manager.h"
#include "Kmpleete/Core/system_metrics_manager.h"
#include "Kmpleete/Localization/localization_manager.h"
#include "Kmpleete/Profile/profiler_fwd.h"


namespace Kmpleete
{
    //! Parameters for Kmpleete application creation
    //! @see Application
    struct ApplicationParameters
    {
        const String applicationName;
        const Filepath settingsFilepath;
        const String defaultSettingsFileName = "Kmpleete_settings.json";
    };
    //--------------------------------------------------------------------------


    //! Base class of the Kmpleete application, which is responsible for
    //! initialization, settings saving and loading of the subsystems
    //! that do not require window creation, additionally it handles logging
    //! initialization and its settings management
    class KMP_API Application
    {
        KMP_LOG_CLASSNAME(Application)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(Application)

    public:
        explicit Application(const ApplicationParameters& parameters);
        virtual ~Application();

        virtual void Run() = 0;

    private:
        void _Initialize(const ApplicationParameters& parameters);
        void _Finalize();

        void _SaveSettings() const;
        void _LoadSettings();

        void _FillDictionary();

    protected:
        bool _running;

        UPtr<SystemMetricsManager> _systemMetricsManager;
        UPtr<LocalizationManager> _localizationManager;
        UPtr<SettingsManager> _settingsManager;
    };
    //--------------------------------------------------------------------------


    //! Client application should define this function in its code, the function
    //! itself is called from main.h header file
    KMP_NODISCARD extern UPtr<Application> CreateApplication(const ProgramOptions& programOptions);
}