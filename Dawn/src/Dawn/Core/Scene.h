#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward Declarations;
	class Actor;
	class Camera;
	class Application;

	struct EnvironmentSettings
	{
		// Default 1.0f
		float bloomRadius = 1.0f;
		// Default 0.01f
		float bloomStrength = 0.01f;

		float fogDensity = 0.0f;
		glm::vec3 fogColor = glm::vec3(1);

		glm::vec3 ambientColor = glm::vec3(0.2f);

		struct DirectionalLight
		{
			glm::vec3 direction = glm::vec3(-1);
			glm::vec3 color = glm::vec3(1);
			float intensity = 1.0f;
		} directionalLight;
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Update(float deltaTime, bool simulate);

		const std::vector<Actor*>& GetActors() const { return mActors; }

		Actor* CreateActor(const std::string& name = "Actor");
		void DestroyActor(Actor* actor);

		EnvironmentSettings& GetEnvironmentSettings() { return mEnvironmentSettings; }
		const EnvironmentSettings& GetEnvironmentSettings() const { return mEnvironmentSettings; }

		void SetMainCamera(Camera* camera);
		Camera* GetMainCamera() const { return mMainCamera; }

		void MarkDirty(bool dirty = true) { mIsDirty = dirty; }
		void ClearDirty() { mIsDirty = false; }
		bool IsDirty() { return mIsDirty; }

		static std::string GetExtension() { return ".scene"; }

	private:
		void DeleteActor(Actor* actor);

	private:
		bool mIsDirty = false;
		bool mUpdatingActors = false;

		Camera* mMainCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;

		EnvironmentSettings mEnvironmentSettings;
	};
}