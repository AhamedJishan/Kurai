#pragma once

#include "Core/Actor.h"

#include <Core/Components/MeshRenderer.h>
#include <Core/Components/Animator.h>
#include <Asset/Assets.h>

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
			const std::string modelPath = "Assets/Models/archer/textures/archer robot dance.fbx";
			MeshRenderer::CreateFromModel(this, modelPath, true);

			Animator* animator = new Animator(this);
			animator->SetSkeleton(Assets::GetSkeleton(modelPath));
			animator->AddClip("Test", Assets::GetAnimationClip(modelPath));

			animator->Play("Test");

			GetTransform().Scale = glm::vec3(0.015f);
		}
	};
}