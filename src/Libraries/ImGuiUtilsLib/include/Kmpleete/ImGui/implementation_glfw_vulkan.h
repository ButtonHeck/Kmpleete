#pragma once

#include "Kmpleete/ImGui/implementation.h"
#include "Kmpleete/ImGui/context_vulkan.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Profile/profiler.h"

#include <vulkan/vulkan.h>


struct GLFWwindow;


namespace Kmpleete
{
    namespace ImGuiUtils
    {
        //! Implementation of an ImGUI instance with Vulkan graphics API. Each ImGUI rendering
        //! invocation should be prepended with SetCommandBuffer call.
        class ImGuiImplementationGlfwVulkan : public ImGuiImplementation
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(ImGuiImplementationGlfwVulkan)

        public:
            explicit ImGuiImplementationGlfwVulkan(ContextVulkan* context);
            virtual ~ImGuiImplementationGlfwVulkan();

            void CreateFontsTexture() const override;
            void AddTexture(StringID sid, void* sampler, void* view) override;
            void RemoveTexture(StringID sid) override;

            void SetCommandBuffer(VkCommandBuffer commandBuffer);

        private:
            void _Initialize() const;
            void _Finalize();

            void _NewFrameImpl() const override;
            void _RenderImpl() const override;

        private:
            VkCommandBuffer _commandBuffer;
        };
        //--------------------------------------------------------------------------
    }
}