#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Graphics/texture.h"
#include "Kmpleete/Assets/asset.h"
#include "Kmpleete/Profile/profiler_fwd.h"


namespace Kmpleete
{
    namespace Assets
    {
        //! Asset of a texture type containing single Texture object
        //! @see Texture
        //! @see Assets::Asset
        class KMP_API TextureAsset : public Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture, TextureSubTypeMaskBits subTypeMask) noexcept;
            ~TextureAsset() = default;

            KMP_NODISCARD const Graphics::Texture& GetTexture() const noexcept;
            KMP_NODISCARD Graphics::Texture& GetTexture() noexcept;

        private:
            UPtr<Graphics::Texture> _texture;
        };
        //--------------------------------------------------------------------------
    }
}