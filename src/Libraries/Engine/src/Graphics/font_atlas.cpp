#include "Kmpleete/Graphics/font_atlas.h"
#include "Kmpleete/Graphics/graphics_base.h"
#include "Kmpleete/Core/assertion.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace Kmpleete
{
    namespace Graphics
    {
        FontAtlas::FontAtlas(FT_FaceRec_* freetypeFace, const CodepointVector& codepoints)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _freetypeFace(freetypeFace)
            , _width(0)
            , _height(0)
            , _atlasImage(nullptr)
        {
            KMP_ASSERT(_freetypeFace);

            _CalculateAtlasSize(codepoints);
            _CreateImage(codepoints);

            KMP_ASSERT(_width > 0 && _height > 0 && _atlasImage);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        Vector<FontCharacterVertex> FontAtlas::GenerateTextVertices(const WString& text, float x, float y, float scale, const Math::Vec2F& framebufferSize) KMP_PROFILING(ProfileLevelImportant)
        {
            Vector<FontCharacterVertex> vertices;
            vertices.reserve(6 * text.size());

            for (const auto& c : text)
            {
                if (_fontCharacters.find(c) == _fontCharacters.end())
                {
                    continue;
                }

                const auto& fontChar = _fontCharacters[c];
                float xpos = x + fontChar.bearing.x * scale;
                float ypos = y + (fontChar.size.y - fontChar.bearing.y) * scale;
                float w = fontChar.size.x * scale;
                float h = fontChar.size.y * scale;

                const auto p1 = Math::ToNDC(xpos, ypos - h, framebufferSize);
                const auto p2 = Math::ToNDC(xpos, ypos, framebufferSize);
                const auto p3 = Math::ToNDC(xpos + w, ypos, framebufferSize);
                const auto p4 = Math::ToNDC(xpos + w, ypos - h, framebufferSize);

                vertices.push_back({ p1, Math::Vec2F(fontChar.uvMin.x, fontChar.uvMin.y) });
                vertices.push_back({ p2, Math::Vec2F(fontChar.uvMin.x, fontChar.uvMax.y) });
                vertices.push_back({ p3, Math::Vec2F(fontChar.uvMax.x, fontChar.uvMax.y) });

                vertices.push_back({ p1, Math::Vec2F(fontChar.uvMin.x, fontChar.uvMin.y) });
                vertices.push_back({ p3, Math::Vec2F(fontChar.uvMax.x, fontChar.uvMax.y) });
                vertices.push_back({ p4, Math::Vec2F(fontChar.uvMax.x, fontChar.uvMin.y) });

                x += (fontChar.advance >> 6) * scale;
            }

            return vertices;
        }}
        //--------------------------------------------------------------------------

        void FontAtlas::_CalculateAtlasSize(const CodepointVector& codepoints) KMP_PROFILING(ProfileLevelImportant)
        {
            for (const auto& codepoint : codepoints)
            {
                if (not _RenderGlyph(codepoint))
                {
                    continue;
                }

                _width += _freetypeFace->glyph->bitmap.width + 1;
                _height = std::max(_height, _freetypeFace->glyph->bitmap.rows);
            }
        }}
        //--------------------------------------------------------------------------

        void FontAtlas::_CreateImage(const CodepointVector& codepoints) KMP_PROFILING(ProfileLevelImportant)
        {
            BinaryBuffer atlasBuffer(_width * _height, 0);
            unsigned int offsetX = 0;
            _fontCharacters.reserve(codepoints.size());

            for (const auto& codepoint : codepoints)
            {
                if (not _RenderGlyph(codepoint))
                {
                    continue;
                }

                const FT_Bitmap& bitmap = _freetypeFace->glyph->bitmap;
                for (unsigned int y = 0; y < bitmap.rows; y++)
                {
                    for (unsigned int x = 0; x < bitmap.width; x++)
                    {
                        unsigned int atlasIndex = y * _width + (offsetX + x);
                        unsigned int glyphIndex = y * bitmap.width + x;
                        atlasBuffer[atlasIndex] = bitmap.buffer[glyphIndex];
                    }
                }

                Graphics::FontCharacter character = {
                    .uvMin = { float(offsetX) / float(_width), 0.0f },
                    .uvMax = { float(offsetX + bitmap.width) / float(_width), float(bitmap.rows) / float(_height) },
                    .size = { int(bitmap.width), int(bitmap.rows) },
                    .bearing = { _freetypeFace->glyph->bitmap_left, _freetypeFace->glyph->bitmap_top },
                    .advance = static_cast<unsigned int>(_freetypeFace->glyph->advance.x)
                };

                _fontCharacters[codepoint] = character;
                offsetX += bitmap.width + 1;
            }

            _atlasImage.reset(new Image(atlasBuffer.data(), int(atlasBuffer.size()), { _width, _height }, Graphics::ImageChannels::Grey));
        }}
        //--------------------------------------------------------------------------

        bool FontAtlas::_RenderGlyph(const Codepoint& codepoint)
        {
            return (FT_Load_Char(_freetypeFace, codepoint, FT_LOAD_RENDER) == FT_Err_Ok) && (FT_Render_Glyph(_freetypeFace->glyph, FT_RENDER_MODE_SDF) == FT_Err_Ok);
        }
        //--------------------------------------------------------------------------
    }
}
