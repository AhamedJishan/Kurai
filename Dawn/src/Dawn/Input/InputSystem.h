#pragma once

#include "Keyboard.h"
#include "Mouse.h"

// Forward declarations
struct GLFWwindow;

namespace Dawn
{
	class InputSystem
	{
	public:
		void Init();

		void Update();

		const Keyboard& GetKeyBoard() { return mKeyboard; }
		Mouse& GetMouse() { return mMouse; }	// non const since ScrollCallbackFn needs it

		void SetCursorLocked(bool value);

	private:
		static void ScrollCallbackFn(GLFWwindow* window, double xOffset, double yOffset);

	private:
		Keyboard mKeyboard;
		Mouse mMouse;
	};
}