#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"

#include <memory>


namespace Kmpleete
{
    //! Shortcut for shared pointer
    template<typename T>
    using Ptr = std::shared_ptr<T>;

    template<typename T, typename... Args>
    KMP_NODISCARD constexpr Ptr<T> CreatePtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    //--------------------------------------------------------------------------


    //! Shortcut for unique pointer
    template<typename T>
    using UPtr = std::unique_ptr<T>;

    template<typename T, typename... Args>
    KMP_NODISCARD constexpr UPtr<T> CreateUPtr(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    //--------------------------------------------------------------------------
}
