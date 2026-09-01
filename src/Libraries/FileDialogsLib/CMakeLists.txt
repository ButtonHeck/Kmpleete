## ~/src/Libraries/FileDialogsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(FileDialogsLib LANGUAGES CXX)

add_library(FileDialogsLib)

AddTargetSourcesGroup(FileDialogsLib "File Dialogs"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/FileDialogs/file_dialogs.h
    ${CMAKE_CURRENT_LIST_DIR}/src/file_dialogs.cpp
)

SetupCompilerOptions(FileDialogsLib)

target_include_directories(FileDialogsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"

    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_link_libraries(FileDialogsLib
    PUBLIC BaseLib

    PRIVATE FilesystemLib
    PRIVATE UtilsLib
)

target_compile_definitions(FileDialogsLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(FileDialogsLib PROPERTIES 
    FOLDER Kmpleete/Libraries/FileDialogsLib
)

if(${KMPLEETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Installation
# --------------------------

install(TARGETS FileDialogsLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleeteInstallEditorComponentName}
)