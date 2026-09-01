#pragma once

#include "push_constants_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class PushConstantsApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(PushConstantsApplication)

    public:
        explicit PushConstantsApplication(const WindowApplicationParameters& parameters);
        ~PushConstantsApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<PushConstantsFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}