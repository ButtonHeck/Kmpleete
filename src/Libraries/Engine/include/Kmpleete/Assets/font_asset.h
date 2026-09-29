#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Graphics/font.h"
#include "Kmpleete/Graphics/texture.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Assets/asset.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log_class_macro.h"


struct FT_LibraryRec_;


namespace Kmpleete
{
    namespace Graphics
    {
        class GraphicsBackend;
    }


    namespace Assets
    {
        //! Asset of a font type containing single font object
        //! @see Font
        //! @see Assets::Asset
        class KMP_API FontAsset : public Asset
        {
            KMP_LOG_CLASSNAME(FontAsset)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer, FontSubTypeMaskBits subTypeMask);
            FontAsset(Graphics::GraphicsBackend& graphicsBackend, StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath, FontSubTypeMaskBits subTypeMask);
            ~FontAsset() = default;

            KMP_NODISCARD const Graphics::Font& GetFont() const noexcept;
            KMP_NODISCARD Graphics::Font& GetFont() noexcept;
            KMP_NODISCARD OptionalRef<const Graphics::Texture> GetAtlasTexture(const LocaleStrSID& localeSid) const noexcept;

            bool SetPointSize(UInt8 size, UInt32 dpi = 0);
            bool SetPixelSize(UInt8 size);

            void ProcessLocaleChange(const LocaleStrSID& localeSid);

        private:
            void _InitializeAtlasesTexturesStorage();
            void _UpdateExistingAtlasesTextures();
            KMP_NODISCARD UPtr<Graphics::Texture> _CreateAtlasTexture(const LocaleStrSID& localeSid) const;

        private:
            Graphics::GraphicsBackend& _graphicsBackend;
            Graphics::Font _font;
            mutable HashMap<LocaleStrSID, UPtr<Graphics::Texture>> _atlasesTextures;
        };
        //--------------------------------------------------------------------------
    }
}