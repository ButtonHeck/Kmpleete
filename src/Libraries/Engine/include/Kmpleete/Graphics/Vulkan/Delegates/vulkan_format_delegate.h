#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Helper delegate class for checking or finding Vulkan formats properties in
        //! places where it is necessary to pick a format based on some criteria (e.g. image creation)
        class KMP_API VulkanFormatDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanFormatDelegate)
            KMP_LOG_CLASSNAME(VulkanFormatDelegate)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit VulkanFormatDelegate(VkPhysicalDevice physicalDevice) noexcept;
            ~VulkanFormatDelegate() = default;

            KMP_NODISCARD VkFormatProperties GetFormatProperties(VkFormat format) const;
            KMP_NODISCARD VkFormat FindImageFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
            KMP_NODISCARD bool IsMipmapCompatible(VkFormat format) const;

        private:
            VkPhysicalDevice _physicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}