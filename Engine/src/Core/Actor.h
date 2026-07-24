#pragma once

#include <vector>
#include "Transform.h"

namespace Dawn
{
	// Forward declaration
	class Component;
	class Scene;

	// Base class for all scene objects.
	// Owns transform state and a set of Components.
	class Actor
	{
	public:
		enum class State
		{
			Active,
			Paused,
			Dead
		};

		Actor(class Scene* scene);
		virtual ~Actor();

		// To be called by Scene
		void UpdateActor(float deltaTime);

		// Component managment
		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		template<typename T>
		T* GetComponent() const
		{
			for (Component* component : mComponents)
			{
				if (T* casted = dynamic_cast<T*>(component))
					return casted;
			}
			return nullptr;
		}
		template<typename T>
		std::vector<T*> GetComponents() const
		{
			std::vector<T*> resultList;
			for (Component* component : mComponents)
			{
				if (T* casted = dynamic_cast<T*>(component))
					resultList.emplace_back(casted);
			}
			return resultList;
		}

		Transform& GetTransform() { return mTransform; }
		Scene* GetScene() const { return mScene; }
		State GetState() const { return mState; }
		void SetState (State state)	{ mState = state; }

	protected:
		// To be implemented by custom Actor
		virtual void Update(float deltaTime) {}

	protected:
		Scene* mScene = nullptr;
		State mState = State::Active;
		Transform mTransform;

		std::vector<Component*> mComponents;
	};
}