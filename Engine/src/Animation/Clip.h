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

		float GetDuration();
		unsigned int GetSize();
		unsigned int GetIdAtIndex(unsigned int index);
		void SetIdAtIndex(unsigned int index, unsigned int id);

		float Sample(Pose& outPose, float time);
		// Mainly used while loading the animation. 
		// Returns the TransformTrack object for a specific joint. 
		// If a track for a specified joint doesn't exist, than it creates a new one.
		TransformTrack& operator[](unsigned int joint);

		bool GetLooping() const { return mLooping; }
		const std::string& GetName() const { return mName; }
		const std::string& GetAssetPath() const { return mAssetPath; }

		void SetLooping(bool isLooping) { mLooping = isLooping; }
		void SetName(const std::string& name) { mName = name; }
		void SetAssetPath(const std::string& assetPath) { mAssetPath = assetPath; }

	protected:
		float AdjustTimeToFitRange(float inTime);

	protected:
		bool mLooping;
		std::string mName;
		std::string mAssetPath;
		std::vector<TransformTrack> mTracks;
	};
}