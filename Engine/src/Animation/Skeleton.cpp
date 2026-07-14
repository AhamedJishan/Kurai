#include "Skeleton.h"

namespace Dawn
{
	Skeleton::Skeleton( const Pose& bindPose,
						const std::vector<int>& parents,
						const std::vector<std::string>& jointNames,
						const std::vector<glm::mat4>& invBindPoseMatrices)
	{
		mBindPose = bindPose;
		mParents = parents;
		mJointNames = jointNames;
		mInvBindPoseMatrices = invBindPoseMatrices;
	}
	
	const Pose& Skeleton::GetBindPose() const
	{
		return mBindPose;
	}

	int Skeleton::GetParent(unsigned int index) const
	{
		return mParents[index];
	}

	const std::vector<int> Skeleton::GetParents() const
	{
		return mParents;
	}

	int Skeleton::GetJointId(const std::string& jointName) const
	{
		for (unsigned int i = 0; i < mJointNames.size(); i++)
			if (jointName == mJointNames[i])
				return static_cast<int>(i);

		return -1;
	}
	
	const std::string& Skeleton::GetJointName(unsigned int index) const
	{
		return mJointNames[index];
	}
	
	const std::vector<std::string>& Skeleton::GetJointNames() const
	{
		return mJointNames;
	}
	
	const std::vector<glm::mat4> Skeleton::GetInvBindPoseMatrices() const
	{
		return mInvBindPoseMatrices;
	}
}