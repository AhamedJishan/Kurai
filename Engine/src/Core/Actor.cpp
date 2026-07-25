#include "Actor.h"

#include "Component.h"

namespace Dawn
{
	Actor::Actor(const std::string& name)
		:mName(name)
	{
	}

	Actor::~Actor()
	{
		for (Component* component : mComponents)
			delete component;
		mComponents.clear();
	}

	void Actor::Update(float deltaTime)
	{
		if (mState == State::Paused || mState == State::Dead)
			return;

		for (Component* component : mComponents)
			component->Update(deltaTime);
	}
}