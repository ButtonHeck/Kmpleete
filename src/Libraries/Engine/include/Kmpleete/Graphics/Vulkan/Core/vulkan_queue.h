#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanCommandBuffer;


        //! Simple Vulkan queue object wrapper that supports basic functions like waiting, 
        //! submitting (with optional synchronization) and presentation (is queue object supports it)
        class KMP_API VulkanQueue
        {
            KMP_DISABLE_COPY(VulkanQueue)
            KMP_LOG_CLASSNAME(VulkanQueue)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanQueue(VkDevice device, UInt32 familyIndex, bool supportPresentation);
            VulkanQueue(VulkanQueue&& other) noexcept;
            VulkanQueue& operator=(VulkanQueue&& other) noexcept;
            ~VulkanQueue() = default;

            void WaitIdle() const;
            void Submit(const VulkanCommandBuffer& commandBuffer, VkFence fence) const;
            void Submit(const Vector<VkSubmitInfo>& submits, VkFence fence) const;
            void SyncSubmit(const VulkanCommandBuffer& commandBuffer) const;
            void SyncSubmit(const Vector<VkSubmitInfo>& submits) const;
            void Present(const VkPresentInfoKHR& presentationInfo) const;

            KMP_NODISCARD bool SupportsPresentation() const noexcept;
            KMP_NODISCARD VkQueue GetVkQueue() const noexcept;

        private:
            VkDevice _device;
            UInt32 _familyIndex;
            VkQueue _queue;
            bool _supportPresentation;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanQueue>::value);
        static_assert(IsMoveAssignable<VulkanQueue>::value);
    }
}