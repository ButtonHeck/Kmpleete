#include "Kmpleete/Base/string_id.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("StringID of nullptr", "[core][string_id]")
{
    constexpr auto nullSid = Kmpleete::ToStringID(nullptr);
    REQUIRE(nullSid == Kmpleete::StringID(0));
}

TEST_CASE("StringID of empty c-string", "[core][string_id]")
{
    constexpr auto emptySid = Kmpleete::ToStringID("");
    REQUIRE(emptySid == Kmpleete::StringID(0));
}

TEST_CASE("StringID of 0 c-string", "[core][string_id]")
{
    constexpr auto sid = Kmpleete::ToStringID("\0");
    REQUIRE(sid == Kmpleete::StringID(0));
}

TEST_CASE("StringID of non-empty c-string", "[core][string_id]")
{
    constexpr auto sid = Kmpleete::ToStringID("some string");
    REQUIRE(sid != Kmpleete::StringID(0));
}

TEST_CASE("StringID of different c-strings are different", "[core][string_id]")
{
    constexpr auto sid1 = Kmpleete::ToStringID("string1");
    constexpr auto sid2 = Kmpleete::ToStringID("string2");
    REQUIRE(sid1 != Kmpleete::StringID(0));
    REQUIRE(sid2 != Kmpleete::StringID(0));
    REQUIRE(sid1 != sid2);
}

TEST_CASE("StringID of c-string via named variable", "[core][string_id]")
{
    constexpr const char* someText = "some_text";
    constexpr auto sid = Kmpleete::ToStringID(someText);
    REQUIRE(sid != Kmpleete::StringID(0));
}
//--------------------------------------------------------------------------


TEST_CASE("StringID of empty String is 0", "[core][string_id]")
{
    const auto str = Kmpleete::String("");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str)) == Kmpleete::StringID(0));
}

TEST_CASE("StringID of 0 String is 0", "[core][string_id]")
{
    const auto str = Kmpleete::String("\0");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str)) == Kmpleete::StringID(0));
}

TEST_CASE("StringID of an String is non-zero", "[core][string_id]")
{
    const auto str = Kmpleete::String("Test");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str)) != Kmpleete::StringID(0));
}

TEST_CASE("StringID of different Strings are different", "[core][string_id]")
{
    const auto str1 = Kmpleete::String("One");
    const auto str2 = Kmpleete::String("Two");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str1)) != Kmpleete::StringID(0));
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str2)) != Kmpleete::StringID(0));
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str1)) != Kmpleete::ToStringID(KMP_SID_PARAM(str2)));
}
//--------------------------------------------------------------------------


TEST_CASE("_sid literal compilation check", "[core][string_id]")
{
    using namespace Kmpleete; // for _sid literal
    const StringID sid = 0;
    KMP_MB_UNUSED int a = 0;

    // make sure _sid is constexpr (can be used in switch cases)
    switch (sid)
    {
    case "one"_sid:
        a = 1;
        break;

    case ""_sid:
        a = 0;
        break;

    case "very long string that contains important information"_sid:
        a = 999;
        break;
    }
    
    SUCCEED();
}

TEST_CASE("_sid of different c-strings are different", "[core][string_id]")
{
    using namespace Kmpleete;
    REQUIRE("one"_sid != "two"_sid);
}
//--------------------------------------------------------------------------


TEST_CASE("_sid and function of a c-string return same results", "[core][string_id]")
{
    using namespace Kmpleete; // for _sid literal

    constexpr auto sid = Kmpleete::ToStringID("test_test");
    REQUIRE(sid == "test_test"_sid);
}

TEST_CASE("_sid and function of different c-strings return different results", "[core][string_id]")
{
    using namespace Kmpleete; // for _sid literal

    constexpr auto sid = Kmpleete::ToStringID("cat");
    REQUIRE(sid != "dog"_sid);
}

TEST_CASE("_sid and function of same String return same results", "[core][string_id]")
{
    using namespace Kmpleete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str1)) == "Test"_sid);
}

TEST_CASE("_sid and function of different String return different results", "[core][string_id]")
{
    using namespace Kmpleete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmpleete::ToStringID(KMP_SID_PARAM(str1)) != "blah"_sid);
}
//--------------------------------------------------------------------------