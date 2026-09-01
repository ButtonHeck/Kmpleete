#pragma once

#include "Kmpleete/Base/platform.h"
#include "Kmpleete/Base/kmpleete_api.h"

#if defined (KMP_PLATFORM_WINDOWS)
    #include <Windows.h>
#elif defined (KMP_PLATFORM_LINUX)
    #include <signal.h>
#endif


#if defined (KMP_PLATFORM_WINDOWS)
    //! Wrapper function for handling uncaught exceptions
    KMP_API LONG WINAPI UnhandledExceptionHandler(PEXCEPTION_POINTERS);

#elif defined (KMP_PLATFORM_LINUX)
    //! Wrapper function for handling signals
    KMP_API void SignalHandler(int signum, siginfo_t*, void*);
#endif

//! Common wrapper for handling uncaught exceptions
KMP_API void SetExceptionHandler();