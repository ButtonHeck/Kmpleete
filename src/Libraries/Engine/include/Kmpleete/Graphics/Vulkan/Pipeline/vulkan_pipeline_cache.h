#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Vulkan pipeline cache object wrapper
        class KMP_API VulkanPipelineCache
        {
            KMP_DISABLE_COPY_MOVE(VulkanPipelineCache)
            KMP_LOG_CLASSNAME(VulkanPipelineCache)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanPipelineCache(VkDevice device, const VulkanContext& context, const Filepath& binaryPath);
            ~VulkanPipelineCache();

            KMP_NODISCARD VkPipelineCache GetVkPipelineCache() const noexcept;

        private:
            //! Vulkan cache header wrapper struct
            struct PipelineCacheHeader
            {
                UInt32 headerLength;
                UInt32 headerVersion;
                UInt32 vendorID;
                UInt32 deviceID;
                UInt8 pipelineCacheUUID[VK_UUID_SIZE];
            };

        private:
            void _Initialize();
            void _Finalize();

            void _SerializeCache();
            void _DestroyCache();

            KMP_NODISCARD bool _LoadedCacheIsValid(const BinaryBuffer& cacheBuffer) const;

        private:
            VkDevice _device;
            const VulkanContext& _context;
            VkPipelineCache _cache;
            Filepath _binaryPath;
        };
        //--------------------------------------------------------------------------
    }
}