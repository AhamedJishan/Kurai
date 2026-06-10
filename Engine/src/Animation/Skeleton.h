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
		Skeleton(const Pose& bindPose, const std::vector<glm::mat4>& invBindPoseMatrices, const std::vector<std::string>& jointNames);

		const Pose& GetBindPose();
		const std::string& GetJointName(unsigned int index);
		const std::vector<std::string>& GetJointNames();
		const std::vector<glm::mat4> GetInvBindPoseMatrices();

	protected:
		Pose mBindPose;
		std::vector<std::string> mJointNames;
		std::vector<glm::mat4> mInvBindPoseMatrices;
	};
}