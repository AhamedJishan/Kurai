#pragma once

#include <vector>
#include <Dawn/Core/Property.h>

// Forward Declaration
namespace YAML
{
	class Node;
}

namespace Dawn
{
	// Forward declarations
	class SerializationContext;

	// NOTE:
	// Every Component subclass should provide a constructor of the form:
	//
	//     ComponentName(Actor* owner);
	//
	// The engine assumes this when creating Components through
	// Actor::AddComponent<T>() and during scene deserialization.
	class Component
	{
	public:
		// Each component should be created via actor: actor->CreateComponent<T>()
		Component(class Actor* owner);
		virtual ~Component();

		virtual void Update(float deltaTime) {}

		class Actor* GetOwner() const { return mOwner; }

		virtual std::vector<Property> GetProperties() = 0;
		virtual void OnPropertiesChanged() {}

	protected:
		class Actor* mOwner;
	};
}