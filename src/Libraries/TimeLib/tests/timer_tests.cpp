#include "Kmpleete/Time/timer.h"
#include "Kmpleete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>

#include <thread>


static constexpr auto timerTimeMs = 200;


TEST_CASE("Timer creation nothrow", "[time][timer]")
{
    Kmpleete::Ptr<Kmpleete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmpleete::CreatePtr<Kmpleete::Time::Timer>(timerTimeMs));
}
//--------------------------------------------------------------------------


TEST_CASE("Timer ReachedTimeout is correct", "[time][timer]")
{
    Kmpleete::Ptr<Kmpleete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmpleete::CreatePtr<Kmpleete::Time::Timer>(timerTimeMs));

    REQUIRE_FALSE(timer->ReachedTimeout());
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());

    Kmpleete::Ptr<Kmpleete::Time::Timer> timer0 = nullptr;
    REQUIRE_NOTHROW(timer0 = Kmpleete::CreatePtr<Kmpleete::Time::Timer>());
    REQUIRE(timer0->ReachedTimeout());
}
//--------------------------------------------------------------------------


TEST_CASE("Timer GetTimeout is correct", "[time][timer]")
{
    Kmpleete::Ptr<Kmpleete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmpleete::CreatePtr<Kmpleete::Time::Timer>(timerTimeMs));
    REQUIRE(timer->GetTimeout() == timerTimeMs);
}
//--------------------------------------------------------------------------


TEST_CASE("Timer SetTimeout is correct", "[time][timer]")
{
    Kmpleete::Ptr<Kmpleete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmpleete::CreatePtr<Kmpleete::Time::Timer>(timerTimeMs));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE_FALSE(timer->ReachedTimeout());

    timer->SetTimeout(timerTimeMs * 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE_FALSE(timer->ReachedTimeout());

    timer->SetTimeout(timerTimeMs / 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());
}
//--------------------------------------------------------------------------
