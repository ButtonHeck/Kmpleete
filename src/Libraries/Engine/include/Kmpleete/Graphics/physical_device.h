#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Graphics/logical_device.h"
#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Graphics/graphics_chain_handler.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base class of a graphics API physical device abstraction
        class KMP_API PhysicalDevice : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(PhysicalDevice)
            KMP_LOG_CLASSNAME(PhysicalDevice)

        public:
            struct GPUInfo
            {
                String vendor;
                String name;
                String driverVersion;
            };

        public:
            explicit PhysicalDevice(GraphicsChainHandler& chainHandler) noexcept;
            virtual ~PhysicalDevice() = default;

            KMP_NODISCARD const GPUInfo& GetGPUInfo() const noexcept;
            void PrintGPUInfo() const noexcept;

            virtual void RecreateResources() {};

            KMP_NODISCARD virtual const LogicalDevice& GetLogicalDevice() const noexcept = 0;
            KMP_NODISCARD virtual LogicalDevice& GetLogicalDevice() noexcept = 0;

        protected:
            virtual void _QueryGPUInfo() = 0;

        protected:
            GPUInfo _gpuInfo;
        };
        //--------------------------------------------------------------------------
    }
}