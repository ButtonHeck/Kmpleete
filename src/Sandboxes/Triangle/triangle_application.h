#pragma once

#include "triangle_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class TriangleApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(TriangleApplication)

    public:
        explicit TriangleApplication(const WindowApplicationParameters& parameters);
        ~TriangleApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<TriangleFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}