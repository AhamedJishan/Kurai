#include "Clip.h"

namespace Dawn
{
	Clip::Clip()
	{
		mName = "--UnNamed--";
		mStartTime = 0.0f;
		mEndTime = 0.0f;
		mLooping = true;
	}

	unsigned int Clip::GetSize()
	{
		return mTracks.size();
	}
	
	unsigned int Clip::GetIdAtIndex(unsigned int index)
	{
		return mTracks[index].GetId();
	}
	
	void Clip::SetIdAtIndex(unsigned int index, unsigned int id)
	{
		mTracks[index].SetId(id);
	}
	
	float Clip::Sample(Pose & outPose, float time)
	{
		if (GetDuration() == 0.0f)
			return 0.0f;

		time = AdjustTimeToFitRange(time);

		for (unsigned int i = 0; i < mTracks.size(); i++)
		{
			unsigned int jointId = GetIdAtIndex(i);
			Transform local = outPose.GetLocalTransform(jointId);
			Transform animated = mTracks[i].Sample(local, time, mLooping);
			outPose.SetLocalTransform(jointId, animated);
		}

		return time;
	}
	
	TransformTrack& Clip::operator[](unsigned int joint)
	{
		unsigned int size = mTracks.size();
		for (unsigned int i = 0; i < size; i++)
		{
			if (mTracks[i].GetId() == joint)
				return mTracks[i];
		}

		mTracks.push_back(TransformTrack());
		mTracks[size - 1].SetId(joint);
		return mTracks[size - 1];
	}
	
	void Clip::RecalculateDuration()
	{
		mStartTime = 0.0f;
		mEndTime = 0.0f;
		bool startSet = false;
		bool endSet = false;

		for (unsigned int i = 0; i < mTracks.size(); i++)
		{
			float startTime = mTracks[i].GetStartTime();
			float endTime = mTracks[i].GetEndTime();

			if (startTime < mStartTime || !startSet)
			{
				mStartTime = startTime;
				startSet = true;
			}
			if (endTime > mEndTime || !endSet)
			{
				mEndTime = endTime;
				endSet = true;
			}
		}
	}
	
	const std::string& Clip::GetName()
	{
		return mName;
	}
	
	void Clip::SetName(const std::string& name)
	{
		mName = name;
	}
	
	float Clip::GetDuration()
	{
		return mEndTime - mStartTime;
	}
	
	float Clip::GetStartTime()
	{
		return mStartTime;
	}
	
	float Clip::GetEndTime()
	{
		return mEndTime;
	}
	
	bool Clip::GetLooping()
	{
		return mLooping;
	}
	
	void Clip::SetLooping(bool isLooping)
	{
		mLooping = isLooping;
	}
	
	float Clip::AdjustTimeToFitRange(float inTime)
	{
		if (mLooping)
		{
			float duration = mEndTime - mStartTime;
			if (duration < 0.0f)
				return 0.0f;

			inTime = fmodf(inTime - mStartTime, duration);
			if (inTime < 0.0f)
				inTime += duration;
			inTime += mStartTime;
		}
		else
		{
			if (inTime < mStartTime)
				return mStartTime;
			if (inTime > mEndTime)
				return mEndTime;
		}

		return inTime;
	}
}