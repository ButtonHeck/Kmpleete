#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanVertexBuffer;


        //! Simple Vulkan API graphics pipeline wrapper
        //! @see VulkanGraphicsPipelineParameters
        class KMP_API VulkanGraphicsPipeline
        {
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsPipeline)
            KMP_LOG_CLASSNAME(VulkanGraphicsPipeline)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanGraphicsPipeline(VkDevice device, StringID sid, VkPipelineLayout layout, VkPipelineCache cache, const VulkanGraphicsPipelineParameters& parameters);
            ~VulkanGraphicsPipeline();

            KMP_NODISCARD VkPipeline GetVkPipeline() const noexcept;
            KMP_NODISCARD UInt32 GetColorAttachmentsCount() const noexcept;

        private:
            void _Initialize(VkPipelineLayout layout, VkPipelineCache cache);
            void _Finalize();

        private:
            VkDevice _device;
            const StringID _sid;
            
            VkPipeline _pipeline;
            VulkanGraphicsPipelineParameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}