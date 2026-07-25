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
		~Animator();

		void Update(float deltaTime) override;

		void Play(const std::string& clipName);

		void SetSkeleton(const Skeleton* skeleton);
		void AddClip(const std::string& clipName, Clip* clip) { mAnimations.emplace(clipName, clip); }

		const std::vector<glm::mat4>& GetMatrixPalette() const { return mMatrixPalette; }

	protected:
		Animator(Actor* owner);

		std::vector<glm::mat4> BuildGlobalTransforms(const std::vector<glm::mat4>& localTransforms);

	private:
		const Skeleton* mSkeleton = nullptr;
		std::unordered_map<std::string, Clip*> mAnimations;
		std::vector<glm::mat4> mMatrixPalette;

		Clip* mActiveClip = nullptr;
		float mPlaybackTime = 0.0f;
	};
}