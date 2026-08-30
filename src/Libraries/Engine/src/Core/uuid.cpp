#include "Kmplete/Core/uuid.h"
#include "Kmplete/Math/rng.h"


namespace Kmplete
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
