#include "Kmpleete/Assets/font_asset.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/texture.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    namespace Assets
    {
        FontAsset::FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer, FontSubTypeMaskBits subTypeMask)
            : Asset(AssetType::Font, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _graphicsBackend(graphicsBackend)
            , _font(freetypeLib, std::move(fontBuffer))
        {
            _UpdateAtlasesTextures();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        FontAsset::FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath, FontSubTypeMaskBits subTypeMask)
            : Asset(AssetType::Font, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _graphicsBackend(graphicsBackend)
            , _font(freetypeLib, filepath)
        {
            _UpdateAtlasesTextures();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        const Graphics::Font& FontAsset::GetFont() const noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------

        Graphics::Font& FontAsset::GetFont() noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------

        OptionalRef<Graphics::Texture> FontAsset::GetAtlasTexture(const LocaleStrSID& localeSid) const noexcept
        {
            if (_atlasesTextures.contains(localeSid))
            {
                return *_atlasesTextures.at(localeSid).get();
            }

            KMP_LOG_ERROR("failed to find font atlas texture for sid '{}' for locale '{}'", _sid, localeSid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------

        bool FontAsset::SetPointSize(UInt8 size, UInt32 dpi /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            const auto result = _font.SetPointSize(size, dpi);
            _UpdateAtlasesTextures();
            return result;
        }}
        //--------------------------------------------------------------------------

        bool FontAsset::SetPixelSize(UInt8 size) KMP_PROFILING(ProfileLevelMinor)
        {
            const auto result = _font.SetPixelSize(size);
            _UpdateAtlasesTextures();
            return result;
        }}
        //--------------------------------------------------------------------------

        void FontAsset::_UpdateAtlasesTextures() KMP_PROFILING(ProfileLevelImportant)
        {
            _atlasesTextures.clear();
            _atlasesTextures.reserve(SupportedLocalesCount);
            const auto atlasTextureMask = TextureSubTypeMaskBits(TextureSubTypeMaskBits::RGB | TextureSubTypeMaskBits::NoMipmap);

            {
                const auto& atlasImage = _font.GetAtlas(SidTrLocaleEnName);
                if (not atlasImage.has_value())
                {
                    KMP_LOG_ERROR("failed to get English locale atlas image from font object for asset '{}'", _sid);
                }

                UPtr<Graphics::Texture> atlasTexturePtr(_graphicsBackend.CreateTexture(atlasImage.value().get().GetImage(), atlasTextureMask));
                const auto [iterator, hasEmplaced] = _atlasesTextures.emplace(SidTrLocaleEnName, std::move(atlasTexturePtr));
                if (not hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to create English locale atlas texture from font object for asset '{}'", _sid);
                }
            }
            {
                const auto& atlasImage = _font.GetAtlas(SidTrLocaleRuName);
                if (not atlasImage.has_value())
                {
                    KMP_LOG_ERROR("failed to get Russian locale atlas image from font object for asset '{}'", _sid);
                }

                UPtr<Graphics::Texture> atlasTexturePtr(_graphicsBackend.CreateTexture(atlasImage.value().get().GetImage(), atlasTextureMask));
                const auto [iterator, hasEmplaced] = _atlasesTextures.emplace(SidTrLocaleRuName, std::move(atlasTexturePtr));
                if (not hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to create Russian locale atlas texture from font object for asset '{}'", _sid);
                }
            }
        }}
        //--------------------------------------------------------------------------
    }
}