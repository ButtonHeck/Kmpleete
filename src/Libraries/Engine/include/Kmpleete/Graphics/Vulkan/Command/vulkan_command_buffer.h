#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Simple Vulkan command buffer object wrapper
        class KMP_API VulkanCommandBuffer
        {
            KMP_DISABLE_COPY(VulkanCommandBuffer)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool);
            VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept;
            VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) noexcept;
            ~VulkanCommandBuffer();

            void Begin(VkCommandBufferUsageFlags flags = 0) const;
            void End() const;
            void Reset() const;

            KMP_NODISCARD VkCommandBuffer GetVkCommandBuffer() const noexcept;

        private:
            void _Initialize();
            void _Finalize();

        private:
            VkDevice _device;
            VkCommandPool _commandPool;
            VkCommandBuffer _commandBuffer;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanCommandBuffer>::value);
        static_assert(IsMoveAssignable<VulkanCommandBuffer>::value);
    }
}