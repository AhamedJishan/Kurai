#include "Component.h"
#include "Utils/Log.h"

#include "Actor.h"

namespace Dawn
{
	Component::Component(Actor* owner, unsigned int updateOrder)
		:mOwner(owner)
		,mUpdateOrder(updateOrder)
	{
		if (!mOwner)
		{
			LOG_ERROR("Component created with null owner");
			return;
		}

		mOwner->AddComponent(this);
	}

	Component::~Component()
	{
		if (mOwner)
			mOwner->RemoveComponent(this);
	}
}