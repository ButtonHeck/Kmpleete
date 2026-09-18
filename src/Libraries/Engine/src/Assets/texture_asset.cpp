#include "Kmpleete/Assets/texture_asset.h"
#include "Kmpleete/Core/assertion.h"


namespace Kmpleete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture, TextureSubTypeMaskBits subTypeMask) noexcept
            : Asset(AssetType::Texture, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _texture(texture)
        {
            KMP_ASSERT(_texture);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        const Graphics::Texture& TextureAsset::GetTexture() const noexcept
        {
            KMP_ASSERT(_texture);

            return *_texture;
        }
        //--------------------------------------------------------------------------

        Graphics::Texture& TextureAsset::GetTexture() noexcept
        {
            KMP_ASSERT(_texture);

            return *_texture;
        }
        //--------------------------------------------------------------------------
    }
}