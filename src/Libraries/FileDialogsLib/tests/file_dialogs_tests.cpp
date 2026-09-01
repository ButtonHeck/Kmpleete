#include "Kmpleete/FileDialogs/file_dialogs.h"
#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Localization/localization_manager.h"
#include "Kmpleete/Profile/profiler.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("File dialog open single file - OPEN", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmpleete::FileDialogs::OpenFile("Open any single file", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - CANCEL", "[file_dialogs][open]")
{
    const auto file = Kmpleete::FileDialogs::OpenFile("Press cancel", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(file.empty());
}

TEST_CASE("File dialog open single file - OPEN json", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmpleete::FileDialogs::OpenFile("Open any single json file", Kmpleete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::ToGenericString(file).ends_with(".json"));
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name non-existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmpleete::FileDialogs::OpenFile("Open any manually entered non-existing file", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmpleete::FileDialogs::OpenFile("Open any manually entered existing file", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing non-match filter", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmpleete::FileDialogs::OpenFile("Open any manually entered existing file that do not match filter", Kmpleete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmpleete::Filesystem::FilepathExists(file));
}
//--------------------------------------------------------------------------


TEST_CASE("File dialog open multiple files - OPEN", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmpleete::FileDialogs::OpenFiles("Open any multiple files", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmpleete::Filesystem::FilepathIsValid(file);
        ok &= Kmpleete::Filesystem::FilepathExists(file);

        if (not ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - CANCEL", "[file_dialogs][open]")
{
    const auto files = Kmpleete::FileDialogs::OpenFiles("Press cancel", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(files.empty());
}

TEST_CASE("File dialog open multiple files - OPEN json", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmpleete::FileDialogs::OpenFiles("Open any multiple json files", Kmpleete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(not files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmpleete::Filesystem::FilepathIsValid(file);
        ok &= Kmpleete::Filesystem::FilepathExists(file);
        ok &= Kmpleete::Filesystem::ToGenericString(file).ends_with(".json");

        if (not ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names non-existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmpleete::FileDialogs::OpenFiles("Open any manually entered non-existing files", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmpleete::Filesystem::FilepathIsValid(file);
        ok &= not Kmpleete::Filesystem::FilepathExists(file);

        if (not ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmpleete::FileDialogs::OpenFiles("Open any manually entered existing files", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmpleete::Filesystem::FilepathIsValid(file);
        ok &= Kmpleete::Filesystem::FilepathExists(file);

        if (not ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}
//--------------------------------------------------------------------------


TEST_CASE("File dialog save file - SAVE new", "[file_dialogs][save]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any new file", Kmpleete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmpleete::FileDialogs::SaveFile("Save to any new file", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog save file - CANCEL", "[file_dialogs][save]")
{
    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("File dialog save", "In the next dialog press Cancel", Kmpleete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmpleete::FileDialogs::SaveFile("Press Cancel", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(file.empty());
}

TEST_CASE("File dialog save file - SAVE overwrite", "[file_dialogs][save]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any existing file", Kmpleete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmpleete::FileDialogs::SaveFile("Save to any existing file", Kmpleete::Filesystem::GetCurrentFilepath(), { "Any files", "*.*" }, true);
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathExists(file));
    REQUIRE(Kmpleete::Filesystem::IsFile(file));
}

TEST_CASE("File dialog save file - SAVE new non-matching filter", "[file_dialogs][save]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any new file that do not match filter", Kmpleete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmpleete::FileDialogs::SaveFile("Save to any new file that do not match filter", Kmpleete::Filesystem::GetCurrentFilepath(), { "JSON files", "*.json" });
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmpleete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog save file - Cyrillic characters", "[file_dialogs][open]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to new file with cyrillic characters", Kmpleete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmpleete::FileDialogs::SaveFile("Save to new file with cyrillic characters", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not file.empty());
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(file));
}
//--------------------------------------------------------------------------


TEST_CASE("File dialog open folder - OPEN", "[file_dialogs][folder]")
{
    auto localizationManager = Kmpleete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto folder = Kmpleete::FileDialogs::OpenDirectory("Open any directory", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(not folder.empty());
    REQUIRE(Kmpleete::Filesystem::IsDirectory(folder));
}

TEST_CASE("File dialog open folder - CANCEL", "[file_dialogs][folder]")
{
    const auto folder = Kmpleete::FileDialogs::OpenDirectory("Press Cancel", Kmpleete::Filesystem::GetCurrentFilepath());
    REQUIRE(folder.empty());
}
//--------------------------------------------------------------------------


TEST_CASE("Message dialog OK button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press OK");
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Ok);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press OK", Kmpleete::FileDialogs::MessageChoice::Ok);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Ok);
}

TEST_CASE("Message dialog CANCEL button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL");
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmpleete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmpleete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog close button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)");
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);

    // Windows returns OK, Ubuntu's Unity returns Cancel
    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect OK or Cancel)", Kmpleete::FileDialogs::MessageChoice::Ok);
    REQUIRE((messageResult == Kmpleete::FileDialogs::MessageButton::Ok || messageResult == Kmpleete::FileDialogs::MessageButton::Cancel));

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmpleete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmpleete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog YES button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmpleete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmpleete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog NO button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmpleete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::No);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmpleete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::No);
}

TEST_CASE("Message dialog RETRY button", "[file_dialogs][message]")
{
    auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmpleete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Retry);

    messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmpleete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Retry);
}

TEST_CASE("Message dialog ABORT button", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press ABORT", Kmpleete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Abort);
}

TEST_CASE("Message dialog IGNORE button", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press IGNORE", Kmpleete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Ignore);
}
//--------------------------------------------------------------------------


TEST_CASE("Message dialog icon INFO", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like information dialog", 
        Kmpleete::FileDialogs::MessageChoice::YesNo, Kmpleete::FileDialogs::MessageIcon::Info);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon WARNING", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like warning dialog",
        Kmpleete::FileDialogs::MessageChoice::YesNo, Kmpleete::FileDialogs::MessageIcon::Warning);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon ERROR", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like error dialog",
        Kmpleete::FileDialogs::MessageChoice::YesNo, Kmpleete::FileDialogs::MessageIcon::Error);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon QUESTION", "[file_dialogs][message]")
{
    const auto messageResult = Kmpleete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like question dialog",
        Kmpleete::FileDialogs::MessageChoice::YesNo, Kmpleete::FileDialogs::MessageIcon::Question);
    REQUIRE(messageResult == Kmpleete::FileDialogs::MessageButton::Yes);
}
//--------------------------------------------------------------------------
