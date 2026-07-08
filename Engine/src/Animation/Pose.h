#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include "Transform.h"

namespace Dawn
{
	class Pose
	{
	public:
		Pose();
		Pose(const Pose& pose);
		Pose(unsigned int numJoints);

		// returns index of the joint added
		int AddJoint(const Transform& localTransform, int parentIndex);

		unsigned int GetSize();
		void SetSize(unsigned int size);

		int GetParent(unsigned int index);
		void SetParent(unsigned int index, int parent);

		const Transform& GetLocalTransform(unsigned int index);
		void SetLocalTransform(unsigned int index, const Transform& transform);

		Transform GetGlobalTransform(unsigned int index);
		Transform operator[](unsigned int index);

		void GetMatrixPalette(std::vector<glm::mat4>& out);

		Pose& operator=(const Pose& pose);
		bool operator==(const Pose& other);
		bool operator!=(const Pose& other);

	public:
		std::vector<Transform> mJoints;
		std::vector<int> mParents;
	};
}