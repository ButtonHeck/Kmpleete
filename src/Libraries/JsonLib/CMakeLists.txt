## ~/src/Libraries/JsonLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(JsonLib LANGUAGES CXX)

add_library(JsonLib)

AddTargetSourcesGroup(JsonLib "Json"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Json/json_document.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Json/json_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Json/json_writer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Json/json_scope.h
    ${CMAKE_CURRENT_LIST_DIR}/src/json_document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_writer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_scope.cpp
)

SetupCompilerOptions(JsonLib)

target_include_directories(JsonLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
)

target_link_libraries(JsonLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC UtilsLib
    PUBLIC FilesystemLib
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
)

target_compile_definitions(JsonLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(JsonLib PROPERTIES 
    FOLDER Kmpleete/Libraries/JsonLib
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Installation
# --------------------------

install(TARGETS JsonLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)