#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"


namespace Kmpleete
{
    //! 64-bit unique identifier based on rng.
    //! An UUID with value 0 treated as invalid.
    //! @see rng.h
    class KMP_API UUID
    {
    public:
        static UUID InvalidUuid;

        UUID();
        explicit UUID(UInt64 uuid) noexcept;
        ~UUID() = default;

        KMP_NODISCARD operator UInt64() const noexcept;

    private:
        UInt64 _uuid;
    };
    //--------------------------------------------------------------------------
}

namespace std
{
    template<>
    struct hash<Kmpleete::UUID>
    {
        size_t operator()(const Kmpleete::UUID& uuid) const
        {
            return Kmpleete::UInt64(uuid);
        }
    };
    //--------------------------------------------------------------------------
}
