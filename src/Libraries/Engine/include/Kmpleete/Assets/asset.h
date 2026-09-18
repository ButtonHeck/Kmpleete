#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Assets/assets_interface.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    namespace Assets
    {
        //! Base class for all the assets used by application. 
        //! Just as simple as a pair of type enumerator and asset identifier
        class KMP_API Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            Asset(AssetType type, StringID sid, AssetSubTypeMask subTypeMask) noexcept;
            ~Asset() = default;

            KMP_NODISCARD AssetType GetType() const noexcept;
            KMP_NODISCARD AssetSubTypeMask GetSubTypeMask() const noexcept;
            KMP_NODISCARD StringID GetStringID() const noexcept;

            KMP_NODISCARD bool operator==(const Asset& other) const;

        protected:
            AssetType _type;
            AssetSubTypeMask _subTypeMask;
            StringID _sid;

        private:
            template <typename T> friend struct std::hash;
        };
        //--------------------------------------------------------------------------
    }
}

namespace std
{
    template<>
    struct hash<Kmpleete::Assets::Asset>
    {
        size_t operator()(const Kmpleete::Assets::Asset& asset) const
        {
            return asset._sid;
        }
    };
    //--------------------------------------------------------------------------
}