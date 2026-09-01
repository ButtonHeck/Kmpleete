#include "Kmpleete/Graphics/Vulkan/Command/vulkan_command_pool.h"
#include "Kmpleete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmpleete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmpleete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Log/log.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanCommandPool::VulkanCommandPool(VkDevice device, UInt32 graphicsQueueIndex)
            : CommandPool()
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _device(device)
            , _commandPool(VK_NULL_HANDLE)
        {
            auto createPoolInfo = VKUtils::InitVkCommandPoolCreateInfo();
            createPoolInfo.queueFamilyIndex = graphicsQueueIndex;
            createPoolInfo.flags = VK_CommandPoolCreate_ResetCommandBuffer;

            const auto result = vkCreateCommandPool(_device, &createPoolInfo, nullptr, &_commandPool);
            VKUtils::CheckResult(result, "VulkanCommandPool: failed to create command pool");

            KMP_ASSERT(_device && _commandPool);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanCommandPool::~VulkanCommandPool() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _commandPool);

            vkDestroyCommandPool(_device, _commandPool, nullptr);
        }}
        //--------------------------------------------------------------------------

        VkCommandPool VulkanCommandPool::GetVkCommandPool() const noexcept
        {
            KMP_ASSERT(_commandPool);

            return _commandPool;
        }
        //--------------------------------------------------------------------------
    }
}