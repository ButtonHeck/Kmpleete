#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
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


    class MultiplePipelinesFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesFrameListener)

    public:
        MultiplePipelinesFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend);
        ~MultiplePipelinesFrameListener() = default;

        void Render() override;

    private:
        void _Initialize();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipelines(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
    };
    //--------------------------------------------------------------------------
}