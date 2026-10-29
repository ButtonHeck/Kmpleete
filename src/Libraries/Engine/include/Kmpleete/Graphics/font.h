#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/optional.h"
#include "Kmpleete/Graphics/font_parameters.h"
#include "Kmpleete/Graphics/font_character.h"
#include "Kmpleete/Graphics/font_atlas.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Log/log_class_macro.h"
#include "Kmpleete/Profile/profiler.h"


struct FT_LibraryRec_;
struct FT_FaceRec_;


namespace Kmpleete
{
    namespace Graphics
    {
        //! Font object class, wrapper for FreeType library "Face" type, can be created both from
        //! a file or from a binary buffer.
        class KMP_API Font
        {
            KMP_LOG_CLASSNAME(Font)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(Font)

        public:
            static constexpr auto DefaultFontPixelSize = 48;            

        public:
            Font(FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
            Font(FT_LibraryRec_& freetypeLib, const Filepath& filepath);
            ~Font();

            bool SetPointSize(UInt8 size, UInt32 dpi = 0);
            bool SetPixelSize(UInt8 size);

            KMP_NODISCARD const BinaryBuffer& GetBuffer() const noexcept;
            KMP_NODISCARD const FontParameters& GetParameters() const noexcept;
            KMP_NODISCARD bool HasStyle(FontParameters::Style flag) const noexcept;
            KMP_NODISCARD OptionalRef<const FontAtlas> GetAtlas(const Localization::LocaleStrSID& localeSid) const noexcept;
            KMP_NODISCARD Vector<FontCharacterVertex> GenerateTextVertices(const Localization::LocaleStrSID& localeSid, const WString& text, float x, float y, const Math::Vec2F& framebufferSize) const;
            KMP_NODISCARD Vector<FontCharacterVertex> GenerateTextVertices(const Localization::LocaleStrSID& localeSid, const WString& text, float x, float y, float pixelSize, const Math::Vec2F& framebufferSize) const;

        private:
            void _UpdateParameters() noexcept;
            void _UpdateSizeMetrics() noexcept;
            void _UpdateAtlases();

        private:
            FT_FaceRec_* _freetypeFace;
            BinaryBuffer _fontBuffer;
            FontParameters _parameters;
            HashMap<Localization::LocaleStrSID, UPtr<FontAtlas>> _atlases;
        };
        //--------------------------------------------------------------------------
    }
}