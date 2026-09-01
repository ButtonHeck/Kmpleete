#pragma once

#include "instanced_rendering_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class InstancedRenderingApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(InstancedRenderingApplication)

    public:
        explicit InstancedRenderingApplication(const WindowApplicationParameters& parameters);
        ~InstancedRenderingApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<InstancedRenderingFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}