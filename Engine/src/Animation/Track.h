#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Frame.h"
#include "Interpolation.h"

namespace Dawn
{
	template<typename T, unsigned int N>
	class Track
	{
	public:
		Track();

		T Sample(float time, bool looping);
		Frame<N>& operator[](unsigned int index);

		void SetSize(unsigned int size);
		void SetInterpolation(Interpolation interpolation);

		unsigned int GetSize();
		Interpolation GetInterpolation();
		float GetStartTime();
		float GetEndTime();

	protected:
		T SampleConstant(float time, bool looping);
		T SampleLinear(float time, bool looping);
		T SampleCubic(float time, bool looping);

		float Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);
		// returns the last frame right before the requested time
		int FrameIndex(float time, bool looping);
		float AdjustTimeToFitTrack(float time, bool looping);
		T Cast(float* value);

	protected:
		std::vector<Frame<N>> mFrames;
		Interpolation mInterpolation;
	};


	typedef Track<float, 1> ScalarTrack;
	typedef Track<glm::vec2, 2> Vec2Track;
	typedef Track<glm::vec3, 3> Vec3Track;
	typedef Track<glm::quat, 4> QuaternionTrack;
}