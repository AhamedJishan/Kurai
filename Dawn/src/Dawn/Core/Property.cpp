#include "Property.h"

#include <utility>
#include <yaml-cpp/yaml.h>
#include <Dawn/Utils/YamlGlm.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	void Property::Serialize(YAML::Node& node) const
	{
		switch (type)
		{
		case Dawn::PropertyType::Int:		node = *static_cast<int*>(data) ; break;
		case Dawn::PropertyType::Bool:		node = *static_cast<bool*>(data) ; break;
		case Dawn::PropertyType::Float:		node = *static_cast<float*>(data) ; break;
		case Dawn::PropertyType::Vec2:		node = *static_cast<glm::vec2*>(data); break;
		case Dawn::PropertyType::Vec3:		node = *static_cast<glm::vec3*>(data); break;
		case Dawn::PropertyType::Vec4:		node = *static_cast<glm::vec4*>(data); break;
		case Dawn::PropertyType::Quat:		node = *static_cast<glm::quat*>(data); break;
		case Dawn::PropertyType::String:	node = *static_cast<std::string*>(data); break;
		case Dawn::PropertyType::StringList:
		{
			for (const std::string& str : *static_cast<std::vector<std::string>*>(data))
				node.push_back(str);
			break;
		}
		case Dawn::PropertyType::StringPairList:
		{
			for (auto& [key, value] : *static_cast<std::vector<std::pair<std::string, std::string>>*>(data))
				node[key] = value;
			break;
		}
		default: break;
		}
	}

	void Property::Deserialize(const YAML::Node& node)
	{
		switch (type)
		{
		case Dawn::PropertyType::Int:		*static_cast<int*>(data) = node.as<int>(); break;
		case Dawn::PropertyType::Bool:		*static_cast<bool*>(data) = node.as<bool>(); break;
		case Dawn::PropertyType::Float:		*static_cast<float*>(data) = node.as<float>(); break;
		case Dawn::PropertyType::Vec2:		*static_cast<glm::vec2*>(data) = node.as<glm::vec2>(); break;
		case Dawn::PropertyType::Vec3:		*static_cast<glm::vec3*>(data) = node.as<glm::vec3>(); break;
		case Dawn::PropertyType::Vec4:		*static_cast<glm::vec4*>(data) = node.as<glm::vec4>(); break;
		case Dawn::PropertyType::Quat:		*static_cast<glm::quat*>(data) = node.as<glm::quat>(); break;
		case Dawn::PropertyType::String:	*static_cast<std::string*>(data) = node.as<std::string>(); break;
		case Dawn::PropertyType::StringList:
		{
			std::vector<std::string>* list = static_cast<std::vector<std::string>*>(data);
			list->clear();
			for (const YAML::Node& elementNode : node)
				list->push_back(elementNode.as<std::string>());
			break;
		}
		case Dawn::PropertyType::StringPairList:
		{
			std::vector<std::pair<std::string, std::string>>* stringPairList = static_cast<std::vector<std::pair<std::string, std::string>>*>(data);
			stringPairList->clear();
			for (auto it = node.begin(); it != node.end(); it++)
				stringPairList->emplace_back(it->first.as<std::string>(), it->second.as<std::string>());
			break;
		}
		default: break;
		}
	}
}