#pragma once

#include "Application/editor_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    class EditorApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(EditorApplication)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(EditorApplication)

    public:
        explicit EditorApplication(const WindowApplicationParameters& parameters);
        ~EditorApplication();

    private:
        void _Initialize();
        void _Finalize();

        void _SaveSettings() const;
        void _LoadSettings();

    private:
        Window& _mainWindow;
        UPtr<EditorFrameListener> _uiFrameListener;
    };
    //--------------------------------------------------------------------------
}