#include "GameLayer.h"

#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderView.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Core/Components/Camera.h>

namespace Dawn
{
	GameLayer::GameLayer(const std::filesystem::path& startScenePath)
		:mStartScenePath(startScenePath)
		,mGameRV(std::make_unique<RenderView>(glm::uvec2(2u), true))
	{
	}
	
	void GameLayer::OnAttach()
	{
		Application::Get()->LoadScene(mStartScenePath);
	}
	
	void GameLayer::OnRender()
	{
		Scene* scene = Application::Get()->GetScene();
		if (!scene) return;

		Camera* camera = scene->GetMainCamera();
		if (!camera) return;

		mGameRV->SetCamera(camera->GetPosition(), camera->GetView(), camera->GetProjection(), camera->GetResolution());
		Application::Get()->GetRenderer()->Render(mGameRV.get());
	}
}