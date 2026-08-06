#pragma once

namespace Dawn
{
    enum class Key : int
    {
        Unknown = -1,
        First = 32,

        /* Printable keys */
        Space = 32,
        Apostrophe = 39,    // '
        Comma = 44,         // ,
        Minus = 45,         // -
        Period = 46,        // .
        Slash = 47,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59,     // ;
        Equal = 61,         // =
        A = 65, B = 66, C = 67, D = 68, E = 69,
        F = 70, G = 71, H = 72, I = 73, J = 74,
        K = 75, L = 76, M = 77, N = 78, O = 79,
        P = 80, Q = 81, R = 82, S = 83, T = 84,
        U = 85, V = 86, W = 87, X = 88, Y = 89, Z = 90,

        LeftBracket = 91,  // [
        Backslash = 92, // \
                RightBracket = 93,  // ]
        GraveAccent = 96,  // `
        World1 = 161,
        World2 = 162,

        /* Function keys */
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,

        F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10,
        F11, F12, F13, F14, F15, F16, F17, F18, F19, F20,
        F21, F22, F23, F24, F25,

        KP_0 = 320, KP_1, KP_2, KP_3, KP_4,
        KP_5, KP_6, KP_7, KP_8, KP_9,
        KP_Decimal = 330,
        KP_Divide,
        KP_Multiply,
        KP_Subtract,
        KP_Add,
        KP_Enter,
        KP_Equal,

        LeftShift = 340,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,

        Last = Menu
    };
}
