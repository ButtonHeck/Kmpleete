#include "Kmpleete/Graphics/font.h"
#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Localization/localization_unicode_map.h"
#include "Kmpleete/Log/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace Kmpleete
{
    namespace Graphics
    {
        Font::Font(FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _freetypeFace(nullptr)
            , _fontBuffer(std::move(fontBuffer))
        {
            const auto freetypeFontInitError = FT_New_Memory_Face(&freetypeLib, _fontBuffer.data(), static_cast<FT_Long>(_fontBuffer.size()), 0, &_freetypeFace);
            if (freetypeFontInitError)
            {
                KMP_LOG_ERROR("failed to load FreeType font from buffer");
                throw RuntimeError("Font: failed to load FreeType font from buffer");
            }

            _UpdateParameters();
            SetPixelSize(DefaultFontPixelSize);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        Font::Font(FT_LibraryRec_& freetypeLib, const Filepath& filepath)
            : Font(freetypeLib, Filesystem::ReadFileAsBinary(filepath))
        {}
        //--------------------------------------------------------------------------

        Font::~Font() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_freetypeFace);

            const auto freetypeFontDoneError = FT_Done_Face(_freetypeFace);
            if (freetypeFontDoneError)
            {
                KMP_LOG_ERROR("failed to destroy FreeType font instance");
            }
        }}
        //--------------------------------------------------------------------------

        bool Font::SetPointSize(UInt8 size, UInt32 dpi) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            KMP_ASSERT(_freetypeFace);

            const auto setSizeError = FT_Set_Char_Size(_freetypeFace, size * 64, 0, dpi, dpi);
            if (setSizeError)
            {
                KMP_LOG_ERROR("failed to set point size to {}", size);
                return false;
            }

            _UpdateSizeMetrics();
            return true;
        }}
        //--------------------------------------------------------------------------

        bool Font::SetPixelSize(UInt8 size) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            KMP_ASSERT(_freetypeFace);

            const auto setSizeError = FT_Set_Pixel_Sizes(_freetypeFace, size, 0);
            if (setSizeError)
            {
                KMP_LOG_ERROR("failed to set pixel size to {}", size);
                return false;
            }

            _UpdateSizeMetrics();
            return true;
        }}
        //--------------------------------------------------------------------------

        const BinaryBuffer& Font::GetBuffer() const noexcept
        {
            return _fontBuffer;
        }
        //--------------------------------------------------------------------------

        const FontParameters& Font::GetParameters() const noexcept
        {
            return _parameters;
        }
        //--------------------------------------------------------------------------

        bool Font::HasStyle(FontParameters::Style flag) const noexcept
        {
            return (_parameters.style & static_cast<UInt8>(flag)) != 0;
        }
        //--------------------------------------------------------------------------

        OptionalRef<const FontAtlas> Font::GetAtlas(const Localization::LocaleStrSID& localeSid) const noexcept
        {
            if (_atlases.contains(localeSid))
            {
                return *_atlases.at(localeSid).get();
            }

            KMP_LOG_ERROR("cannot find atlas for locale SID '{}'", localeSid);
            return NullOptional;
        }
        //--------------------------------------------------------------------------

        Vector<FontCharacterVertex> Font::GenerateTextVertices(const Localization::LocaleStrSID& localeSid, const WString& text, float x, float y, const Math::Vec2F& framebufferSize) const KMP_PROFILING(ProfileLevelImportant)
        {
            const auto& atlasOptional = GetAtlas(localeSid);
            if (not atlasOptional.has_value())
            {
                KMP_LOG_ERROR("cannot find atlas for locale '{}' to generate text vertices", localeSid);
                return Vector<FontCharacterVertex>();
            }

            const auto& atlas = atlasOptional.value().get();
            return atlas.GenerateTextVertices(text, x, y, framebufferSize);
        }}
        //--------------------------------------------------------------------------

        Vector<FontCharacterVertex> Font::GenerateTextVertices(const Localization::LocaleStrSID& localeSid, const WString& text, float x, float y, float pixelSize, const Math::Vec2F& framebufferSize) const KMP_PROFILING(ProfileLevelImportant)
        {
            const auto& atlasOptional = GetAtlas(localeSid);
            if (not atlasOptional.has_value())
            {
                KMP_LOG_ERROR("cannot find atlas for locale '{}' to generate text vertices", localeSid);
                return Vector<FontCharacterVertex>();
            }

            const auto& atlas = atlasOptional.value().get();
            return atlas.GenerateTextVertices(text, x, y, pixelSize, framebufferSize);
        }}
        //--------------------------------------------------------------------------

        void Font::_UpdateParameters() noexcept
        {
            KMP_ASSERT(_freetypeFace);

            _parameters.familyName = _freetypeFace->family_name;

            const auto style = _freetypeFace->style_flags;
            _parameters.style = FontParameters::Style::Regular;
            if (style & FT_STYLE_FLAG_BOLD)
            {
                _parameters.style |= FontParameters::Style::Bold;
            }
            if (style & FT_STYLE_FLAG_ITALIC)
            {
                _parameters.style |= FontParameters::Style::Italic;
            }

            _parameters.scalable = _freetypeFace->face_flags & FT_FACE_FLAG_SCALABLE;
            _parameters.monospace = _freetypeFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH;
            _parameters.numGlyphs = _freetypeFace->num_glyphs;
            _parameters.unitsPerEM = _freetypeFace->units_per_EM;
            _parameters.lineHeight = _freetypeFace->height;
            _parameters.ascender = _freetypeFace->ascender;
            _parameters.descender = _freetypeFace->descender;
            _parameters.maxAdvance = _freetypeFace->max_advance_width;
        }
        //--------------------------------------------------------------------------

        void Font::_UpdateSizeMetrics() noexcept
        {
            KMP_ASSERT(_freetypeFace);

            _parameters.sizeMetrics.xPixelsPerEM = _freetypeFace->size->metrics.x_ppem;
            _parameters.sizeMetrics.yPixelsPerEM = _freetypeFace->size->metrics.y_ppem;
            _parameters.sizeMetrics.xScale = _freetypeFace->size->metrics.x_scale >> 16;
            _parameters.sizeMetrics.yScale = _freetypeFace->size->metrics.y_scale >> 16;
            _parameters.sizeMetrics.height = _freetypeFace->size->metrics.height >> 6;

            _UpdateAtlases();
        }
        //--------------------------------------------------------------------------

        void Font::_UpdateAtlases() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_freetypeFace);

            _atlases.clear();
            _atlases.reserve(Localization::SupportedLocalesCount);

            {
                const auto& enLocaleCodepoints = Localization::UnicodeMap::GetCodepointVector(Localization::SidLocaleEnUTF8);
                const auto [iterator, hasEmplaced] = _atlases.emplace(Localization::SidLocaleEnUTF8, CreateUPtr<FontAtlas>(_freetypeFace, _parameters, enLocaleCodepoints));
                if (not hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to create English locale atlas object");
                }
            }
            {
                const auto& ruLocaleCodepoints = Localization::UnicodeMap::GetCodepointVector(Localization::SidLocaleRuUTF8);
                const auto [iterator, hasEmplaced] = _atlases.emplace(Localization::SidLocaleRuUTF8, CreateUPtr<FontAtlas>(_freetypeFace, _parameters, ruLocaleCodepoints));
                if (not hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to create Russian locale atlas object");
                }
            }
        }}
        //--------------------------------------------------------------------------
    }
}