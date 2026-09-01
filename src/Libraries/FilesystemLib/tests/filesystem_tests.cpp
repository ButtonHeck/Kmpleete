#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Utils/string_utils.h"
#include "Kmpleete/Core/uuid.h"
#include "Kmpleete/Base/platform.h"

#include <catch2/catch_test_macros.hpp>

#include <fstream>


#if defined (CreateFile)
    #pragma push_macro("CreateFile")
    #undef CreateFile
    #define KMP_UNDEF_CreateFile
#endif
#if defined (CopyFile)
    #pragma push_macro("CopyFile")
    #undef CopyFile
    #define KMP_UNDEF_CopyFile
#endif

TEST_CASE("Filesystem Current path", "[core][filesystem]")
{
    SECTION("Current path is not empty")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        REQUIRE(not path.empty());
    }

    SECTION("Current path exists")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        REQUIRE(Kmpleete::Filesystem::FilepathExists(path));
    }

    SECTION("Current path is directory")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        REQUIRE(Kmpleete::Filesystem::IsDirectory(path));
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem string conversions", "[core][filesystem]")
{
    SECTION("Current path ToGenericU8String is not empty")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmpleete::Filesystem::ToGenericU8String(path);
        REQUIRE(not pathStr.empty());
    }

    SECTION("Current path ToGenericString is not empty")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmpleete::Filesystem::ToGenericString(path);
        REQUIRE(not pathStr.empty());
    }

    SECTION("Current path ToNativeU8String is not empty")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmpleete::Filesystem::ToNativeU8String(path);
        REQUIRE(not pathStr.empty());
    }

    SECTION("Current path ToNativeString is not empty")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmpleete::Filesystem::ToNativeString(path);
        REQUIRE(not pathStr.empty());
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem directories functions", "[core][filesystem]")
{
    SECTION("CreateDirectories/RemoveDirectories test directory")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        path /= "test";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(path));
        REQUIRE(Kmpleete::Filesystem::RemoveDirectories(path));
    }

    SECTION("CreateDirectories test nested directories")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        path /= "test_nest_0";
        path /= "test_nest_1";
        path /= "test_nest_2";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(path));
    }

    SECTION("CreateDirectories test empty directory")
    {
        REQUIRE_FALSE(Kmpleete::Filesystem::CreateDirectories(""));
    }

    SECTION("CreateDirectories test invalid character in a name")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
#if defined (KMP_PLATFORM_WINDOWS)
        path /= "*";
        REQUIRE_FALSE(Kmpleete::Filesystem::CreateDirectories(path));
#else
        SUCCEED();
