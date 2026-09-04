#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base class of a graphics API physical device abstraction
        class KMP_API PhysicalDevice
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
            PhysicalDevice() noexcept;
            virtual ~PhysicalDevice() = default;

            KMP_NODISCARD const GPUInfo& GetGPUInfo() const noexcept;
            void PrintGPUInfo() const noexcept;

            virtual void RecreateResources() {};

        protected:
            virtual void _QueryGPUInfo() = 0;

        protected:
            GPUInfo _gpuInfo;
        };
        //--------------------------------------------------------------------------
    }
}