#include "Gun.h"
#include "Utils/Log.h"

#include <glm/vec3.hpp>
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Scene.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/Camera.h"
#include "Core/Components/Audio.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
#include "Player.h"
#include "Projectile.h"
#include "Components/FPSCamera.h"

namespace Dawn
{
	const glm::vec4 COLOR_ORANGE = glm::vec4(0.85f, 0.10f, 0.04f, 1.0f);

	Gun::Gun(Scene* scene, Player* player)
		:Actor(scene)
	{
		MeshRenderer::CreateFromModel(this, "Assets/Models/gun/gun.obj");

		mAudioComponent = new Audio(this);
		mPlayer = player;

		mMuzzleFlashDesc.initialBurst = 10;
		mMuzzleFlashDesc.colorOverTime.AddKey(0.0f, 10.0f * COLOR_ORANGE);
		mMuzzleFlashDesc.colorOverTime.AddKey(0.7f, COLOR_ORANGE);
		mMuzzleFlashDesc.colorOverTime.AddKey(1.0f, glm::vec4(0.0f));
		mMuzzleFlashDesc.scaleOverTime.AddKey(0.0f, glm::vec3(0.07f));
		mMuzzleFlashDesc.scaleOverTime.AddKey(1.0f, glm::vec3(0.03f));
		mMuzzleFlashDesc.directionMax = glm::vec3(1);
		mMuzzleFlashDesc.directionMin = glm::vec3(-1);
		mMuzzleFlashDesc.particleLifetime = .05f;
		mMuzzleFlashDesc.speed = 2.5f;
	}

	void Gun::Update(float deltaTime)
	{
		glm::quat baseRotation = mScene->GetActiveCamera()->GetOwner()->GetRotation();
		glm::vec3 basePosition = mPlayer->GetGunPosition();

		// Need this since rest of the code requires correct orientation
		SetRotation(baseRotation);

		UpdateSwayOffsets(deltaTime);
		UpdateBobOffset(deltaTime);
		RecoilRecovery(deltaTime);

		mTimeSinceLastFire += deltaTime;
		if (mTimeSinceLastFire >= mFireCooldown && Input::GetMouseButtonDown(MouseButton::Left))
		{
			Fire();
			mTimeSinceLastFire = 0.0f;

			RecoilKickBack();
		}

		// --- FINAL TRANSFORM ---
		glm::vec3 finalPosition = basePosition + mSwayMoveOffset + mBobMoveOffset + mRecoilOffset;
		SetPosition(finalPosition);

		Rotate(mSwayRotationOffset.y, glm::vec3(1, 0, 0));
		Rotate(mSwayRotationOffset.x, glm::vec3(0, 1, 0));
		Rotate(mRecoilPitch, glm::vec3(1, 0, 0));
	}

	void Gun::Fire()
	{
		mAudioComponent->PlayEvent("event:/gunshot");

		FPSCamera* camera = dynamic_cast<FPSCamera*>(mScene->GetActiveCamera());
		if (!camera)
			return;

		float screenX = static_cast<float>(Application::Get()->GetWindow()->GetWidth());
		float screenY = static_cast<float>(Application::Get()->GetWindow()->GetHeight());

		glm::vec3 origin = camera->UnProject(screenX / 2.0f, screenY / 2.0f, 0.0f);
		glm::vec3 end = camera->UnProject(screenX / 2.0f, screenY / 2.0f, 1.0f);

		RaycastHit hitinfo;
		Physics::Ray ray;
		ray.origin = origin;
		ray.direction = glm::normalize(end - origin);

		glm::vec3 targetLocation;

		if (mScene->RayCast(ray, 1000.0f, hitinfo))
			targetLocation = hitinfo.position;
		else
			targetLocation = ray.origin + ray.direction * 1000.0f;

		glm::vec3 projectileDirection = glm::normalize(targetLocation - GetPosition());
		glm::quat projectileRotation = glm::quatLookAt(projectileDirection, glm::vec3(0, 1, 0));

		glm::vec3 projectilePosition = glm::vec3(0);
		projectilePosition += GetUp() * mProjectileSpawnOffset.y;
		projectilePosition += GetRight() * mProjectileSpawnOffset.x;
		projectilePosition += GetForward() * mProjectileSpawnOffset.z;
		projectilePosition += GetPosition();

		float finalDmg = mBaseDamage + mBonusDamage * mBonusDmgMultiplier;

		// Spawn projectile
		for (int i = -mBulletSpread; i <= mBulletSpread; i++)
		{
			Projectile* projectile = new Projectile(mScene, mPlayer, finalDmg, glm::vec4(0.85f, 0.10f, 0.04f, 1.0f));
			projectile->SetPosition(projectilePosition);
			projectile->SetRotation(glm::angleAxis(glm::radians(-mBulletSpreadAngle * i), glm::vec3(0, 1, 0)) * projectileRotation);
		}

		// Muzzle flash
		mMuzzleFlashDesc.directionMax = GetForward() + glm::vec3(2);
		mMuzzleFlashDesc.directionMin = GetForward() - glm::vec3(2);
		ParticleSystem* muzzleFlash = new ParticleSystem(mScene, mMuzzleFlashDesc, projectilePosition);

		// Camera Pitch Recoil
		camera->Recoil();
	}

