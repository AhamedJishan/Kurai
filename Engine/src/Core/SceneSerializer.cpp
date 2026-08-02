#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Utils/Log.h>
#include "Application.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "Scene.h"
#include "Actor.h"
#include "Transform.h"
#include "Components/Camera.h"

namespace Dawn
{
	// --- YAML helpers ---
	glm::vec3 ToVec3(const YAML::Node& node)
	{
		return {
			node[0].as<float>(),
			node[1].as<float>(),
			node[2].as<float>()
		};
	}
	// quat: w, x, y, z
	glm::quat ToQuat(const YAML::Node& node)
	{
		return {
			node[0].as<float>(),
			node[1].as<float>(),
			node[2].as<float>(),
			node[3].as<float>()
		};
	}
	// --------------------

	// --- SCENE SERIALIZER Helper ---
	void DeserializeEnvSettings(const YAML::Node& envSettingsNode, Scene* scene)
	{
		EnvironmentSettings& env = scene->GetEnvironmentSettings();
		EnvironmentSettings::DirectionalLight& dirLight = env.directionalLight;

		const YAML::Node& dirLightNode = envSettingsNode["DirectionalLight"];

		env.ambientColor = ToVec3(envSettingsNode["AmbientColor"]);
		env.bloomRadius = envSettingsNode["BloomRadius"].as<float>();
		env.bloomStrength = envSettingsNode["BloomStrength"].as<float>();
		env.fogColor = ToVec3(envSettingsNode["FogColor"]);
		env.fogDensity = envSettingsNode["FogDensity"].as<float>();
		dirLight.color = ToVec3(dirLightNode["Color"]);
		dirLight.direction = ToVec3(dirLightNode["Direction"]);
		dirLight.intensity = dirLightNode["Intensity"].as<float>();
	}

	void DeserializeComponents(const YAML::Node& componentsNode, SerializationContext& ctx)
	{
		for (const YAML::Node& componentNode : componentsNode)
		{
			Component* component = ctx.GetComponentById(componentNode["Id"].as<unsigned int>());
			component->Deserialize(componentNode, ctx);
		}
	}

	void DeserializeActors(const YAML::Node& actorsNode, SerializationContext& ctx)
	{
		for (const YAML::Node& actorNode : actorsNode)
		{
			const YAML::Node& transformNode = actorNode["Transform"];
			Actor* actor = ctx.GetActorById(actorNode["Id"].as<unsigned int>());

			Transform& transform = actor->GetTransform();
			transform.Scale = ToVec3(transformNode["Scale"]);
			transform.Position = ToVec3(transformNode["Position"]);
			transform.Rotation = ToQuat(transformNode["Rotation"]);

			const std::string& actorStateStr = actorNode["State"].as<std::string>();
			if		(actorStateStr == "Active") actor->SetState(Actor::State::Active);
			else if (actorStateStr == "Paused") actor->SetState(Actor::State::Paused);
			else								actor->SetState(Actor::State::Dead);

			DeserializeComponents(actorNode["Components"], ctx);
		}
	}

	void CreateComponents(const YAML::Node& componentsNode, SerializationContext& ctx, Actor* owner)
	{
		ComponentFactory* componentFactory = Application::Get()->GetComponentFactory();
		for (const YAML::Node& componentNode : componentsNode)
		{
			unsigned int id = componentNode["Id"].as<unsigned int>();
			Component* component = componentFactory->Create(componentNode["Type"].as<std::string>(), owner);
			ctx.Register(id, component);
		}
	}

	void CreateActors(const YAML::Node& actorsNode, SerializationContext& ctx, Scene* scene)
	{
		for (const YAML::Node& actorNode : actorsNode)
		{
			unsigned int id = actorNode["Id"].as<unsigned int>();
			Actor* actor = scene->CreateActor(actorNode["Name"].as<std::string>());
			ctx.Register(id, actor);

			CreateComponents(actorNode["Components"], ctx, actor);
		}
	}
	// -------------------------------

	// --- SCENE SERIALIZER ---
	Scene* SceneSerializer::Load(const std::string& scenePath)
	{
		try
		{
			YAML::Node sceneNode = YAML::LoadFile(scenePath);

			Scene* scene = new Scene();
			SerializationContext ctx;

			// Creation
			CreateActors(sceneNode["Actors"], ctx, scene);

			// Deserialization
			DeserializeEnvSettings(sceneNode["EnvironmentSettings"], scene);
			DeserializeActors(sceneNode["Actors"], ctx);

			Camera* camera = static_cast<Camera*>(ctx.GetComponentById(sceneNode["ActiveCamera"].as<unsigned int>()));
			scene->SetActiveCamera(camera);

			return scene;
		}
		catch (const YAML::BadFile& e)
		{
			LOG_ERROR("Scene file: '%s' cannot be loaded!", scenePath.c_str());
			return nullptr;
		}
		catch (const YAML::ParserException& e)
		{
			LOG_ERROR("Failed to parse scene file: '%s'. Line: '%d', Column: '%d', Msg: '%s'", scenePath.c_str(), e.mark.line, e.mark.column, e.msg.c_str());
			return nullptr;
		}
		catch (const YAML::Exception& e)
		{
			LOG_ERROR("%s", e.what());
			return nullptr;
		}

		return nullptr;
	}
	// ------------------------

	// --- SERIALIZATION CONTEXT ---
	void SerializationContext::Register(Actor* actor)
	{
		mIdToActorMap.emplace(mNextId, actor);
		mActorToIdMap.emplace(actor, mNextId);
		mNextId++;
	}

	void SerializationContext::Register(unsigned int id, Actor * actor)
	{
		mIdToActorMap.emplace(id, actor);
		mActorToIdMap.emplace(actor, id);
		if (id >= mNextId)
			mNextId = id + 1;
	}

	void SerializationContext::Register(Component * component)
	{
		mIdToComponentMap.emplace(mNextId, component);
		mComponentToIdMap.emplace(component, mNextId);
		mNextId++;

	}

	void SerializationContext::Register(unsigned int id, Component * component)
	{
		mIdToComponentMap.emplace(id, component);
		mComponentToIdMap.emplace(component, id);
		if (id >= mNextId)
			mNextId = id + 1;
	}

	Actor* SerializationContext::GetActorById(unsigned int id) const
	{
		auto it = mIdToActorMap.find(id);
		if (it == mIdToActorMap.end())
		{
			LOG_ERROR("No Actor by the id '%d' exists", id);
			return nullptr;
		}

		return it->second;
	}

	Component* SerializationContext::GetComponentById(unsigned int id) const
	{
		auto it = mIdToComponentMap.find(id);
		if (it == mIdToComponentMap.end())
		{
			LOG_ERROR("No Component by the id '%d' exists", id);
			return nullptr;
		}

		return it->second;
	}

	unsigned int SerializationContext::GetIdByActor(Actor* actor) const
	{
		auto it = mActorToIdMap.find(actor);
		if (it == mActorToIdMap.end())
		{
			LOG_ERROR("Tried to get id of an unregistered Actor");
			return 0;
		}

		return it->second;
	}

	unsigned int SerializationContext::GetIdByComponent(Component* component) const
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