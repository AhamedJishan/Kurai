#include "Animator.h"

#include <Dawn/Asset/Assets.h>
#include <Dawn/Animation/Skeleton.h>
#include <Dawn/Animation/Clip.h>
#include <Dawn/Animation/Pose.h>
#include <Dawn/Utils/Log.h>

namespace Dawn
{
	std::vector<Property> Animator::GetProperties()
	{
		return 
		{
			{"Skeleton", &mSkeletonAssetPath, PropertyType::String},
			{"Clips", &mClipNameToPathCache, PropertyType::StringPairList},
			{"ActiveClip", &mActiveClipName, PropertyType::String}
		};
	}

	void Animator::OnPropertiesChanged()
	{
		if (!mSkeletonAssetPath.empty())
			SetSkeleton(mSkeletonAssetPath);

		mClips.clear();
		for (auto& [clipName, clipAssetPath] : mClipNameToPathCache)
			if (!clipName.empty() && !clipAssetPath.empty())
				AddClip(clipName, Assets::GetAnimationClip(clipAssetPath));

		if (!mActiveClipName.empty())
			Play(mActiveClipName);
	}

	Animator::Animator(Actor* owner)
		:Component(owner)
	{
	}
	
	Animator::~Animator()
	{
	}

	void Animator::Update(float deltaTime)
	{
		if (mSkeleton == nullptr)
			return;

		Pose animatedPose = mSkeleton->GetBindPose();	// giving bind pose as default pose

		if (!mActiveClipName.empty() && mClips.count(mActiveClipName) != 0)
			mClips[mActiveClipName]->Sample(animatedPose, mPlaybackTime);

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
		auto it = mClips.find(clipName);
		if (it == mClips.end())
		{
			LOG_WARN("Couldn't find a clip named '%s'", clipName.c_str());
			mActiveClipName = "";
			return;
		}

		mPlaybackTime = 0.0f;
		mActiveClipName = clipName;
	}

	void Animator::SetSkeleton(const std::string& skeletonAssetPath)
	{
		Skeleton* skeleton = Assets::GetSkeleton(skeletonAssetPath);
		if (!skeleton)
		{
			LOG_WARN("Tried to assign an empty skeleton.");
			mSkeletonAssetPath = "";
			mSkeleton = nullptr;
			return;
		}
		mSkeleton = skeleton; 
		mSkeletonAssetPath = skeletonAssetPath;
		mMatrixPalette.resize(skeleton->GetNumBones());
	}

	void Animator::AddClip(const std::string& clipName, Clip* clip)
	{
		if (!clip)
		{
			LOG_ERROR("Clip '%s' is an invalid clip", clipName.c_str());
			return;
		}
		mClips.emplace(clipName, clip);
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