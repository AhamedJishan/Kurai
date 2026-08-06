#include "Mouse.h"

namespace Dawn
{
	bool Mouse::GetButton(MouseButton button) const
	{
		return mCurrentState[static_cast<int>(button)];
	}

	bool Mouse::GetButtonUp(MouseButton button) const
	{
		return mPreviousState[static_cast<int>(button)] && !mCurrentState[static_cast<int>(button)];
	}

	bool Mouse::GetButtonDown(MouseButton button) const
	{
		return !mPreviousState[static_cast<int>(button)] && mCurrentState[static_cast<int>(button)];
	}

	glm::vec2 Mouse::GetPosition() const
	{
		return mCurrentPos;
	}
	
	glm::vec2 Mouse::GetDeltaPosition() const
	{
		if (mCursorLocked)
			return mCurrentPos;
		else
			return (mCurrentPos - mPreviousPos);
	}
}