#pragma once

#include "Core/Component.h"

#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward declarations
	class Actor;
	namespace Physics
	{
		struct Sphere;
	}

	class SphereCollider : public Component
	{
	public:
		SphereCollider(class Actor* owner, unsigned int updateOrder = 100);
		~SphereCollider();

		Actor* CheckCollisions();

		Physics::Sphere GetWorldSphere() const;

		bool IsDynamic() const { return mIsDynamic; }
		bool IsTrigger() const { return mIsTrigger; }
		float GetRadius() const { return mRadius; }
		glm::vec3 GetOffset() const { return mOffset; }

		void SetIsTrigger(bool value) { mIsTrigger = value; }
		void SetIsDynamic(bool value) { mIsDynamic = value; }
		void SetRadius(float radius) { mRadius = radius; }
		void SetOffset(glm::vec3 offset) { mOffset = offset; }

	private:
		// if true, won't take part in Collision resolutions
		bool mIsTrigger = false;
		// if true, then Collision resolution can push it
		bool mIsDynamic = false;

		float mRadius = 0.5f;
		glm::vec3 mOffset = glm::vec3(0);
	};
}