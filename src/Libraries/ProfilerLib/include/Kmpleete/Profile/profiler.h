#pragma once

//! Helper macro definition to manually turn on/off profiling in the code (static builds only)
//! for Debug and Release builds
#define KMP_PROFILE_MANUAL_SET true

namespace Kmpleete
{
    //! Enumeration of the profiling levels from the most important (0) to the least (4).
    //! There is no strict rule which level in which function should be used, but the common
    //! rule in this project is to use ProfileLevelAlways in constructors/destructors and functions
    //! like Initialize/Finalize, ProfileLevelImportant(Verbose) for somewhat "heavy" and
    //! important functions, and ProfileLevelMinor(Verbose) for any other functions
    enum ProfileLevel : unsigned int
    {
        ProfileLevelAlways = 0,
        ProfileLevelImportant = 1,
        ProfileLevelImportantVerbose = 2,
        ProfileLevelMinor = 3,
        ProfileLevelMinorVerbose = 4
    };
    //--------------------------------------------------------------------------
}


#if ((not defined KMP_BUILD_STATIC || KMP_PROFILE_MANUAL_SET) && not defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)
#define KMP_PROFILE

#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Base/type_traits.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Base/macro.h"
#include "Kmpleete/Log/log_class_macro.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>


namespace Kmpleete
{
    //! Single profiling metrics unit
    struct ProfileResult
    {
        String name;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;
        std::thread::id threadId;
    };
    static_assert(IsMoveConstructible<ProfileResult>::value);
    //--------------------------------------------------------------------------


    //! A scope-like object for profiling sessions, each such object represents
    //! some stage of the application (initialization, runtime, finalization)
    //! @see main.h in Kmpleete engine library - function named "Main"
    struct ProfilingSession
    {
        String name;
        int profilesCount;
    };
    //--------------------------------------------------------------------------


    //! Global handler of profiling applications' and libraries' functions performance,
    //! responsible for collecting profiling metrics results, writing them in files
    //! according to the current session. All the profiling split to several levels
    //! (similar to levels of Log class) from level 0 (always profile - most important) 
    //! to level 4 (least important), level is defined in the "--profile_level" argument.
    //! In order to mitigate number of accesses to a disk, profiling units are stored 
    //! in a temporary storage (whose capacity is set when starting a session)
    //! that flushes to a file when: 1) the number of profiling
    //! units exceeds storage's capacity 2) the current session ends.
    //! This profiler is capable of turning profiling on/off at runtime (by default with
    //! an Alt+F11 shortcut). Initial activation flag is defined in the "--profile_on_demand" argument
    //! (false by default - profiler is active from the beginning, otherwise - activate manually).
    //! @see ProfilingSession
    //! @see ProfileResult
    //! @see ProfilerTimer
    class KMP_API Profiler
    {
        KMP_LOG_CLASSNAME(Profiler)
        KMP_DISABLE_COPY_MOVE(Profiler)

    public:
        KMP_NODISCARD static Profiler& Get();

        void SetLevel(unsigned int level);
        KMP_NODISCARD unsigned int GetLevel() const;

        void SetActive(bool active);
        KMP_NODISCARD bool IsActive() const;

        void BeginSession(const String& name, const Filepath& filepath, int storageSize);
        void EndSession();

    private:
        Profiler() noexcept;
        ~Profiler();

        void _BeginSessionInternal(const String& name, const Filepath& filepath, int storageSize);
        void _EndSessionInternal();

        void _WriteProfileHeader(std::ofstream& outputFileStream) const;
        void _WriteProfileResults(std::ofstream& outputFileStream) const;
        void _WriteProfileResultsToIntermediate() const;
        void _WriteProfileResultsFromIntermediate(std::ofstream& outputFileStream) const;
        void _WriteProfileFooter(std::ofstream& outputFileStream) const;
        Filepath _CreateIntermediateFilepath(int intermediateCount) const;
        void _BeginNewCycle();

    private:
        friend class ProfilerTimer;

    private:
        unsigned int _level;
        bool _active;
        std::mutex _mutex;
        UPtr<ProfilingSession> _currentSession;
        Filepath _outputFilepath;
        Vector<ProfileResult> _profileResults;
        int _storageSize;
        int _storeCycles;
    };
    //--------------------------------------------------------------------------


