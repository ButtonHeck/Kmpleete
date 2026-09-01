#pragma once

#include "Kmpleete/Base/kmpleete_api.h"


namespace Kmpleete
{
    //! Print current stacktrace to a console and dump it to a file
    //! with application name and the current timestamp
    KMP_API void DumpStacktrace();
}