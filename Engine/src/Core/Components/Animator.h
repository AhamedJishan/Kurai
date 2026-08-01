#pragma once

#include <Core/Component.h>

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

		void SetSkeleton(const Skeleton* skeleton);
		void AddClip(const std::string& clipName, Clip* clip) { mClips.emplace(clipName, clip); }

		const std::vector<glm::mat4>& GetMatrixPalette() const { return mMatrixPalette; }

		void Serialize(YAML::Node& node) const override;
		void Deserialize(const YAML::Node& node) override;

	private:
		std::vector<glm::mat4> BuildGlobalTransforms(const std::vector<glm::mat4>& localTransforms);

	private:
		const Skeleton* mSkeleton = nullptr;
		std::unordered_map<std::string, Clip*> mClips;
		std::vector<glm::mat4> mMatrixPalette;

		std::string mActiveClipName;
		float mPlaybackTime = 0.0f;
	};
}