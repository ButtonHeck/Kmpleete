## ~/src/FilesystemLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)


set(FilesystemLib_UnitTests_FILESYSTEM
    ${CMAKE_CURRENT_LIST_DIR}/filesystem_tests.cpp
)

add_executable(FilesystemLib_UnitTests
    ${FilesystemLib_UnitTests_FILESYSTEM}
)

SetupUnitTestsTargetProperties(
    FilesystemLib_UnitTests 
    Kmpleete/Tests/Auto
    Catch2::Catch2WithMain FilesystemLib Kmpleete BaseLib UtilsLib
)
