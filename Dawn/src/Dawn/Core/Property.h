#pragma once

#include <string>

// Forward declarations
namespace YAML
{
	class Node;
}

namespace Dawn
{
	enum class PropertyType
	{
		Bool,
		Int,
		Float,
		String,
		Vec2,
		Vec3,
		Vec4,
		Quat
	};

	struct Property
	{
	public:
		void Serialize(YAML::Node& node) const;
		void Deserialize(const YAML::Node& node);

	public:
		std::string name;
		void* data;
		PropertyType type;

	};
}