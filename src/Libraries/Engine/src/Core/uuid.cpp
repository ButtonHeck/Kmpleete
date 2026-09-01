#include "Kmpleete/Core/uuid.h"
#include "Kmpleete/Math/rng.h"


namespace Kmpleete
{
    static Math::RNGUInt64 rng;

    UUID UUID::InvalidUuid(0);

    UUID::UUID()
        : _uuid(rng.Generate())
    {
        while (_uuid == 0)
        {
            _uuid = rng.Generate();
        }
    }
    //--------------------------------------------------------------------------

    UUID::UUID(UInt64 uuid) noexcept
        : _uuid(uuid)
    {}
    //--------------------------------------------------------------------------

    UUID::operator UInt64() const noexcept
    {
        return _uuid;
    }
    //--------------------------------------------------------------------------
}
