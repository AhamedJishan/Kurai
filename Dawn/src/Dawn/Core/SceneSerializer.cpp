#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Dawn/Utils/Log.h>
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
	YAML::Node ToYaml(const glm::vec3& vec)
	{
		YAML::Node vecNode;
		vecNode.SetStyle(YAML::EmitterStyle::Flow);
		vecNode.push_back(vec.x);
		vecNode.push_back(vec.y);
		vecNode.push_back(vec.z);
		return vecNode;
	}
	YAML::Node ToYaml(const glm::quat& quat)
	{
		YAML::Node quatNode;
		quatNode.SetStyle(YAML::EmitterStyle::Flow);
		quatNode.push_back(quat.w);
		quatNode.push_back(quat.x);
		quatNode.push_back(quat.y);
		quatNode.push_back(quat.z);
		return quatNode;
	}
	// --------------------

	// --- SCENE SERIALIZER Helper ---
	YAML::Node SerializeEnvSettings()
	{
		YAML::Node envSettingsNode;
		EnvironmentSettings& env = Application::Get()->GetScene()->GetEnvironmentSettings();
		EnvironmentSettings::DirectionalLight& dirLight = env.directionalLight;

		YAML::Node dirLightNode = envSettingsNode["DirectionalLight"];

		envSettingsNode["AmbientColor"] = ToYaml(env.ambientColor);
		envSettingsNode["BloomRadius"] = env.bloomRadius;
		envSettingsNode["BloomStrength"] = env.bloomStrength;
		envSettingsNode["FogColor"] = ToYaml(env.fogColor);
		envSettingsNode["FogDensity"] = env.fogDensity;
		dirLightNode["Color"] = ToYaml(dirLight.color);
		dirLightNode["Intensity"] = dirLight.intensity;
		dirLightNode["Direction"] = ToYaml(dirLight.direction);
		return envSettingsNode;
	}

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

	void BuildSerializationContext(SerializationContext& ctx)
	{
		ctx.Clear();
		for (Actor* actor : Application::Get()->GetScene()->GetActors())
		{
			ctx.Register(actor);

			for (Component* component : actor->GetComponents())
				ctx.Register(component);
		}
	}

	YAML::Node SerializeActors(SerializationContext& ctx)
	{
		YAML::Node actorsNode;
		for (Actor* actor : Application::Get()->GetScene()->GetActors())
		{
			YAML::Node actorNode;
			actorNode["Id"] = ctx.GetIdByActor(actor);
			actorNode["Name"] = actor->GetName();

			Actor::State state = actor->GetState();
			if		(state == Actor::State::Active) actorNode["State"] = "Active";
			else if (state == Actor::State::Paused) actorNode["State"] = "Paused";
			// no need to serialize if the actor is already dead

			YAML::Node transformNode = actorNode["Transform"];

			Transform& transform = actor->GetTransform();
			transformNode["Scale"] = ToYaml(transform.Scale);
			transformNode["Position"] = ToYaml(transform.Position);
			transformNode["Rotation"] = ToYaml(transform.Rotation);

			YAML::Node componentsNode = actorNode["Components"];
			for (Component* component : actor->GetComponents())
			{
				YAML::Node componentNode;
				componentNode["Id"] = ctx.GetIdByComponent(component);
				componentNode["Type"] = Application::Get()->GetComponentFactory()->GetComponentName(component);
				component->Serialize(componentNode, ctx);
				componentsNode.push_back(componentNode);
			}

			actorsNode.push_back(actorNode);
		}
		return actorsNode;
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
			else if (actorStateStr == "Dead")	actor->SetState(Actor::State::Dead);
			else LOG_ERROR("Actor '%s' has invalid state '%s'", actor->GetName().c_str(), actorStateStr.c_str());

			const YAML::Node& componentsNode = actorNode["Components"];
			for (const YAML::Node& componentNode : componentsNode)
			{
				Component* component = ctx.GetComponentById(componentNode["Id"].as<unsigned int>());
				component->Deserialize(componentNode, ctx);
			}
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

			unsigned int cameraId = sceneNode["ActiveCamera"].as<unsigned int>();
			if (cameraId != 0)
			{
				Camera* camera = static_cast<Camera*>(ctx.GetComponentById(cameraId));
				scene->SetActiveCamera(camera);
			}

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

	void SceneSerializer::Save(const std::string& scenePath)
	{
		std::ofstream sceneFile(scenePath);
		if (!sceneFile)
		{
			LOG_ERROR("Failed to open '%s' for saving scene!", scenePath.c_str());
			return;
		}

		YAML::Node sceneNode;

		SerializationContext ctx;
		BuildSerializationContext(ctx);
		
		Camera* camera = Application::Get()->GetScene()->GetActiveCamera();
		unsigned int cameraId = 0;
		if (camera) cameraId = ctx.GetIdByComponent(camera);

		sceneNode["ActiveCamera"] = cameraId;
		sceneNode["EnvironmentSettings"] = SerializeEnvSettings();
		sceneNode["Actors"] = SerializeActors(ctx);

		sceneFile << sceneNode;
	}
	// ------------------------

	// --- SERIALIZATION CONTEXT ---
	void SerializationContext::Clear()
	{
		mNextId = 1;
		mActorToIdMap.clear();
		mIdToActorMap.clear();
		mComponentToIdMap.clear();
		mIdToComponentMap.clear();
	}

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

	Component* SerializationContext::GetComponentById(unsigned int id) const
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