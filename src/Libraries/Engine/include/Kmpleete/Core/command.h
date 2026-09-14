#pragma once

#include "Kmpleete/Base/kmpleete_api.h"


namespace Kmpleete
{
    //! Basic interface for command pattern objects
    class KMP_API Command
    {
    public:
        virtual ~Command() = default;

        virtual void Execute() = 0;
        virtual void Undo();

        virtual void operator()();
    };
    //--------------------------------------------------------------------------
}
