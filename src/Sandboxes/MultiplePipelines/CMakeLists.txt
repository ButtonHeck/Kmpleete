## ~/src/Sandboxes/MultiplePipelines/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(MultiplePipelines LANGUAGES CXX)

set(MultiplePipelines_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_application.h
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_frame_listener.h
)
set(MultiplePipelines_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_frame_listener.cpp
)
set(MultiplePipelines_SHADERS
    ${KmpleeteSandboxResourcesFolder}/multiple_pipelines_buffered_color.vert
    ${KmpleeteSandboxResourcesFolder}/multiple_pipelines_buffered_color.frag
    ${KmpleeteSandboxResourcesFolder}/multiple_pipelines_fixed_color.vert
    ${KmpleeteSandboxResourcesFolder}/multiple_pipelines_fixed_color.frag
)
source_group("Shaders" FILES ${MultiplePipelines_SHADERS})  

add_executable(MultiplePipelines
    ${MultiplePipelines_HEADERS}
    ${MultiplePipelines_SOURCES}
    ${MultiplePipelines_SHADERS}
)

SetupCompilerOptions(MultiplePipelines)

set_target_properties(MultiplePipelines PROPERTIES
    FOLDER Kmpleete/Sandboxes/MultiplePipelines
)

target_compile_definitions(MultiplePipelines
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleeteSandboxResourcesFolder}/\"
)

target_link_libraries(MultiplePipelines
    PRIVATE Kmpleete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(MultiplePipelines PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()