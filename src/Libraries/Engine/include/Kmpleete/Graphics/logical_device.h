#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Graphics/command_pool.h"
#include "Kmpleete/Graphics/swapchain.h"
#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Graphics/graphics_chain_handler.h"
#include "Kmpleete/Assets/assets_interface.h"


namespace Kmpleete
{
    namespace Graphics
    {
        class Texture;
        class Image;


        //! Base class of a graphics API logical device abstraction
        class KMP_API LogicalDevice : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(LogicalDevice)

        public:
            explicit LogicalDevice(GraphicsChainHandler& chainHandler);
            virtual ~LogicalDevice() = default;

            virtual void RecreateResources() {};

            KMP_NODISCARD virtual const Swapchain& GetSwapchain() const noexcept = 0;

            KMP_NODISCARD virtual Nullable<Texture*> CreateTexture(const Image& image, Assets::TextureSubTypeMaskBits subTypeMask) const = 0;
        };
        //--------------------------------------------------------------------------
    }
}