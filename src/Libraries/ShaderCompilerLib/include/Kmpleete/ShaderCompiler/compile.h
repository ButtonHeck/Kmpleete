#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/optional.h"


namespace Kmpleete
{
    namespace ShaderCompiler
    {
        enum class ShaderType
        {
            Vertex,
            Fragment,
            Geometry,
            Compute,
            RayTracing
        };
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API String ShaderTypeToString(ShaderType shaderType) noexcept;
        KMP_NODISCARD KMP_API Optional<ShaderType> StringToShaderType(const String& shaderTypeString) noexcept;

        KMP_NODISCARD KMP_API BinaryBuffer32 CompileGLSLToSpirvFromSource(const String& sourceName, ShaderType shaderType, const String& shaderCode, bool optimize = true);
        KMP_NODISCARD KMP_API BinaryBuffer32 CompileGLSLToSpirvFromFile(const String& sourceName, ShaderType shaderType, const Filepath& shaderFile, bool optimize = true);
    }
}