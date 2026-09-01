#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"


namespace Kmpleete
{
    namespace MemoryChecker
    {
        //! Helper function to initialize memory leaks detection mechanism.
        //! Currently implemented only for Windows
        KMP_API void Prepare();
    };
    //--------------------------------------------------------------------------
}
