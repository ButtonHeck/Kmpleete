## ~/src/Sandboxes/PostProcessing/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(PostProcessing LANGUAGES CXX)

set(PostProcessing_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/post_processing_application.h
    ${CMAKE_CURRENT_LIST_DIR}/post_processing_frame_listener.h
)
set(PostProcessing_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/post_processing_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/post_processing_frame_listener.cpp
)
set(PostProcessing_SHADERS
    ${KmpleeteSandboxResourcesFolder}/post_processing_pre.vert
    ${KmpleeteSandboxResourcesFolder}/post_processing_pre.frag
    ${KmpleeteSandboxResourcesFolder}/post_processing_post.vert
    ${KmpleeteSandboxResourcesFolder}/post_processing_post.frag
)
source_group("Shaders" FILES ${PostProcessing_SHADERS})

add_executable(PostProcessing
    ${PostProcessing_HEADERS}
    ${PostProcessing_SOURCES}
    ${PostProcessing_SHADERS}
)

SetupCompilerOptions(PostProcessing)

set_target_properties(PostProcessing PROPERTIES
    FOLDER Kmpleete/Sandboxes/PostProcessing
)

target_compile_definitions(PostProcessing
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleeteSandboxResourcesFolder}/\"
)

target_link_libraries(PostProcessing
    PRIVATE Kmpleete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(PostProcessing PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()