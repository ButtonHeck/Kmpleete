#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Graphics/graphics_backend.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class InstancedRenderingFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(InstancedRenderingFrameListener)

    public:
        InstancedRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~InstancedRenderingFrameListener() = default;

        void Render() override;

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;

        UInt32 _indexCount;
    };
    //--------------------------------------------------------------------------
}