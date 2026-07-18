#include "Animator.h"

#include <Animation/Skeleton.h>
#include <Animation/Clip.h>
#include <Animation/Pose.h>
#include <Utils/Log.h>

namespace Dawn
{
	Animator::Animator(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
	}
	
	Animator::~Animator()
	{
	}

	void Animator::Update(float deltaTime)
	{
		if (mActiveClip == nullptr || mSkeleton == nullptr)
			return;

		Pose animatedPose = mSkeleton->GetBindPose();	// giving bind pose as default pose
		mActiveClip->Sample(animatedPose, mPlaybackTime);

		const std::vector<glm::mat4> localTransforms = animatedPose.GetLocalTransformMatrices();
		const std::vector<glm::mat4> globalTransforms = BuildGlobalTransforms(localTransforms);
		const std::vector<glm::mat4>& invBindPoseMatrices = mSkeleton->GetInvBindPoseMatrices();

		mMatrixPalette.resize(globalTransforms.size(), glm::mat4(1.0f));
		for (size_t i = 0; i < globalTransforms.size(); i++)
			mMatrixPalette[i] = mSkeleton->GetRootGlobalInvMatrix() * globalTransforms[i] * invBindPoseMatrices[i];

		mPlaybackTime += deltaTime;
	}

	void Animator::Play(const std::string& clipName)
	{
		mActiveClip = nullptr;
		mPlaybackTime = 0.0f;

		auto it = mAnimations.find(clipName);
		if (it == mAnimations.end())
		{
			LOG_WARN("Couldn't find a clip named '%s'", clipName.c_str());
			return;
		}

		mActiveClip = it->second;
	}

	void Animator::SetSkeleton(const Skeleton* skeleton)
	{
		if (!skeleton)
		{
			LOG_WARN("Tried to assign an empty skeleton.");
			return;
		}
		mSkeleton = skeleton; 
		mMatrixPalette.resize(skeleton->GetNumBones());
	}


	std::vector<glm::mat4> Animator::BuildGlobalTransforms(const std::vector<glm::mat4>& localTransforms)
	{
		if (!mSkeleton)
			return {};

		std::vector<glm::mat4> globalTransforms(localTransforms.size());

		for (size_t i = 0; i < localTransforms.size(); i++)
		{
			int parentId = mSkeleton->GetParent(i);

			if (parentId == -1)
				globalTransforms[i] = localTransforms[i];
			else
				globalTransforms[i] = globalTransforms[parentId] * localTransforms[i];
		}

		return globalTransforms;
	}
}