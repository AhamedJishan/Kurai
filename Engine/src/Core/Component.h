#pragma once

namespace Dawn
{
	// Base class for all Actor-attached components.
	// Components are owned by their Actor and are updated in order.
	class Component
	{
	public:

		// Creates a component and attaches it to the given Actor.
		// Ownership is transferred to the Actor.
		Component(class Actor* owner, unsigned int updateOrder = 100);
		virtual ~Component();

		virtual void Update(float deltaTime) {}

		class Actor* GetOwner() const { return mOwner; }
		unsigned int GetUpdateOrder() const { return mUpdateOrder; }

	protected:
		unsigned int mUpdateOrder;

		class Actor* mOwner;
	};
}