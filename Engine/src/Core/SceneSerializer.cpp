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

	void DeserializeComponents(const YAML::Node& componentsNode, Actor* owner)
	{
		ComponentFactory* componentFactory = Application::Get()->GetComponentFactory();

		for (const YAML::Node& componentNode : componentsNode)
		{
			Component* component = componentFactory->Create(componentNode["Type"].as<std::string>(), owner);
			component->Deserialize(componentNode);
		}
	}

	void DeserializeActors(const YAML::Node& actorsNode, Scene* scene)
	{
		for (const YAML::Node& actorNode : actorsNode)
		{
			const YAML::Node& transformNode = actorNode["Transform"];
			const std::string& actorStateStr = actorNode["State"].as<std::string>();

			Actor* actor = scene->CreateActor(actorNode["Name"].as<std::string>());
			Transform& transform = actor->GetTransform();

			transform.Scale = ToVec3(transformNode["Scale"]);
			transform.Position = ToVec3(transformNode["Position"]);
			transform.Rotation = ToQuat(transformNode["Rotation"]);

			if		(actorStateStr == "Active") actor->SetState(Actor::State::Active);
			else if (actorStateStr == "Paused") actor->SetState(Actor::State::Paused);
			else								actor->SetState(Actor::State::Dead);

			DeserializeComponents(actorNode["Components"], actor);
		}
	}

	Scene* SceneSerializer::Load(const std::string& scenePath)
	{
		try
		{
			YAML::Node sceneFile = YAML::LoadFile(scenePath);

			Scene* scene = new Scene();
			DeserializeEnvSettings(sceneFile["EnvironmentSettings"], scene);
			DeserializeActors(sceneFile["Actors"], scene);

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
}