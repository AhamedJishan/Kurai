#include "Player.h"
#include "Utils/Log.h"

#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Scenes/GameScene.h"
#include "FPSCameraActor.h"
#include "Components/Damageable.h"
#include "Core/Components/SphereCollider.h"
#include "Arena.h"

namespace Dawn
{
	Player::Player(GameScene* scene, FPSCameraActor* fpsCameraActor, Arena* arena)
		:Actor(scene)
	{
		if (!fpsCameraActor)
		{
			LOG_WARN("Player Actor needs valid 'FPSCameraActor'");
			return;
		}

		mGameScene = scene;
		mCamera = fpsCameraActor;
		mCameraBaseFOV = mCamera->GetFOV();
		mArena = arena;
		mDamageable = new Damageable(this, 100.0f);

		SphereCollider* collider = new SphereCollider(this);
		collider->SetIsDynamic(true);
		collider->SetIsTrigger(false);

		mGameScene->SetPlayerHealth(mDamageable->GetHealth());
		mGameScene->SetMaxPlayerHealth(mDamageable->GetMaxHealth());
	}

	void Player::Update(float deltaTime)
	{
		if (mDamageable->IsDead())
			mGameScene->GameOver();

		// sync camera pos
		mCamera->SetPosition(GetPosition() + mCameraOffset);

		float yaw = mCamera->GetYaw();
		SetRotation(glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)));

		// WASD movement
		glm::vec3 moveDir = glm::vec3(0);
		if (!mIsDashing)
		{
			if (Input::GetKey(Key::W)) moveDir += GetForward();
			if (Input::GetKey(Key::S)) moveDir -= GetForward();
			if (Input::GetKey(Key::A)) moveDir -= GetRight();
			if (Input::GetKey(Key::D)) moveDir += GetRight();

			if (glm::length(moveDir) > 0.0001f)
				moveDir = glm::normalize(moveDir);

			SetPosition(GetPosition() + moveDir * mSpeed * deltaTime);
		}

		// Dash should happen before arena bounds resolution
		mDashCooldownTimer -= deltaTime;
		mDashCooldownTimer = glm::clamp(mDashCooldownTimer, 0.0f, mDashCooldownDuration);
		if (!mIsDashing && mDashCooldownTimer <= 0.0f && Input::GetKeyDown(Key::LeftShift))
			DashStart(moveDir);
		if (mIsDashing)
			DashUpdate(deltaTime);
		// dash recovery
		if (mCamera->GetFOV() > mCameraBaseFOV)
			mCamera->SetFOV(glm::mix(mCamera->GetFOV(), mCameraBaseFOV, 10 * deltaTime));

		// Resolve Arena Bound Collision
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
	}

	void Player::TakeDamage(float dmg)
	{
		if (mIsDashing)
			return;

		mDamageable->TakeDamage(dmg);
		mGameScene->SetPlayerHealth(mDamageable->GetHealth());

		if (mDamageable->IsDead())
		{
			LOG_WARN("PLAYER DIED!");
			mGameScene->GameOver();
		}
	}

	glm::vec3 Player::GetGunPosition()
	{
		glm::vec3 gunPos = mCamera->GetPosition();

		gunPos += mCamera->GetUp() * mGunOffset.y;
		gunPos += mCamera->GetRight() * mGunOffset.x;
		gunPos += mCamera->GetForward() * mGunOffset.z;

		return gunPos;
	}

	void Player::DashStart(glm::vec3 dashDirection)
	{
		mIsDashing = true;
		mDashTimer = mDashDuration;

		mDashStartPos = GetPosition();
		mDashEndPos = GetPosition() + dashDirection * mDashDistance;

		Application::Get()->GetAudioSystem()->PlayEvent("event:/dash");
	}
	
	void Player::DashUpdate(float deltaTime)
	{
		if (mDashTimer > 0.0f)
			mDashTimer -= deltaTime;

		float t = 1.0f - mDashTimer / mDashDuration;
		t = glm::clamp(t, 0.0f, 1.0f);

		float easedT = 1.0f - (1.0f - t) * (1.0f - t);

		glm::vec3 newPos = glm::mix(mDashStartPos, mDashEndPos, easedT);
		SetPosition(newPos);

		float fovOffset = (1.0f - t) * mDashFOVBoost;
		float newFOV = glm::mix(mCamera->GetFOV(), mCameraBaseFOV + fovOffset, 15 * deltaTime);
		mCamera->SetFOV(newFOV);

		if (mDashTimer <= 0.0f)
			DashEnd();
	}

	void Player::DashEnd()
	{
		mIsDashing = false;
		mDashCooldownTimer = mDashCooldownDuration;
	}
}