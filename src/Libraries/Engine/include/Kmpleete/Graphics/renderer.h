#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Graphics/graphics_chain_handler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base class of a graphics API renderer abstraction
        class KMP_API Renderer : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(Renderer)

        public:
            explicit Renderer(GraphicsChainHandler& chainHandler);
            virtual ~Renderer() = default;
        };
        //--------------------------------------------------------------------------
    }
}