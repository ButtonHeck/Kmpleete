#include "text_rendering_frame_listener.h"

#include "Kmpleete/Application/application_context.h"
#include "Kmpleete/Localization/localization_manager.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Localization/localization_unicode_map.h"
#include "Kmpleete/Utils/function_utils.h"
#include "Kmpleete/Utils/string_utils.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Graphics/font.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_graphics_backend.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_logical_device.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_renderer.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmpleete/Graphics/Vulkan/Command/vulkan_command_pool.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmpleete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmpleete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmpleete/Graphics/Vulkan/Utils/presets.h"
#include "Kmpleete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/named_bool.h"
#include "Kmpleete/ImGui/helper_functions.h"
#include "Kmpleete/ImGui/scope_guards.h"
#include "Kmpleete/ImGui/context_vulkan.h"
#include "Kmpleete/ImGui/implementation_glfw_vulkan.h"
#include "Kmpleete/Assets/assets_manager.h"
#include "Kmpleete/Assets/font_asset_manager.h"
#include "Kmpleete/Assets/font_asset.h"


namespace Kmpleete
{
    static constexpr auto FontDSLayout_SID = "FontAtlasLayout_DS"_sid;
    static constexpr auto FontDS_SID = "FontAtlas_Set"_sid;

    static constexpr auto PipelineLayout_SID = "PipelineLayout"_sid;
    static constexpr auto Pipeline_SID = "TextRendering_Pipeline"_sid;

    static constexpr auto VertexShader_SID = "TextRendering_vertex"_sid;
    static constexpr auto FragmentShader_SID = "TextRendering_fragment"_sid;

    static constexpr auto TextureFontAtlas_SID = "texture_font_atlas"_sid;

    static constexpr auto SamplerBindingIndex = 0;
    static constexpr auto TextureBindingIndex = 1;

    static constexpr auto VertexBufferBinding = 0;

    static constexpr auto VertexPositionAttributeIndex = 0;
    static constexpr auto VertexUVAttributeIndex = 1;

    static constexpr auto MS_ColorAttachment = "color_attachment_ms"_sid;
    static constexpr auto MS_DepthStencilAttachment = "depth_attachment_ms"_sid;

    static constexpr auto VertexBuffer_SID = "vertex_buffer"_sid;
    static constexpr auto UniformBuffers_SID = "uniform_buffers"_sid;

    static constexpr auto TranslationDomain = ToStringID(KMP_TR_DOMAIN_TEXT_RENDERING);


    using namespace Graphics::VKBits;


