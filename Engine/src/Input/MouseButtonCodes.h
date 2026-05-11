#pragma once

namespace Dawn
{
	enum class MouseButton : int
    {
        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,

        // Aliases (match GLFW semantics)
        Left   = Button1,
        Right  = Button2,
        Middle = Button3,

        First = Button1,
        Last  = Button8
    };
}