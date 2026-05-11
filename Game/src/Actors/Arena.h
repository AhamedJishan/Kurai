#pragma once

#include "Core/Actor.h"

#include <glm/vec3.hpp>
#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class Arena : public Actor
	{
	public:
		Arena(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/arena/arena.obj");
		}

		// Ignores top bound
		bool IsOutOfBounds(glm::vec3 position)
		{
			if (position.x > mBounds.x	|| 
				position.x < -mBounds.x ||
				position.z > mBounds.y	||
				position.z < -mBounds.y ||
				position.y < 0.0f)
				return true;

			return false;
		}

		glm::vec2 GetBounds() const { return mBounds; }

	private:
		glm::vec2 mBounds = glm::vec2(28.0f);
	};
}