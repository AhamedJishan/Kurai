#pragma once

namespace Dawn
{
	template <unsigned int N>
	class Frame
	{
	public:
		float mValues[N];
		float mTime;
	};

	typedef Frame<1> ScalarFrame;
	typedef Frame<2> Vec2Frame;
	typedef Frame<3> Vec3Frame;
	typedef Frame<4> QuaternionFrame;
}