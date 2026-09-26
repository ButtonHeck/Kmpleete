## ~/src/Libraries/MathLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(MathLib LANGUAGES CXX)

add_library(MathLib)

AddTargetSourcesGroup(MathLib "Math"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/math.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/geometry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Math/rng.h
    ${CMAKE_CURRENT_LIST_DIR}/src/geometry.cpp
)

SetupCompilerOptions(MathLib)

target_include_directories(MathLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${glm_SOURCE_DIR}"
)

target_link_libraries(MathLib
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
)

target_compile_definitions(MathLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(MathLib PROPERTIES
    FOLDER Kmpleete/Libraries/MathLib
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Installation
# --------------------------

install(TARGETS MathLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)