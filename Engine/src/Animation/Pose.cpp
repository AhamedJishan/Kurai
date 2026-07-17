#include "Pose.h"

#include <cstring>

namespace Dawn
{
	Pose::Pose()
	{}

	Pose::Pose(const Pose& pose)
	{
		*this = pose;
	}

	Pose::Pose(unsigned int numJoints)
	{
		SetSize(numJoints);
	}

	int Pose::AddJoint(const Transform& localTransform)
	{
		mJoints.push_back(localTransform);
		return mJoints.size();
	}

	unsigned int Pose::GetSize()
	{
		return mJoints.size();
	}

	void Pose::SetSize(unsigned int size)
	{
		mJoints.resize(size);
	}

	std::vector<glm::mat4> Pose::GetLocalTransformMatrices()
	{
		std::vector<glm::mat4> retMatrices(mJoints.size());

		for (unsigned int i = 0; i < mJoints.size(); i++)
			retMatrices[i] = TransformToMat4(mJoints[i]);

		return retMatrices;
	}

	const Transform& Pose::GetLocalTransform(unsigned int index)
	{
		return mJoints[index];
	}

	void Pose::SetLocalTransform(unsigned int index, const Transform& transform)
	{
		mJoints[index] = transform;
	}

	Pose& Pose::operator=(const Pose & pose)
	{
		if (this == &pose)
			return *this;

		if (mJoints.size() != pose.mJoints.size())
			mJoints.resize(pose.mJoints.size());

		if (mJoints.size() != 0)
			memcpy(&mJoints[0], &pose.mJoints[0], sizeof(Transform) * mJoints.size());

		return *this;
	}

	bool Pose::operator==(const Pose& other)
	{
		if (mJoints.size() != other.mJoints.size())
			return false;

		unsigned int size = mJoints.size();
		for (unsigned int i = 0; i < size; i++)
		{
			Transform thisLocal = mJoints[i];
			Transform otherLocal = other.mJoints[i];

			if (thisLocal.Position != otherLocal.Position)
				return false;
			if (thisLocal.Rotation != otherLocal.Rotation)
				return false;
			if (thisLocal.Scale != otherLocal.Scale)
				return false;
		}
	}

	bool Pose::operator!=(const Pose& other)
	{
		return !(*this == other);
	}
}