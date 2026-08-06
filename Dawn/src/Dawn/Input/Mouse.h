#pragma once

#include <glm/vec2.hpp>
#include "MouseButtonCodes.h"

namespace Dawn
{
	class Mouse
	{
	public:
		bool GetButton(MouseButton button) const;
		bool GetButtonUp(MouseButton button) const;
		bool GetButtonDown(MouseButton button) const;

		glm::vec2 GetPosition() const;
		glm::vec2 GetDeltaPosition() const;

		bool IsCursorLocked() const { return mCursorLocked; }

	private:
		friend class InputSystem;

		// Suggest a better name, to store cursor visibility as a bool
		bool mCursorLocked = false;

		glm::vec2 mPreviousPos = {};
		glm::vec2 mCurrentPos = {};

		bool mPreviousState[static_cast<int>(MouseButton::Last) + 1] = {};
		bool mCurrentState[static_cast<int>(MouseButton::Last) + 1] = {};
	};
}