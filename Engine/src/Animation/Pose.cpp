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

	int Pose::AddJoint(const Transform& localTransform, int parentIndex)
	{
		mJoints.push_back(localTransform);
		mParents.push_back(parentIndex);
		return mJoints.size();
	}

	unsigned int Pose::GetSize()
	{
		return mJoints.size();
	}

	void Pose::SetSize(unsigned int size)
	{
		mParents.resize(size);
		mJoints.resize(size);
	}

	int Pose::GetParent(unsigned int index)
	{
		return mParents[index];
	}

	void Pose::SetParent(unsigned int index, int parent)
	{
		mParents[index] = parent;
	}

	const Transform& Pose::GetLocalTransform(unsigned int index)
	{
		return mJoints[index];
	}

	void Pose::SetLocalTransform(unsigned int index, const Transform& transform)
	{
		mJoints[index] = transform;
	}

	Transform Pose::GetGlobalTransform(unsigned int index)
	{
		Transform result = mJoints[index];

		for (int p = mParents[index]; p >= 0; p = mParents[p])
			result = Combine(mJoints[p], result);

		return result;
	}

	Transform Pose::operator[](unsigned int index)
	{
		return mJoints[index];
	}

	void Pose::GetMatrixPalette(std::vector<glm::mat4>& out)
	{
		unsigned int size = GetSize();
		if (size != out.size())
			out.resize(size);

		for (int i = 0; i < size; i++)
			out[i] = TransformToMat4(GetGlobalTransform(i));
	}

	Pose& Pose::operator=(const Pose & pose)
	{
		if (this == &pose)
			return *this;

		if (mParents.size() != pose.mParents.size())
			mParents.resize(pose.mParents.size());
		if (mJoints.size() != pose.mJoints.size())
			mJoints.resize(pose.mJoints.size());

		if (mParents.size() != 0)
			memcpy(&mParents[0], &pose.mParents[0], sizeof(int) * mParents.size());
		if (mJoints.size() != 0)
			memcpy(&mJoints[0], &pose.mJoints[0], sizeof(Transform) * mJoints.size());

		return *this;
	}

	bool Pose::operator==(const Pose& other)
	{
		if (mJoints.size() != other.mJoints.size())
			return false;
		if (mParents.size() != other.mParents.size())
			return false;

		unsigned int size = mJoints.size();
		for (unsigned int i = 0; i < size; i++)
		{
			Transform thisLocal = mJoints[i];
			Transform otherLocal = other.mJoints[i];
			int thisParent = mParents[i];
			int otherParent = other.mParents[i];

			if (thisParent != otherParent)
				return false;

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