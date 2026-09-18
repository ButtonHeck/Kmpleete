#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Graphics/physical_device.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmpleete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    class Window;


    namespace Graphics
    {
        //! Vulkan API physical device object wrapper implementation
        class KMP_API VulkanPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanPhysicalDevice)
            KMP_LOG_CLASSNAME(VulkanPhysicalDevice)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_NODISCARD static const Vector<const char*>& GetEnabledDeviceExtensions();

        public:
            VulkanPhysicalDevice(const Window& window, const UInt32& currentBufferIndex, VkInstance instance, VkSurfaceKHR surface);
            ~VulkanPhysicalDevice();

            void RecreateResources() override;

            KMP_NODISCARD VkPhysicalDevice GetVkPhysicalDevice() const noexcept;
            KMP_NODISCARD const VulkanContext& GetVulkanContext() const noexcept;
            KMP_NODISCARD const VulkanFormatDelegate& GetVulkanFormatDelegate() const noexcept;
            KMP_NODISCARD const VulkanMemoryTypeDelegate& GetVulkanMemoryTypeDelegate() const noexcept;

        private:
            void _Initialize();
            void _Finalize();

            KMP_NODISCARD Vector<VkPhysicalDevice> _GetListOfPhysicalDevices() const;
            void _PickSuitablePhysicalDevice(const Vector<VkPhysicalDevice>& physicalDevices);
            void _QueryGPUInfo() override;

        private:
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkInstance _instance;
            VkSurfaceKHR _surface;
            VkPhysicalDevice _physicalDevice;
            UPtr<VulkanFormatDelegate> _formatDelegate;
            VulkanContext _vulkanContext;
            UPtr<VulkanMemoryTypeDelegate> _memoryTypeDelegate;
        };
        //--------------------------------------------------------------------------
    }
}