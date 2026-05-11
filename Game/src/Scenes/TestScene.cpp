#include "TestScene.h"

#include <glm/vec3.hpp>
#include "Actors/Arena.h"
#include "Actors/Player.h"
#include "Actors/FPSCameraActor.h"
#include "Actors/Gun.h"
#include "Actors/SkyDome.h"
#include "Core/Components/Camera.h"
#include "Input/Input.h"

namespace Dawn
{
	TestScene::TestScene()
	{
		GetEnvironmentSettings().bloomRadius = 2.0f;
		GetEnvironmentSettings().bloomStrength = 0.01f;
		GetEnvironmentSettings().fogDensity = 0.02f;
		GetEnvironmentSettings().fogColor = glm::vec3(0.06f, 0.075f, 0.10f) * 0.65f;
		GetEnvironmentSettings().ambientColor = glm::vec3(0.09f);
		GetEnvironmentSettings().directionalLight.color = glm::vec3(0.6f, 0.7f, 0.85f) * 1.3f;
		GetEnvironmentSettings().directionalLight.direction = glm::vec3(-0.5f, -0.2f, -0.2f);
		GetEnvironmentSettings().directionalLight.intensity = 1.0f;
	}

	TestScene::~TestScene()
	{

	}
	
	void TestScene::Init()
	{
		Input::SetCursorLocked(true);

		// Sky dome
		SkyDome* sky = new SkyDome(this);

		// ARENA
		Arena* arena = new Arena(this);

		// PLAYER
		FPSCameraActor* cameraActor = new FPSCameraActor(this);
		mPlayer = new Player(this, cameraActor);
		//mPlayer->SetPosition(glm::vec3(0, 0, 4));
		Gun* gun = new Gun(this, mPlayer);

		Camera* cam = cameraActor->GetComponent<Camera>();
		if (cam)
		{
			cam->SetFarPlane(1000.0f);	// So that skydome is visible
			SetActiveCamera(cam);
		}
	}

	void TestScene::Update(float deltaTime)
	{

	}
	
	void TestScene::ImGuiRender()
	{
		DrawCrossHair();
	}

	void TestScene::DrawCrossHair()
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImDrawList* drawList = ImGui::GetForegroundDrawList();
		drawList->AddCircleFilled(center, 2.0f, IM_COL32(255, 255, 255, 255), 8);
	}
}