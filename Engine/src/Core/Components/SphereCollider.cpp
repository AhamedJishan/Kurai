#include "SphereCollider.h"

#include "Core/Actor.h"
#include "Core/Scene.h"
#include "Physics/Physics.h"

namespace Dawn
{
	SphereCollider::SphereCollider(Actor* owner, unsigned int updateOrder)
		:Component(owner, updateOrder)
	{
		mOwner->GetScene()->AddSphereCollider(this);
	}

	SphereCollider::~SphereCollider()
	{
		mOwner->GetScene()->RemoveSphereCollider(this);
	}

	Actor* SphereCollider::CheckCollisions()
	{
		const std::vector<SphereCollider*>& colliders = mOwner->GetScene()->GetColliderList();

		Physics::Sphere mySphere = GetWorldSphere();

		for (SphereCollider* collider : colliders)
		{
			if (collider == this)
				continue;

			if (Physics::Intersects(mySphere, collider->GetWorldSphere()))
				return collider->GetOwner();
		}

		return nullptr;
	}

	Physics::Sphere SphereCollider::GetWorldSphere() const
	{
		Physics::Sphere sphere;
		sphere.center = mOwner->GetPosition() + mOffset;
		sphere.radius = mRadius;

		return sphere;
	}
}