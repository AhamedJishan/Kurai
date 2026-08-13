#pragma once

#include <string>

namespace Dawn
{
	enum class PropertyType
	{
		None = 0,
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
		std::string name;
		PropertyType type;
		void* data;
	};
}