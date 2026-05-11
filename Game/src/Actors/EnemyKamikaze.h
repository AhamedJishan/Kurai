#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Audio/SoundEvent.h"
#include "Rendering/ParticleSystem.h"

namespace Dawn
{
	// Forward declarations
	class Scene;
	class Damageable;
	class PhongMaterial;
	class SphereCollider;
	class WaveManager;
	class Player;
	class Audio;
	class Arena;

	class EnemyKamikaze : public Actor
	{
	public:
		enum class ActionState
		{
			Chasing,
			Exploding,
			Explode
		};

		EnemyKamikaze(Scene* scene, Player* player, WaveManager* waveManager, Arena* arena);
		~EnemyKamikaze();

		void Update(float deltaTime) override;

		// returns current health after dmg is dealt
		float TakeDamage(float dmg);

	private:
		void Chase(float deltaTime);
		void Exploding(float deltaTime);
		void Explode(float deltaTime);
		glm::vec3 GetFinalMoveDirection();

	private:
		WaveManager* mWaveManager = nullptr;
		Player* mPlayer = nullptr;
		Arena* mArena = nullptr;
		SphereCollider* mCollider = nullptr;
		PhongMaterial* mFuseMaterial = nullptr;
		Audio* mAudioComponent = nullptr;
		SoundEvent mEnemyPresence;
		Damageable* mDamageable = nullptr;
		ActionState mActionState = ActionState::Chasing;

		float mExplosionTimer = 0.8f;
		const float mExplosionTriggerRadius = 2.0f;
		const float mExplosionRadius = 4.0f;
		const float mExplosionDamage = 55.0f;
		const float mScaleAtExplosion = 1.3f;
		const float mExplosionScaleDuration = 0.1f;
		ParticleSystemDesc mExplosionFXDesc;

		glm::vec3 mFuseColor;
		const float mFusePulseColorFactor = 30.0f;

		float mSpeed = 6.0f;
		const float mSpeedVariation = 4.0f;
		const float mSteeringRange = 4.0f;
		const float mSteeringCutOffRange = 1.0f;
		const float mMoveDirectionSmoothing = 2.5f;
		glm::vec3 mMoveDirection = glm::vec3(0);

		bool mIsInImpactState = false;
		float mHitImpactTimer = 0.0f;
		const float mHitImpactDuration = 0.2f;
		const float mBaseScale = 0.75f;
		const float mScalePunchAmount = 0.15f;
		const float mKnockBackDistance = 0.1f;
	};
}