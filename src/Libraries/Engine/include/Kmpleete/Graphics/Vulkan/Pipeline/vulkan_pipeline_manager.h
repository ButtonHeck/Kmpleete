#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanDescriptorSetManager;


        //! Manager of Vulkan pipeline objects, pipeline caches and pipeline layouts.
        //! @see VulkanGraphicsPipeline
        //! @see VulkanPipelineCache
        class KMP_API VulkanPipelineManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanPipelineManager)
            KMP_LOG_CLASSNAME(VulkanPipelineManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanPipelineManager(VkDevice device, const VulkanContext& context, const VulkanDescriptorSetManager& descriptorSetManager);
            ~VulkanPipelineManager();

            bool AddPipelineLayoutWithSetsSids(StringID layoutSid, const Vector<StringID>& descriptorSetLayoutsSids, const Vector<VkPushConstantRange>& pushConstantRanges = {});
            bool AddPipelineLayout(StringID layoutSid, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, const Vector<VkPushConstantRange>& pushConstantRanges = {});
            KMP_NODISCARD VkPipelineLayout GetPipelineLayout(StringID layoutSid) const noexcept;

            bool AddPipelineCache(StringID pipelineSid, const Filepath& binaryPath);

            bool AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath);
            bool AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath);
            bool AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters);
            bool AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters);
            KMP_NODISCARD OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID pipelineSid) const;

        private:
            VkDevice _device;
            const VulkanContext& _context;
            const VulkanDescriptorSetManager& _descriptorSetManager;
            StringIDHashMap<VkPipelineLayout> _layouts;
            StringIDHashMap<UPtr<VulkanGraphicsPipeline>> _pipelines;
            StringIDHashMap<UPtr<VulkanPipelineCache>> _pipelineCaches;
        };
        //--------------------------------------------------------------------------
    }
}