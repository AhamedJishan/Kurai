#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include "Core/Transform.h"

namespace Dawn
{
	class Pose
	{
	public:
		Pose();
		Pose(const Pose& pose);
		Pose(unsigned int numJoints);

		// returns index of the joint added
		int AddJoint(const Transform& localTransform);

		unsigned int GetSize();
		void SetSize(unsigned int size);

		std::vector<glm::mat4> GetLocalTransformMatrices();
		const Transform& GetLocalTransform(unsigned int index);
		void SetLocalTransform(unsigned int index, const Transform& transform);

		Pose& operator=(const Pose& pose);
		bool operator==(const Pose& other);
		bool operator!=(const Pose& other);

	public:
		std::vector<Transform> mJoints;
	};
}