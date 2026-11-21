#include "SceneSerializer.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Dawn/Utils/Log.h>
#include <Dawn/Utils/YamlGlm.h>
#include <Dawn/Core/Property.h>
#include "Application.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "Scene.h"
#include "Actor.h"
#include "Transform.h"
#include "Components/Camera.h"
#include "SceneSerializationContext.h"

namespace Dawn
{
	// --- SCENE SERIALIZER Helper ---
	YAML::Node SerializeEnvSettings(const EnvironmentSettings& env)
	{
		YAML::Node envSettingsNode;
		const EnvironmentSettings::DirectionalLight& dirLight = env.directionalLight;

		YAML::Node dirLightNode = envSettingsNode["DirectionalLight"];

		envSettingsNode["AmbientColor"] = env.ambientColor;
		envSettingsNode["BloomRadius"] = env.bloomRadius;
		envSettingsNode["BloomStrength"] = env.bloomStrength;
		envSettingsNode["FogColor"] = env.fogColor;
		envSettingsNode["FogDensity"] = env.fogDensity;
		dirLightNode["Color"] = dirLight.color;
		dirLightNode["Intensity"] = dirLight.intensity;
		dirLightNode["Direction"] = dirLight.direction;
		return envSettingsNode;
	}

	void DeserializeEnvSettings(const YAML::Node& envSettingsNode, Scene* scene)
	{
		EnvironmentSettings& env = scene->GetEnvironmentSettings();
		EnvironmentSettings::DirectionalLight& dirLight = env.directionalLight;

		const YAML::Node& dirLightNode = envSettingsNode["DirectionalLight"];

		env.ambientColor = envSettingsNode["AmbientColor"].as<glm::vec3>();
		env.bloomRadius = envSettingsNode["BloomRadius"].as<float>();
		env.bloomStrength = envSettingsNode["BloomStrength"].as<float>();
		env.fogColor = envSettingsNode["FogColor"].as<glm::vec3>();
		env.fogDensity = envSettingsNode["FogDensity"].as<float>();
		dirLight.color = dirLightNode["Color"].as<glm::vec3>();
		dirLight.direction = dirLightNode["Direction"].as<glm::vec3>();
		dirLight.intensity = dirLightNode["Intensity"].as<float>();
	}

	void BuildSerializationContext(const Scene* scene, SceneSerializationContext& ctx)
	{
		if (!scene)
			return;

		ctx.Clear();
		for (Actor* actor : scene->GetActors())
		{
			ctx.Register(actor);

			for (Component* component : actor->GetComponents())
				ctx.Register(component);
		}
	}

