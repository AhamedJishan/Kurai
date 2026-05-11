#pragma once
#include "Utils/Log.h"

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// NOTE:	Right now each Particle system has their own gpu buffers,
//			So when there are too many particle systems(around 5000) draw calls would increase
//			and drop the fps.
// FIX:		use a global buffer for particle system, all particle system append their data to that buffer
//			this will reduce the draw calls, as well as buffer swapping

namespace Dawn
{
	// should remain in sync with particle.vert shader
	const unsigned int MAX_CURVE_KEYS = 5;

	// Generic struct for over time data types
	template<typename T>
	struct Curve
	{
		void Reset()
		{
			mNumKeys = 0;
		}

		void AddKey(float time, T value)
		{
			if (mNumKeys >= MAX_CURVE_KEYS)
			{
				LOG_WARN("Curve can only have max 5 keys!");
				return;
			}

			mKeyValues[mNumKeys] = value;
			mKeyTimes[mNumKeys] = time;

			mNumKeys++;
		}

		int GetNumKeys() const { return mNumKeys; }
		const std::array<T, MAX_CURVE_KEYS>& GetKeyValues() const { return mKeyValues; }
		const std::array<float, MAX_CURVE_KEYS>& GetKeyTimes() const { return mKeyTimes; }

	private:
		// should be provided sorted
		std::array<T, MAX_CURVE_KEYS> mKeyValues{};
		std::array<float, MAX_CURVE_KEYS> mKeyTimes{};
		int mNumKeys = 0;
	};

	struct ParticleSystemDesc
	{
		bool isLooping = false;

		float duration = 1.0f;
		float particleLifetime = 1.0f;
		float emissionRate = 1.0f;
		float speed = 0.0f;
		
		unsigned int initialBurst = 0;
		
		glm::vec3 directionMin = glm::vec3(0);
		glm::vec3 directionMax = glm::vec3(0);

		Curve<glm::vec3> scaleOverTime;
		Curve<glm::vec4> colorOverTime;
	};

	struct ParticlePool
	{
		unsigned int particleCount = 0;

		std::vector<float> tValues;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> velocities;
	};

	// Forward declarations
	class Scene;
	class Shader;

	class ParticleSystem
	{
	public:
		ParticleSystem(Scene* scene, ParticleSystemDesc particleSystemDesc, glm::vec3 position);
		~ParticleSystem();

		// initialized in Renderer
		static void Init(unsigned int maxParticleCount = 50);
		static void Shutdown();

		void Update(float deltaTime);
		void Render(Shader* shader);

		const ParticlePool* GetParticlePool() const { return mParticlePool; }

		bool IsStopped() { return mIsStopped; }
		glm::vec3 GetPosition() const { return mPosition; }
		void SetPosition(glm::vec3 position) { mPosition = position; }

	private:
		static void InitCubeVAO();
		static void DestroyCubeVAO();
		void SpawnParticles(unsigned int count);

	private:
		Scene* mScene = nullptr;

		float mSystemTime = 0.0f;
		float mTimeSinceLastSpawn = 0.0f;

		bool mIsStopped = false;
		glm::vec3 mPosition = glm::vec3(0);

		ParticleSystemDesc mParticleSystemDesc;
		ParticlePool* mParticlePool = nullptr;

		static bool sIsInitialized;

		static unsigned int sCubeVAO;
		static unsigned int sCubeVBO;
		static unsigned int sCubeEBO;
		static unsigned int sParticleTVBO;
		static unsigned int sParticlePositionVBO;
		static unsigned int sMaxParticleCount;
	};
}