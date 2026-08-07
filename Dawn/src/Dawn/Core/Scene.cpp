#include "Scene.h"

#include "Actor.h"

namespace Dawn
{
	Scene::Scene()
	{
		// TODO: Load the scene from the filepath. To be done when/if Serialization is implemented
	}

	Scene::~Scene()
	{
		for (Actor* actor : mActors)
			delete actor;
		mActors.clear();

		for (Actor* actor : mPendingActors)
			delete actor;
		mPendingActors.clear();
	}

	void Scene::Update(float deltaTime)
	{
		if (mIsPaused)
			return;

		// Update Actors
		mUpdatingActors = true;
		for (Actor* actor : mActors)
			actor->Update(deltaTime);
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
			DeleteActor(actor);					// Actor::~Actor() calls Scene::RemoveActor() to remove itself from mActors
		deadActors.clear();
	}

	Actor* Scene::CreateActor(const std::string& name)
	{
		Actor* actor = new Actor(name);

		if (mUpdatingActors)
			mPendingActors.emplace_back(actor);
		else
			mActors.emplace_back(actor);

		return actor;
	}

	void Scene::DestroyActor(Actor* actor)
	{
		actor->SetState(Actor::State::Dead);
	}

	void Scene::DeleteActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
		{
			mActors.erase(it);
			delete actor;
			return;
		}

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
		{
			mPendingActors.erase(it);
			delete actor;
			return;
		}
	}
}