    TextRenderingFrameListener::TextRenderingFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow, Graphics::GraphicsBackend& graphicsBackend, 
                                                           Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _assetsManager(assetsManager)
        , _localizationManager(localizationManager)
        , _imguiImpl(nullptr)
        , _verticesCount(0)
        , _windowContentScaleHandler(_eventDispatcher, KMP_BIND(TextRenderingFrameListener::_OnWindowContentScaleEvent))
        , _windowFramebufferResizeHandler(_eventDispatcher, KMP_BIND(TextRenderingFrameListener::_OnWindowFramebufferResizeEvent))
        , _localeChangeHandler(_eventDispatcher, KMP_BIND(TextRenderingFrameListener::_OnLocaleChangeEvent))
    {
        _FillDictionary();
        _Initialize();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_Initialize()
    {
        const auto& vulkanPhysicalDevice = dynamic_cast<const Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
        auto& vulkanDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetLogicalDevice());

        _UpdateTextBuffer(true);
        _InitializeUniformBuffers(vulkanDevice);
        _InitializePipeline(vulkanDevice, vulkanPhysicalDevice.GetVulkanContext());
        _InitializeImGui();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeUniformBuffers(Graphics::VulkanLogicalDevice& vulkanDevice)
    {
        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        const auto currentLocaleSid = _localizationManager.GetLocaleSID();
        const auto& defaultFontAtlasTexture = defaultFontAsset.GetAtlasTexture(currentLocaleSid);
        auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();
        const auto& samplersStorage = vulkanDevice.GetSamplersStorage();

        VkDescriptorSetLayoutBinding samplerLayoutBinding{ SamplerBindingIndex, VK_DescriptorType_Sampler, 1, VK_ShaderStage_Fragment };
        VkDescriptorSetLayoutBinding textureLayoutBinding{ TextureBindingIndex, VK_DescriptorType_SampledImage, 1, VK_ShaderStage_Fragment };
        const auto fontRenderingLayout = descriptorSetManager.AddDescriptorSetLayout(FontDSLayout_SID, { samplerLayoutBinding, textureLayoutBinding });
        descriptorSetManager.AllocateDescriptorSets(fontRenderingLayout, FontDS_SID, 1, "per frame"_true);

        for (auto i = 0; i < Graphics::NumConcurrentFrames; i++)
        {
            descriptorSetManager.SetSamplerDescriptor(FontDS_SID, 0, "per frame"_true, i, samplersStorage.GetSampler(Graphics::SamplerDefaultLinearSid), SamplerBindingIndex);
            descriptorSetManager.SetSampledImageDescriptor(
                FontDS_SID, 0, "per frame"_true, i,
                dynamic_cast<const Graphics::VulkanTexture&>(defaultFontAtlasTexture.value().get()).GetVkImageView(), TextureBindingIndex
            );
        }
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializePipeline(Graphics::VulkanLogicalDevice& vulkanDevice, const Graphics::VulkanContext& vulkanContext)
    {
        auto& textureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        textureAttachmentManager.AddTextureColorAttachment(MS_ColorAttachment, vulkanContext.surfaceFormatLinear.format, VK_ImageUsage_TransientAttachment);
        textureAttachmentManager.AddTextureDepthStencilAttachment(MS_DepthStencilAttachment, vulkanContext.defaultDepthFormat);

        auto& pipelineManager = vulkanDevice.GetPipelineManager();
        pipelineManager.AddPipelineLayoutWithSetsSids(PipelineLayout_SID, { FontDSLayout_SID });

        auto& shaderManager = vulkanDevice.GetShaderManager();
        const auto vertexShaderPath = Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/text_rendering.vert.spv");
        const auto fragmentShaderPath = Filepath(KMP_SANDBOX_RESOURCES_FOLDER).append("spv/text_rendering.frag.spv");
        shaderManager.AddShaderModules({
            { VertexShader_SID, vertexShaderPath, Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Vertex },
            { FragmentShader_SID, fragmentShaderPath, Graphics::ShaderSourceType::BinaryFile, ShaderCompiler::ShaderType::Fragment }
        });
        const auto shaderStages = shaderManager.GetShaderStageCreateInfos({
            { VertexShader_SID, VK_ShaderStage_Vertex, "main" },
            { FragmentShader_SID, VK_ShaderStage_Fragment, "main" }
        });

        auto pipelineParams = Graphics::VulkanGraphicsPipelineParameters();
        pipelineParams.SetRenderingDepthStencilFormats(vulkanContext.defaultDepthFormat, vulkanContext.defaultDepthFormat);
        pipelineParams.AddColorAttachmentInfo(vulkanContext.surfaceFormatLinear.format, Graphics::VKPresets::ColorBlendAttachmentState_AlphaBlending);
        pipelineParams.AddShaderStages(shaderStages);
        pipelineParams.AddVertexBufferAttributesBindings(*vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID), VertexBufferBinding);
        pipelineParams.AddDynamicStates({ VK_Dynamic_Viewport, VK_Dynamic_Scissor, VK_Dynamic_RasterizationSamples });

        pipelineManager.AddGraphicsPipeline(Pipeline_SID, PipelineLayout_SID, pipelineParams, ApplicationContext::GetApplicationDataPath() / "text_rendering_pipeline_cache.bin");
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_InitializeImGui()
    {
        const auto isFramebufferAutoScaled = _mainWindow.IsWaylandWindow() || _mainWindow.IsCocoaWindow();
        const auto contentScale = isFramebufferAutoScaled ? 1.0f : _mainWindow.GetContentScale();
        const auto fontDensity = _mainWindow.GetContentScale();

        ImGuiUtils::Context* context = nullptr;
        if (_graphicsBackend.GetType() == Graphics::GraphicsBackendType::Vulkan)
        {
            const auto& vulkanBackend = dynamic_cast<const Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
            const auto& physicalDevice = dynamic_cast<const Graphics::VulkanPhysicalDevice&>(_graphicsBackend.GetPhysicalDevice());
            auto& logicalDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetLogicalDevice());

            logicalDevice.GetDescriptorSetManager().AllocateAuxDescriptorPool("ImGui_Pool"_sid, 100, {
                { VK_DescriptorType_Sampler, 100 },
                { VK_DescriptorType_CombinedImageSampler, 100 },
                { VK_DescriptorType_SampledImage, 100 },
                { VK_DescriptorType_StorageImage, 100 },
                { VK_DescriptorType_UniformTexelBuffer, 100 },
                { VK_DescriptorType_StorageTexelBuffer, 100 },
                { VK_DescriptorType_UniformBuffer, 100 },
                { VK_DescriptorType_StorageBuffer, 100 },
                { VK_DescriptorType_UniformBufferDynamic, 100 },
                { VK_DescriptorType_StorageBufferDynamic, 100 },
                { VK_DescriptorType_InputAttachment, 100 }
                });

            const auto viewportEnabled = not _mainWindow.IsWaylandWindow();

            ImGui_ImplVulkan_InitInfo initInfo{};
            initInfo.Instance = vulkanBackend.GetVkInstance();
            initInfo.PhysicalDevice = physicalDevice.GetVkPhysicalDevice();
            initInfo.Device = logicalDevice.GetVkDevice();
            initInfo.QueueFamily = physicalDevice.GetVulkanContext().graphicsFamilyIndex;
            initInfo.Queue = logicalDevice.GetGraphicsQueue().GetVkQueue();
            initInfo.PipelineCache = VK_NULL_HANDLE;
            initInfo.DescriptorPool = logicalDevice.GetDescriptorSetManager().GetAuxDescriptorPool("ImGui_Pool"_sid);
            initInfo.Allocator = VK_NULL_HANDLE;
            initInfo.MinImageCount = Graphics::NumConcurrentFrames;
            initInfo.ImageCount = Graphics::NumConcurrentFrames;
            initInfo.CheckVkResultFn = nullptr;
            initInfo.UseDynamicRendering = true;
            initInfo.MSAASamples = logicalDevice.GetMultisampling();
            initInfo.PipelineRenderingCreateInfo = Graphics::VKUtils::InitVkPipelineRenderingCreateInfoKHR();
            initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
            initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &physicalDevice.GetVulkanContext().surfaceFormatLinear.format;
            initInfo.PipelineRenderingCreateInfo.depthAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            initInfo.PipelineRenderingCreateInfo.stencilAttachmentFormat = physicalDevice.GetVulkanContext().defaultDepthFormat;
            context = new ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), Graphics::GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_false, viewportEnabled, contentScale, fontDensity, initInfo);
            context->configName = "TextRendering_imgui.ini";
        }
        _imguiImpl.reset(ImGuiUtils::ImGuiImplementation::CreateImpl(context));

