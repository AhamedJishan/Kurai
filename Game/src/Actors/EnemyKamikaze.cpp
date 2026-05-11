#include "EnemyKamikaze.h"
#include "Utils/Log.h"
#include "Utils/Random.h"

#include "Rendering/Mesh.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Core/Components/Audio.h"
#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Components/Damageable.h"
#include "WaveManager.h"
#include "Player.h"
#include "Arena.h"

namespace Dawn
{
	EnemyKamikaze::EnemyKamikaze(Scene* scene, Player* player, WaveManager* waveManager, Arena* arena)
		:Actor(scene)
		,mPlayer(player)
		,mWaveManager(waveManager)
		,mArena(arena)
	{
		LOG_INFO("Enemy spawned");

		MeshRenderer::CreateFromModel(this, "Assets/Models/kamikaze/kamikaze.obj");
		SetScale(glm::vec3(mBaseScale));
		mCollider = new SphereCollider(this);
		mCollider->SetRadius(0.75f);
		mCollider->SetIsDynamic(true);
		mCollider->SetIsTrigger(false);

		// Fetch the body material
		const std::vector<MeshRenderer*> meshRenderers = GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRenderers)
		{
			if (meshRenderer->GetMesh()->GetName() == "neon")
			{
				PhongMaterial* mat = dynamic_cast<PhongMaterial*>(meshRenderer->GetMaterial());
				if (mat)
				{
					mFuseMaterial = mat;
					mFuseColor = mat->GetEmissiveColor();
				}
			}
		}

		mDamageable = new Damageable(this, 90.0f);

		mAudioComponent = new Audio(this);
		mEnemyPresence = mAudioComponent->PlayEvent("event:/enemy_presence");
		mEnemyPresence.SetVolume(10.0f);

		glm::vec3 toPlayerDir = mPlayer->GetPosition() - GetPosition();
		if (glm::length(toPlayerDir) < 0.0001f)
			mMoveDirection = glm::vec3(0, 0, -1);
		else
			mMoveDirection = glm::normalize(toPlayerDir);

		float variation = Random::Float(0.0f, 1.0f) * mSpeedVariation;
		mSpeed += variation;

