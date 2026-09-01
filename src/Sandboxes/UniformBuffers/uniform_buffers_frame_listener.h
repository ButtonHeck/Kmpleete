#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/orthographic_camera.h"
#include "Kmpleete/Event/event_handler_guard.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/Event/mouse_events.h"
#include "Kmpleete/Input/input_manager.h"
#include "Kmpleete/Utils/memory_utils.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class UniformBuffersFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(UniformBuffersFrameListener)

    public:
        UniformBuffersFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, Input::InputManager* inputManager);
        ~UniformBuffersFrameListener() = default;

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeCamera();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct CommonShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
        };
        struct InstanceShaderData
        {
            InstanceShaderData(std::size_t size, std::size_t alignment)
                : model((Math::Mat4*)Utils::AlignedAlloc(size, alignment))
            {}

            ~InstanceShaderData()
            {
                Utils::AlignedFree(model);
            }

            Math::Mat4* model = nullptr;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        CommonShaderData _commonShaderData;
        UPtr<InstanceShaderData> _instanceShaderData;
        size_t _dynamicAlignment;
        Vector<float> _rotationsAngles;
        Graphics::OrthographicCamera _camera;

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
    };
    //--------------------------------------------------------------------------
}