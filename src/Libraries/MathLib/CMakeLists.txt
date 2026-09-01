## ~/src/Libraries/MathLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(MathLib LANGUAGES CXX)

add_library(MathLib INTERFACE)

AddTargetSourcesGroup(MathLib "Math"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/math.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/geometry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/rng.h
)

#TODO: uncomment after MathLib becomes non header-only and change INTERFACE to PUBLIC/PRIVATE
#SetupCompilerOptions(MathLib)

target_include_directories(MathLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
    INTERFACE "${glm_SOURCE_DIR}"
)

target_link_libraries(MathLib
    INTERFACE BaseLib
)

#TODO: uncomment after MathLib becomes non header-only and change INTERFACE to PUBLIC/PRIVATE
#target_compile_definitions(MathLib
#    PRIVATE KMP_EXPORTS
#)

set_target_properties(MathLib PROPERTIES
    FOLDER Kmpleete/Libraries/MathLib
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()