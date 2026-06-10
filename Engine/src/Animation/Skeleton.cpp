#include "Skeleton.h"

namespace Dawn
{
	Skeleton::Skeleton(const Pose & bindPose, const std::vector<glm::mat4>& invBindPoseMatrices, const std::vector<std::string>& jointNames)
	{
		mBindPose = bindPose;
		mJointNames = jointNames;
		mInvBindPoseMatrices = invBindPoseMatrices;
	}
	
	const Pose& Skeleton::GetBindPose()
	{
		return mBindPose;
	}
	
	const std::string& Skeleton::GetJointName(unsigned int index)
	{
		return mJointNames[index];
	}
	
	const std::vector<std::string>& Skeleton::GetJointNames()
	{
		return mJointNames;
	}
	
	const std::vector<glm::mat4> Skeleton::GetInvBindPoseMatrices()
	{
		return mInvBindPoseMatrices;
	}
}