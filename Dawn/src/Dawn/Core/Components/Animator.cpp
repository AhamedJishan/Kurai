#include "Animator.h"

#include <yaml-cpp/yaml.h>
#include <Dawn/Asset/Assets.h>
#include <Dawn/Animation/Skeleton.h>
#include <Dawn/Animation/Clip.h>
#include <Dawn/Animation/Pose.h>
#include <Dawn/Utils/Log.h>

namespace Dawn
{
	void Animator::Serialize(YAML::Node& node, SerializationContext& serializationContext) const
	{
		node["Skeleton"] = mSkeleton->GetAssetPath();
		node["ActiveClipName"] = mActiveClipName;

		YAML::Node& clipsNode = node["Clips"];
		for (auto& clipEntry : mClips)
			clipsNode[clipEntry.first] = clipEntry.second->GetAssetPath();
	}

	void Animator::Deserialize(const YAML::Node & node, SerializationContext& serializationContext)
	{
		SetSkeleton(Assets::GetSkeleton(node["Skeleton"].as<std::string>()));

		const YAML::Node& clipsNode = node["Clips"];
		for (auto it = clipsNode.begin(); it != clipsNode.end(); it++)
		{
			std::string clipName = it->first.as<std::string>();
			std::string clipAssetPath = it->second.as<std::string>();

			Clip* clip = Assets::GetAnimationClip(clipAssetPath);
			AddClip(clipName, clip);
		}

		Play(node["ActiveClipName"].as<std::string>());
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
		if (mActiveClipName == "" || mSkeleton == nullptr)
			return;

		Pose animatedPose = mSkeleton->GetBindPose();	// giving bind pose as default pose
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
		mActiveClipName = "";
		mPlaybackTime = 0.0f;

		auto it = mClips.find(clipName);
		if (it == mClips.end())
		{
			LOG_WARN("Couldn't find a clip named '%s'", clipName.c_str());
			return;
		}

		mActiveClipName = clipName;
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