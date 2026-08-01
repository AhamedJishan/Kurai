#pragma once

// Forward Declaration
namespace YAML
{
	class Node;
}

namespace Dawn
{
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

		virtual void Serialize(YAML::Node& node) const = 0;
		virtual void Deserialize(const YAML::Node& node) = 0;

	protected:
		class Actor* mOwner;
	};
}