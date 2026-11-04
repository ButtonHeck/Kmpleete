#include "Kmpleete/Graphics/font_atlas.h"
#include "Kmpleete/Graphics/graphics_base.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Math/math.h"
#include "Kmpleete/Window/window.h"

#include <ft2build.h>
#include FT_FREETYPE_H


namespace Kmpleete
{
    namespace Graphics
    {
        FontAtlas::FontAtlas(FT_FaceRec_* freetypeFace, const FontParameters& parameters, const Localization::CodepointVector& codepoints)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _freetypeFace(freetypeFace)
            , _parameters(parameters)
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

        FontVerticesBuffer FontAtlas::GenerateTextVertices(const WString& text, float x, float y, const Window& window) const
        {
            return GenerateTextVertices(text, x, y, window, float(_parameters.sizeMetrics.xPixelsPerEM));
        }
        //--------------------------------------------------------------------------

        FontVerticesBuffer FontAtlas::GenerateTextVertices(const WString& text, float x, float y, const Window& window, float pixelSize) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_parameters.sizeMetrics.xPixelsPerEM > 0);

            Vector<FontCharacterVertex> vertices;
            vertices.reserve(6 * text.size());

            const auto framebufferSize = window.GetFramebufferSize();
            const auto scale = pixelSize / float(_parameters.sizeMetrics.xPixelsPerEM) * window.GetContentScale();
            const auto offsetToTop = scale * float(_parameters.sizeMetrics.boundingBoxMaxY);
            const auto alignedScaledHeight = float(_parameters.sizeMetrics.alignedHeight) * scale;
            Math::Rect2F boundingBox = { 
                { x, y - offsetToTop },
                { 0, alignedScaledHeight }
            };

            for (const auto& c : text)
            {
                const auto fontCharIter = _fontCharacters.find(c);
                if (fontCharIter == _fontCharacters.cend())
                {
                    continue;
                }

                const auto& fontChar = fontCharIter->second;
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

                boundingBox.size.x = Math::Max(boundingBox.size.x, x - boundingBox.position.x);
            }

            return { vertices, boundingBox };
        }}
        //--------------------------------------------------------------------------

        const Image& FontAtlas::GetImage() const noexcept
        {
            return *_atlasImage.get();
        }
        //--------------------------------------------------------------------------

        void FontAtlas::_CalculateAtlasSize(const Localization::CodepointVector& codepoints) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_freetypeFace);

            for (const auto& codepoint : codepoints)
            {
                if (not _RenderGlyph(codepoint))
                {
                    continue;
                }

                _width += _freetypeFace->glyph->bitmap.width + 1;
                _height = Math::Max(_height, _freetypeFace->glyph->bitmap.rows);
            }
        }}
        //--------------------------------------------------------------------------

        void FontAtlas::_CreateImage(const Localization::CodepointVector& codepoints) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_freetypeFace);

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

        bool FontAtlas::_RenderGlyph(const Localization::Codepoint& codepoint)
        {
            KMP_ASSERT(_freetypeFace);

            return (FT_Load_Char(_freetypeFace, codepoint, FT_LOAD_RENDER) == FT_Err_Ok) && (FT_Render_Glyph(_freetypeFace->glyph, FT_RENDER_MODE_SDF) == FT_Err_Ok);
        }
        //--------------------------------------------------------------------------
    }
}
