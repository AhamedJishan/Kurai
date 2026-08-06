#pragma once

#include "Track.h"
#include "Dawn/Core/Transform.h"

namespace Dawn
{
	class TransformTrack
	{
	public:
		TransformTrack();

		void SetId(unsigned int id);
		unsigned int GetId();
		Vec3Track& GetScaleTrack();
		Vec3Track& GetPositionTrack();
		QuaternionTrack& GetRotationTrack();
		float GetStartTime();
		float GetEndTime();
		bool IsValid();

		Transform Sample(const Transform& ref, float time, bool looping);

	protected:
		unsigned int mId;
		Vec3Track mScale;
		Vec3Track mPosition;
		QuaternionTrack mRotation;
	};
}