#include "Kmpleete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_descriptor_set_manager.h"
#include "Kmpleete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmpleete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    namespace Graphics
    {
        VulkanPipelineManager::VulkanPipelineManager(VkDevice device, const VulkanContext& context, const VulkanDescriptorSetManager& descriptorSetManager)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _context(context)
            , _descriptorSetManager(descriptorSetManager)
            , _pipelines()
            , _pipelineCaches()
        {
            KMP_ASSERT(_device);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanPipelineManager::~VulkanPipelineManager() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            _pipelineCaches.clear();
            _pipelines.clear();

            for (const auto& [sid, layout] : _layouts)
            {
                vkDestroyPipelineLayout(_device, layout, nullptr);
            }
            _layouts.clear();
        }}
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddPipelineLayoutWithSetsSids(StringID layoutSid, const Vector<StringID>& descriptorSetLayoutsSids, const Vector<VkPushConstantRange>& pushConstantRanges /*= {}*/) KMP_PROFILING(ProfileLevelImportant)
        {
            return AddPipelineLayout(layoutSid, _descriptorSetManager.GetDescriptorSetLayouts(descriptorSetLayoutsSids), pushConstantRanges);
        }}
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddPipelineLayout(StringID layoutSid, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, const Vector<VkPushConstantRange>& pushConstantRanges /*= {}*/) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_layouts.contains(layoutSid))
            {
                KMP_LOG_WARN("pipeline layout with sid '{}' has already been added", layoutSid);
                return true;
            }

            VkPipelineLayoutCreateInfo layoutCreateInfo = VKUtils::InitVkPipelineLayoutCreateInfo();
            if (not descriptorSetLayouts.empty())
            {
                layoutCreateInfo.setLayoutCount = UInt32(descriptorSetLayouts.size());
                layoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
            }

            if (not pushConstantRanges.empty())
            {
                layoutCreateInfo.pushConstantRangeCount = UInt32(pushConstantRanges.size());
                layoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
            }

            VkPipelineLayout layout;
            auto result = vkCreatePipelineLayout(_device, &layoutCreateInfo, nullptr, &layout);
            VKUtils::CheckResult(result, "VulkanPipelineManager: failed to build graphics pipeline layout");

            const auto [iterator, hasEmplaced] = _layouts.emplace(layoutSid, layout);
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        VkPipelineLayout VulkanPipelineManager::GetPipelineLayout(StringID layoutSid) const noexcept
        {
            const auto layoutIter = _layouts.find(layoutSid);
            if (layoutIter != _layouts.cend())
            {
                return layoutIter->second;
            }

            KMP_LOG_ERROR("pipeline layout with sid '{}' not found", layoutSid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddPipelineCache(StringID pipelineSid, const Filepath& binaryPath) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_pipelineCaches.contains(pipelineSid))
            {
                KMP_LOG_WARN("pipeline cache with sid '{}' has already been added", pipelineSid);
                return true;
            }

            const auto [iterator, hasEmplaced] = _pipelineCaches.emplace(pipelineSid, CreateUPtr<VulkanPipelineCache>(_device, _context, binaryPath));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath)
        {
            AddPipelineCache(pipelineSid, cacheBinaryPath);
            return AddGraphicsPipeline(pipelineSid, layoutSid, parameters);
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath)
        {
            AddPipelineCache(pipelineSid, cacheBinaryPath);
            return AddGraphicsPipeline(pipelineSid, layout, parameters);
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters)
        {
            return AddGraphicsPipeline(pipelineSid, GetPipelineLayout(layoutSid), parameters);
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_pipelines.contains(pipelineSid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", pipelineSid);
                return true;
            }

            if (layout == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("cannot create pipeline with sid '{}' - pipeline layout is null", pipelineSid);
                return false;
            }

            VkPipelineCache pipelineCache = VK_NULL_HANDLE;
            const auto pipelineCacheIter = _pipelineCaches.find(pipelineSid);
            if (pipelineCacheIter != _pipelineCaches.cend())
            {
                pipelineCache = pipelineCacheIter->second->GetVkPipelineCache();
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(pipelineSid, CreateUPtr<VulkanGraphicsPipeline>(_device, pipelineSid, layout, pipelineCache, parameters));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanGraphicsPipeline> VulkanPipelineManager::GetGraphicsPipeline(StringID pipelineSid) const
        {
            const auto pipelineIter = _pipelines.find(pipelineSid);
            if (pipelineIter != _pipelines.cend())
            {
                return *pipelineIter->second.get();
            }
            
            KMP_LOG_ERROR("graphics pipeline with sid '{}' not found", pipelineSid);
            return NullOptional;
        }
        //--------------------------------------------------------------------------
    }
}