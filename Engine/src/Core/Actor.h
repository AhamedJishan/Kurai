#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include "Transform.h"

namespace Dawn
{
	// Forward declaration
	class Component;

	class Actor
	{
	public:
		enum class State
		{
			Active,
			Paused,
			Dead
		};

		~Actor();

		Transform& GetTransform() { return mTransform; }
		State GetState() const { return mState; }
		void SetState(State state) { mState = state; }
		const std::string& GetName() const { return mName; }
		void SetName(const std::string& name) { mName = name; }

		// --- COMPONENT MANAGEMENT ---
		template<typename T>
		Component* AddComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "Not a component");
			static_assert(std::is_constructible_v<T, Actor*>, "Component type must provide a constructor of the form T(Actor* owner).");

			T* component = new T(this);
			mComponents.push_back(component);
			return component;
		}

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
		// ----------------------------

	private:
		friend class Scene;
		Actor(const std::string& name);	// Only scene is capable of creating actors

		// To be called by Scene
		void Update(float deltaTime);

	private:
		std::string mName;
		Transform mTransform;
		State mState = State::Active;

		std::vector<Component*> mComponents;
	};
}