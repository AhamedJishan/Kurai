#include "Camera.h"

#include "Dawn/Core/Actor.h"
#include <Dawn/Core/Application.h>
#include <Dawn/Core/Window.h>
#include <Dawn/Core/Property.h>
#include <Dawn/Rendering/Renderer.h>
#include <Dawn/Rendering/RenderView.h>
#include <Dawn/Rendering/Texture.h>

namespace Dawn
{
	static const glm::uvec2 MIN_RESOLUTION = glm::uvec2(2);

	std::vector<Property> Camera::GetProperties()
	{
		return 
		{
			{"FOV", &mFOV, PropertyType::Float},
			{"Near", &mNear, PropertyType::Float},
			{"Far", &mFar, PropertyType::Float},
			{"Resolution", &mResolution, PropertyType::UVec2},
			{"RendersToScreen",& mRendersToScreen, PropertyType::Bool }
		};
	}

	Camera::Camera(Actor* owner)
		:Component(owner)
	{
		Window* window = Application::Get()->GetWindow();
		mResolution = glm::max(glm::uvec2(window->GetWidth(), window->GetHeight()), MIN_RESOLUTION);

		mRenderView = std::make_unique<RenderView>(mResolution, true);
		Application::Get()->GetRenderer()->AddRenderView(mRenderView.get());

		// TODO: SetMain camera if it's the first camera
		// SyncREnderView()
	}

	Camera::~Camera()
	{
		Application::Get()->GetRenderer()->RemoveRenderView(mRenderView.get());

		// TODO: Remove main camera if it is.
	}

	void Camera::OnPropertiesChanged()
	{
		mFOV = glm::max(mFOV, 0.001f);
		mResolution = glm::max(mResolution, MIN_RESOLUTION);

		// TODO: handle scene main camera

		if (mRendersToScreen != mRenderView->RendersToScreen())
			SetRenderToScreen(mRendersToScreen);

		if (mResolution != mRenderView->GetResolution())
			SetResolution(mResolution);
	}

	void Camera::Update(float deltaTime)
	{
		mRenderView->Update(mOwner->GetTransform().position, GetView(), GetProjection());
	}

	glm::mat4 Camera::GetView() const
	{
		glm::vec3 position = mOwner->GetTransform().position;
		glm::vec3 forward  = mOwner->GetTransform().GetForward();
		glm::vec3 up = mOwner->GetTransform().GetUp();

		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 Camera::GetProjection() const
	{
		glm::uvec2 res = mRenderView->GetResolution();
		return glm::perspectiveFov(glm::radians(mFOV), static_cast<float>(res.x), static_cast<float>(res.y), mNear, mFar);
	}

	glm::vec3 Camera::UnProject(float screenX, float screenY, float depth)
	{
		float screenWidth = static_cast<float>(Application::Get()->GetWindow()->GetWidth());
		float screenHeight = static_cast<float>(Application::Get()->GetWindow()->GetHeight());

		float ndcX = (screenX / screenWidth) * 2.0f - 1.0f;
		float ndcY = 1.0f - (screenY / screenHeight) * 2.0f;
		float ndcZ = depth * 2.0f - 1.0f;
		glm::vec4 clipPos(ndcX, ndcY, ndcZ, 1.0f);

		glm::mat4 projection = glm::perspectiveFov(glm::radians(mFOV), static_cast<float>(mResolution.x), static_cast<float>(mResolution.y), mNear, mFar);
		glm::mat4 invViewProjection = glm::inverse(projection * GetView());

		glm::vec4 worldPos = invViewProjection * clipPos;
		worldPos /= worldPos.w;

		return glm::vec3(worldPos);
	}

	void Camera::SetResolution(glm::uvec2 resolution)
	{
		mResolution = glm::max(resolution, MIN_RESOLUTION);
		mRenderView->SetResolution(mResolution);
	}

	void Camera::SetRenderToScreen(bool enable)
	{
		mRendersToScreen = enable;
		mRenderView->SetRenderToScreen(mRendersToScreen);
	}

	bool Camera::RendersToScreen() const
	{
		return mRendersToScreen;
	}

	const Texture* Camera::GetOutputTexture() const
	{
		return mRenderView->GetOutputTexture();
	}
}