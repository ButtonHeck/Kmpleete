#pragma once

#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Base/string_id.h"


namespace Kmpleete
{
    enum class FrameListenerCommandCode
    {
        Activate,
        Deactivate,
        Create,
        Delete
    };
    //--------------------------------------------------------------------------


    //! Description of a single command used for intercommunication between frame listeners and application
    //! @see FrameListener 
    //! @see FrameListenerManager
    struct FrameListenerCommand
    {
        FrameListenerCommandCode code;
        StringID sid;
    };
    static_assert(IsMoveConstructible<FrameListenerCommand>::value);
    static_assert(IsMoveAssignable<FrameListenerCommand>::value);

    using FrameListenerCommandBuffer = Vector<FrameListenerCommand>;
    //--------------------------------------------------------------------------
}