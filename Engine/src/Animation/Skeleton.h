#pragma once

#include <vector>
#include <string>
#include <glm/mat4x4.hpp>
#include "Pose.h"

namespace Dawn
{
	class Skeleton
	{
	public:
		Skeleton(const Pose& bindPose, 
				const std::vector<int>& parents,
				const std::vector<std::string>& jointNames,
				const std::vector<glm::mat4>& invBindPoseMatrices);

		const Pose& GetBindPose() const;
		int GetNumBones() const { return mJointNames.size(); }
		int GetParent(unsigned int index) const;
		const std::vector<int> GetParents() const;
		int GetJointId(const std::string& jointName) const;
		const std::string& GetJointName(unsigned int index) const;
		const std::vector<std::string>& GetJointNames() const;
		const std::vector<glm::mat4>& GetInvBindPoseMatrices() const;

	protected:
		Pose mBindPose;
		std::vector<int> mParents;
		std::vector<std::string> mJointNames;
		std::vector<glm::mat4> mInvBindPoseMatrices;
	};
}