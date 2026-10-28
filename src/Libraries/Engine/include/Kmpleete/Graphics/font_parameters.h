#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //! Font object parameters helper struct
        struct FontParameters
        {
            enum Style : UInt8
            {
                Regular    = 0,
                Italic     = 1 << 0,
                Bold       = 1 << 1,
                ItalicBold = Italic | Bold
            };

            struct SizeMetrics
            {
                UInt16 xPixelsPerEM = 0;
                UInt16 yPixelsPerEM = 0;
                Int32 xScale = 0;
                Int32 yScale = 0;
                Int32 height = 0;
            };

            String familyName = "";
            UInt8 style = Style::Regular;
            bool scalable = true;
            bool monospace = false;
            Int32 numGlyphs = 0;
            UInt16 unitsPerEM = 0;
            Int16 lineHeight = 0;
            Int16 ascender = 0;
            Int16 descender = 0;
            Int16 maxAdvance = 0;
            SizeMetrics sizeMetrics = {};
        };
        //--------------------------------------------------------------------------
    }
}