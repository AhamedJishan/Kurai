#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Dawn/Core/Property.h>
#include "EditorContext.h"

namespace Dawn
{
	// Forward declarations
	class Actor;
	class Component;

	class Inspector
	{
	public:
		void Draw(EditorContext& ctx);

	private:
		void DrawComponent(Component* component);
		bool DrawProperty(Property property);
		void DrawAddComponent(Actor* actor);

		void DrawTransform(Actor* actor);
		bool DrawQuatInputField(const char* label, glm::quat& value, float speed = 0.1f, float min = 0.0f, float max = 0.0f, const char* format = "%.6g");
		bool DrawStringInputField(const char* label, std::string* value);
		bool DrawAssetPathInputField(const char* label, std::string* value);
		bool DrawStringListInputField(const char* label, std::vector<std::string>* value);
		bool DrawStringPairListInputField(const char* label, std::vector<std::pair<std::string, std::string>>* value);
	};
}