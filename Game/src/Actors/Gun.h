#pragma once

#include "Core/Actor.h"
#include "glm/vec3.hpp"
#include "Rendering/ParticleSystem.h"

namespace Dawn
{
	// Forward declarations
	class Player;
	class Scene;
	class Audio;
	class PhongMaterial;

	class Gun : public Actor
	{
	public:
		Gun(Scene* scene, Player* player);

		void Update(float deltaTime) override;

		void Fire();

		void IncreaseBulletSpread() { mBulletSpread = (mBulletSpread < mMaxBulletSpread) ? (mBulletSpread + 1) : mMaxBulletSpread; }
		void IncreaseDamage() { mBonusDmgMultiplier = (mBonusDmgMultiplier < mMaxDmgMultiplier) ? (mBonusDmgMultiplier + 1) : mMaxDmgMultiplier; }

		bool IsSpreadUpgradeable();
		bool IsDamageUpgradeable();

	private:
		// Updates 'mSwayMoveOffset' and 'mSwayRotationOffset'
		void UpdateSwayOffsets(float deltaTime);
		glm::vec2 GetSwayMovementOffset();
		glm::vec2 GetSwayRotationOffset();

		void RecoilKickBack();
		void RecoilRecovery(float deltaTime);

		void UpdateBobOffset(float deltaTime);

		glm::vec3 GetMoveInput();

	private:
		Player* mPlayer = nullptr;
		Audio* mAudioComponent = nullptr;

		// Damage
		const float mBaseDamage = 20.0f;
		const float mBonusDamage = 20.0f;
		unsigned int mBonusDmgMultiplier = 0;
		static constexpr unsigned int mMaxDmgMultiplier = 3;

		const float mBulletSpreadAngle = 5.f;
		const int mMaxBulletSpread = 2;
		int mBulletSpread = 0;

		const float mFireCooldown = 0.1f;
		float mTimeSinceLastFire = 0.0f;

		glm::vec3 mProjectileSpawnOffset = glm::vec3(0.0f, 0.05f, 0.15f);
		ParticleSystemDesc mMuzzleFlashDesc;

		// Recoil
		glm::vec3 mRecoilOffset = glm::vec3(0.0f);
		float mRecoilPitch = 0.0f;
		const float mRecoilKickAmount = 0.05f;
		const float mRecoilPitchAmount = 0.2f;
		const float mRecoilRecoverySpeed = 5.0f;


		// Sway
		const float mSwaySmooth = 4.0f;
		const float mSwayMoveStep = 0.02f;
		const float mMaxSwayMoveStep = 0.12f;
		const float mSwayRotationStep = 0.2f;
		const float mMaxSwayRotationStep = 0.3f;
		glm::vec3 mSwayMoveOffset = glm::vec3(0);
		glm::vec2 mSwayRotationOffset = glm::vec2(0);

		// Bob
		const float mBobSpeed = 13.0f;
		const float mBobAmount = 0.01f;
		const float mBobLerpSpeed = 9.0f;
		float mBobTime = 0.0f;
		float mCurrentBobAmount = 0.0f;
		glm::vec3 mBobMoveOffset = glm::vec3(0);
	};
}