#pragma once

#include "UI/editor_ui_compositor.h"

#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Core/settings_document.h"
#include "Kmpleete/Time/timer.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/Event/application_events.h"
#include "Kmpleete/Event/event_handler_guard.h"
#include "Kmpleete/ImGui/implementation.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    class LocalizationManager;
    class SystemMetricsManager;

    namespace Graphics
    {
        class GraphicsBackend;
    }

    namespace Assets
    {
        class AssetsManager;
    }

    namespace Input
    {
        class InputManager;
    }


    class EditorFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(EditorFrameListener)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(EditorFrameListener)

    public:
        EditorFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Assets::AssetsManager& assetsManager,
                            LocalizationManager& localizationManager, SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager);
        ~EditorFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void _Initialize(Input::InputManager& inputManager);
        void _InitializeGraphics();
        void _InitializeImGui();
        void _Finalize();

        void _NewFrame();
        void _BeginApplicationArea() const;
        void _BeginMainWorkingArea() const;
        void _ComposeMainArea();
        void _EndMainWorkingArea() const;
        void _BeginStatusBarArea() const;
        void _ComposeStatusBar();
        void _EndStatusBarArea() const;
        void _EndApplicationArea() const;
        void _EndFrame() const;

        KMP_NODISCARD bool _OnWindowCloseEvent(Events::WindowCloseEvent& event);
        KMP_NODISCARD bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);
        KMP_NODISCARD bool _OnWindowScreenModeEvent(Events::WindowScreenModeEvent& event);
        bool _OnLocaleChangeEvent(Events::LocaleChangeEvent& event);

        void _AddImGuiFonts();
        void _RenderImGui();
        void _FillDictionary();

    private:
        SystemMetricsManager& _systemMetricsManager;
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        LocalizationManager& _localizationManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        UPtr<EditorUICompositor> _uiCompositor;
        Time::Timer _metricsTimer;

        Events::EventHandlerGuard<Events::WindowCloseEvent> _windowCloseHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
        Events::EventHandlerGuard<Events::WindowScreenModeEvent> _windowScreenModeHandler;
        Events::EventHandlerGuard<Events::LocaleChangeEvent> _localeChangeHandler;
    };
    //--------------------------------------------------------------------------
}