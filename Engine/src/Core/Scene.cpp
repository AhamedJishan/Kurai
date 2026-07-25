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
}