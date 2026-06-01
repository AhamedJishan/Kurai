#include "TransformTrack.h"

namespace Dawn
{
	TransformTrack::TransformTrack()
	{
		mId = 0;
	}

	void TransformTrack::SetId(unsigned int id)
	{
		mId = id;
	}
	
	unsigned int TransformTrack::GetId()
	{
		return mId;
	}
	
	Vec3Track& TransformTrack::GetScaleTrack()
	{
		return mScale;
	}
	
	Vec3Track& TransformTrack::GetPositionTrack()
	{
		return mPosition;
	}
	
	QuaternionTrack& TransformTrack::GetRotationTrack()
	{
		return mRotation;
	}
	
	float TransformTrack::GetStartTime()
	{
		float result = 0.0f;
		bool isSet = false;

		if (mScale.GetSize() > 1)
		{
			result = mScale.GetStartTime();
			isSet = true;
		}

		if (mPosition.GetSize() > 1)
		{
			float positionStartTime = mPosition.GetStartTime();
			if (positionStartTime < result || !isSet)
			{
				result = positionStartTime;
				isSet = true;
			}
		}

		if (mRotation.GetSize() > 1)
		{
			float rotationStartTime = mRotation.GetStartTime();
			if (rotationStartTime < result || !isSet)
			{
				result = rotationStartTime;
				isSet = true;
			}
		}

		return result;
	}
	
	float TransformTrack::GetEndTime()
	{
		float result = 0.0f;
		bool isSet = false;

		if (mScale.GetSize() > 1)
		{
			result = mScale.GetEndTime();
			isSet = true;
		}

		if (mPosition.GetSize() > 1)
		{
			float positionEndTime = mPosition.GetEndTime();
			if (positionEndTime > result || !isSet)
			{
				result = positionEndTime;
				isSet = true;
			}
		}

		if (mRotation.GetSize() > 1)
		{
			float rotationEndTime = mRotation.GetEndTime();
			if (rotationEndTime > result || !isSet)
			{
				result = rotationEndTime;
				isSet = true;
			}
		}

		return result;
	}
	
	bool TransformTrack::IsValid()
	{
		return mPosition.GetSize() > 1 || mRotation.GetSize() > 1 || mScale.GetSize() > 1;
	}

	Transform TransformTrack::Sample(const Transform& ref, float time, bool looping)
	{
		Transform result = ref;

		if (mScale.GetSize() > 1)
			result.Scale = mScale.Sample(time, looping);

		if (mRotation.GetSize() > 1)
			result.Rotation = mRotation.Sample(time, looping);

		if (mPosition.GetSize() > 1)
			result.Position = mPosition.Sample(time, looping);

		return Transform();
	}
}