#include "Kmpleete/Json/json_scope.h"
#include "Kmpleete/Log/log.h"
#include "Kmpleete/Utils/string_utils.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    void JsonScope::Push(const String& entry) KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        scope.push_back(entry);
        scopeString = Utils::StringVectorToString(scope, '/');
    }}
    //--------------------------------------------------------------------------

    bool JsonScope::Pop() KMP_PROFILING(ProfileLevelMinorVerbose)
    {
        if (not scope.empty())
        {
            scope.pop_back();
            scopeString = scope.empty() ? "" : Utils::StringVectorToString(scope, '/');
            return true;
        }

        KMP_LOG_ERROR("cannot pop from empty scope");
        return false;
    }}
    //--------------------------------------------------------------------------
}