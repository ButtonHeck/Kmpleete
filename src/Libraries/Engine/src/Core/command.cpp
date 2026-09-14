#include "Kmpleete/Core/command.h"


namespace Kmpleete
{
    void Command::Undo()
    {}
    //--------------------------------------------------------------------------

    void Command::operator()()
    {
        Execute();
    }
    //--------------------------------------------------------------------------
}