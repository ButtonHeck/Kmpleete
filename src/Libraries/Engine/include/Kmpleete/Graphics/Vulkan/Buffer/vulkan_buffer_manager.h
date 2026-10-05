#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Graphics/graphics_base.h"
#include "Kmpleete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmpleete/Graphics/Vulkan/Buffer/vulkan_vertex_buffer.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmpleete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //! Manager class for creating and storing Vulkan buffer objects.
        //! All the buffers stored as pointers in StringIDHashMap-s and are split into two categories:
        //! 1) ones which don't need to be bound and used during a frame preparation 
        //! and rendering - e.g. vertex/index buffers to store static geometry
        //! 2) ones which are supposed to be updated and acquired during each frame - e.g. uniform/storage
        //! buffers with MVP or other per-frame related data.
        //! @see StringID
        class KMP_API VulkanBufferManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferManager)
            KMP_LOG_CLASSNAME(VulkanBufferManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanBufferManager(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate);
            ~VulkanBufferManager() = default;

            KMP_NODISCARD VulkanBuffer CreateStagingBuffer(const Vector<VulkanBufferDataMappingParameters>& dataMappingParameters, bool autoOffsetAdjacent = false) const;

            KMP_NODISCARD VulkanBuffer CreateBuffer(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD VulkanBuffer CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, 
                                                    const Vector<VulkanBufferDataMappingParameters>& dataMappingParameters, bool autoOffsetAdjacent = false) const;
            bool CreateBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            bool CreateVertexBuffer(StringID bufferSid, VkDeviceSize size, const Vector<BufferLayout>& layouts = {}, bool perFrame = false);
            bool CreateVertexBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, 
                                    const Vector<BufferLayout>& layouts = {}, bool perFrame = false);

            bool CreateIndexBuffer(StringID bufferSid, VkDeviceSize size, bool perFrame = false);
            bool CreateIndexBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            bool CreateUniformBuffer(StringID bufferSid, VkDeviceSize size, bool perFrame = false);
            bool CreateUniformBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            bool CreateStorageBuffer(StringID bufferSid, VkDeviceSize size, bool perFrame = false);
            bool CreateStorageBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            bool CreateIndirectBuffer(StringID bufferSid, VkDeviceSize size, bool perFrame = false);
            bool CreateIndirectBuffer(StringID bufferSid, const VulkanBufferParameters& parameters, bool perFrame = false);

            KMP_NODISCARD Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD Nullable<VulkanBuffer*> GetBuffer(StringID bufferSid, UInt32 index) const noexcept;
            KMP_NODISCARD Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid) const noexcept;
            KMP_NODISCARD Nullable<VulkanVertexBuffer*> GetVertexBuffer(StringID bufferSid, UInt32 index) const noexcept;

            void ResizeBuffer(StringID bufferSid, VkDeviceSize size);
            void ResizeBuffers(StringID bufferSid, VkDeviceSize size);

        private:
            KMP_NODISCARD VulkanBuffer _CreateBuffer(const VulkanBufferParameters& parameters) const;

            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateBufferPtr(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanVertexBuffer*> _CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateStorageBufferPtr(const VulkanBufferParameters& parameters) const;
            KMP_NODISCARD Nullable<VulkanBuffer*> _CreateIndirectBufferPtr(const VulkanBufferParameters& parameters) const;

        private:
            VkDevice _device;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            StringIDHashMap<UPtr<VulkanBuffer>> _buffers;
            StringIDHashMap<UPtr<VulkanVertexBuffer>> _vertexBuffers;
            StringIDHashMap<Array<UPtr<VulkanBuffer>, NumConcurrentFrames>> _perFrameBuffers;
            StringIDHashMap<Array<UPtr<VulkanVertexBuffer>, NumConcurrentFrames>> _perFrameVertexBuffers;
        };
        //--------------------------------------------------------------------------
    }
}