		mExplosionFXDesc.initialBurst = 30;
		mExplosionFXDesc.scaleOverTime.AddKey(0.0f, glm::vec3(1.5f));
		mExplosionFXDesc.scaleOverTime.AddKey(1.0f, glm::vec3(0.1f));
		mExplosionFXDesc.colorOverTime.AddKey(0.0f, glm::vec4(0.6f, 0.9f, 0.9f, 1.0f) * 12.0f);
		mExplosionFXDesc.colorOverTime.AddKey(0.4f, glm::vec4(0.9f, 0.5f, 0.2f, 1.0f) * 8.0f);
		mExplosionFXDesc.colorOverTime.AddKey(0.7f, glm::vec4(0.85f, 0.10f, 0.04f, 1.0f));
		mExplosionFXDesc.colorOverTime.AddKey(1.0f, glm::vec4(0.0f));
		mExplosionFXDesc.directionMax = glm::vec3(1);
		mExplosionFXDesc.directionMin = glm::vec3(-1);
		mExplosionFXDesc.particleLifetime = 0.25f;
		mExplosionFXDesc.speed = 8.0f;
	}

	EnemyKamikaze::~EnemyKamikaze()
	{
		LOG_INFO("Enemy died");
	}

	void EnemyKamikaze::Update(float deltaTime)
	{
		// Arena resolution
		if (mArena->IsOutOfBounds(GetPosition()))
		{
			glm::vec3 pos = GetPosition();
			glm::vec2 bounds = mArena->GetBounds();

			if (pos.x > bounds.x)
				pos.x = bounds.x;
			if (pos.x < -bounds.x)
				pos.x = -bounds.x;
			if (pos.z > bounds.y)
				pos.z = bounds.y;
			if (pos.z < -bounds.y)
				pos.z = -bounds.y;

			SetPosition(pos);
		}


		if (mActionState == ActionState::Chasing)
		{
			Chase(deltaTime);
		}
		else if (mActionState == ActionState::Exploding)
		{
			Exploding(deltaTime);
			mEnemyPresence.Stop();
		}
		else
		{
			Explode(deltaTime);
		}

		// --- FEEDBACK STUFF ---
		if (mActionState == ActionState::Chasing && mHitImpactTimer > 0.0f)
		{
			mHitImpactTimer -= deltaTime;
			float t = 1 - mHitImpactTimer / mHitImpactDuration;
			float scaleOffset = mScalePunchAmount * glm::sin(t * glm::pi<float>());
			SetScale(glm::vec3(mBaseScale + scaleOffset));

			// ease out
			float knockbackT = 1 - (1 - t) * (1 - t);
			SetPosition(GetPosition() - GetForward() * mKnockBackDistance * knockbackT);
		}
		else if (mIsInImpactState)
		{
			mIsInImpactState = false;
			if (mFuseMaterial)
				mFuseMaterial->SetEmissiveColor(mFuseColor);
		}
	}

	float EnemyKamikaze::TakeDamage(float dmg)
	{
		float health = mDamageable->TakeDamage(dmg);

		if (mDamageable->IsDead())
		{
			SetState(Actor::State::Dead);
			new ParticleSystem(mScene, mExplosionFXDesc, GetPosition());
		}

		mHitImpactTimer = mHitImpactDuration;

		if (mFuseMaterial)
			mFuseMaterial->SetEmissiveColor(mFuseColor * mFusePulseColorFactor);
		mIsInImpactState = true;

		return health;
	}

	void EnemyKamikaze::Chase(float deltaTime)
	{
		if (!mPlayer)
			return;

		float distanceToPlayer = glm::length(mPlayer->GetPosition() - GetPosition());

		glm::vec3 targetMoveDirection = GetFinalMoveDirection();
		glm::vec3 moveDir = glm::mix(mMoveDirection, targetMoveDirection, mMoveDirectionSmoothing * deltaTime);
		mMoveDirection = moveDir;
		moveDir.y = 0;

		if (distanceToPlayer < mExplosionTriggerRadius)
		{
			mAudioComponent->PlayEvent("event:/enemy_fuse");
			mActionState = ActionState::Exploding;
		}
		else
		{
			moveDir = glm::normalize(moveDir);
			glm::vec3 newPos = GetPosition() + mSpeed * moveDir * deltaTime;

			SetPosition(newPos);
			SetRotation(glm::quatLookAt(moveDir, GetUp()));
		}
	}

	void EnemyKamikaze::Exploding(float deltaTime)
	{
		mExplosionTimer -= deltaTime;
		if (mExplosionTimer <= 0.0f)
		{
			Application::Get()->GetAudioSystem()->PlayEvent("event:/enemy_explode", GetPosition());
			mActionState = ActionState::Explode;
			new ParticleSystem(mScene, mExplosionFXDesc, GetPosition());
		}
		else if (mExplosionTimer <= mExplosionScaleDuration)
		{
			// linearly interpolate to scale
			float t = 1 - mExplosionTimer / mExplosionScaleDuration;
			SetScale(glm::vec3(1 + t * (mScaleAtExplosion - 1.0f)));
		}

		if (!mFuseMaterial)
			return;

		bool pulse = (static_cast<int>(3 * mExplosionTimer) % 2) == 0;
		if (pulse)
			mFuseMaterial->SetEmissiveColor(mFuseColor * mFusePulseColorFactor);
		else
			mFuseMaterial->SetEmissiveColor(mFuseColor);
	}

	void EnemyKamikaze::Explode(float deltaTime)
	{
		SetState(Actor::State::Dead);

		if (!mPlayer)
			return;

		float distanceFromPlayer = glm::length(mPlayer->GetPosition() - GetPosition());
		if (distanceFromPlayer >= mExplosionRadius)
			return;

		mPlayer->TakeDamage(mExplosionDamage);
	}

	glm::vec3 EnemyKamikaze::GetFinalMoveDirection()
	{
		EnemyKamikaze* closestEnemy = nullptr;
		float closestDistanceToEnemy = std::numeric_limits<float>::max();
		for (EnemyKamikaze* enemy : mWaveManager->GetEnemies())
		{
			float distanceToEnemy = glm::length(enemy->GetPosition() - GetPosition());
			if (distanceToEnemy < closestDistanceToEnemy && enemy != this)
			{
				closestDistanceToEnemy = distanceToEnemy;
				closestEnemy = enemy;
			}
		}

		glm::vec3 directionToPlayer = glm::normalize(mPlayer->GetPosition() - GetPosition());
		if (closestEnemy == nullptr || closestDistanceToEnemy > mSteeringRange)
			return glm::normalize(directionToPlayer);

		glm::vec3 directionAwayFromClosestEnemy = glm::normalize(GetPosition() - closestEnemy->GetPosition());
		if (closestDistanceToEnemy <= mSteeringCutOffRange)
			return glm::normalize(directionAwayFromClosestEnemy);

		float distanceFactor = glm::clamp((closestDistanceToEnemy / mSteeringRange), 0.0f, 1.0f);
		glm::vec3 direction = distanceFactor * directionToPlayer + (1 - distanceFactor) * directionAwayFromClosestEnemy;
		return glm::normalize(direction);
	}
}