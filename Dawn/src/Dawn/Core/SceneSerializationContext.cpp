#include "SceneSerializationContext.h"

#include <Dawn/Utils/Log.h>

namespace Dawn
{
	// --- SERIALIZATION CONTEXT ---
	void SceneSerializationContext::Clear()
	{
		mNextId = 1;
		mActorToIdMap.clear();
		mIdToActorMap.clear();
		mComponentToIdMap.clear();
		mIdToComponentMap.clear();
	}

	void SceneSerializationContext::Register(Actor* actor)
	{
		mIdToActorMap.emplace(mNextId, actor);
		mActorToIdMap.emplace(actor, mNextId);
		mNextId++;
	}

	void SceneSerializationContext::Register(unsigned int id, Actor* actor)
	{
		mIdToActorMap.emplace(id, actor);
		mActorToIdMap.emplace(actor, id);
		if (id >= mNextId)
			mNextId = id + 1;
	}

	void SceneSerializationContext::Register(Component* component)
	{
		mIdToComponentMap.emplace(mNextId, component);
		mComponentToIdMap.emplace(component, mNextId);
		mNextId++;

	}

	void SceneSerializationContext::Register(unsigned int id, Component* component)
	{
		mIdToComponentMap.emplace(id, component);
		mComponentToIdMap.emplace(component, id);
		if (id >= mNextId)
			mNextId = id + 1;
	}

	Actor* SceneSerializationContext::GetActorById(unsigned int id) const
	{
		if (id == 0)
		{
			LOG_ERROR("Tried to resolve a null Actor reference (ID 0).");
			return nullptr;
		}

		auto it = mIdToActorMap.find(id);
		if (it == mIdToActorMap.end())
		{
			LOG_ERROR("No Actor by the id '%d' exists", id);
			return nullptr;
		}

		return it->second;
	}

	Component* SceneSerializationContext::GetComponentById(unsigned int id) const
	{
		if (id == 0)
		{
			LOG_ERROR("Tried to resolve a null Component reference (ID 0).");
			return nullptr;
		}

		auto it = mIdToComponentMap.find(id);
		if (it == mIdToComponentMap.end())
		{
			LOG_ERROR("No Component by the id '%d' exists", id);
			return nullptr;
		}

		return it->second;
	}

	unsigned int SceneSerializationContext::GetIdByActor(Actor* actor) const
	{
		auto it = mActorToIdMap.find(actor);
		if (it == mActorToIdMap.end())
		{
			LOG_ERROR("Tried to get id of an unregistered Actor");
			return 0;
		}

		return it->second;
	}

	unsigned int SceneSerializationContext::GetIdByComponent(Component* component) const
	{
		auto it = mComponentToIdMap.find(component);
		if (it == mComponentToIdMap.end())
		{
			LOG_ERROR("Tried to get id of an unregistered Component");
			return 0;
		}

		return it->second;
	}
	// -----------------------------
}