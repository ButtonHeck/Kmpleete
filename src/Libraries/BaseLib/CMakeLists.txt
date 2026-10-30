## ~/src/Libraries/BaseLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(BaseLib LANGUAGES CXX)

add_library(BaseLib INTERFACE)

AddTargetSourcesGroup(BaseLib "Base"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/Kmpleete_api.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/platform.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/macro.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/nullability.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/pointers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/types_aliases.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/optional.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/type_traits.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/string_id.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/functional.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/named_bool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/exception.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Base/source_location_utils.h
)

target_compile_options(BaseLib INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror -Wno-missing-field-initializers>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror -Wno-missing-field-initializers>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor /W4 /WX /we4388 /wd4251 $<$<BOOL:${KMPLEETE_MULTIPROCESSOR_BUILD}>:/MP>>
)

target_include_directories(BaseLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(BaseLib PROPERTIES 
    FOLDER Kmpleete/Libraries/BaseLib
)

target_link_libraries(BaseLib
    INTERFACE GSL
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()