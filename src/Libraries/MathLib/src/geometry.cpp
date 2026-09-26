#include "Kmpleete/Math/geometry.h"


namespace Kmpleete
{
    namespace Math
    {
        Vec2F ToNDC(float px, float py, const Vec2F& framebufferSize)
        {
            if (framebufferSize.x == 0.0f || framebufferSize.y == 0.0f)
            {
                return Vec2F();
            }

            return Vec2F((px / framebufferSize.x) * 2.0f - 1.0f, (py / framebufferSize.y) * 2.0f - 1.0f);
        }
        //--------------------------------------------------------------------------
    }
}