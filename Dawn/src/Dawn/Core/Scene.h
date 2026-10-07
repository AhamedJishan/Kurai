#pragma once

#include <vector>
#include <string>
#include <filesystem>
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
		Scene(const std::filesystem::path& path);
		~Scene();

		void Update(float deltaTime);

		const std::vector<Actor*>& GetActors() const { return mActors; }

		Actor* CreateActor(const std::string& name = "Actor");
		void DestroyActor(Actor* actor);

		void SetPaused(bool value) { mIsPaused = value; }
		bool IsPaused() const { return mIsPaused; }

		EnvironmentSettings& GetEnvironmentSettings() { return mEnvironmentSettings; }

		void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
		Camera* GetActiveCamera() const { return mActiveCamera; }

		void SetPath(const std::filesystem::path& path) { mPath = path; }
		const std::filesystem::path& GetPath() { return mPath; }
		std::string GetName() { return mPath.stem().string(); }

		void MarkDirty() { mIsDirty = true; }
		void ClearDirty() { mIsDirty = false; }
		bool IsDirty() { return mIsDirty; }

	private:
		void DeleteActor(Actor* actor);

	private:
		std::filesystem::path mPath;

		bool mIsDirty = false;
		// if true, actors won't get updated
		bool mIsPaused = false;
		bool mUpdatingActors = false;

		Camera* mActiveCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;

		EnvironmentSettings mEnvironmentSettings;
	};
}