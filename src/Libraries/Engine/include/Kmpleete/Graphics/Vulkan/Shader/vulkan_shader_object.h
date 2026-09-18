#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        //! Simple Vulkan API shader object wrapper used for dynamic rendering
        class KMP_API VulkanShaderObject
        {
            KMP_DISABLE_COPY(VulkanShaderObject)
            KMP_LOG_CLASSNAME(VulkanShaderObject)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanShaderObject(VkDevice device, const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                               const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            VulkanShaderObject(VkDevice device, const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                               const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            VulkanShaderObject(VulkanShaderObject&& other) noexcept;
            VulkanShaderObject& operator=(VulkanShaderObject&& other) noexcept;
            ~VulkanShaderObject();

            KMP_NODISCARD VkShaderEXT GetVkShader() const noexcept;

        private:
            void _Initialize(const Filepath& filepathBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            void _Initialize(const BinaryBuffer32& shaderBinary, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                             const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name = "main");
            void _Finalize();

        private:
            VkDevice _device;
            VkShaderEXT _shaderObject;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanShaderObject>::value);
        static_assert(IsMoveAssignable<VulkanShaderObject>::value);
    }
}