#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Graphics/graphics_base.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        KMP_NODISCARD KMP_API VkFormat ImageChannelsToVkFormat(ImageChannels channels, bool srgb) noexcept;
        KMP_NODISCARD KMP_API VkFormat ShaderDataTypeToVkFormat(ShaderDataType type) noexcept;

        static constexpr auto SamplerDefaultNearestSid = "DefaultNearest"_sid;
        static constexpr auto SamplerDefaultLinearSid = "DefaultLinear"_sid;
    }
}