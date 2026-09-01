#pragma once

#if defined (KMP_ENGINE_PRIVATE)
    #error "main.h should be included only in client code"
#endif

#include "Kmpleete/Application/application.h"
#include "Kmpleete/Base/platform.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Core/memory_checker.h"
#include "Kmpleete/Core/exception_handler.h"
#include "Kmpleete/Utils/function_utils.h"
#include "Kmpleete/Profile/profiler.h"
#include "Kmpleete/Log/log.h"


//! Wrapper function that tries to flush all the profiler data
//! during program termination and rethrows an exception (if any)
void TerminationHandler()
{
    try
    {
#if defined(KMP_PROFILE)
        Kmpleete::Profiler::Get().EndSession();
#endif

        const auto exception = std::current_exception();
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    catch (KMP_MB_UNUSED const Kmpleete::Exception& e)
    {
        KMP_LOG_CRITICAL_FN("Uncaught exception: '{}'", e.what());
    }
    catch (...)
    {
        KMP_LOG_CRITICAL_FN("Uncaught exception");
    }

    std::exit(2);
}
//--------------------------------------------------------------------------


int Main(const Kmpleete::ProgramOptions& programOptions);

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_WINMAIN) 
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
    std::set_terminate(TerminationHandler);
    SetExceptionHandler();

    Kmpleete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    return Main(programOptions);
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
    std::set_terminate(TerminationHandler);
    SetExceptionHandler();

    Kmpleete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(argc, argv);

    return Main(programOptions);
}
#endif
//--------------------------------------------------------------------------

namespace Kmpleete
{
    const char* ApplicationProfileSessionPrefix();
}

int Main(const Kmpleete::ProgramOptions& programOptions)
{
    Kmpleete::MemoryChecker::Prepare();

#if defined(KMP_PROFILE)
    Kmpleete::Profiler::Get().SetActive(not programOptions.IsProfilingOnDemand());
    Kmpleete::Profiler::Get().SetLevel(programOptions.GetProfilingLevel());
#endif

    KMP_MB_UNUSED const auto startupSessionCapacity = 600;
    KMP_PROFILE_BEGIN_SESSION("Startup", Kmpleete::Utils::Concatenate(Kmpleete::ApplicationProfileSessionPrefix(), "-Profile-Startup.json"), startupSessionCapacity);
    auto app = Kmpleete::CreateApplication(programOptions);
    KMP_PROFILE_END_SESSION();

    if (not app)
    {
        return 1;
    }

    KMP_MB_UNUSED const auto runtimeSessionCapacity = 10'000;
    KMP_PROFILE_BEGIN_SESSION("Runtime", Kmpleete::Utils::Concatenate(Kmpleete::ApplicationProfileSessionPrefix(), "-Profile-Runtime.json"), runtimeSessionCapacity);
    app->Run();
    KMP_PROFILE_END_SESSION();

    KMP_MB_UNUSED const auto shutdownSessionCapacity = 200;
    KMP_PROFILE_BEGIN_SESSION("Shutdown", Kmpleete::Utils::Concatenate(Kmpleete::ApplicationProfileSessionPrefix(), "-Profile-Shutdown.json"), shutdownSessionCapacity);
    app.reset();
    KMP_PROFILE_END_SESSION();

    return 0;
}
//--------------------------------------------------------------------------
