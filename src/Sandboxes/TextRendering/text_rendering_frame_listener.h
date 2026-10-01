#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Application/frame_listener.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/font_character.h"
#include "Kmpleete/ImGui/implementation.h"
#include "Kmpleete/Event/event_handler_guard.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/Event/application_events.h"


namespace Kmpleete
{
    class LocalizationManager;

    namespace Assets
    {
        class AssetsManager;
    }

    namespace Graphics
    {
        class VulkanLogicalDevice;
        struct VulkanContext;
    }


    class TextRenderingFrameListener : public FrameListener
    {
        KMP_DISABLE_COPY_MOVE(TextRenderingFrameListener)

    public:
        TextRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend,
                                   Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager);
        ~TextRenderingFrameListener() = default;

        void Render() override;

    private:
        void _Initialize();
        void _InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice);
        void _InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext);
        void _InitializeImGui();

        void _RenderTexts();
        void _RenderImGui();

        bool _OnWindowContentScaleEvent(Events::WindowContentScaleEvent& event);
        bool _OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent& event);
        bool _OnLocaleChangeEvent(Events::LocaleChangeEvent& event);

        void _FillDictionary();

        void _UpdateTextBuffer(bool create);
        Vector<Graphics::FontCharacterVertex> _GenerateTextVertices();

    private:
        Window& _mainWindow;
        Graphics::GraphicsBackend& _graphicsBackend;
        Assets::AssetsManager& _assetsManager;
        LocalizationManager& _localizationManager;
        UPtr<ImGuiUtils::ImGuiImplementation> _imguiImpl;
        UInt32 _verticesCount;

        Events::EventHandlerGuard<Events::WindowContentScaleEvent> _windowContentScaleHandler;
        Events::EventHandlerGuard<Events::WindowFramebufferResizeEvent> _windowFramebufferResizeHandler;
        Events::EventHandlerGuard<Events::LocaleChangeEvent> _localeChangeHandler;
    };
    //--------------------------------------------------------------------------
}