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
        class VulkanMemoryTypeDelegate;


        //! Helper struct for storing Vulkan buffer creation parameters
        //! @see VulkanBuffer
        struct VulkanBufferParameters
        {
            VkBufferUsageFlags usageFlags;
            VkMemoryPropertyFlags memoryPropertyFlags;
            VkDeviceSize size;
        };
        //--------------------------------------------------------------------------


        //! Helper struct for storing a single data chunk CPU->GPU transfer parameters into a mapped buffer region.
        //! Describes a memcpy-like write of size bytes from the CPU-side data pointer into the mapped
        //! buffer memory at byte offset mappedOffset relative to the mapped base pointer. When a Vector
        //! of such parameters is passed to buffer creation functions, the
        //! resulting buffer size equals the summary of all sizes, so each chunk must satisfy
        //! mappedOffset + size <= resulting buffer size. Explicit offsets allow non-contiguous placement
        //! (e.g. alignment padding), unlike VulkanBufferDataMappingParametersAdjacent where offsets are
        //! accumulated from sizes of previous entries automatically.
        //! @see VulkanBuffer
        //! @see VulkanBufferDataMappingParametersAdjacent
        struct VulkanBufferDataMappingParameters
        {
            UInt32 mappedOffset;
            void* data;
            VkDeviceSize size;
        };
        //--------------------------------------------------------------------------

        //! Helper struct for storing a single data chunk CPU->GPU transfer parameters into a mapped buffer region,
        //! where the destination offset is not specified explicitly but accumulated from the sizes of previous
        //! entries. When a Vector of such parameters is passed to buffer creation functions
        //! chunks are packed back-to-back in vector order starting at offset 0, and
        //! the resulting buffer size equals the summary of all sizes. This is a more compact alternative to
        //! VulkanBufferDataMappingParameters for contiguous layouts without padding.
        //! @see VulkanBuffer
        //! @see VulkanBufferDataMappingParameters
        struct VulkanBufferDataMappingParametersAdjacent
        {
            void* data;
            VkDeviceSize size;
        };
        //--------------------------------------------------------------------------


        //! Base class of a Vulkan buffer object, supports memory management functionality such as: (un)mapping,
        //! flushing, invalidation, copying. Buffer's intentional usage can be acquired either by using VkBufferUsageFlags getter or 
        //! by an exact type getter.
        class KMP_API VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanBuffer)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters);
            VulkanBuffer(VulkanBuffer&& other) noexcept;
            VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;
            virtual ~VulkanBuffer();

            VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Unmap(bool flush = false, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void CopyToMappedMemory(const VulkanBufferDataMappingParameters& parameters);
            void CopyToMappedMemory(UInt32 mappedOffset, void* data, VkDeviceSize size);
            void Resize(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDeviceSize size);

            KMP_NODISCARD VkBuffer GetVkBuffer() const noexcept;
            KMP_NODISCARD VkDeviceSize GetSize() const noexcept;
            KMP_NODISCARD void* GetMappedPtr() const noexcept;
            KMP_NODISCARD VkBufferUsageFlags GetUsageFlags() const noexcept;

            KMP_NODISCARD bool IsTransferSourceBuffer() const noexcept;
            KMP_NODISCARD bool IsTransferDestinationBuffer() const noexcept;
            KMP_NODISCARD bool IsUniformTexelBuffer() const noexcept;
            KMP_NODISCARD bool IsStorageTexelBuffer() const noexcept;
            KMP_NODISCARD bool IsUniformBuffer() const noexcept;
            KMP_NODISCARD bool IsStorageBuffer() const noexcept;
            KMP_NODISCARD bool IsIndexBuffer() const noexcept;
            KMP_NODISCARD bool IsVertexBuffer() const noexcept;
            KMP_NODISCARD bool IsIndirectBuffer() const noexcept;
            KMP_NODISCARD bool IsShaderDeviceAddressBuffer() const noexcept;

        private:
            void _Initialize(const VulkanMemoryTypeDelegate& memoryTypeDelegate);
            void _Finalize();

        protected:
            VkDevice _device;
            VkBuffer _buffer;

        private:
            VkDeviceMemory _memory;
            VkDeviceSize _size;
            void* _mapped;
            VkBufferUsageFlags _usageFlags;
            VkMemoryPropertyFlags _memoryPropertyFlags;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanBuffer>::value);
        static_assert(IsMoveAssignable<VulkanBuffer>::value);
    }
}