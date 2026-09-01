#pragma once

#include "texture_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    class TextureApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TextureApplication)
        KMP_DISABLE_COPY_MOVE(TextureApplication)

    public:
        explicit TextureApplication(const WindowApplicationParameters& parameters);
        ~TextureApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<TextureFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}