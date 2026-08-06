#include "Component.h"
#include "Dawn/Utils/Log.h"

#include "Actor.h"

namespace Dawn
{
	Component::Component(Actor* owner)
		:mOwner(owner)
	{
		if (!mOwner)
		{
			LOG_ERROR("Component created without an owner");
			return;
		}
	}

	Component::~Component()
	{
	}
}