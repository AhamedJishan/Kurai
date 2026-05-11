#pragma once

#include "Core/Component.h"

namespace Dawn
{
	// Forward declarations
	class Actor;

	class Damageable : public Component
	{
	public:
		Damageable(Actor* owner, float maxHealth = 100.0f)
			:Component(owner)
			,mMaxHealth(maxHealth)
			,mHealth(maxHealth)
		{
		}

		// Returns health left after dmg is dealt
		float TakeDamage(float dmg)
		{
			mHealth -= dmg;
			mHealth = glm::clamp(mHealth, 0.0f, mMaxHealth);

			if (mHealth == 0.0f)
				mIsDead = true;

			return mHealth;
		}

		bool IsDead() const { return mIsDead; }
		float GetHealth() const { return mHealth; }
		float GetMaxHealth() const { return mMaxHealth; }

	private:
		const float mMaxHealth;
		float mHealth;
		bool mIsDead = false;
	};
}