#include "ParticleSystem.h"

#include "Utils/Random.h"
#include <glad/glad.h>
#include "Rendering/Shader.h"
#include "Core/Scene.h"

namespace Dawn
{
	bool ParticleSystem::sIsInitialized = false;

	unsigned  int ParticleSystem::sCubeVAO = 0;
	unsigned  int ParticleSystem::sCubeVBO = 0;
	unsigned  int ParticleSystem::sCubeEBO = 0;
	unsigned  int ParticleSystem::sParticleTVBO = 0;
	unsigned  int ParticleSystem::sParticlePositionVBO = 0;
	unsigned  int ParticleSystem::sMaxParticleCount = 0;

	ParticleSystem::ParticleSystem(Scene* scene, ParticleSystemDesc particleSystemDesc, glm::vec3 position)
		:mScene(scene)
		,mParticleSystemDesc(particleSystemDesc)
		,mParticlePool(new ParticlePool())
		,mPosition(position)
	{
		mScene->AddParticleSystem(this);

		if (!sIsInitialized)
		{
			mIsStopped = true;
			LOG_WARN("ParticleSystem hasn't been initialized yet!");
			return;
		}

		SpawnParticles(mParticleSystemDesc.initialBurst);
	}

	ParticleSystem::~ParticleSystem()
	{
		delete mParticlePool;
		mScene->RemoveParticleSystem(this);
	}

	void ParticleSystem::Init(unsigned int maxParticleCount)
	{
		sIsInitialized = true;
		sMaxParticleCount = maxParticleCount;
		InitCubeVAO();
	}

	void ParticleSystem::Shutdown()
	{
		sIsInitialized = false;
		DestroyCubeVAO();
	}
	
	void ParticleSystem::Update(float deltaTime)
	{
		if (!sIsInitialized)
		{
			mIsStopped = true;
			LOG_WARN("ParticleSystem hasn't been initialized yet!");
			return;
		}

		mSystemTime += deltaTime;

		if (!mParticleSystemDesc.isLooping && mSystemTime >= mParticleSystemDesc.duration + mParticleSystemDesc.particleLifetime)
		{
			mIsStopped = true;
			return;
		}

		bool emissionActive = mParticleSystemDesc.emissionRate > 0.0f && 
			(mParticleSystemDesc.isLooping || mSystemTime < mParticleSystemDesc.duration);
		if (emissionActive)
		{
			mTimeSinceLastSpawn += deltaTime;
			unsigned int particlesToSpawn = 0;
			float spawnInterval = 1.0f / glm::max(mParticleSystemDesc.emissionRate, 0.0001f);
			while (mTimeSinceLastSpawn >= spawnInterval)
			{
				mTimeSinceLastSpawn -= spawnInterval;
				particlesToSpawn++;
			}
			SpawnParticles(particlesToSpawn);
		}

		for (int i = mParticlePool->particleCount - 1; i >= 0; i--)
		{
			mParticlePool->tValues[i] += deltaTime / mParticleSystemDesc.particleLifetime;
			mParticlePool->positions[i] += mParticlePool->velocities[i] * deltaTime;

			// check for dead particles
			if (mParticlePool->tValues[i] >= 1.0f)
			{
				// swap with last and pop
				int last = mParticlePool->particleCount - 1;
				mParticlePool->tValues[i] = mParticlePool->tValues[last];
				mParticlePool->positions[i] = mParticlePool->positions[last];
				mParticlePool->velocities[i] = mParticlePool->velocities[last];

				mParticlePool->tValues.pop_back();
				mParticlePool->positions.pop_back();
				mParticlePool->velocities.pop_back();
				mParticlePool->particleCount--;
			}
		}
	}

