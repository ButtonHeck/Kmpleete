#pragma once


namespace Kmpleete
{
    namespace SourceLocation
    {
        //! Compile-time friendly struct for storing postprocessed name of a metrics unit
        template<size_t length>
        struct ReplaceResult
        {
            char data[length];
        };
        //--------------------------------------------------------------------------


        //! Helper function to prettify name of a metrics unit (like remove __cdecl, replace std:: with custom aliases etc.)
        template<size_t lengthSrc, size_t lengthRemove, size_t lengthReplace>
        consteval auto ReplaceString(const char(&src)[lengthSrc], const char(&remove)[lengthRemove], const char(&replace)[lengthReplace])
        {
            ReplaceResult<lengthSrc> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < lengthSrc)
            {
                size_t matchIndex = 0;
                while ((matchIndex < lengthRemove - 1) && (srcIndex + matchIndex < lengthSrc - 1) && (src[srcIndex + matchIndex] == remove[matchIndex]))
                {
                    matchIndex++;
                }

                if (matchIndex == lengthRemove - 1)
                {
                    size_t replaceIndex = 0;
                    while (replaceIndex < lengthReplace - 1)
                    {
                        result.data[dstIndex++] = replace[replaceIndex++];
                    }

                    srcIndex += matchIndex;
                }
                else
                {
                    result.data[dstIndex++] = src[srcIndex];
                    srcIndex++;
                }
            }

            return result;
        }
        //--------------------------------------------------------------------------
    }
}