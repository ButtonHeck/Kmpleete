#pragma once

#include <functional>


namespace Kmpleete
{
    //! Codestyle-friendly function alias
    template<typename FuncType>
    using Function = std::function<FuncType>;
}