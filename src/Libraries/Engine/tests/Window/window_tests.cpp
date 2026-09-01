#include "Kmpleete/Window/window_backend.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/FileDialogs/file_dialogs.h"
#include "Kmpleete/Event/event.h"
#include "Kmpleete/Event/window_events.h"
#include "Kmpleete/Event/key_events.h"
#include "Kmpleete/Event/event_dispatcher.h"
#include "Kmpleete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>


using namespace Kmpleete;
struct WindowCallbackUserSingleCondition
{
    WindowCallbackUserSingleCondition(Kmpleete::Window& window)
        : window(window)
    {
        window.SetEventCallback(KMP_BIND(WindowCallbackUserSingleCondition::Callback));
    }

    void Callback(Kmpleete::Events::Event& evt)
    {
        if (evt.GetTypeID() == "WindowCloseEvent"_sid)
        {
            window.SetShouldClose(true);
        }
        else if (evt.GetTypeID() == "KeyPressEvent"_sid)
        {
            auto keyEvent = static_cast<Events::KeyPressEvent&>(evt);
            if (keyEvent.GetKeyCode() == Kmpleete::Input::Code::Key_Y)
            {
                conditionOk = true;
            }
            window.SetShouldClose(true);
        }
    }

    Kmpleete::Window& window;
    bool conditionOk = false;
};
//--------------------------------------------------------------------------


struct TestStartResult
{
    Kmpleete::UPtr<Kmpleete::WindowBackend> windowBackend = nullptr;
    Kmpleete::Window& mainWindow;
    bool mainWindowNameIsMain = false;

    static TestStartResult InitializeTestData()
    {
        auto windowBackend = Kmpleete::WindowBackend::Create(Kmpleete::Graphics::GraphicsBackendType::Vulkan);
        auto& mainWindow = windowBackend->CreateMainWindow();

        return TestStartResult{ std::move(windowBackend), mainWindow, mainWindow.GetName() == "Main" };
    }
};
//--------------------------------------------------------------------------


TEST_CASE("Multiple windows test", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("Multiple windows test",
        "Make sure both windows can be resized, moved, hid. Then close both windows",
        Kmpleete::FileDialogs::MessageChoice::Ok);

    auto [windowBackend, mainWindow, windowNameIsMain] = TestStartResult::InitializeTestData();
    REQUIRE((windowBackend && windowNameIsMain));

    auto auxWindow = windowBackend->CreateAuxWindow("Aux");
    REQUIRE(auxWindow);
    auxWindow->SetTitle("Aux window");

    while (not mainWindow.ShouldClose())
    {
        mainWindow.FetchEvents();
        mainWindow.SwapBuffers();

        if (auxWindow && not auxWindow->ShouldClose())
        {
            auxWindow->FetchEvents();
            auxWindow->SwapBuffers();
        }
        else
        {
            windowBackend->DestroyAuxWindow("Aux");
            auxWindow = nullptr;
        }
    }

    SUCCEED();
}
//--------------------------------------------------------------------------


TEST_CASE("Window create via existing valid WindowSettings", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("Window with premade settings",
        "Press Y if window is 200x200, otherwise - any other key",
        Kmpleete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmpleete::WindowBackend::Create(Kmpleete::Graphics::GraphicsBackendType::Vulkan);
    REQUIRE(windowBackend);

    Kmpleete::Window::WindowSettings settings;
    settings.name = "Some window";
    settings.size = { 200, 200 };
    settings.windowedSize = { 200, 200 };
    settings.updateContinuously = true;

    Kmpleete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings));
    REQUIRE(window);

    WindowCallbackUserSingleCondition windowCb(*window);

    while (not window->ShouldClose())
    {
        window->FetchEvents();
        window->SwapBuffers();
    }

    REQUIRE(windowCb.conditionOk);
}
//--------------------------------------------------------------------------


TEST_CASE("Window create via existing invalid WindowSettings", "[core][window_backend][window]")
{
    KMP_MB_UNUSED const auto res = Kmpleete::FileDialogs::OpenMessage("Window with invalid settings",
        "Close this window",
        Kmpleete::FileDialogs::MessageChoice::Ok);

    const auto windowBackend = Kmpleete::WindowBackend::Create(Kmpleete::Graphics::GraphicsBackendType::Vulkan);
    REQUIRE(windowBackend);

    Kmpleete::Window::WindowSettings settings;
    REQUIRE(settings.name == "");

    Kmpleete::Window* window;
    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings)); //expect exception during creation but catching it in window backend
    REQUIRE_FALSE(window);

    Kmpleete::Window::WindowSettings settings2;
    settings2.name = "ValidName-InvalidWidth";
    settings2.size = { 65000, 200 };

    REQUIRE_NOTHROW(window = windowBackend->CreateAuxWindow(settings2));
    REQUIRE(window);

    while (not window->ShouldClose())
    {
        window->FetchEvents();
        window->SwapBuffers();
    }

    SUCCEED();
}
//--------------------------------------------------------------------------