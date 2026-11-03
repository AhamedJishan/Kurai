#include "GameLayer.h"

#include <Dawn/Utils/Log.h>
#include <Dawn/Core/Window.h>
#include <Dawn/Core/Application.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderView.h>
#include <Dawn/Core/Scene.h>
#include <Dawn/Core/Components/Camera.h>
#include <Dawn/Core/Actor.h>

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

		int resX, resY;
		Application::Get()->GetWindow()->GetFrameBufferSize(resX, resY);

		mGameRV->SetResolution(glm::uvec2(static_cast<unsigned int>(resX), static_cast<unsigned int>(resY)));	// early-outs if unchanged
		mGameRV->SetCamera(camera->GetOwner()->GetTransform().position, camera->GetView(), camera->GetProjection());
		Application::Get()->GetRenderer()->Render(mGameRV.get());
	}
}