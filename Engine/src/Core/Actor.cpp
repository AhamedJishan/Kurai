#include "Actor.h"
#include "Utils/Log.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"
#include "Scene.h"

namespace Dawn
{
	Actor::Actor(Scene* scene)
	{
		if (!scene)
		{
			LOG_ERROR("Invalid scene passed to the Actor");
			return;
		}

		mScene = scene;
		mScene->AddActor(this);
	}

	Actor::~Actor()
	{
		if (mScene)
			mScene->RemoveActor(this);

		// Actor deletes Component
		// ~Component() removes itself from Actor::mComponents
		while (!mComponents.empty())
			delete mComponents.back();
	}

	void Actor::UpdateActor(float deltaTime)
	{
		if (mState == State::Paused || mState == State::Dead)
			return;

		for (Component* component : mComponents)
			component->Update(deltaTime);

		Update(deltaTime);
	}

	void Actor::AddComponent(Component* component)
	{
		unsigned int updateOrder = component->GetUpdateOrder();
		auto it = mComponents.begin();
		for (; it != mComponents.end(); it++)
			if ((*it)->GetUpdateOrder() > updateOrder)
				break;
		mComponents.insert(it, component);
	}

	void Actor::RemoveComponent(Component* component)
	{
		auto it = std::find(mComponents.begin(), mComponents.end(), component);
		if (it != mComponents.end())
			mComponents.erase(it);
		else
			LOG_WARN("Tried to remove component that doesn't exist");
	}

	glm::mat4 Actor::GetWorldTransform() const
	{
		glm::mat4 worldTransform;
		worldTransform = glm::translate(glm::mat4(1), mPosition);
		worldTransform = worldTransform * glm::mat4_cast(mRotation);
		worldTransform = worldTransform * glm::scale(glm::mat4(1), mScale);
		return worldTransform;
	}
	
	void Actor::Rotate(float angle, const glm::vec3& axisOfRotation)
	{
		mRotation = glm::normalize(mRotation * glm::angleAxis(angle, axisOfRotation));
	}
}