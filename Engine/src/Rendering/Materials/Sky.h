#pragma once

#include <glm/vec3.hpp>
#include "Rendering/Material.h"

namespace Dawn
{
	class Sky : public Material
	{
	public:
		Sky();
		~Sky();

		void Apply() override;

		void SetTopColor(glm::vec3 color) { mTop = color; }
		void SetHorizonColor(glm::vec3 color) { mHorizon = color; }

		glm::vec3 GetTopColor() { return mTop; }
		glm::vec3 GetHorizonColor() { return mHorizon; }

	private:
		glm::vec3 mTop;
		glm::vec3 mHorizon;
	};
}