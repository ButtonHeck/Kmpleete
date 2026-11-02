#pragma once


namespace Kmpleete
{
    namespace SourceLocation
    {
        //! Compile-time friendly struct for storing postprocessed function signature
        template<size_t length>
        struct ReplaceResult
        {
            char data[length];
        };
        //--------------------------------------------------------------------------


        //! Helper function to prettify function signature (like remove __cdecl, replace std:: with custom aliases etc.)
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

        //! Helper function to strip function's full signature to a "ClassName::FunctionName" form,
        //! currently works only for non-templated classes/functions
        template<size_t lengthSrc>
        consteval auto ShortenFunctionName(const char(&src)[lengthSrc])
        {
            ReplaceResult<lengthSrc> result = {};
            size_t srcIndex = 0;

            size_t functionNameEnd = lengthSrc;
            while (srcIndex < lengthSrc - 1)
            {
                if (src[srcIndex] == '(')
                {
                    functionNameEnd = srcIndex;
                    break;
                }
                srcIndex++;
            }

            srcIndex = functionNameEnd;
            bool classLevelNamespaceFound = false;
            size_t classNameBegin = 0;
            while (srcIndex > 0)
            {
                if (src[srcIndex] == ':')
                {
                    if (not classLevelNamespaceFound)
                    {
                        classLevelNamespaceFound = true;
                        srcIndex--;
                    }
                    else
                    {
                        classNameBegin = srcIndex + 1;
                        break;
                    }
                }
                srcIndex--;
            }

            for (size_t i = 0; i < (functionNameEnd - classNameBegin); i++)
            {
                result.data[i] = src[i + classNameBegin];
            }

            return result;
        }
        //--------------------------------------------------------------------------
    }
}