    //! A timer for a single profiling metrics unit whose timing is defined
    //! by the lifetime of this object. If the level is of this timer is higher
    //! than the level set for profiling or the profiler is currently inactive,
    //! this object's timing is skipped.
    //! @see Profiler
    class KMP_API ProfilerTimer
    {
        KMP_DISABLE_COPY_MOVE(ProfilerTimer)

    public:
        explicit ProfilerTimer(const char* name, unsigned int level = ProfileLevelAlways);
        ~ProfilerTimer();

        void SetName(const char* name);

    private:
        const char* _name;
        const bool _skip;
        std::chrono::high_resolution_clock::time_point _start;
    };
    //--------------------------------------------------------------------------


    namespace ProfilerUtils
    {
        //! Compile-time friendly struct for storing postprocessed name of a metrics unit
        template<size_t length>
        struct ReplaceResult
        {
            char data[length];
        };

        //! Helper function to prettify name of a metrics unit (like remove __cdecl, replace std:: with custom aliases etc.)
        template<size_t lengthSrc, size_t lengthRemove, size_t lengthReplace>
        consteval auto ReplaceString(const char(&src)[lengthSrc], const char(&remove)[lengthRemove], const char(&replace)[lengthReplace])
        {
            ReplaceResult<lengthSrc> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < lengthSrc)
            {
                size_t matchIndex = 0;
                while ((matchIndex < lengthRemove - 1) && (srcIndex + matchIndex < lengthSrc - 1) && (src[srcIndex + matchIndex] == remove[matchIndex]))
                {
                    matchIndex++;
                }

                if (matchIndex == lengthRemove - 1)
                {
                    size_t replaceIndex = 0;
                    while (replaceIndex < lengthReplace - 1)
                    {
                        result.data[dstIndex++] = replace[replaceIndex++];
                    }

                    srcIndex += matchIndex;
                }
                else
                {
                    result.data[dstIndex++] = src[srcIndex];
                    srcIndex++;
                }
            }

            return result;
        }
    }
    //--------------------------------------------------------------------------
}

//! Shortcut macros to begin/end profiling sessions
#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize) ::Kmpleete::Profiler::Get().BeginSession(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION() ::Kmpleete::Profiler::Get().EndSession()

//! The "meat" macro that formats metrics unit name
#define _KMP_PROFILE_SCOPE_LINE2(name, line, level) \
    constexpr auto fixedNameCdecl##line      = ::Kmpleete::ProfilerUtils::ReplaceString(name, "__cdecl ", "");\
    constexpr auto fixedNameKmpleete##line   = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmpleete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameKmpleete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Filepath");\
    constexpr auto fixedNameArray##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "std::array", "Array");\
    constexpr auto fixedNameClass##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameArray##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    constexpr auto fixedNameRapidjson##line  = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVoidParams##line.data, "rapidjson::GenericDocument<rapidjson::UTF8<char>,rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>,rapidjson::CrtAllocator>", "rapidjson::Document");\
    constexpr auto fixedName_T##line         = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameRapidjson##line.data, "_T *", "");\
    constexpr auto fixedNameKHR_T##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedName_T##line.data, "KHR_T *", "");\
    ::Kmpleete::ProfilerTimer timer##line(fixedNameKHR_T##line.data, level)
#define _KMP_PROFILE_SCOPE_LINE(name, line, level) _KMP_PROFILE_SCOPE_LINE2(name, line, level)
#define KMP_PROFILE_SCOPE(name, level) _KMP_PROFILE_SCOPE_LINE(name, __LINE__, level)
#define KMP_PROFILE_FUNCTION(level) KMP_PROFILE_SCOPE(KMP_FUNC_SIG, level)

//! Codestyle-friendly wrapper for insertion profiling macro at the beginning of the function,
//! Do not forget to add closing curly bracket at the end
#define KMP_PROFILING(level) { KMP_PROFILE_FUNCTION(level);


