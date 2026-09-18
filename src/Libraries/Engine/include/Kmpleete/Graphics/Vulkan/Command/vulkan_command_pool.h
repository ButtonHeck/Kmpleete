#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Graphics/command_pool.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Simple Vulkan command pool object wrapper
        class KMP_API VulkanCommandPool : public CommandPool
        {
            KMP_DISABLE_COPY_MOVE(VulkanCommandPool)
            KMP_LOG_CLASSNAME(VulkanCommandPool)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanCommandPool(VkDevice device, UInt32 graphicsQueueIndex);
            ~VulkanCommandPool();

            KMP_NODISCARD VkCommandPool GetVkCommandPool() const noexcept;

        private:
            VkDevice _device;
            VkCommandPool _commandPool;
        };
        //--------------------------------------------------------------------------
    }
}