#pragma once

#include "Keyboard.h"
#include "Mouse.h"

namespace Dawn
{
	class InputSystem
	{
	public:
		// TBDL: when using callbacks instead of polling
		// void Init();

		void Update();

		const Keyboard& GetKeyBoard() { return mKeyboard; }
		const Mouse& GetMouse() { return mMouse; }

		void SetCursorLocked(bool value);

	private:
		Keyboard mKeyboard;
		Mouse mMouse;
	};
}