#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"


namespace Kmpleete
{
    //! Utility functions related with the time
    namespace Time
    {
        KMP_NODISCARD KMP_API String GetCurrentTimeString(const char* format = "%Y-%m-%d %T");
    }
}