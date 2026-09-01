#include "Kmpleete/Time/helper_functions.h"
#include "Kmpleete/Base/pointers.h"
#include "Kmpleete/Base/types_aliases.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("GetCurrentTimeString default format", "[time][clock]")
{
    Kmpleete::String result;
    REQUIRE_NOTHROW(result = Kmpleete::Time::GetCurrentTimeString());
    REQUIRE(not result.empty());
}

TEST_CASE("GetCurrentTimeString nullptr format", "[time][clock]")
{
    Kmpleete::String result;
    REQUIRE_NOTHROW(result = Kmpleete::Time::GetCurrentTimeString(nullptr));
    REQUIRE(result.empty());
}

TEST_CASE("GetCurrentTimeString empty format", "[time][clock]")
{
    Kmpleete::String result;
    REQUIRE_NOTHROW(result = Kmpleete::Time::GetCurrentTimeString(""));
    REQUIRE(result.empty());
}

TEST_CASE("GetCurrentTimeString invalid format", "[time][clock]")
{
    Kmpleete::String result;
    REQUIRE_NOTHROW(result = Kmpleete::Time::GetCurrentTimeString("garbage"));
    REQUIRE(not result.empty());
    REQUIRE(result == "garbage");
}
//--------------------------------------------------------------------------