#pragma once

#include "uniform_buffers_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class UniformBuffersApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(UniformBuffersApplication)

    public:
        explicit UniformBuffersApplication(const WindowApplicationParameters& parameters);
        ~UniformBuffersApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<UniformBuffersFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}