#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Graphics/graphics_base.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base struct for graphics API parameters that each application should provide for its work
        struct GraphicsParameters
        {
            explicit GraphicsParameters(GraphicsBackendType type) noexcept
                : type(type)
            {}

            virtual ~GraphicsParameters() = default;

            const GraphicsBackendType type;
        };
        //--------------------------------------------------------------------------


        //! Client application should assign this to the function for graphics parameters initialization
        KMP_API void (*ClientInitializeGraphicsParametersFn)(GraphicsParameters& parameters);
    }
}