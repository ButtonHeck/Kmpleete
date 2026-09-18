#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Simple Vulkan API fence object wrapper
        class KMP_API VulkanFence
        {
            KMP_DISABLE_COPY(VulkanFence)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit VulkanFence(VkDevice device, bool signaled = true);
            VulkanFence(VulkanFence&& other) noexcept;
            VulkanFence& operator=(VulkanFence&& other) noexcept;
            ~VulkanFence();

            void Wait() const;
            void Wait(UInt64 timeout) const;
            void Reset() const;

            KMP_NODISCARD VkFence GetVkFence() const noexcept;

        private:
            void _Initialize(bool signaled);
            void _Finalize();

        private:
            VkDevice _device;
            VkFence _fence;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanFence>::value);
        static_assert(IsMoveAssignable<VulkanFence>::value);
    }
}