#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Log/log.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        UPtr<GraphicsBackend> GraphicsBackend::Create(Window& window) KMP_PROFILING(ProfileLevelAlways)
        {
            const auto graphicsBackendType = window.GetGraphicsBackendType();

            switch (graphicsBackendType)
            {
            case GraphicsBackendType::Vulkan:
                return CreateUPtr<VulkanGraphicsBackend>(window);
            default:
                KMP_LOG_ERROR("cannot create graphics backend instance for graphics backend '{}'", GraphicsBackendTypeToString(graphicsBackendType));
                return nullptr;
            }
        }}
        //--------------------------------------------------------------------------

        GraphicsBackend::GraphicsBackend(Window& window)
            : _window(window)
            , _chainHandler(CreateUPtr<GraphicsChainHandler>())
        {}
        //--------------------------------------------------------------------------

        GraphicsBackendType GraphicsBackend::GetType() const noexcept
        {
            return _window.GetGraphicsBackendType();
        }
        //--------------------------------------------------------------------------

        Nullable<Texture*> GraphicsBackend::CreateTexture(const Filepath& filepath, Assets::TextureSubTypeMaskBits subTypeMask, bool flipVertically /*= false*/) KMP_PROFILING(ProfileLevelAlways)
        {
            try
            {
                Image image(filepath, flipVertically);
                return CreateTexture(image, subTypeMask);
            }
            catch (const Exception&)
            {
                KMP_LOG_ERROR("failed to create texture '{}'", filepath);
            }

            return nullptr;
        }}
        //--------------------------------------------------------------------------
    }
}