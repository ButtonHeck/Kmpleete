#pragma once

#include "text_rendering_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class TextRenderingApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(TextRenderingApplication)

    public:
        explicit TextRenderingApplication(const WindowApplicationParameters& parameters);
        ~TextRenderingApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<TextRenderingFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}