	void ParticleSystem::Render(Shader* shader)
	{
		if (!sIsInitialized)
		{
			mIsStopped = true;
			LOG_WARN("ParticleSystem hasn't been initialized yet!");
			return;
		}

		if (mParticlePool->particleCount == 0)
			return;

		shader->SetInt("u_NumScaleKeys", mParticleSystemDesc.scaleOverTime.GetNumKeys());
		shader->SetVec3s("u_ScaleKeyValues",
			mParticleSystemDesc.scaleOverTime.GetKeyValues().data(),
			mParticleSystemDesc.scaleOverTime.GetNumKeys());
		shader->SetFloats("u_ScaleKeyTimes",
			mParticleSystemDesc.scaleOverTime.GetKeyTimes().data(),
			mParticleSystemDesc.scaleOverTime.GetNumKeys());

		shader->SetInt("u_NumColorKeys", mParticleSystemDesc.colorOverTime.GetNumKeys());
		shader->SetVec4s("u_ColorKeyValues",
			mParticleSystemDesc.colorOverTime.GetKeyValues().data(),
			mParticleSystemDesc.colorOverTime.GetNumKeys());
		shader->SetFloats("u_ColorKeyTimes",
			mParticleSystemDesc.colorOverTime.GetKeyTimes().data(),
			mParticleSystemDesc.colorOverTime.GetNumKeys());

		// upload tValues
		glBindBuffer(GL_ARRAY_BUFFER, sParticleTVBO);
		glBufferData(GL_ARRAY_BUFFER, mParticlePool->particleCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mParticlePool->particleCount * sizeof(float), mParticlePool->tValues.data());

		// upload positions
		glBindBuffer(GL_ARRAY_BUFFER, sParticlePositionVBO);
		glBufferData(GL_ARRAY_BUFFER, mParticlePool->particleCount * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mParticlePool->particleCount * 3 * sizeof(float), mParticlePool->positions.data());

		glBindVertexArray(sCubeVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, mParticlePool->particleCount);
		glBindVertexArray(0);
	}

	void ParticleSystem::SpawnParticles(unsigned int count)
	{
		if (!sIsInitialized)
		{
			mIsStopped = true;
			LOG_WARN("ParticleSystem hasn't been initialized yet!");
			return;
		}

		if (count == 0)
			return;

		unsigned int particleCount = mParticlePool->particleCount;
		if (particleCount > sMaxParticleCount)
			return;

		if (particleCount + count > sMaxParticleCount)
			count = sMaxParticleCount - particleCount;


		mParticlePool->tValues.reserve(mParticlePool->tValues.size() + count);
		mParticlePool->positions.reserve(mParticlePool->positions.size() + count);
		mParticlePool->velocities.reserve(mParticlePool->velocities.size() + count);

		for (unsigned int i = 0; i < count; i++)
		{
			glm::vec3 velocity = Random::Vec3(mParticleSystemDesc.directionMin, mParticleSystemDesc.directionMax);
			if (glm::length(velocity) > 0.0001f)
				velocity = glm::normalize(velocity) * mParticleSystemDesc.speed;
			else
				velocity = glm::vec3(0);

			mParticlePool->velocities.push_back(velocity);
			mParticlePool->tValues.push_back(0.0f);
			mParticlePool->positions.push_back(mPosition);
		}

		mParticlePool->particleCount += count;
	}

	void ParticleSystem::InitCubeVAO()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f, // 0
			 0.5f, -0.5f, -0.5f, // 1
			 0.5f,  0.5f, -0.5f, // 2
			-0.5f,  0.5f, -0.5f, // 3
			-0.5f, -0.5f,  0.5f, // 4
			 0.5f, -0.5f,  0.5f, // 5
			 0.5f,  0.5f,  0.5f, // 6
			-0.5f,  0.5f,  0.5f  // 7
		};

		// 12 triangles (36 indices)
		unsigned int indices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,
			// Front face
			4, 5, 6,
			6, 7, 4,
			// Left face
			7, 3, 0,
			0, 4, 7,
			// Right face
			1, 2, 6,
			6, 5, 1,
			// Bottom face
			0, 1, 5,
			5, 4, 0,
			// Top face
			3, 2, 6,
			6, 7, 3
		};

		glGenVertexArrays(1, &sCubeVAO);
		glGenBuffers(1, &sCubeVBO);
		glGenBuffers(1, &sCubeEBO);
		glGenBuffers(1, &sParticleTVBO);
		glGenBuffers(1, &sParticlePositionVBO);

		glBindVertexArray(sCubeVAO);

		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sCubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, sCubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Particle tValues
		glBindBuffer(GL_ARRAY_BUFFER, sParticleTVBO);
		glBufferData(GL_ARRAY_BUFFER, sMaxParticleCount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 1);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
		
		// Particle ppositions
		glBindBuffer(GL_ARRAY_BUFFER, sParticlePositionVBO);
		glBufferData(GL_ARRAY_BUFFER, sMaxParticleCount* 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}

	void ParticleSystem::DestroyCubeVAO()
	{
		glDeleteBuffers(1, &sCubeVBO);
		glDeleteBuffers(1, &sCubeEBO);
		glDeleteBuffers(1, &sParticleTVBO);
		glDeleteBuffers(1, &sParticlePositionVBO);
		glDeleteVertexArrays(1, &sCubeVAO);
	}
}