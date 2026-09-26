#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Graphics/font_character.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Localization/localization_base.h"
#include "Kmpleete/Math/geometry.h"
#include "Kmpleete/Profile/profiler.h"


struct FT_FaceRec_;


namespace Kmpleete
{
    namespace Graphics
    {
        //! Font atlas containing an atlas image and a map of FontCharacters for a single locale. 
        class KMP_API FontAtlas
        {
            KMP_DISABLE_COPY_MOVE(FontAtlas)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            FontAtlas(FT_FaceRec_* freetypeFace, const CodepointVector& codepoints);
            ~FontAtlas() = default;

            KMP_NODISCARD Vector<FontCharacterVertex> GenerateTextVertices(const WString& text, float x, float y, float scale, const Math::Vec2F& framebufferSize);

        private:
            void _CalculateAtlasSize(const CodepointVector& codepoints);
            void _CreateImage(const CodepointVector& codepoints);

            KMP_NODISCARD bool _RenderGlyph(const Codepoint& codepoint);

        private:
            FT_FaceRec_* _freetypeFace;
            unsigned int _width;
            unsigned int _height;
            HashMap<Codepoint, FontCharacter> _fontCharacters;
            UPtr<Image> _atlasImage;
        };
        //--------------------------------------------------------------------------
    }
}