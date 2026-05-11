#pragma once

#include "Core/Actor.h"
#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	class DummyEnemy : public Actor
	{
	public:
		DummyEnemy(class Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/kamikaze/kamikaze.obj");
			SetPosition(mBasePosition);
			SetRotation(glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)));

			mTime = 0.0f;
		}

		void Update(float deltaTime)
		{
			mTime += mWaveSpeed * deltaTime;

			glm::vec3 offset = glm::vec3(0, mWaveSize * glm::sin(mTime), 0);

			SetPosition(mBasePosition + offset);
		}

	private:
		float mTime;
		const float mWaveSize = 0.2f;
		const float mWaveSpeed = 1.0f;

		const glm::vec3 mBasePosition = glm::vec3(0, 2, -7.0f);
	};
}