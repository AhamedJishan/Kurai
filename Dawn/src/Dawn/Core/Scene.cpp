#include "Scene.h"

#include <glm/mat4x4.hpp>
#include "Actor.h"
#include "Application.h"
#include <Dawn/Core/Components/Camera.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Audio/AudioSystem.h>

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

		if (mActiveCamera)
		{
			glm::mat4 view = mActiveCamera->GetView();
			glm::mat4 projection = mActiveCamera->GetProjection();

			Application::Get()->GetAudioSystem()->SetListener(view);
			Application::Get()->GetRenderer()->SetView(view);
			Application::Get()->GetRenderer()->SetProjection(projection);
		}
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