	YAML::Node SerializeActors(const Scene* scene, SceneSerializationContext& ctx)
	{
		YAML::Node actorsNode;
		for (Actor* actor : scene->GetActors())
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
			transformNode["Scale"] = transform.scale;
			transformNode["Position"] = transform.position;
			transformNode["Rotation"] = transform.rotation;

			// --- COMPONENTS ---
			YAML::Node componentsNode = actorNode["Components"];
			for (Component* component : actor->GetComponents())
			{
				YAML::Node componentNode;
				componentNode["Id"] = ctx.GetIdByComponent(component);
				componentNode["Type"] = Application::Get()->GetComponentFactory()->GetComponentName(component);

				std::vector<Property> properties = component->GetProperties();
				for (Property property : properties)
					property.Serialize(componentNode[property.name]);

				componentsNode.push_back(componentNode);
			}

			actorsNode.push_back(actorNode);
		}
		return actorsNode;
	}

	void DeserializeActors(const YAML::Node& actorsNode, SceneSerializationContext& ctx)
	{
		for (const YAML::Node& actorNode : actorsNode)
		{
			const YAML::Node& transformNode = actorNode["Transform"];
			Actor* actor = ctx.GetActorById(actorNode["Id"].as<unsigned int>());

			Transform& transform = actor->GetTransform();
			transform.scale = transformNode["Scale"].as<glm::vec3>();
			transform.position = transformNode["Position"].as<glm::vec3>();
			transform.rotation = transformNode["Rotation"].as<glm::quat>();

			const std::string& actorStateStr = actorNode["State"].as<std::string>();
			if		(actorStateStr == "Active") actor->SetState(Actor::State::Active);
			else if (actorStateStr == "Paused") actor->SetState(Actor::State::Paused);
			else if (actorStateStr == "Dead")	actor->SetState(Actor::State::Dead);
			else LOG_ERROR("Actor '%s' has invalid state '%s'", actor->GetName().c_str(), actorStateStr.c_str());

			const YAML::Node& componentsNode = actorNode["Components"];
			for (const YAML::Node& componentNode : componentsNode)
			{
				Component* component = ctx.GetComponentById(componentNode["Id"].as<unsigned int>());

				std::vector<Property> properties = component->GetProperties();
				for (Property& property : properties)
				{
					if (!componentNode[property.name].IsDefined())
						continue;
					property.Deserialize(componentNode[property.name]);
				}
				component->OnPropertiesChanged();
			}
		}
	}

	void CreateComponents(const YAML::Node& componentsNode, SceneSerializationContext& ctx, Actor* owner)
	{
		ComponentFactory* componentFactory = Application::Get()->GetComponentFactory();
		for (const YAML::Node& componentNode : componentsNode)
		{
			unsigned int id = componentNode["Id"].as<unsigned int>();
			Component* component = componentFactory->Create(componentNode["Type"].as<std::string>(), owner);
			ctx.Register(id, component);
		}
	}

	void CreateActors(const YAML::Node& actorsNode, SceneSerializationContext& ctx, Scene* scene)
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
	Scene* SceneSerializer::Deserialize(const std::filesystem::path& scenePath)
	{
		std::ifstream sceneFile(scenePath);
		if (!sceneFile)
		{
			LOG_ERROR("Failed to open '%s' for deserializing scene!", scenePath.string().c_str());
			return nullptr;
		}
		
		std::stringstream buffer;
		buffer << sceneFile.rdbuf();
		std::string serializedScene = buffer.str();

		return Deserialize(serializedScene);
	}

	bool SceneSerializer::Serialize(const Scene* scene, const std::filesystem::path& scenePath)
	{
		std::string serializedScene;

		if (!Serialize(scene, serializedScene))
			return false;

		std::ofstream sceneFile(scenePath);
		if (!sceneFile)
		{
			LOG_ERROR("Failed to open '%s' for serializing scene!", scenePath.string().c_str());
			return false;
		}

		sceneFile << serializedScene;

		return true;
	}

	bool SceneSerializer::Serialize(const Scene* scene, std::string& outSerializedScene)
	{
		if (!scene)
		{
			LOG_ERROR("Null scene provided to serialize");
			return false;
		}

		YAML::Node sceneNode;

		SceneSerializationContext ctx;
		BuildSerializationContext(scene, ctx);

		Camera* camera = scene->GetMainCamera();
		unsigned int cameraId = 0;
		if (camera) cameraId = ctx.GetIdByComponent(camera);

		sceneNode["MainCamera"] = cameraId;
		sceneNode["EnvironmentSettings"] = SerializeEnvSettings(scene->GetEnvironmentSettings());
		sceneNode["Actors"] = SerializeActors(scene, ctx);

		outSerializedScene = YAML::Dump(sceneNode);

		return true;
	}

	Scene* SceneSerializer::Deserialize(const std::string& serializedScene)
	{
		Scene* scene = new Scene();
		try
		{
			YAML::Node sceneNode = YAML::Load(serializedScene.c_str());

			SceneSerializationContext ctx;
			CreateActors(sceneNode["Actors"], ctx, scene);
			DeserializeActors(sceneNode["Actors"], ctx);
			DeserializeEnvSettings(sceneNode["EnvironmentSettings"], scene);

			unsigned int cameraId = sceneNode["MainCamera"].as<unsigned int>(0);
			if (cameraId != 0)
			{
				Camera* camera = static_cast<Camera*>(ctx.GetComponentById(cameraId));
				scene->SetMainCamera(camera);
			}

			return scene;
		}
		catch (const YAML::ParserException& e)
		{
			LOG_ERROR("Failed to parse scene file. Line: '%d', Column: '%d', Msg: '%s'",
				e.mark.line, e.mark.column, e.msg.c_str());
		}
		catch (const YAML::Exception& e)
		{
			LOG_ERROR("%s", e.what());
		}

		delete scene;
		return nullptr;
	}
	// -----------------------
}