#endif
    }

    SECTION("CopyDirectories non-existing 'from' path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "non-existing_directory";
        const auto to = path / "existing_test_directory";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(to));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyDirectories(from, to));
    }

    SECTION("CopyDirectories same 'from' and 'to' paths")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyDirectories(from, from));
    }

    SECTION("CopyDirectories invalid 'to' path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(from));
        const auto to = "\0DD:\\con";
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyDirectories(from, to));
    }

    SECTION("CopyDirectories with 'none' behaviour")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory1";
        const auto fromSecondDir = from / "internal_test_dir1";
        const auto to = path / "existing_test_directory_copy1";
        const auto toSecondDir = to / "internal_test_dir1";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(to));
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(fromSecondDir));
        REQUIRE(Kmpleete::Filesystem::CopyDirectories(from, to, std::filesystem::copy_options::none));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(toSecondDir));
    }

    SECTION("CopyDirectories with default 'recursive' behaviour")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory2";
        const auto fromSecondDir = from / "internal_test_dir2";
        const auto to = path / "existing_test_directory_copy2";
        const auto toSecondDir = to / "internal_test_dir2";
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(to));
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(fromSecondDir));
        REQUIRE(Kmpleete::Filesystem::CopyDirectories(from, to));
        REQUIRE(Kmpleete::Filesystem::FilepathExists(toSecondDir));
    }

    SECTION("CopyDirectories with 'from' as a filepath")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "not_a_directory.txt";
        const auto to = path / "existing_test_directory";
        REQUIRE(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE(Kmpleete::Filesystem::CreateDirectories(to));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyDirectories(from, to));
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem files functions", "[core][filesystem]")
{
    SECTION("CreateFile/RemoveFile in existing directory")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        path /= "test_file.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));
        REQUIRE(Kmpleete::Filesystem::RemoveFile(path));
    }

    SECTION("CreateFile/RemoveFile with cyrillic characters")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto cyrillic = Kmpleete::Utils::NarrowToFilepath("Привет_мир");
        REQUIRE_NOTHROW(path.append(cyrillic));
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));
        REQUIRE(Kmpleete::Filesystem::RemoveFile(path));
    }

    SECTION("CreateFile in non-existent directory")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        path /= "test";
        Kmpleete::UUID uuid;
        path /= std::to_string(uuid);
        path /= "test_file.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));
    }

    SECTION("CopyFile non-existing file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "non-existing.file";
        const auto to = path / "non-existing_copy.file";
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with invalid 'to' path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = "\0DD:\\con\\blah.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with same 'from' and 'to'")
    {
        auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        path /= "test_file_to_copy.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyFile(path, path));
    }

    SECTION("CopyFile with default skip-existing behaviour")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE(Kmpleete::Filesystem::CreateFile(to));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with overwrite behaviour")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE(Kmpleete::Filesystem::CreateFile(to));
        REQUIRE(Kmpleete::Filesystem::CopyFile(from, to, std::filesystem::copy_options::overwrite_existing));
    }

    SECTION("CopyFile with 'none' behaviour")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE(Kmpleete::Filesystem::CreateFile(to));
        REQUIRE_FALSE(Kmpleete::Filesystem::CopyFile(from, to, std::filesystem::copy_options::none));
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem renaming tests", "[core][filesystem]")
{
    SECTION("Rename valid case no overwrite")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, false));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename invalid source valid destination")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
    }

    SECTION("Rename valid source invalid destination")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "::invalid.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename valid source existing destination no overwrite")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename valid source existing destination overwrite")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, true));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename source directory to destination file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path;
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, true));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename source file to destination directory")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path;
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, true));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename existing directory to non-existing directory")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(from));
    }

    SECTION("Rename existing directory to existing directory no overwrite")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(to));
    }

    SECTION("Rename existing directory to existing directory overwrite")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::CreateDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmpleete::Filesystem::Rename(from, to, true));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(from));
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveDirectories(to));
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem read text files functions", "[core][filesystem]")
{
    SECTION("ReadFileAsText non-existing file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_do_not_exist.blah";
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String content;
        REQUIRE_NOTHROW(content = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsText empty file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_is_empty.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));

        Kmpleete::String content;
        REQUIRE_NOTHROW(content = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsText regular text file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_contains_text.txt";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));

        const auto contentToWrite = Kmpleete::String("13 characters");
        std::ofstream outputFile(path);
        if (outputFile.is_open())
        {
            outputFile << contentToWrite;
            outputFile.close();

            Kmpleete::String contentToRead;
            REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsText(path));
            REQUIRE_FALSE(contentToRead.empty());
            REQUIRE(contentToRead == "13 characters");
        }
        else
        {
            FAIL();
        }
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem read binary files functions", "[core][filesystem]")
{
    SECTION("ReadFileAsBinary non-existing file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_do_not_exist.bin";
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::Vector<Kmpleete::UByte> content;
        REQUIRE_NOTHROW(content = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsBinary empty file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_is_empty.bin";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));

        Kmpleete::Vector<Kmpleete::UByte> content;
        REQUIRE_NOTHROW(content = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsBinary regular binary file")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "this_file_contains_binary.bin";
        REQUIRE(Kmpleete::Filesystem::CreateFile(path));

        Kmpleete::UByte contentToWrite[] = {
            Kmpleete::UByte(10),
            Kmpleete::UByte(11),
            Kmpleete::UByte(12),
            Kmpleete::UByte(13),
            Kmpleete::UByte(14)
        };

        std::ofstream outputFile(path, std::ios::binary);
        if (outputFile.is_open())
        {
            outputFile.write(reinterpret_cast<const char*>(contentToWrite), sizeof(contentToWrite));
            outputFile.close();

            Kmpleete::Vector<Kmpleete::UByte> contentToRead;
            REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
            REQUIRE_FALSE(contentToRead.empty());
            REQUIRE(contentToRead.size() == size_t(5));
        }
        else
        {
            FAIL();
        }
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write string function", "[core][filesystem]")
{
    SECTION("WriteFile empty string")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String emptyString;
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, emptyString, true));
    }

    SECTION("WriteFile valid string and path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String string = "string";
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, string, true));
    }

    SECTION("WriteFile invalid file valid string")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String string = "string";
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, string, true));
    }

    SECTION("WriteFile append string")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String string = "string";
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, string, true));

        Kmpleete::String contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, string, true));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == "stringstring");
    }

    SECTION("WriteFile truncate string")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::String string = "string";
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, string, false));

        Kmpleete::String contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, string, false));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write binary buffer function", "[core][filesystem]")
{
    SECTION("WriteFile empty binary buffer")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer emptyBuffer;
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, emptyBuffer, true));
    }

    SECTION("WriteFile valid buffer and path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, true));
    }

    SECTION("WriteFile invalid file valid buffer")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, true));
    }

    SECTION("WriteFile append binary buffer")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, true));

        Kmpleete::Vector<Kmpleete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, true));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 8ULL);
    }

    SECTION("WriteFile truncate binary buffer")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, false));

        Kmpleete::Vector<Kmpleete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer, false));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write binary buffer 32 function", "[core][filesystem]")
{
    SECTION("WriteFile empty binary buffer 32")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer32 emptyBuffer32;
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, emptyBuffer32, true));
    }

    SECTION("WriteFile valid buffer 32 and path")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, true));

        auto binaryBuffer = Kmpleete::Filesystem::ReadFileAsBinary(path);
        Kmpleete::UInt32* dataPtr = reinterpret_cast<Kmpleete::UInt32*>(binaryBuffer.data());
        REQUIRE(*dataPtr == 10);
        dataPtr++;
        REQUIRE(*dataPtr == 11);
        dataPtr++;
        REQUIRE(*dataPtr == 12);
        dataPtr++;
        REQUIRE(*dataPtr == 13);
    }

    SECTION("WriteFile invalid file valid buffer 32")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE_FALSE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, true));
    }

    SECTION("WriteFile append binary buffer 32")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, true));

        Kmpleete::Vector<Kmpleete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmpleete::UInt32));

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, true));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 8ULL * sizeof(Kmpleete::UInt32));
    }

    SECTION("WriteFile truncate binary buffer 32")
    {
        const auto path = Kmpleete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmpleete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(path));

        Kmpleete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, false));

        Kmpleete::Vector<Kmpleete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmpleete::UInt32));

        REQUIRE(Kmpleete::Filesystem::WriteFile(path, binaryBuffer32, false));
        REQUIRE_NOTHROW(contentToRead = Kmpleete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmpleete::UInt32));
    }
}
//--------------------------------------------------------------------------

#if defined (KMP_UNDEF_CreateFile)
    #pragma pop_macro("CreateFile")
    #undef KMP_UNDEF_CreateFile
#endif
#if defined (KMP_UNDEF_CopyFile)
    #pragma pop_macro("CopyFile")
    #undef KMP_UNDEF_CopyFile
#endif