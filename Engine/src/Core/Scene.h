#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

namespace Dawn
{
	// Forward Declarations;
	class Actor;
	class Camera;
	class SphereCollider;
	class Application;
	class ParticleSystem;
	namespace Physics
	{
		struct Ray;
		struct Sphere;
	}

	struct RaycastHit
	{
		Actor* actor;
		glm::vec3 position;
	};

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
	private:
		friend class Application;
		void UpdateActors(float deltaTime);
		void ResolveCollisions();

		friend class Actor;
		void AddActor(Actor* actor);
		void RemoveActor(Actor* actor);

		friend class SphereCollider;
		void AddSphereCollider(SphereCollider* collider);
		void RemoveSphereCollider(SphereCollider* collider);

		friend class ParticleSystem;
		void AddParticleSystem(ParticleSystem* particleSystem);
		void RemoveParticleSystem(ParticleSystem* particleSystem);

	public:
		Scene();
		virtual ~Scene();

		// Scenes can implement this to populate the Scene with Actors
		virtual void Init() {}
		virtual void Update(float deltaTime) {}
		virtual void ImGuiRender() {}


		bool ContainsActor(Actor* actor);

		void SetPaused(bool value) { mIsPaused = value; }
		bool IsPaused() const { return mIsPaused; }

		void SetEnvironmentSettings(const EnvironmentSettings& settings) { mEnvironmentSettings = settings; }
		EnvironmentSettings& GetEnvironmentSettings() { return mEnvironmentSettings; }
		const EnvironmentSettings& GetEnvironmentSettings() const { return mEnvironmentSettings; }

		void SetActiveCamera(Camera* camera) { mActiveCamera = camera; }
		Camera* GetActiveCamera() const { return mActiveCamera; }
		const std::vector<ParticleSystem*>& GetParticleSystems() const { return mParticleSystems; }

		// Physics Utilities
		const std::vector<SphereCollider*>& GetColliderList() const { return mColliders; }
		bool RayCast(const Physics::Ray& ray, float maxDistance, RaycastHit& outHitInfo);

	private:
		bool mUpdatingActors = false;
		// if true, actors won't get updated
		bool mIsPaused = false;

		Camera* mActiveCamera = nullptr;

		std::vector<Actor*> mActors;
		std::vector<Actor*> mPendingActors;

		std::vector<SphereCollider*> mColliders;

		std::vector<ParticleSystem*> mParticleSystems;

		EnvironmentSettings mEnvironmentSettings;
	};
}