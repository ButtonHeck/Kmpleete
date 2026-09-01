#include "Kmpleete/Graphics/logical_device.h"


namespace Kmpleete
{
    namespace Graphics
    {
        LogicalDevice::LogicalDevice(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::LogicalDeviceUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}