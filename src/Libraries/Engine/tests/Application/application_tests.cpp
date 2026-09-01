#include "Kmpleete/Application/application.h"
#include "Kmpleete/Application/application_context.h"
#include "Kmpleete/Filesystem/filesystem.h"
#include "Kmpleete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <thread>


namespace Kmpleete
{
    class TestApplication : public Application
    {
    public:
        explicit TestApplication(const ApplicationParameters& parameters)
            : Application(parameters)
        {}

        void Run() override {}
    };

    class MetricsTestApplication : public Application
    {
    public:
        MetricsTestApplication(const ApplicationParameters& parameters)
            : Application(parameters)
        {}

        void Run() override {}
        bool UpdateMetrics() { return _systemMetricsManager->Update(SystemMetricsManager::SystemMetricsUpdateMode::Everything); }

        bool IsMetricsNumProcessorsOk() const { return _systemMetricsManager->GetMetrics().numProcessors > 0; }
        bool IsMetricsNumThreadsOk() const { return _systemMetricsManager->GetMetrics().numThreads > 0; }
        bool IsMetricsCPUUsageOk() const { return _systemMetricsManager->GetMetrics().cpuUsagePercent >= 0.0f; }
        bool IsMetricsTotalPhysicalMemoryOk() const { return _systemMetricsManager->GetMetrics().totalPhysicalMemoryMib > 0.0f; }
        bool IsMetricsPhysicalMemoryUsedOk() const { return _systemMetricsManager->GetMetrics().physicalMemoryUsedMib > 0.0f; }
        bool IsMetricsTotalVirtualMemoryOk() const { return _systemMetricsManager->GetMetrics().totalVirtualMemoryMib > 0.0f; }
        bool IsMetricsVirtualMemoryUsedOk() const { return _systemMetricsManager->GetMetrics().virtualMemoryUsedMib > 0.0f; }
    };
}
//--------------------------------------------------------------------------


TEST_CASE("Test application initialize", "[application]")
{
    {
        const auto application = Kmpleete::CreateUPtr<Kmpleete::TestApplication>(Kmpleete::ApplicationParameters("TestApplication", "", "Kmpleete_unit_tests_settings.json"));

        REQUIRE(application);
        REQUIRE(not Kmpleete::Filesystem::GetCurrentFilepath().empty());
    }

    const auto settingsFilepath = Kmpleete::Filesystem::GetCurrentFilepath().append("Kmpleete_unit_tests_settings.json");
    REQUIRE(Kmpleete::Filesystem::FilepathIsValid(settingsFilepath));
    REQUIRE(Kmpleete::Filesystem::FilepathExists(settingsFilepath));
}
//--------------------------------------------------------------------------


TEST_CASE("Test application name", "[application]")
{
    const auto application = Kmpleete::CreateUPtr<Kmpleete::MetricsTestApplication>(Kmpleete::ApplicationParameters("MetricsTestApplication", "", "Kmpleete_unit_tests_settings.json"));
    REQUIRE(application);
    Kmpleete::String appName = "";
    REQUIRE_NOTHROW(appName = Kmpleete::ApplicationContext::GetApplicationName());
    REQUIRE(not appName.empty());
}
//--------------------------------------------------------------------------


TEST_CASE("Test application metrics update", "[application][metrics]")
{
    const auto application = Kmpleete::CreateUPtr<Kmpleete::MetricsTestApplication>(Kmpleete::ApplicationParameters("MetricsTestApplication", "", "Kmpleete_unit_tests_settings.json"));

    REQUIRE(application);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    REQUIRE(application->UpdateMetrics());
    REQUIRE(application->IsMetricsNumProcessorsOk());
    REQUIRE(application->IsMetricsNumThreadsOk());
    REQUIRE(application->IsMetricsCPUUsageOk());
    REQUIRE(application->IsMetricsPhysicalMemoryUsedOk());
    REQUIRE(application->IsMetricsTotalPhysicalMemoryOk());
    REQUIRE(application->IsMetricsTotalVirtualMemoryOk());
    REQUIRE(application->IsMetricsVirtualMemoryUsedOk());
}
//--------------------------------------------------------------------------