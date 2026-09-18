#pragma once

#include "Kmpleete/Graphics/graphics_base.h"
#include "Kmpleete/Graphics/logical_device.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_swapchain.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_fence.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_queue.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_renderer.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_samplers_storage.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_metrics_manager.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmpleete/Graphics/Vulkan/Buffer/vulkan_buffer_manager.h"
#include "Kmpleete/Graphics/Vulkan/Texture/vulkan_texture.h"
#include "Kmpleete/Graphics/Vulkan/Texture/vulkan_texture_attachment_manager.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h"
#include "Kmpleete/Graphics/Vulkan/Shader/vulkan_shader_manager.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    class Window;


    namespace Graphics
    {
        class Image;
        struct VulkanGraphicsParameters;


        //! Vulkan API logical device wrapper object. Additionally represents the storage for every other Vulkan related
        //! objects that somehow depends on logical device.
        class KMP_API VulkanLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanLogicalDevice)
            KMP_LOG_CLASSNAME(VulkanLogicalDevice)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanLogicalDevice(GraphicsChainHandler& chainHandler, const VulkanPhysicalDevice& physicalDevice,
                                VkSurfaceKHR surface, const Window& window, const UInt32& currentBufferIndex);
            ~VulkanLogicalDevice();

            void RecreateResources() override;
            void WaitIdle() const;

            KMP_NODISCARD VkSampleCountFlagBits GetMultisampling() const noexcept;
            void SetMultisampling(VkSampleCountFlagBits samples);

            KMP_NODISCARD bool IsVSync() const noexcept;
            void SetVSync(bool vSync);

            KMP_NODISCARD const VulkanSwapchain& GetSwapchain() const noexcept override;
            KMP_NODISCARD VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD const VulkanQueue& GetGraphicsQueue() const noexcept;
            KMP_NODISCARD const VulkanQueue& GetPresentationQueue() const noexcept;
            KMP_NODISCARD const VulkanImageCreatorDelegate& GetVulkanImageCreatorDelegate() const noexcept;
            KMP_NODISCARD const VulkanRenderer& GetRenderer() const noexcept;
            KMP_NODISCARD const VkExtent2D& GetCurrentExtent() const noexcept;
            KMP_NODISCARD const VulkanSamplersStorage& GetSamplersStorage() const noexcept;
            KMP_NODISCARD const VulkanDescriptorSetManager& GetDescriptorSetManager() const noexcept;
            KMP_NODISCARD VulkanDescriptorSetManager& GetDescriptorSetManager() noexcept;
            KMP_NODISCARD const VulkanPipelineManager& GetPipelineManager() const noexcept;
            KMP_NODISCARD VulkanPipelineManager& GetPipelineManager() noexcept;
            KMP_NODISCARD const VulkanTextureAttachmentManager& GetTextureAttachmentManager() const noexcept;
            KMP_NODISCARD VulkanTextureAttachmentManager& GetTextureAttachmentManager() noexcept;
            KMP_NODISCARD const VulkanShaderManager& GetShaderManager() const noexcept;
            KMP_NODISCARD VulkanShaderManager& GetShaderManager() noexcept;
            KMP_NODISCARD const VulkanBufferManager& GetBufferManager() const noexcept;
            KMP_NODISCARD VulkanBufferManager& GetBufferManager() noexcept;
            KMP_NODISCARD const VulkanMetricsManager& GetMetricsManager() const noexcept;
            KMP_NODISCARD VulkanMetricsManager& GetMetricsManager() noexcept;

            KMP_NODISCARD Nullable<VulkanTexture*> CreateTexture(const Image& image, Assets::TextureSubTypeMaskBits subTypeMask) const override;

        private:
            void _CreateLogicalDeviceObject();
            void _DeleteLogicalDeviceObject();

            void _CreateDeviceQueues();
            void _DeleteDeviceQueues();

            void _CreateImageCreatorDelegate();
            void _DeleteImageCreatorDelegate();

            void _CreateSynchronizationObjects();
            void _DeleteSyncronizationObjects();

            void _CreateSwapchain();
            void _DeleteSwapchain();

            void _CreateDescriptorSetManager();
            void _DeleteDescriptorSetManager();

            void _CreateBufferManager();
            void _DeleteBufferManager();

            void _CreateSamplersStorage();
            void _DeleteSamplersStorage();

            void _CreatePipelineManager();
            void _DeletePipelineManager();

            void _CreateTextureAttachmentManager();
            void _DeleteTextureAttachmentManager();

            void _CreateShaderManager();
            void _DeleteShaderManager();

            void _CreateRenderer();
            void _DeleteRenderer();

            void _CreateMetricsManager();
            void _DeleteMetricsManager();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;
            void _RecreateSwapchain();

            KMP_NODISCARD bool _StartFrame(float frameTimestep) override;
            void _EndFrame() override;

        private:
            const VulkanContext& _vulkanContext;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;
            const VulkanFormatDelegate& _formatDelegate;
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkPhysicalDevice _physicalDevice;
            VkSurfaceKHR _surface;
            UPtr<VulkanGraphicsParameters> _graphicsParameters;

            VkDevice _device;
            UPtr<VulkanQueue> _graphicsQueue;
            UPtr<VulkanQueue> _presentQueue;
            UPtr<VulkanImageCreatorDelegate> _imageCreatorDelegate;
            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
            Vector<VulkanFence> _waitFences;
            UPtr<VulkanSwapchain> _swapchain;
            UPtr<VulkanDescriptorSetManager> _descriptorSetManager;
            UPtr<VulkanBufferManager> _bufferManager;
            VkExtent2D _currentExtent;
            VkSampleCountFlagBits _msaaSamples;
            bool _vSync;
            UPtr<VulkanSamplersStorage> _samplersStorage;
            UPtr<VulkanPipelineManager> _pipelineManager;
            UPtr<VulkanTextureAttachmentManager> _textureAttachmentManager;
            UPtr<VulkanShaderManager> _shaderManager;
            UPtr<VulkanRenderer> _renderer;
            UPtr<VulkanMetricsManager> _metricsManager;
        };
        //--------------------------------------------------------------------------
    }
}