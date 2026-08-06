#pragma once

#include <glm/vec3.hpp>
#include "Dawn/Rendering/Material.h"

namespace Dawn
{
	// forward declaration
	class Shader;

	class Sky : public Material
	{
	public:
		Sky();
		~Sky();

		void Apply(const Shader* shader) override;
		const char* GetName() const override;

		void SetTopColor(glm::vec3 color) { mTop = color; }
		void SetHorizonColor(glm::vec3 color) { mHorizon = color; }

		glm::vec3 GetTopColor() { return mTop; }
		glm::vec3 GetHorizonColor() { return mHorizon; }

	private:
		glm::vec3 mTop;
		glm::vec3 mHorizon;
	};
}