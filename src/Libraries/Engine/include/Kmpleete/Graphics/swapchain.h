#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Graphics/graphics_chain_handler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base class of a graphics API swapchain abstraction
        class KMP_API Swapchain : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            explicit Swapchain(GraphicsChainHandler& chainHandler);
            virtual ~Swapchain() = default;
        };
        //--------------------------------------------------------------------------
    }
}