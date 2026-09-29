#include "Kmpleete/Assets/font_asset.h"
#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Graphics/texture.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Log/log.h"


namespace Kmpleete
{
    namespace Assets
    {
        static constexpr auto AtlasTextureMask = TextureSubTypeMaskBits(TextureSubTypeMaskBits::RGB | TextureSubTypeMaskBits::NoMipmap);


        FontAsset::FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer, FontSubTypeMaskBits subTypeMask)
            : Asset(AssetType::Font, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _graphicsBackend(graphicsBackend)
            , _font(freetypeLib, std::move(fontBuffer))
        {
            _InitializeAtlasesTexturesStorage();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        FontAsset::FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath, FontSubTypeMaskBits subTypeMask)
            : Asset(AssetType::Font, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _graphicsBackend(graphicsBackend)
            , _font(freetypeLib, filepath)
        {
            _InitializeAtlasesTexturesStorage();

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

        OptionalRef<const Graphics::Texture> FontAsset::GetAtlasTexture(const LocaleStrSID& localeSid) const noexcept
        {
            if (_atlasesTextures.contains(localeSid))
            {
                if (_atlasesTextures.at(localeSid) != nullptr)
                {
                    return *_atlasesTextures.at(localeSid).get();
                }

                auto atlasTexture = _CreateAtlasTexture(localeSid);
                if (not atlasTexture)
                {
                    KMP_LOG_ERROR("failed to create '{}' locale texture atlas for asset '{}'", localeSid, _sid);
                    std::nullopt;
                }

                _atlasesTextures[localeSid] = std::move(atlasTexture);
                return *_atlasesTextures.at(localeSid).get();
            }

            KMP_LOG_ERROR("failed to find font atlas texture for sid '{}' for locale '{}'", _sid, localeSid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------

        bool FontAsset::SetPointSize(UInt8 size, UInt32 dpi /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            const auto result = _font.SetPointSize(size, dpi);
            _UpdateExistingAtlasesTextures();
            return result;
        }}
        //--------------------------------------------------------------------------

        bool FontAsset::SetPixelSize(UInt8 size) KMP_PROFILING(ProfileLevelMinor)
        {
            const auto result = _font.SetPixelSize(size);
            _UpdateExistingAtlasesTextures();
            return result;
        }}
        //--------------------------------------------------------------------------

        void FontAsset::ProcessLocaleChange(const LocaleStrSID& localeSid) KMP_PROFILING(ProfileLevelMinor)
        {
            _InitializeAtlasesTexturesStorage();

            auto atlasTexture = _CreateAtlasTexture(localeSid);
            if (not atlasTexture)
            {
                KMP_LOG_ERROR("failed to update '{}' locale texture atlas for asset '{}'", localeSid, _sid);
                return;
            }

            _atlasesTextures[localeSid] = std::move(atlasTexture);
        }}
        //--------------------------------------------------------------------------

        void FontAsset::_InitializeAtlasesTexturesStorage()
        {
            _atlasesTextures.clear();
            _atlasesTextures.reserve(SupportedLocalesCount);

            _atlasesTextures[SidLocaleEnUTF8] = nullptr;
            _atlasesTextures[SidLocaleRuUTF8] = nullptr;
        }
        //--------------------------------------------------------------------------

        void FontAsset::_UpdateExistingAtlasesTextures() KMP_PROFILING(ProfileLevelImportant)
        {
            for (auto& [localeSid, atlasTexturePtr] : _atlasesTextures)
            {
                if (not atlasTexturePtr)
                {
                    continue;
                }

                auto atlasTexture = _CreateAtlasTexture(localeSid);
                if (not atlasTexture)
                {
                    KMP_LOG_ERROR("failed to update '{}' locale texture atlas for asset '{}'", localeSid, _sid);
                    continue;
                }

                _atlasesTextures[localeSid] = std::move(atlasTexture);
            }
        }}
        //--------------------------------------------------------------------------

        UPtr<Graphics::Texture> FontAsset::_CreateAtlasTexture(const LocaleStrSID& localeSid) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            const auto& atlasImage = _font.GetAtlas(localeSid);
            if (not atlasImage.has_value())
            {
                KMP_LOG_ERROR("failed to get '{}' locale atlas image from font object for asset '{}'", localeSid, _sid);
                return nullptr;
            }

            UPtr<Graphics::Texture> atlasTexturePtr(_graphicsBackend.CreateTexture(atlasImage.value().get().GetImage(), AtlasTextureMask));
            return std::move(atlasTexturePtr);
        }}
        //--------------------------------------------------------------------------
    }
}