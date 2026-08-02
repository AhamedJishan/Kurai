#pragma once

#include <string>
#include <unordered_map>

namespace Dawn
{
	// Forward declarations
	class Scene;
	class Actor;
	class Component;

	namespace SceneSerializer
	{
		Scene* Load(const std::string& scenePath);
		// TODO: Save();
	}

	class SerializationContext
	{
	public:
		void Register(Actor* actor);					// While Serializing
		void Register(unsigned int id, Actor* actor);	// While Deserializing

		void Register(Component* component);
		void Register(unsigned int id, Component* component);

		Actor* GetActorById(unsigned int id) const;
		Component* GetComponentById(unsigned int id) const;

		unsigned int GetIdByActor(Actor* actor) const;
		unsigned int GetIdByComponent(Component* component) const;

	private:
		unsigned int mNextId = 1;

		std::unordered_map<unsigned int, Actor*> mIdToActorMap;
		std::unordered_map<unsigned int, Component*> mIdToComponentMap;

		std::unordered_map<Actor*, unsigned int> mActorToIdMap;
		std::unordered_map<Component*, unsigned int> mComponentToIdMap;
	};
}