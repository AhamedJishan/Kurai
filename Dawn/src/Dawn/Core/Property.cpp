#include "Property.h"

#include <yaml-cpp/yaml.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Dawn
{
	// --- YAML helpers ---
	glm::vec2 ToVec2(const YAML::Node& node)
	{
		return {
			node[0].as<float>(),
			node[1].as<float>()
		};
	}
	glm::vec3 ToVec3(const YAML::Node& node)
	{
		return {
			node[0].as<float>(),
			node[1].as<float>(),
			node[2].as<float>()
		};
	}
	glm::vec4 ToVec4(const YAML::Node& node)
	{
		return {
			node[0].as<float>(),
			node[1].as<float>(),
			node[2].as<float>(),
			node[3].as<float>()
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
	// -------------------------


	void Property::Serialize(YAML::Node& node) const
	{
		YAML::Node& valueNode = node[name];
		switch (type)
		{
		case Dawn::PropertyType::Bool:		valueNode = *static_cast<bool*>(data) ; break;
		case Dawn::PropertyType::Int:		valueNode = *static_cast<int*>(data) ; break;
		case Dawn::PropertyType::Float:		valueNode = *static_cast<float*>(data) ; break;
		case Dawn::PropertyType::String:	valueNode = *static_cast<std::string*>(data); break;
		case Dawn::PropertyType::Vec2:		valueNode = *static_cast<glm::vec2*>(data); break;
		case Dawn::PropertyType::Vec3:		valueNode = *static_cast<glm::vec3*>(data); break;
		case Dawn::PropertyType::Vec4:		valueNode = *static_cast<glm::vec4*>(data); break;
		case Dawn::PropertyType::Quat:		valueNode = *static_cast<glm::quat*>(data); break;
		default: break;
		}
	}

	void Property::Deserialize(const YAML::Node& node)
	{
		const YAML::Node& valueNode = node[name];
		switch (type)
		{
		case Dawn::PropertyType::Bool:		*static_cast<bool*>(data) = valueNode.as<bool>(); break;
		case Dawn::PropertyType::Int:		*static_cast<int*>(data) = valueNode.as<int>(); break;
		case Dawn::PropertyType::Float:		*static_cast<float*>(data) = valueNode.as<float>(); break;
		case Dawn::PropertyType::String:	*static_cast<std::string*>(data) = valueNode.as<std::string>(); break;
		case Dawn::PropertyType::Vec2:		*static_cast<glm::vec2*>(data) = ToVec2(valueNode); break;
		case Dawn::PropertyType::Vec3:		*static_cast<glm::vec3*>(data) = ToVec3(valueNode); break;
		case Dawn::PropertyType::Vec4:		*static_cast<glm::vec4*>(data) = ToVec4(valueNode); break;
		case Dawn::PropertyType::Quat:		*static_cast<glm::quat*>(data) = ToQuat(valueNode); break;
		default: break;
		}
	}
}