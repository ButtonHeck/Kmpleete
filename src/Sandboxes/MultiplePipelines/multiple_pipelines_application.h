#pragma once

#include "multiple_pipelines_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class MultiplePipelinesApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesApplication)

    public:
        explicit MultiplePipelinesApplication(const WindowApplicationParameters& parameters);
        ~MultiplePipelinesApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MultiplePipelinesFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}