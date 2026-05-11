#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"
#include "Rendering/Materials/Sky.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class SkyDome : public Actor
	{
	public:
		SkyDome(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/skydome/skydome.obj");

			MeshRenderer* mr = GetComponent<MeshRenderer>();

			Sky* skyMat = new Sky();
			skyMat->SetTopColor(glm::vec3(0.025f, 0.035f, 0.05f) * 0.4f);
			skyMat->SetHorizonColor(glm::vec3(0.06f, 0.075f, 0.10f) * 0.65f);

			mr->SetMaterial(skyMat);

			SetScale(glm::vec3(500.0f));
		}
	};
}