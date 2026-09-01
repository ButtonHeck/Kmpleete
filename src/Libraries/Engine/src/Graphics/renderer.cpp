#include "Kmpleete/Graphics/renderer.h"


namespace Kmpleete
{
    namespace Graphics
    {
        Renderer::Renderer(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::RendererUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}