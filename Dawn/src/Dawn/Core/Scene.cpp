#include "Scene.h"

#include "Actor.h"
#include "Application.h"
#include <Dawn/Core/Components/Camera.h>
#include <Dawn/Audio/AudioSystem.h>
#include <Dawn/Utils/Log.h>
#include <Dawn/Utils/FileSystem.h>

namespace Dawn
{
	Scene::Scene()
	{
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

	void Scene::Update(float deltaTime, bool simulate)
	{
		// Update Actors
		if (simulate)
		{
			mUpdatingActors = true;
			for (Actor* actor : mActors)
				actor->Update(deltaTime);
			mUpdatingActors = false;
		}

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

		if (mMainCamera)
			Application::Get()->GetAudioSystem()->SetListener(mMainCamera->GetView());
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

	void Scene::SetMainCamera(Camera* camera)
	{
		if (camera == mMainCamera) return;

		if (mMainCamera) mMainCamera->mIsMainCamera = false;

		mMainCamera = camera;

		if (mMainCamera) mMainCamera->mIsMainCamera = true;
	}
}