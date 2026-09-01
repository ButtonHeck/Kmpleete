#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Base class for texture objects backed by graphics API
        class KMP_API Texture
        {
            KMP_DISABLE_COPY_MOVE(Texture)

        public:
            Texture() = default;
            virtual ~Texture() = default;
        };
        //--------------------------------------------------------------------------
    }
}