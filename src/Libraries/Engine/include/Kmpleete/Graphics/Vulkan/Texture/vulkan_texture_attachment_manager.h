#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Graphics/Vulkan/Texture/vulkan_texture_attachment.h"
#include "Kmpleete/Graphics/Vulkan/Core/vulkan_swapchain.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanImageCreatorDelegate;


        //! Manager of Vulkan texture attachments. It is capable of recreating non sample-fixed
        //! attachments in case multisampling count was changed and recreate all attachments in case
        //! render area size was changed. Coupled with VulkanSwapchain in order to get its' texture
        //! attachment in cases when a separate attachment is unnecessary
        //! @see VulkanTextureAttachment
        //! @see VulkanSwapchain
        class KMP_API VulkanTextureAttachmentManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanTextureAttachmentManager)
            KMP_LOG_CLASSNAME(VulkanTextureAttachmentManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanTextureAttachmentManager(VkDevice device, const VkExtent3D& extent, VkSampleCountFlagBits msaaSamples, 
                                           const VulkanImageCreatorDelegate& imageCreatorDelegate, const VulkanSwapchain& swapchain);
            ~VulkanTextureAttachmentManager() = default;

            bool AddTextureColorAttachment(StringID attachmentSid, VkFormat format, VkImageUsageFlags usageFlags = 0, bool fixedSamples = false);
            bool AddTextureColorAttachment(StringID attachmentSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                           VkImageUsageFlags usageFlags = 0, bool fixedSamples = false);

            bool AddTextureDepthStencilAttachment(StringID attachmentSid, VkFormat format, VkImageUsageFlags usageFlags = 0, bool fixedSamples = false);
            bool AddTextureDepthStencilAttachment(StringID attachmentSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                                  VkImageUsageFlags usageFlags = 0, bool fixedSamples = false);

            bool AddTextureAttachment(StringID attachmentSid, VkFormat format, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples = false);
            bool AddTextureAttachment(StringID attachmentSid, VkFormat format, const VkExtent3D& extent, VkSampleCountFlagBits samples,
                                      VkImageUsageFlags usageFlags, VkImageAspectFlags aspectMask, bool fixedSamples = false);
            KMP_NODISCARD OptionalRef<VulkanTextureAttachment> GetTextureAttachment(StringID attachmentSid) const;

            KMP_NODISCARD VkRenderingAttachmentInfo GetRenderingAttachmentInfo(VkRenderingAttachmentInfo preset, StringID imageViewAttachmentSid, StringID resolveImageViewAttachmentSid, 
                                                                               VkResolveModeFlagBits resolveMode, VkImageLayout resolveImageLayout, 
                                                                               bool useSwapchainForNonMSAA = false, bool useSwapchainSRGB = true) const;

            void RecreateTextureAttachmentsWithNewSize(const VkExtent3D& newExtent);
            void RecreateTextureAttachmentsWithNewSamples(VkSampleCountFlagBits newSamples);

        private:
            KMP_NODISCARD VkRenderingAttachmentInfo _GetRenderingAttachmentInfoSingleSampled(VkRenderingAttachmentInfo& preset, const UPtr<VulkanTextureAttachment>& textureAttachment,
                                                                                             bool useSwapchainForNonMSAA, bool useSwapchainSRGB) const;
            KMP_NODISCARD VkRenderingAttachmentInfo _GetRenderingAttachmentInfoMultiSampled(VkRenderingAttachmentInfo& preset, const UPtr<VulkanTextureAttachment>& textureAttachment,
                                                                                            StringID resolveImageViewAttachmentSid, VkResolveModeFlagBits resolveMode, 
                                                                                            VkImageLayout resolveImageLayout, bool useSwapchainSRGB) const;

        private:
            VkDevice _device;
            const VulkanImageCreatorDelegate& _imageCreatorDelegate;
            VkExtent3D _extent;
            VkSampleCountFlagBits _msaaSamples;
            const VulkanSwapchain& _swapchain;

            StringIDHashMap<UPtr<VulkanTextureAttachment>> _textureAttachments;
        };
        //--------------------------------------------------------------------------
    }
}