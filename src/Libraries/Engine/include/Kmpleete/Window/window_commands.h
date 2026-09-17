#pragma once

#include "Kmpleete/Base/kmpleete_api.h"
#include "Kmpleete/Window/window.h"
#include "Kmpleete/Core/command.h"


namespace Kmpleete
{
    //! Command for switching a window from a windowed to windowed fullscreen mode
    //! and vice versa
    class KMP_API CommandSwitchFullscreen : public Command
    {
    public:
        explicit CommandSwitchFullscreen(Window& window) noexcept;

        void Execute() override;

    private:
        Window& _window;
    };
    //--------------------------------------------------------------------------


    //! Command for switching a window to an "Always on top" mode on/off
    class KMP_API CommandSwitchAlwaysOnTop : public Command
    {
    public:
        explicit CommandSwitchAlwaysOnTop(Window& window) noexcept;

        void Execute() override;

    private:
        Window& _window;
    };
    //--------------------------------------------------------------------------
}