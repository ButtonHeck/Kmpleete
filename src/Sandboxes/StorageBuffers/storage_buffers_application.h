#pragma once

#include "storage_buffers_frame_listener.h"

#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Application/window_application.h"


namespace Kmpleete
{
    class StorageBuffersApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(StorageBuffersApplication)

    public:
        explicit StorageBuffersApplication(const WindowApplicationParameters& parameters);
        ~StorageBuffersApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<StorageBuffersFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}