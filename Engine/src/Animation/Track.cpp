#include "Track.h"

#include <glm/gtx/spline.hpp>

namespace Dawn
{
	template Track<float, 1>;
	template Track<glm::vec2, 2>;
	template Track<glm::vec3, 3>;
	template Track<glm::quat, 4>;

	template<typename T, unsigned int N>
	Track<T, N>::Track()
	{
		mInterpolation = Interpolation::Linear;
	}

	template<typename T, unsigned int N>
	T Track<T, N>::Sample(float time, bool looping)
	{
		switch (mInterpolation)
		{
		case Dawn::Interpolation::Constant:
			return SampleConstant(time, looping);
			break;
		case Dawn::Interpolation::Linear:
			return SampleLinear(time, looping);
			break;
		default:
			return SampleCubic(time, looping);
			break;
		}
	}
	
	template<typename T, unsigned int N>
	Frame<N>& Track<T, N>::operator[](unsigned int index)
	{
		return mFrames[index];
	}
	
	template<typename T, unsigned int N>
	void Track<T, N>::SetSize(unsigned int size)
	{
		mFrames.resize(size);
	}
	
	template<typename T, unsigned int N>
	void Track<T, N>::SetInterpolation(Interpolation interpolation)
	{
		mInterpolation = interpolation;
	}
	
	template<typename T, unsigned int N>
	unsigned int Track<T, N>::GetSize()
	{
		return mFrames.size();
	}

	template<typename T, unsigned int N>
	Interpolation Track<T, N>::GetInterpolation()
	{
		return mInterpolation;
	}

	template<typename T, unsigned int N>
	float Track<T, N>::GetStartTime()
	{
		return mFrames[0].mTime;
	}

	template<typename T, unsigned int N>
	float Track<T, N>::GetEndTime()
	{
		return mFrames[mFrames.size() - 1].mTime;
	}

	template<typename T, unsigned int N>
	T Track<T, N>::SampleConstant(float time, bool looping)
	{
		int frame = FrameIndex(time, looping);
		if (frame < 0 || frame >= mFrames.size())
			return T();

		return Cast(&mFrames[frame].mValues[0]);
	}

	template<typename T, unsigned int N>
	T Track<T, N>::SampleLinear(float time, bool looping)
	{
		unsigned int thisFrame = FrameIndex(time, looping);
		if (thisFrame < 0 || thisFrame >= mFrames.size() - 1)
			return T();

		float trackTime = AdjustTimeToFitTrack(time, looping);
		float thisFrameTime = mFrames[thisFrame].mTime;
		float deltaFrameTime = mFrames[thisFrame + 1].mTime - thisFrameTime;
		if (deltaFrameTime < 0.0f)
			return T();

		float t = (trackTime - thisFrameTime) / deltaFrameTime;
		T start = Cast(&mFrames[thisFrame].mValues[0]);
		T end = Cast(&mFrames[thisFrame + 1].mValues[0]);
		return glm::mix(start, end, t);
	}
	
	template<typename T, unsigned int N>
	T Track<T, N>::SampleCubic(float time, bool looping)
	{
		return T();
	}
	
	template<typename T, unsigned int N>
	float Track<T, N>::Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2)
	{
		return glm::hermite(p1, s1, p2, s2, time);
	}

	template<typename T, unsigned int N>
	int Track<T, N>::FrameIndex(float time, bool looping)
	{
		unsigned int size = mFrames.size();
		if (size <= 1)
			return -1;

		if (looping)
		{
			float startTime = mFrames[0].mTime;
			float endTime = mFrames[size - 1].mTime;
			float duration = endTime - startTime;

			time = fmodf(time - startTime, duration);

			if (time < 0.0f)
				time += duration;
		}
		else
		{
			if (time <= mFrames[0].mTime)
				return 0;
			else if (time >= mFrames[size - 2].mTime)
				return (int)size - 2;					// -2 beacause we are returning the last frame right before the requested time
		}

		for (int i = (int)size - 1; i >= 0; i--)
		{
			if (time >= mFrames[i].mTime)
				return i;
		}

		// Invalid code, we should not reach here
		return -1;
	}

	template<typename T, unsigned int N>
	float Track<T, N>::AdjustTimeToFitTrack(float time, bool looping)
	{
		unsigned int size  = mFrames.size();
		if (size <= 1)
			return 0.0f;

		float startTime = mFrames[0].mTime;
		float endTime = mFrames[size - 1].mTime;
		float duration = endTime - startTime;

		if (duration <= 0.0f)
			return 0.0f;

		if (looping)
		{
			time = fmodf(time - startTime, duration);
			if (time < 0.0f)
				time += duration;

			time += startTime;
		}
		else
		{
			if (time < mFrames[0].mTime)
				time = mFrames[0].mTime;
			else if (time > mFrames[size - 1].mTime)
				time = mFrames[size - 1].mTime;
		}

		return time;
	}

	template<>
	float Track<float, 1>::Cast(float* value)
	{
		return value[0];
	}
	template<>
	glm::vec2 Track<glm::vec2, 2>::Cast(float* value)
	{
		return glm::vec2(value[0], value[1]);
	}
	template<>
	glm::vec3 Track<glm::vec3, 3>::Cast(float* value)
	{
		return glm::vec3(value[0], value[1], value[2]);
	}
	template<>
	glm::quat Track<glm::quat, 4>::Cast(float* value)
	{
		glm::quat q(value[3], value[0], value[1], value[2]);
		return glm::normalize(q);
	}
}