#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/orthographic_camera.h"
#include "Kmpleete/Graphics/perspective_camera.h"
#include "Kmpleete/Graphics/Vulkan/Texture/vulkan_texture.h"
#include "Kmpleete/ImGui/implementation.h"
#include "Kmpleete/Event/event_handler_guard.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/Event/mouse_events.h"
#include "Kmpleete/Input/input_manager.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Assets
    {
        class AssetsManager;
    }

    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


#define USE_ORTHOGRAPHIC_CAMERA false

    class TextureFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(TextureFrameListener)

    public:
        TextureFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, 
                             Assets::AssetsManager& assetsManager, Input::InputManager* inputManager);
        ~TextureFrameListener() = default;

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        void _Initialize();
        void _InitializeCamera();
        void _InitializeBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializeImGui();

        void _RenderTextureQuad();
        void _RenderImGui();

        bool _OnWindowResizeEvent(Events::WindowResizeEvent& evt);
        bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);
        bool _OnMouseScrollEvent(Events::MouseScrollEvent& evt);

    private:
        struct MatrixShaderData
        {
            Math::Mat4 projectionMatrix;
            Math::Mat4 viewMatrix;
            Math::Mat4 modelMatrix;
            float lodBias = 0.0f;
            int tiling = 1;
        };

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Input::InputManager* _inputManager;

        UInt32 _indexCount;
        Assets::AssetsManager& _assetsManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        MatrixShaderData _matrixShaderData;

#if USE_ORTHOGRAPHIC_CAMERA
        Graphics::OrthographicCamera _camera;
#else
        Graphics::PerspectiveCamera _camera;
#endif

        Events::EventHandlerGuard<Events::WindowResizeEvent> _windowResizeHandler;
        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
        Events::EventHandlerGuard<Events::MouseScrollEvent> _mouseScrollHandler;
    };
    //--------------------------------------------------------------------------
}