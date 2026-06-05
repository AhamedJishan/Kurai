#pragma once

#include <vector>
#include <string>
#include "TransformTrack.h"
#include "Pose.h"

namespace Dawn
{
	class Clip
	{
	public:
		Clip();

		unsigned int GetSize();
		unsigned int GetIdAtIndex(unsigned int index);
		void SetIdAtIndex(unsigned int index, unsigned int id);

		float Sample(Pose& outPose, float time);
		// Mainly used while loading the animation. 
		// Returns the TransformTrack object for a specific joint. 
		// If a track for a specified joint doesn't exist, than it creates a new one.
		TransformTrack& operator[](unsigned int joint);

		void RecalculateDuration();

		const std::string& GetName();
		void SetName(const std::string& name);
		float GetDuration();
		float GetStartTime();
		float GetEndTime();
		bool GetLooping();
		void SetLooping(bool isLooping);

	protected:
		float AdjustTimeToFitRange(float inTime);

	protected:
		std::vector<TransformTrack> mTracks;
		std::string mName;
		float mStartTime;
		float mEndTime;
		bool mLooping;
	};
}