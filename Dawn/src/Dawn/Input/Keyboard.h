#pragma once

#include "KeyCodes.h"

namespace Dawn
{
	class Keyboard
	{
	public:
		bool GetKey(Key key) const;
		bool GetKeyUp(Key key) const;
		bool GetKeyDown(Key key) const;

	private:
		friend class InputSystem;

		bool mPreviousState[static_cast<int>(Key::Last) + 1] = {};
		bool mCurrentState[static_cast<int>(Key::Last) + 1] = {};
	};
}