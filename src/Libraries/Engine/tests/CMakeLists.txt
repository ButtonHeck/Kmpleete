## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)


# Unit Tests

set(Kmpleete_UnitTests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/uuid_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_document_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/assertion_tests.cpp
)
source_group("Core" FILES ${Kmpleete_UnitTests_CORE})

set(Kmpleete_UnitTests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/application_tests.cpp
)
source_group("Application" FILES ${Kmpleete_UnitTests_APPLICATION})

add_executable(Kmpleete_UnitTests
    ${Kmpleete_UnitTests_CORE}
    ${Kmpleete_UnitTests_APPLICATION}
)


SetupBoost(Kmpleete_UnitTests)

target_include_directories(Kmpleete_UnitTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
)

set(KmpleeteUnitTestsCopySettingsScriptName "CopySettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${KmpleeteUnitTestsCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmpleete_UnitTests>/Kmpleete_settings_unit_tests.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmpleete_settings_tests_example.json"
        "$<TARGET_FILE_DIR:Kmpleete_UnitTests>/Kmpleete_settings_unit_tests.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmpleete_UnitTests
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleeteResourcesFolder} -P "${KmpleeteUnitTestsCopySettingsScriptName}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

SetupUnitTestsTargetProperties(
    Kmpleete_UnitTests 
    Kmpleete/Tests/Auto
    Catch2::Catch2WithMain Kmpleete
)


# Test Window Application

set(TestWindowApplication_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/window_application_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/shared_state.h
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/events.h
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/test_frame_listeners.h
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/main_frame_listener.h
    ${CMAKE_CURRENT_LIST_DIR}/TestWindowApplication/main_frame_listener.cpp
)
source_group("Application" FILES ${TestWindowApplication_APPLICATION})

add_executable(TestWindowApplication
    ${TestWindowApplication_APPLICATION}
)

target_include_directories(TestWindowApplication
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"
)

set(TestWindowApplicationSettingsFile "TestWindowApplication_settings.json")
target_compile_definitions(TestWindowApplication
    PRIVATE KMP_ICONS_FOLDER=\"${KmpleeteResourcesFolder}/icons/\"
    PRIVATE KMP_FONTS_FOLDER=\"${KmpleeteResourcesFolder}/fonts/\"
    PRIVATE KMP_TEST_ICON_PATH=\"${KmpleeteResourcesFolder}/icons/test_icon64.png\"
    PRIVATE KMP_TEST_SETTINGS_JSON=\"${TestWindowApplicationSettingsFile}\"
)

set(TestWindowApplicationCopySettingsScriptName "CopyWindowTestsSettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${TestWindowApplicationCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:TestWindowApplication>/${TestWindowApplicationSettingsFile}")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmpleete_settings_example.json"
        "$<TARGET_FILE_DIR:TestWindowApplication>/${TestWindowApplicationSettingsFile}")
    endif() 
    ]=]
)

add_custom_command(
    TARGET TestWindowApplication
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleeteResourcesFolder} -P "${TestWindowApplicationCopySettingsScriptName}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

SetupUnitTestsTargetProperties(
    TestWindowApplication 
    Kmpleete/Tests/Manual
    Catch2::Catch2WithMain Kmpleete FileDialogsLib ImGuiUtilsLib
)


# Window Application Tests

set(Kmpleete_WindowApplicationTests_WINDOW
    ${CMAKE_CURRENT_LIST_DIR}/Window/window_backend_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Window/window_tests.cpp
)
source_group("Window" FILES ${Kmpleete_WindowApplicationTests_WINDOW})

set(Kmpleete_WindowApplicationTests_GRAPHICS
    ${CMAKE_CURRENT_LIST_DIR}/Graphics/graphics_backend_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Graphics/image_tests.cpp
)
source_group("Graphics" FILES ${Kmpleete_WindowApplicationTests_GRAPHICS})

set(Kmpleete_WindowApplicationTests_ASSETS
    ${CMAKE_CURRENT_LIST_DIR}/Assets/font_asset_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Assets/texture_asset_manager_tests.cpp
)
source_group("Assets" FILES ${Kmpleete_WindowApplicationTests_ASSETS})

add_executable(Kmpleete_WindowApplicationTests
    ${Kmpleete_WindowApplicationTests_WINDOW}
    ${Kmpleete_WindowApplicationTests_GRAPHICS}
    ${Kmpleete_WindowApplicationTests_ASSETS}
)


target_include_directories(Kmpleete_WindowApplicationTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"
)

target_compile_definitions(Kmpleete_WindowApplicationTests
    PRIVATE KMP_ICONS_FOLDER=\"${KmpleeteResourcesFolder}/icons/\"
    PRIVATE KMP_FONTS_FOLDER=\"${KmpleeteResourcesFolder}/fonts/\"
    PRIVATE KMP_TEST_ICON_PATH=\"${KmpleeteResourcesFolder}/icons/test_icon64.png\"
)

SetupUnitTestsTargetProperties(
    Kmpleete_WindowApplicationTests 
    Kmpleete/Tests/Manual
    Catch2::Catch2WithMain Kmpleete FileDialogsLib ImGuiUtilsLib
)


# Locale tests

set(Kmpleete_LocaleTests_LOCALIZATION
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_base_tests.cpp
)
source_group("Localization" FILES ${Kmpleete_LocaleTests_LOCALIZATION})

add_executable(Kmpleete_LocaleTests
    ${Kmpleete_LocaleTests_LOCALIZATION}
)


target_include_directories(Kmpleete_LocaleTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    PUBLIC "${spdlog_SOURCE_DIR}/include"
)


# Locale tests - localization

set(KmpleeteTestsDomain "Tests")
target_compile_definitions(Kmpleete_LocaleTests
    PRIVATE KMP_TR_DOMAIN_TESTS=\"${KmpleeteTestsDomain}\"
)
get_target_property(exeDir Kmpleete_LocaleTests RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Kmpleete_LocaleTests_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for Kmpleete_LocaleTests done"
)
add_custom_command(TARGET Kmpleete_LocaleTests_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
)
set_target_properties(Kmpleete_LocaleTests_copy_translations PROPERTIES
    FOLDER Kmpleete/Tests/Auto
)

add_dependencies(Kmpleete_LocaleTests Kmpleete_LocaleTests_copy_translations)

CreateTranslationHelperTargets(
    Kmpleete_LocaleTests 
    Kmpleete/Tests/Auto
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/Localization\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleeteTestsDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleeteTestsDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
)

SetupUnitTestsTargetProperties(
    Kmpleete_LocaleTests 
    Kmpleete/Tests/Auto
    Catch2::Catch2WithMain Kmpleete
)
