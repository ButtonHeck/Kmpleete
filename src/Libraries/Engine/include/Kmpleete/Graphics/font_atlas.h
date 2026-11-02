#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Graphics/font_parameters.h"
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
            FontAtlas(FT_FaceRec_* freetypeFace, const FontParameters& parameters, const Localization::CodepointVector& codepoints);
            ~FontAtlas() = default;

            KMP_NODISCARD FontVerticesBuffer GenerateTextVertices(const WString& text, float x, float y, const Math::Vec2F& framebufferSize) const;
            KMP_NODISCARD FontVerticesBuffer GenerateTextVertices(const WString& text, float x, float y, float pixelSize, const Math::Vec2F& framebufferSize) const;
            KMP_NODISCARD const Image& GetImage() const noexcept;

        private:
            void _CalculateAtlasSize(const Localization::CodepointVector& codepoints);
            void _CreateImage(const Localization::CodepointVector& codepoints);

            KMP_NODISCARD bool _RenderGlyph(const Localization::Codepoint& codepoint);

        private:
            FT_FaceRec_* _freetypeFace;
            const FontParameters& _parameters;
            unsigned int _width;
            unsigned int _height;
            HashMap<Localization::Codepoint, FontCharacter> _fontCharacters;
            UPtr<Image> _atlasImage;
        };
        //--------------------------------------------------------------------------
    }
}