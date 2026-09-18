#pragma once

#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/ImGui/context.h"
#include "Kmpleete/Profile/profiler.h"

#include <backends/imgui_impl_vulkan.h>


namespace Kmpleete
{
    namespace ImGuiUtils
    {
        //! ImGUI Vulkan API implementation context
        struct ContextVulkan : public Context
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            ContextVulkan(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, float baseScale, float fontDensity, ImGui_ImplVulkan_InitInfo initInfo);
            ~ContextVulkan() = default;

            ImGui_ImplVulkan_InitInfo initInfo;
        };
        //--------------------------------------------------------------------------
    }
}