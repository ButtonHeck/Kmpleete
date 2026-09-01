#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        namespace VKUtils
        {
            KMP_API VkResult CheckResult(VkResult result, const char* message, bool throwException = true);
            KMP_NODISCARD KMP_API String VkResultToString(VkResult result) noexcept;
        }
    }
}