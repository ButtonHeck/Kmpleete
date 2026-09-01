#pragma once

#include "Kmpleete/Base/Kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Graphics/font.h"
#include "Kmpleete/Assets/asset.h"
#include "Kmpleete/Profile/profiler_fwd.h"


struct FT_LibraryRec_;


namespace Kmpleete
{
    namespace Assets
    {
        //! Asset of a font type containing single font object
        //! @see Font
        //! @see Assets::Asset
        class KMP_API FontAsset : public Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer, FontSubTypeMaskBits subTypeMask);
            FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath, FontSubTypeMaskBits subTypeMask);
            ~FontAsset() = default;

            KMP_NODISCARD const Graphics::Font& GetFont() const noexcept;
            KMP_NODISCARD Graphics::Font& GetFont() noexcept;

        private:
            Graphics::Font _font;
        };
        //--------------------------------------------------------------------------
    }
}