	bool Gun::IsSpreadUpgradeable()
	{
		return mBulletSpread < mMaxBulletSpread;
	}

	bool Gun::IsDamageUpgradeable()
	{
		return mBonusDmgMultiplier < mMaxDmgMultiplier;
	}

	glm::vec2 Gun::GetSwayMovementOffset()
	{
		glm::vec2 invertLook = Input::GetCursorDeltaPos() * (-mSwayMoveStep);
		invertLook.x = glm::clamp(invertLook.x, -mMaxSwayMoveStep, mMaxSwayMoveStep);
		invertLook.y = glm::clamp(invertLook.y, -mMaxSwayMoveStep, mMaxSwayMoveStep);

		return invertLook;
	}

	glm::vec2 Gun::GetSwayRotationOffset()
	{
		glm::vec2 invertLook = Input::GetCursorDeltaPos() * mSwayRotationStep;
		invertLook.x = glm::clamp(invertLook.x, -mMaxSwayRotationStep, mMaxSwayRotationStep);
		invertLook.y = glm::clamp(invertLook.y, -mMaxSwayRotationStep, mMaxSwayRotationStep);

		return invertLook;
	}

	void Gun::UpdateSwayOffsets(float deltaTime)
	{
		// Sway Movement
		glm::vec2 swayMoveOffset = GetSwayMovementOffset();
		glm::vec3 targetSway = GetRight() * swayMoveOffset.x - GetUp() * swayMoveOffset.y;
		mSwayMoveOffset = glm::mix(mSwayMoveOffset, targetSway, deltaTime * mSwaySmooth);
		// Sway Rotation
		glm::vec2 swayRotationOffset = GetSwayRotationOffset();
		mSwayRotationOffset = glm::mix(mSwayRotationOffset, swayRotationOffset, deltaTime * mSwaySmooth);
	}

	void Gun::RecoilKickBack()
	{
		mRecoilOffset -= GetForward() * mRecoilKickAmount;
		mRecoilPitch += mRecoilPitchAmount;
	}

	void Gun::RecoilRecovery(float deltaTime)
	{
		mRecoilOffset = glm::mix(mRecoilOffset, glm::vec3(0.0f), deltaTime * mRecoilRecoverySpeed);
		mRecoilPitch = glm::mix(mRecoilPitch, 0.0f, deltaTime * mRecoilRecoverySpeed);
	}

	void Gun::UpdateBobOffset(float deltaTime)
	{
		glm::vec3 moveInput = GetMoveInput();
		bool isMoving = (glm::length(moveInput) > 0.01f);
		bool isSprinting = isMoving && Input::GetKey(Key::LeftShift);

		// --- Lerp the amplitude ---
		float speedFactor = isMoving ? (isSprinting ? 1.0f : 0.75f) : 0.25f;
		float targetAmount = mBobAmount * speedFactor;

		float lerpFactor = 1 - glm::exp(-mBobLerpSpeed * deltaTime);
		mCurrentBobAmount += (targetAmount - mCurrentBobAmount) * lerpFactor;

		// --- Advance BobTime ---
		mBobTime += mBobSpeed * speedFactor * deltaTime;

		// --- Compute offsets ---
		float verticalOffset	= glm::sin(mBobTime)		* mCurrentBobAmount;
		float horizontalOffset	= glm::sin(mBobTime * 0.5f)	* mCurrentBobAmount * 0.5f;

		mBobMoveOffset = glm::vec3(0);
		mBobMoveOffset += GetUp() * verticalOffset;
		mBobMoveOffset += GetRight() * horizontalOffset;
	}

	glm::vec3 Gun::GetMoveInput()
	{
		glm::vec3 moveInput = glm::vec3(0);

		if (Input::GetKey(Key::W)) moveInput.y += 1.0f;
		if (Input::GetKey(Key::S)) moveInput.y -= 1.0f;
		if (Input::GetKey(Key::A)) moveInput.x -= 1.0f;
		if (Input::GetKey(Key::D)) moveInput.x += 1.0f;

		return (glm::length(moveInput) > 0.01f) ? moveInput : glm::vec3(0);
	}
}