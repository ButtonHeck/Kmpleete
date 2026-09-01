#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Log/log_class_macro.h"


namespace Kmpleete
{
    //! Utility struct to keep track which node is currently used for
    //! reading or writing
    struct KMP_API JsonScope
    {
        KMP_LOG_CLASSNAME(JsonScope)

    public:
        void Push(const String& entry);
        KMP_NODISCARD bool Pop();

        StringVector scope;
        String scopeString;
    };
    //--------------------------------------------------------------------------
}