#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/nullability.h"


namespace Kmpleete
{
    //! Simple wrapper of a window cursor object
    //! @see Window
    class KMP_API WindowCursor
    {
    public:
        virtual ~WindowCursor() = default;

        KMP_NODISCARD virtual NonNull<void*> GetImplPointer() const noexcept = 0;
    };
    //--------------------------------------------------------------------------
}