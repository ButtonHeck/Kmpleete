#include "Kmpleete/Window/window_backend.h"
#include "Kmpleete/Window/window_backend_glfw.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    UPtr<WindowBackend> WindowBackend::Create(Graphics::GraphicsBackendType graphicsBackendType) KMP_PROFILING(ProfileLevelAlways)
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        return CreateUPtr<WindowBackendGlfw>(graphicsBackendType);
#else
    #error "No window backend is provided!"
#endif
    }}
    //--------------------------------------------------------------------------

    WindowBackend::WindowBackend(Graphics::GraphicsBackendType graphicsBackendType) noexcept
        : _graphicsBackendType(graphicsBackendType)
        , _nativePlatformType(WindowNativePlatformType::Undefined)
    {}
    //--------------------------------------------------------------------------

    WindowNativePlatformType WindowBackend::GetNativePlatformType() const noexcept
    {
        return _nativePlatformType;
    }
    //--------------------------------------------------------------------------
}
