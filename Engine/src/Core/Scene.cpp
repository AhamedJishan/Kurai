#include "Scene.h"

#include <algorithm>
#include "Actor.h"
#include "Physics/Physics.h"
#include "Components/SphereCollider.h"
#include "Rendering/ParticleSystem.h"

namespace Dawn
{
	Scene::Scene()
	{
		// TODO: Load the scene from the filepath. To be done when/if Serialization is implemented
	}

	// Relies on Actor::~Actor() to call Scene::RemoveActor()
	Scene::~Scene()
	{
		while (!mActors.empty())
			delete mActors.back();

		while (!mPendingActors.empty())
			delete mPendingActors.back();

		while (!mParticleSystems.empty())
			delete mParticleSystems.back();
	}
	
	void Scene::UpdateActors(float deltaTime)
	{
		if (mIsPaused)
			return;

		// Update Actors
		mUpdatingActors = true;
		for (Actor* actor : mActors)
			actor->UpdateActor(deltaTime);
		mUpdatingActors = false;

		// Move pending actors to mActors
		for (Actor* actor : mPendingActors)
			mActors.emplace_back(actor);
		mPendingActors.clear();

		// Gather the actors to be deleted
		std::vector<Actor*> deadActors;
		for (Actor* actor : mActors)
			if (actor->GetState() == Actor::State::Dead)
				deadActors.emplace_back(actor);

		// Delete dead actors
		for (Actor* actor : deadActors)
			delete actor;					// Actor::~Actor() calls Scene::RemoveActor() to remove itself from mActors
		deadActors.clear();

		// Particle System
		std::vector<ParticleSystem*> psToBeDeleted;
		for (ParticleSystem* particle : mParticleSystems)
		{
			particle->Update(deltaTime);
			if (particle->IsStopped())
				psToBeDeleted.push_back(particle);
		}
		for (ParticleSystem* particle : psToBeDeleted)
			delete particle;
	}

	void Scene::ResolveCollisions()
	{
		unsigned int numColliders = mColliders.size();

		if (numColliders < 2) return;

		for (unsigned int i = 0; i < numColliders - 1; i++)
		{
			SphereCollider* a = mColliders[i];

			if (a->IsTrigger())
				continue;

			for (unsigned int j = i + 1; j < numColliders; j++)
			{
				Physics::Sphere aSphere = a->GetWorldSphere();

				SphereCollider* b = mColliders[j];
				Physics::Sphere bSphere = b->GetWorldSphere();
				
				if (b->IsTrigger())
					continue;

				glm::vec3 diffB_A = bSphere.center - aSphere.center;
				diffB_A.y = 0;	// lock on y axis

				float distance = glm::length(diffB_A);
				float radiusA = aSphere.radius;
				float radiusB = bSphere.radius;
				float radiiSum = radiusA + radiusB;
				float overlap = radiiSum - distance;

				if (distance >= radiiSum || distance <= 0.0001f)
					continue;

				bool aIsDynamic = a->IsDynamic();
				bool bIsDynamic = b->IsDynamic();

				if (!aIsDynamic && !bIsDynamic)
					continue;

				glm::vec3 directionAtoB = glm::normalize(diffB_A);

				if (aIsDynamic && bIsDynamic)
				{
					float factorA = radiusA / radiiSum;
					a->GetOwner()->SetPosition(a->GetOwner()->GetPosition() - directionAtoB * factorA * overlap);
					b->GetOwner()->SetPosition(b->GetOwner()->GetPosition() + directionAtoB * (1.0f - factorA) * overlap);
				}
				else if (aIsDynamic)
					a->GetOwner()->SetPosition(a->GetOwner()->GetPosition() - directionAtoB * overlap);
				else
					b->GetOwner()->SetPosition(b->GetOwner()->GetPosition() + directionAtoB * overlap);
			}
		}
	}
	
	void Scene::AddActor(Actor* actor)
	{
		if (mUpdatingActors)
			mPendingActors.emplace_back(actor);
		else
			mActors.emplace_back(actor);
	}
	
	// Called by Actor::~Actor()
	// Removes the actor from both active and pending lists.
	void Scene::RemoveActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
			mActors.erase(it);

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
			mPendingActors.erase(it);

	}

	bool Scene::ContainsActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
			return true;

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
			return true;

		return false;
	}

	void Scene::AddSphereCollider(SphereCollider* collider)
	{
		mColliders.emplace_back(collider);
	}
	// Called by SphereCollider::~SphereCollider()
	// Removes the SphereCollider from the list.
	void Scene::RemoveSphereCollider(SphereCollider* collider)
	{
		auto it = std::find(mColliders.begin(), mColliders.end(), collider);
		if (it != mColliders.end())
			mColliders.erase(it);
	}

	void Scene::AddParticleSystem(ParticleSystem* particleSystem)
	{
		mParticleSystems.push_back(particleSystem);
	}

	void Scene::RemoveParticleSystem(ParticleSystem* particleSystem)
	{
		auto it = std::find(mParticleSystems.begin(), mParticleSystems.end(), particleSystem);
		if (it != mParticleSystems.end())
			mParticleSystems.erase(it);
	}

	bool Scene::RayCast(const Physics::Ray& ray, float maxDistance, RaycastHit& outHitInfo)
	{
		float closestT = maxDistance;
		bool hit = false;
		outHitInfo.actor = nullptr;

		for (SphereCollider* sphereCollider : mColliders)
		{
			Physics::Sphere sphere = sphereCollider->GetWorldSphere();

			float t = 0.0f;
			if (Physics::Intersects(ray, sphere, t) && t < closestT)
			{
				closestT =  t;
				hit = true;
				outHitInfo.actor = sphereCollider->GetOwner();
				outHitInfo.position = ray.origin + ray.direction * t;
			}
		}

		return hit;
	}
}