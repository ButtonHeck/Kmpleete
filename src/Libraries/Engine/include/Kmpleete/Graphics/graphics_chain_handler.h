#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/string_id.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/nullability.h"
#include "Kmpleete/Graphics/graphics_chain_unit.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    namespace Graphics
    {
        //TODO: comments
        class KMP_API GraphicsChainHandler
        {
            KMP_DISABLE_COPY_MOVE(GraphicsChainHandler)
            KMP_LOG_CLASSNAME(GraphicsChainHandler)

        public:
            static constexpr auto PhysicalDeviceUnitSID = "PhysicalDevice"_sid;
            static constexpr auto LogicalDeviceUnitSID = "LogicalDevice"_sid;
            static constexpr auto SwapchainUnitSID = "Swapchain"_sid;
            static constexpr auto RendererUnitSID = "Renderer"_sid;

        public:
            GraphicsChainHandler() = default;
            ~GraphicsChainHandler() = default;

            bool AddHandler(Nullable<GraphicsChainUnit*> unit);
            KMP_NODISCARD bool HandleStartFrame(StringID unitSid, float frameTimestep);
            bool HandleEndFrame(StringID unitSid);

        private:
            StringIDHashMap<Nullable<GraphicsChainUnit*>> _units;
        };
        //--------------------------------------------------------------------------
    }
}