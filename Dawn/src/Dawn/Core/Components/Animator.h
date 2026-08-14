#pragma once

#include <Dawn/Core/Component.h>

#include <unordered_map>
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

namespace Dawn
{
	// Forward declarations
	class Skeleton;
	class Clip;

	class Animator : public Component
	{
	public:
		Animator(Actor* owner);
		~Animator();

		void Update(float deltaTime) override;

		void Play(const std::string& clipName);

		void SetSkeleton(const std::string& skeletonAssetPath);
		void AddClip(const std::string& clipName, Clip* clip);

		const std::vector<glm::mat4>& GetMatrixPalette() const { return mMatrixPalette; }

		std::vector<Property> GetProperties() override;
		void OnPropertiesChanged() override;

	private:
		std::vector<glm::mat4> BuildGlobalTransforms(const std::vector<glm::mat4>& localTransforms);

	private:
		std::string mSkeletonAssetPath;
		const Skeleton* mSkeleton = nullptr;

		std::vector<std::pair<std::string, std::string>> mClipNameToPathCache;
		std::unordered_map<std::string, Clip*> mClips;

		std::vector<glm::mat4> mMatrixPalette;

		std::string mActiveClipName;
		float mPlaybackTime = 0.0f;
	};
}