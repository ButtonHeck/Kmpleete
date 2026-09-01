#include "Kmpleete/Graphics/graphics_backend.h"
#include "Kmpleete/Window/window_backend.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


using namespace Kmpleete;
using namespace Kmpleete::Graphics;


TEST_CASE("Graphics backend initialization error type", "[graphics]")
{
    auto windowBackend = Kmpleete::WindowBackend::Create(GraphicsBackendType::Unknown);
    auto& mainWindow = windowBackend->CreateMainWindow();

    UPtr<GraphicsBackend> backend;
    backend = GraphicsBackend::Create(mainWindow);
    REQUIRE(backend == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Graphics backend initialization Vulkan", "[graphics]")
{
    auto windowBackend = Kmpleete::WindowBackend::Create(GraphicsBackendType::Vulkan);
    auto& mainWindow = windowBackend->CreateMainWindow();

    UPtr<GraphicsBackend> backend;
    REQUIRE_NOTHROW(backend = GraphicsBackend::Create(mainWindow));
    REQUIRE(backend);
}
//--------------------------------------------------------------------------