        const auto& defaultFontAsset = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID);
        _imguiImpl->AddFont(defaultFontAsset.GetFont().GetBuffer(), 15);
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::Render()
    {
        _RenderTexts();
        _RenderImGui();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_RenderTexts()
    {
        const auto& vulkanGraphicsBackend = dynamic_cast<const Graphics::VulkanGraphicsBackend&>(_graphicsBackend);
        const auto& vulkanDevice = vulkanGraphicsBackend.GetLogicalDevice();
        const auto& vulkanTextureAttachmentManager = vulkanDevice.GetTextureAttachmentManager();
        const auto& renderer = vulkanDevice.GetRenderer();
        const auto drawArea = VkRect2D{ VkOffset2D{.x = 0, .y = 0 }, vulkanDevice.GetCurrentExtent() };
        const auto viewport = Graphics::VKUtils::CreateViewport(_mainWindow);
        const auto& descriptorSetManager = vulkanDevice.GetDescriptorSetManager();

        renderer.SetViewport(viewport);
        renderer.SetScissor(drawArea);
        renderer.SetRasterizationSamples(vulkanDevice.GetMultisampling());
        renderer.BindGraphicsPipeline(Pipeline_SID);
        renderer.BindVertexBuffers(VertexBufferBinding, { vulkanDevice.GetBufferManager().GetVertexBuffer(VertexBuffer_SID)->GetVkBuffer() }, { 0 });

        auto colorImageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_ColorAttachment_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_ColorAttachment), colorImageBarrierParameters);

        auto imageBarrierParameters = Graphics::VKPresets::MemoryBarrierParameters_DepthStencil_PrepareWriting;
        renderer.InsertImageMemoryBarrier(vulkanTextureAttachmentManager.GetTextureAttachment(MS_DepthStencilAttachment), imageBarrierParameters);

        const auto colorAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_Color_ClearStore,
            MS_ColorAttachment, 0ULL, VK_Resolve_Average, VK_ImageLayout_AttachmentOptimal, "swapchain image for non-MSAA"_true, "use swapchain SRGB"_false
        );
        const auto depthStencilAttachmentInfo = vulkanTextureAttachmentManager.GetRenderingAttachmentInfo(
            Graphics::VKPresets::RenderingAttachmentInfo_DepthStencil_ClearStore,
            MS_DepthStencilAttachment, 0ULL, VK_Resolve_None, VK_ImageLayout_DontCare
        );

        renderer.BeginRendering(drawArea, { colorAttachmentInfo }, depthStencilAttachmentInfo);
        renderer.BindDescriptorSets(PipelineLayout_SID, 0, {
            descriptorSetManager.GetDescriptorSet(FontDS_SID, 0, "per frame"_true)
        });
        renderer.Draw(_verticesCount, 1, 0, 0);
        renderer.EndRendering();
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_RenderImGui()
    {
    }
    //--------------------------------------------------------------------------

    bool TextRenderingFrameListener::_OnWindowContentScaleEvent(Events::WindowContentScaleEvent&)
    {
        _imguiImpl.reset();
        _InitializeImGui();

        return true;
    }
    //--------------------------------------------------------------------------

    bool TextRenderingFrameListener::_OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent&)
    {
        _UpdateTextBuffer(false);

        return true;
    }
    //--------------------------------------------------------------------------

    bool TextRenderingFrameListener::_OnLocaleChangeEvent(Events::LocaleChangeEvent&)
    {
        _FillDictionary();
        return false;
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_FillDictionary()
    {
        _localizationManager.Translate(KMP_TR_DOMAIN_TEXT_RENDERING, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        _localizationManager.Translate(KMP_TR_DOMAIN_TEXT_RENDERING, "0123456789!@#$%^&*()'~`,./<>+-_=;:?");
    }
    //--------------------------------------------------------------------------

    void TextRenderingFrameListener::_UpdateTextBuffer(bool create)
    {
        auto& vulkanDevice = dynamic_cast<Graphics::VulkanLogicalDevice&>(_graphicsBackend.GetLogicalDevice());
        auto& vulkanBufferManager = vulkanDevice.GetBufferManager();
        const auto& vulkanRenderer = vulkanDevice.GetRenderer();

        const auto vertices = _GenerateTextVertices();
        const auto vertexBufferSize = UInt32(vertices.size() * sizeof(Graphics::FontCharacterVertex));

        Graphics::VulkanBuffer stagingBuffer = vulkanBufferManager.CreateBuffer(VK_BufferUsage_TransferSrc, VK_Memory_HostVisible, {{ 0, (char*)vertices.data(), vertexBufferSize }});

        Graphics::VulkanVertexBuffer* vertexBuffer = nullptr;
        if (create)
        {
            vulkanBufferManager.CreateVertexBuffer(VertexBuffer_SID, { VK_BufferUsage_TransferDst, VK_Memory_DeviceLocal, vertexBufferSize });
            vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
            vertexBuffer->AddLayout(Graphics::BufferLayout{
                Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexPositionAttributeIndex },
                Graphics::BufferElement{ Graphics::ShaderDataType::Float2, VertexUVAttributeIndex }
            });
        }
        else
        {
            vulkanBufferManager.ResizeBuffer(VertexBuffer_SID, vertexBufferSize);
            vertexBuffer = vulkanBufferManager.GetVertexBuffer(VertexBuffer_SID);
        }

        vulkanRenderer.CopyBuffers(stagingBuffer, {
            { *vertexBuffer, 0, 0, vertexBufferSize }
        }, vulkanDevice.GetGraphicsQueue());
    }
    //--------------------------------------------------------------------------

    Vector<Graphics::FontCharacterVertex> TextRenderingFrameListener::_GenerateTextVertices()
    {
        const auto& alphabet = _localizationManager.Translation(TranslationDomain, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"_sid);
        const auto wideAlphabet = Utils::NarrowToWide(Utils::Utf8ToNarrow(alphabet));

        const auto windowFramebufferSize = _mainWindow.GetFramebufferSize();
        const auto& defaultFont = _assetsManager.GetFontAssetManager().GetAsset(Assets::FontAssetManager::DefaultFontSID).GetFont();
        const auto& defaultFontAtlas = defaultFont.GetAtlas(_localizationManager.GetLocaleSID()).value().get();
        const auto vertices = defaultFontAtlas.GenerateTextVertices(wideAlphabet, 0.0f, 100.0f, 1.0f, windowFramebufferSize);
        _verticesCount = UInt32(vertices.size());

        return vertices;
    }
    //--------------------------------------------------------------------------
}