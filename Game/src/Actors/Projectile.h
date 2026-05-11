#pragma once

#include "Core/Actor.h"

#include "Core/Application.h"
#include "Audio/AudioSystem.h"
#include "Core/Components/MeshRenderer.h"
#include "Core/Components/SphereCollider.h"
#include "Rendering/ParticleSystem.h"
#include "Rendering/Materials/PhongMaterial.h"
#include "Player.h"
#include "Arena.h"
#include "EnemyKamikaze.h"

namespace Dawn
{
	class Projectile : public Actor
	{
	public:
		Projectile(class Scene* scene, Player* player, float damage, glm::vec4 projectileColor)
			:Actor(scene)
			,mDamage(damage)
		{
			SetScale(glm::vec3(0.5f));

			MeshRenderer::CreateFromModel(this, "Assets/Models/ball/ball.obj");

			PhongMaterial* projectileMat = dynamic_cast<PhongMaterial*>(GetComponent<MeshRenderer>()->GetMaterial());
			if (projectileMat)
			{
				projectileMat->SetDiffuseColor(projectileColor);
				projectileMat->SetEmissiveColor(projectileColor * 10.0f);
			}

			mHitParticleDesc.initialBurst = 10;
			mHitParticleDesc.emissionRate = 0.0f;
			mHitParticleDesc.particleLifetime = 0.1f;
			mHitParticleDesc.directionMin = glm::vec3(-1.0f, -1.0f, -1.0f);
			mHitParticleDesc.directionMax = glm::vec3(1.0f, 1.0f, 1.0f);
			mHitParticleDesc.speed = 10.0f;
			mHitParticleDesc.scaleOverTime.AddKey(0.0f, glm::vec3(0.3f));
			mHitParticleDesc.scaleOverTime.AddKey(1.0f, glm::vec3(0.1f));
			mHitParticleDesc.colorOverTime.AddKey(0.0f, projectileColor * 10.0f);
			mHitParticleDesc.colorOverTime.AddKey(0.7f, projectileColor);
			mHitParticleDesc.colorOverTime.AddKey(1.0f, glm::vec4(0.0f));
		}

		void Update(float deltaTime) override
		{
			mLifeTime -= deltaTime;
			if (mLifeTime <= 0.0f)
			{
				SetState(Actor::State::Dead);
			}

			SetPosition(GetPosition() + GetForward() * mSpeed * deltaTime);
		}

	private:
		float mLifeTime = 1.0f;
		float mSpeed = 100.0f;
		float mDamage = 35.0f;

		Player* mPlayer = nullptr;

		ParticleSystemDesc mHitParticleDesc;
	};
}