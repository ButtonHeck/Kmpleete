#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Vulkan sampler objects storage wrapper. Simplest linear/nearest filtering 
        //! samplers already registered by a logical device object during its creation.
        //! Sampler objects are referenced by StringID.
        //! @see VulkanLogicalDevice
        //! @see StringID
        class KMP_API VulkanSamplersStorage
        {
            KMP_DISABLE_COPY_MOVE(VulkanSamplersStorage)
            KMP_LOG_CLASSNAME(VulkanSamplersStorage)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit VulkanSamplersStorage(VkDevice device);
            ~VulkanSamplersStorage();

            VkSampler AddSampler(StringID sid, const VkSamplerCreateInfo& createInfo);
            KMP_NODISCARD VkSampler GetSampler(StringID sid) const noexcept;

        private:
            VkDevice _device;

            StringIDHashMap<VkSampler> _samplers;
        };
        //--------------------------------------------------------------------------
    }
}