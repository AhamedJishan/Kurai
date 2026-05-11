#pragma once

#include <glm/vec2.hpp>
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Dawn
{
	// Facade for InputSystem
	class Input
	{
	public:
		static bool GetKey(Key key);
		static bool GetKeyDown(Key key);
		static bool GetKeyUp(Key key);

		static void SetCursorLocked(bool value);
		static glm::vec2 GetCursorPos();
		static glm::vec2 GetCursorDeltaPos();

		static bool GetMouseButton(MouseButton button);
		static bool GetMouseButtonDown(MouseButton button);
		static bool GetMouseButtonUp(MouseButton button);
	};
}