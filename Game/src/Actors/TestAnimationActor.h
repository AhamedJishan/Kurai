#pragma once

#include "Core/Actor.h"

#include "Core/Components/MeshRenderer.h"

namespace Dawn
{
	// Forward declarations
	class Scene;

	class TestAnimationActor : public Actor
	{
	public:
		TestAnimationActor(Scene* scene)
			:Actor(scene)
		{
			MeshRenderer::CreateFromModel(this, "Assets/Models/archer/Hip Hop Dancing.dae");
		}
	};
}