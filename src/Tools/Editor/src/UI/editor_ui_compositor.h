#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Window/window_commands.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/ImGui/implementation.h"
#include "Kmpleete/Core/command.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class SettingsDocument;

    namespace Assets
    {
        class AssetsManager;
    }

    namespace Time
    {
        class Timer;
    }

    namespace Input
    {
        class InputManager;
    }


    class EditorUICompositor
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(EditorUICompositor)

    public:
        EditorUICompositor(Window& mainWindow, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, 
                           const SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager);
        ~EditorUICompositor() = default;

        void ComposeMainArea(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void ComposeStatusBar(Time::Timer& metricsTimer, const ImGuiUtils::ImGuiImplementation& imguiImpl);

        KMP_NODISCARD bool OnWindowCloseEvent(Events::WindowCloseEvent& event);

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void _ComposeDefaultLayout();

        void _ComposeMenu(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void _ComposeMenuLanguage(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void _ComposeMenuFile();
        void _ComposeMenuView();
        void _ComposeMenuFileQuit();
        void _ComposeMenuViewFullscreen();
        void _ComposeMenuViewAlwaysOnTop();

        void _ComposePopups();
        void _PopupQuit();

    private:
        struct _UIComponentsState
        {
            bool metricsFractional = true;
        };

        struct _UIPopupsState
        {
            bool quit = false;
        };

    private:
        Window& _mainWindow;
        Assets::AssetsManager& _assetsManager;
        LocalizationManager& _localizationManager;
        const SystemMetricsManager& _systemMetricsManager;
        _UIComponentsState _state;
        _UIPopupsState _popups;
        bool _needCheckImguiIniFile;

    private:
        class CommandQuit : public Command
        {
        public:
            explicit CommandQuit(_UIPopupsState& popups) : _popups(popups) {}

            void Execute() override
            {
                _popups.quit = true;
            }

        private:
            _UIPopupsState& _popups;
        };
        
        CommandQuit _commandQuit;
        CommandSwitchFullscreen _commandSwitchFullscreen;
        CommandSwitchAlwaysOnTop _commandSwitchAlwaysOnTop;
    };
    //--------------------------------------------------------------------------
}