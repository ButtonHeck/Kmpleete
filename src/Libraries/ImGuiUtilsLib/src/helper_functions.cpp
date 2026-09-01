#include "Kmpleete/ImGui/helper_functions.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Base/macro.h"


namespace Kmpleete
{
    namespace ImGuiUtils
    {
        void SetItemTooltip(const char* text, ImGuiHoveredFlags_ flags /*= ImGuiHoveredFlags_AllowWhenDisabled*/) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            if (ImGui::IsItemHovered(flags))
            {
#if not defined (KMP_COMPILER_MSVC)
                KMP_COMPILER_DIAGNOSTIC_PUSH
                KMP_COMPILER_DIAGNOSTIC_IGNORE("-Wformat-security")
                ImGui::SetTooltip(text);
                KMP_COMPILER_DIAGNOSTIC_POP
#else
                ImGui::SetTooltip(text);
#endif
            }
        }}
        //--------------------------------------------------------------------------
    }
}