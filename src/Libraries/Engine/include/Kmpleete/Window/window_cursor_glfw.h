#pragma once

#include "Kmpleete/Window/window_cursor.h"
#include "Kmpleete/Math/geometry.h"
#include "Kmpleete/Base/types_aliases.h"
#include "Kmpleete/Log/log_class_macro.h"


struct GLFWcursor;


namespace Kmpleete
{
    //! GLFW implementation wrapper of a window cursor.
    //! @see WindowCursor
    //! @see Window
    class KMP_API WindowCursorGlfw : public WindowCursor
    {
        KMP_LOG_CLASSNAME(WindowCursorGlfw)
        KMP_DISABLE_COPY_MOVE(WindowCursorGlfw)

    public:
        explicit WindowCursorGlfw(const Filepath& filepath, const Math::Point2I& hotspot = Math::Point2I());
        ~WindowCursorGlfw();

        KMP_NODISCARD virtual NonNull<void*> GetImplPointer() const noexcept override;

    private:
        GLFWcursor* _cursor;
    };
    //--------------------------------------------------------------------------
}