#include "InputSystem.h"

#include <cstring>
#include <GLFW/glfw3.h>
#include "Core/Application.h"
#include "Core/Window.h"

namespace Dawn
{
	void InputSystem::Update()
	{
		GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());

		// Update Keyboard state
		std::memcpy(mKeyboard.mPreviousState, mKeyboard.mCurrentState, sizeof(mKeyboard.mCurrentState));

		const int firstKey = static_cast<int>(Key::First);
		const int lastKey = static_cast<int>(Key::Last);

		for (int i = firstKey; i <= lastKey; i++)
		{
			int state = glfwGetKey(window, i);
			bool pressed = state == GLFW_PRESS;
			mKeyboard.mCurrentState[i] = pressed;
		}

		// Update Mouse state
		std::memcpy(mMouse.mPreviousState, mMouse.mCurrentState, sizeof(mMouse.mCurrentState));

		const int firstButton = static_cast<int>(MouseButton::First);
		const int lastButton= static_cast<int>(MouseButton::Last);

		for (int i = firstButton; i <= lastButton; i++)
		{
			int state = glfwGetMouseButton(window, i);
			bool pressed = state == GLFW_PRESS;
			mMouse.mCurrentState[i] = pressed;
		}

		mMouse.mPreviousPos = mMouse.mCurrentPos;

		double xPos = 0.0, yPos = 0.0;
		glfwGetCursorPos(window, &xPos, &yPos);
		mMouse.mCurrentPos = { xPos, yPos };
	}

	void InputSystem::SetCursorLocked(bool value)
	{
		GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());

		int cursorMode = value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
	}
}