#include "Kmpleete/Graphics/swapchain.h"


namespace Kmpleete
{
    namespace Graphics
    {
        Swapchain::Swapchain(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::SwapchainUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}