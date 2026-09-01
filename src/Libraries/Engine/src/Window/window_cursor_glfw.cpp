#include "Kmpleete/Window/window_cursor_glfw.h"
#include "Kmpleete/Graphics/image.h"
#include "Kmpleete/Base/exception.h"
#include "Kmpleete/Core/assertion.h"
#include "Kmpleete/Log/log.h"
#include "Kmpleete/Profile/profiler.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace Kmpleete
{
    WindowCursorGlfw::WindowCursorGlfw(const Filepath& filepath, const Math::Point2I& hotspot /*= Math::Point2I()*/)
        : _cursor(nullptr)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        try
        {
            Graphics::Image img(filepath, Graphics::ImageChannels::RGBAlpha);
            GLFWimage cursorImg{ img.GetWidth(), img.GetHeight(), img.GetPixels() };

            if (cursorImg.pixels && cursorImg.width > 0 && cursorImg.height > 0)
            {
                _cursor = glfwCreateCursor(&cursorImg, hotspot.x, hotspot.y);
            }
            else
            {
                KMP_LOG_WARN("cannot create cursor object from '{}'", filepath);
                throw RuntimeError("WindowCursorGlfw object creation failed");
            }
        }
        catch (const Exception&)
        {
            KMP_LOG_WARN("cannot create cursor image from '{}'", filepath);
            throw RuntimeError("WindowCursorGlfw image creation failed");
        }

        KMP_ASSERT(_cursor);
    }
    //--------------------------------------------------------------------------

    WindowCursorGlfw::~WindowCursorGlfw() KMP_PROFILING(ProfileLevelAlways)
    {
        if (_cursor)
        {
            glfwDestroyCursor(_cursor);
        }
    }}
    //--------------------------------------------------------------------------

    NonNull<void*> WindowCursorGlfw::GetImplPointer() const noexcept
    {
        KMP_ASSERT(_cursor);

        return _cursor;
    }
    //--------------------------------------------------------------------------
}