//! Helper macro for declaring profiler timer for non-copyable objects' constructors, should be added before any other class member
#define KMP_PROFILE_CONSTRUCTOR_DECLARE() \
    private:\
    UPtr<ProfilerTimer> _constructorProfilerTimer;

//! Helper macro for declaring profiler timer for copyable objects' constructors, should be added before any other class member
#define KMP_PROFILE_CONSTRUCTOR_DECLARE_COPYABLE() \
    private:\
    Ptr<ProfilerTimer> _constructorProfilerTimer;


//! Shortcut macro for profiling non-copyable objects' constructors including members initializer lists stage
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS() \
    _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("")) ,

//! Shortcut macro for profiling non-copyable derived objects' constructors including members initializer lists stage
#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS() \
    , _constructorProfilerTimer(CreateUPtr<ProfilerTimer>(""))


//! Shortcut macro for profiling copyable objects' constructors including members initializer lists stage
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS_COPYABLE() \
    _constructorProfilerTimer(CreatePtr<ProfilerTimer>("")) ,

//! Shortcut macro for profiling copyable derived objects' constructors including members initializer lists stage
#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS_COPYABLE() \
    , _constructorProfilerTimer(CreatePtr<ProfilerTimer>(""))


//! Analogous to _KMP_PROFILE_SCOPE_LINE2 macro for non-copyable objects' constructors
#define KMP_PROFILE_CONSTRUCTOR_END() \
    constexpr auto fixedNameCdecl##line      = ::Kmpleete::ProfilerUtils::ReplaceString(KMP_FUNC_SIG, "__cdecl ", "");\
    constexpr auto fixedNameKmpleete##line   = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmpleete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameKmpleete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Filepath");\
    constexpr auto fixedNameArray##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "std::array", "Array");\
    constexpr auto fixedNameClass##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameArray##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    constexpr auto fixedNameRapidjson##line  = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVoidParams##line.data, "rapidjson::GenericDocument<rapidjson::UTF8<char>,rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>,rapidjson::CrtAllocator>", "rapidjson::Document");\
    constexpr auto fixedName_T##line         = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameRapidjson##line.data, "_T *", "");\
    constexpr auto fixedNameKHR_T##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedName_T##line.data, "KHR_T *", "");\
    _constructorProfilerTimer->SetName(fixedNameKHR_T##line.data);\
    _constructorProfilerTimer.reset(nullptr);

//! Analogous to _KMP_PROFILE_SCOPE_LINE2 macro for copyable objects' constructors
#define KMP_PROFILE_CONSTRUCTOR_END_COPYABLE() \
    constexpr auto fixedNameCdecl##line      = ::Kmpleete::ProfilerUtils::ReplaceString(KMP_FUNC_SIG, "__cdecl ", "");\
    constexpr auto fixedNameKmpleete##line   = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmpleete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameKmpleete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Filepath");\
    constexpr auto fixedNameArray##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "std::array", "Array");\
    constexpr auto fixedNameClass##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameArray##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    constexpr auto fixedNameRapidjson##line  = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameVoidParams##line.data, "rapidjson::GenericDocument<rapidjson::UTF8<char>,rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>,rapidjson::CrtAllocator>", "rapidjson::Document");\
    constexpr auto fixedName_T##line         = ::Kmpleete::ProfilerUtils::ReplaceString(fixedNameRapidjson##line.data, "_T *", "");\
    constexpr auto fixedNameKHR_T##line      = ::Kmpleete::ProfilerUtils::ReplaceString(fixedName_T##line.data, "KHR_T *", "");\
    _constructorProfilerTimer->SetName(fixedNameKHR_T##line.data);\
    _constructorProfilerTimer.reset();

#else
#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION()

#define KMP_PROFILE_SCOPE(name, level)
#define KMP_PROFILE_FUNCTION(level)
#define KMP_PROFILING(level) {

#define KMP_PROFILE_CONSTRUCTOR_DECLARE()
#define KMP_PROFILE_CONSTRUCTOR_DECLARE_COPYABLE()
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
#define KMP_PROFILE_CONSTRUCTOR_END()
#define KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS_COPYABLE()
#define KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS_COPYABLE()
#define KMP_PROFILE_CONSTRUCTOR_END_COPYABLE()
#endif
