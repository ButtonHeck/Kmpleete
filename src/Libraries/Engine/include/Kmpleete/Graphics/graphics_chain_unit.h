#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/string_id.h"


namespace Kmpleete
{
    namespace Graphics
    {
        class GraphicsChainHandler;


        //TODO: comments
        class KMP_API GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(GraphicsChainUnit)

        public:
            GraphicsChainUnit(GraphicsChainHandler& chainHandler, StringID sid);
            virtual ~GraphicsChainUnit() = default;

        protected:
            friend class GraphicsChainHandler;

            KMP_NODISCARD virtual bool _StartFrame(float frameTimestep) = 0;
            virtual void _EndFrame() = 0;

        protected:
            GraphicsChainHandler& _chainHandler;
            const StringID _sid;
        };
        //--------------------------------------------------------------------------
    }
}