#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Graphics/graphics_chain_handler.h"


namespace Kmpleete
{
    namespace Graphics
    {
        GraphicsChainUnit::GraphicsChainUnit(GraphicsChainHandler& chainHandler, StringID sid)
            : _chainHandler(chainHandler)
            , _sid(sid)
        {
            _chainHandler.AddHandler(this);
        }
        //--------------------------------------------------------------------------
    }
}