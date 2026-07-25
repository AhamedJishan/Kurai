#include "Actor.h"
#include "Utils/Log.h"

#include <algorithm>
#include "Component.h"

namespace Dawn
{
	Actor::Actor(const std::string& name)
		:mName(name)
	{
	}

	Actor::~Actor()
	{
		// Actor deletes Component
		// ~Component() removes itself from Actor::mComponents
		while (!mComponents.empty())
			delete mComponents.back();
	}

	void Actor::Update(float deltaTime)
	{
		if (mState == State::Paused || mState == State::Dead)
			return;

		for (Component* component : mComponents)
			component->Update(deltaTime);
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
}