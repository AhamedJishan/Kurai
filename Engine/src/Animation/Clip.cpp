#include "Clip.h"

namespace Dawn
{
	Clip::Clip()
	{
		mName = "--UnNamed--";
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
	
	float Clip::Sample(Pose& outPose, float time)
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
		mTracks[mTracks.size() - 1].SetId(joint);
		return mTracks[mTracks.size() - 1];
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
		float endTime = 0.0f;
		bool endSet = false;

		for (unsigned int i = 0; i < mTracks.size(); i++)
		{
			float endTime = mTracks[i].GetEndTime();

			if (endTime > endTime || !endSet)
			{
				endTime = endTime;
				endSet = true;
			}
		}
		return endTime;
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
			float duration = GetDuration();
			if (duration < 0.0f)
				return 0.0f;

			inTime = fmodf(inTime, duration);
			if (inTime < 0.0f)
				inTime += duration;
		}
		else
		{
			float duration = GetDuration();

			if (inTime < 0.0f)
				return 0.0f;
			if (inTime > duration)
				return duration;
		}

		return inTime;
	}
}