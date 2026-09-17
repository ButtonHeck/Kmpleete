#include "Kmpleete/Window/window_commands.h"
#include "Kmpleete/Profile/profiler.h"


namespace Kmpleete
{
    CommandSwitchFullscreen::CommandSwitchFullscreen(Window& window) noexcept
        : _window(window) 
    {}
    //--------------------------------------------------------------------------

    void CommandSwitchFullscreen::Execute()
    {
        _window.SetScreenMode(_window.IsWindowedFullscreen() ? Window::ScreenMode::Windowed : Window::ScreenMode::WindowedFullscreen);
    }
    //--------------------------------------------------------------------------


    CommandSwitchAlwaysOnTop::CommandSwitchAlwaysOnTop(Window& window) noexcept
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    void CommandSwitchAlwaysOnTop::Execute()
    {
        _window.SetAlwaysOnTop(not _window.IsAlwaysOnTop());
    }
    //--------------------------------------------------------------------------
}