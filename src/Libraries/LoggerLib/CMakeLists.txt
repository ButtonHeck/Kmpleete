## ~/src/Libraries/LoggerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(LoggerLib LANGUAGES CXX)


# Logger library for anything but "Production" build

add_library(LoggerLib)

AddTargetSourcesGroup(LoggerLib "Log"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Log/log.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Log/log_api.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Log/log_class_macro.h
    ${CMAKE_CURRENT_LIST_DIR}/src/log.cpp
)

SetupCompilerOptions(LoggerLib)

target_include_directories(LoggerLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${spdlog_SOURCE_DIR}/include"
)

target_link_libraries(LoggerLib 
    PUBLIC BaseLib
    PUBLIC TimeLib
    PUBLIC spdlog_header_only
)

target_compile_definitions(LoggerLib
    PRIVATE KMP_LOG_EXPORTS
)

set_target_properties(LoggerLib PROPERTIES 
    FOLDER Kmpleete/Libraries/LoggerLib
)


# Logger library for "Production" build

add_library(LoggerInterfaceLib INTERFACE)

AddTargetSourcesGroup(LoggerInterfaceLib "Log"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Log/log.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmpleete/Log/log_class_macro.h
)

target_compile_definitions(LoggerInterfaceLib
    INTERFACE KMP_CONFIG_TYPE_PRODUCTION
)

target_include_directories(LoggerInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
    INTERFACE "${spdlog_SOURCE_DIR}/include"
)

set_target_properties(LoggerInterfaceLib PROPERTIES 
    FOLDER Kmpleete/Libraries/LoggerLib
)


# Installation
# --------------------------

install(TARGETS LoggerLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${KmpleeteInstallEditorComponentName}
)
install(TARGETS spdlog
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleeteInstallEditorComponentName}
)