#pragma once


namespace Kmpleete
{
    //! Kmpleete project's version getters functions, definitions are
    //! generated automatically by CMake based on version variables and
    //! hash of the latest commit of the build

    const char* GetKmpleeteVersion();
    unsigned int GetKmpleeteVersionMajor();
    unsigned int GetKmpleeteVersionMinor();
    unsigned int GetKmpleeteVersionPatch();
    unsigned int GetKmpleeteVersionTweak();
    
    const char* GetKmpleeteGitHash();
    const char* GetKmpleeteGitHashShort();
    const char* GetKmpleeteGitBranch();
}