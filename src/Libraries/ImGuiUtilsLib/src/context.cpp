#include "Kmpleete/ImGui/context.h"


namespace Kmpleete
{
    namespace ImGuiUtils
    {
        Context::Context(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, float baseScale, float fontDensity)
            : window(window)
            , graphicsBackendType(graphicsBackendType)
            , dockingEnabled(dockingEnabled)
            , viewportsEnabled(viewportsEnabled)
            , baseScale(baseScale)
            , fontDensity(fontDensity)
        {}
        //--------------------------------------------------------------------------
    }
}