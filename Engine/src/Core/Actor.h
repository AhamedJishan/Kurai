#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

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

		glm::mat4 GetWorldTransform() const;
		glm::vec3 GetUp()		const { return glm::normalize(glm::mat3_cast(mRotation) * glm::vec3(0, 1, 0)); }
		glm::vec3 GetRight()	const { return glm::normalize(glm::mat3_cast(mRotation) * glm::vec3(1, 0, 0)); }
		glm::vec3 GetForward()	const { return glm::normalize(glm::mat3_cast(mRotation) * glm::vec3(0, 0, -1)); }

		Scene* GetScene()		const { return mScene; }
		State GetState()		const { return mState; }
		glm::vec3 GetScale()	const { return mScale; }
		glm::vec3 GetPosition() const { return mPosition; }
		glm::quat GetRotation() const { return mRotation; }
		void SetState			(State state)				{ mState = state; }
		void SetScale			(const glm::vec3& scale)	{ mScale = scale; }
		void SetPosition		(const glm::vec3& position) { mPosition = position; }
		void SetRotation		(const glm::quat& rotation) { mRotation = rotation; }
		// Angle is in radians
		void Rotate(float angle, const glm::vec3& axisOfRotation);

	protected:
		// To be implemented by custom Actor
		virtual void Update(float deltaTime) {}

	protected:
		State mState = State::Active;
		glm::vec3 mPosition = glm::vec3(0);
		glm::vec3 mScale = glm::vec3(1);
		glm::quat mRotation = glm::quat(1, 0, 0, 0);

		std::vector<Component*> mComponents;

		Scene* mScene = nullptr;
	};
}