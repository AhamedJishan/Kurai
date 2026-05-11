#include "Keyboard.h"

namespace Dawn
{
	bool Keyboard::GetKey(Key key) const
	{
		return mCurrentState[static_cast<int>(key)];
	}

	bool Keyboard::GetKeyDown(Key key) const
	{
		return !mPreviousState[static_cast<int>(key)] && mCurrentState[static_cast<int>(key)];
	}
	
	bool Keyboard::GetKeyUp(Key key) const
	{
		return mPreviousState[static_cast<int>(key)] && !mCurrentState[static_cast<int>(key)];
	}
}