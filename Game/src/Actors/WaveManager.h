#pragma once

#include "Core/Actor.h"

#include <vector>
#include <array>
#include <glm/vec3.hpp>

namespace Dawn
{
	const unsigned int NUM_SPAWN_LOCATIONS = 4;
	const float ARENA_RADIUS = 27.0f;

	// A wave will have multiple phases
	struct Phase
	{
		Phase(float _triggerTime, unsigned int _enemyCount, unsigned int _spawnLocationIndex)
		{
			triggerTime = _triggerTime;
			enemyCount = _enemyCount;
			spawnLocationIndex = _spawnLocationIndex;
		}

		// Time needed after the execution of the previous phase
		float  triggerTime = 0.0f;

		unsigned int enemyCount = 0;
		unsigned int spawnLocationIndex = 0;
		
		bool hasTriggered = false;
	};

	struct Wave
	{
		std::vector<Phase> phases;
	};

	enum class WaveState
	{
		Begin,
		WaveStarting,
		WaveActive,
		WaveClear,
		End
	};

	// Forward declarations
	class Scene;
	class EnemyKamikaze;
	class Player;
	class Arena;

	class WaveManager : public Actor
	{
	public:
		WaveManager(Scene* scene, Player* player, Arena* arena);
		~WaveManager();

		void Init();

		void Update(float deltaTime) override;

		const std::vector<EnemyKamikaze*>& GetEnemies() const { return mEnemies; }
		WaveState GetWaveState() const { return mWaveState; }
		unsigned int GetWaveEnemiesRemaining() const { return mWaveEnemiesRemaining; }
		unsigned int GetCurrentWaveIndex() const { return mCurrentWaveIndex; }

		float GetWaveTimer() { return mWaveTimer; }
		// time taken before wave active
		float GetWaveStartDuration() const { return mWaveStartDuration; }
		// time taken before next wave start
		float GetWaveClearDuration() const { return mWaveClearDuration; }
		void SkipWaveClearDelay() { 
			if (mWaveState == WaveState::WaveClear)
				mWaveTimer = mWaveClearDuration;
		}

	private:
		void QueueSpawn(unsigned int enemyCount, unsigned int spawnLocationIndex);
		unsigned int GetTotalEnemyCountForWave(unsigned int waveIndex);
		void Begin();
		void UpdateWaveEnemyState();

	private:
		Player* mPlayer = nullptr;
		Arena* mArena = nullptr;
		WaveState mWaveState = WaveState::Begin;

		std::array<glm::vec3, NUM_SPAWN_LOCATIONS> mSpawnLocations = 
		{
			glm::vec3(0.0f, 1.0f,  ARENA_RADIUS),
			glm::vec3(ARENA_RADIUS, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, -ARENA_RADIUS),
			glm::vec3(-ARENA_RADIUS, 1.0f, 0.0f)
		};

		std::array<unsigned int, NUM_SPAWN_LOCATIONS> mSpawnQueue = {0, 0, 0, 0};
		// prevents enemies from being spawned on top of each other
		std::array<float, NUM_SPAWN_LOCATIONS> mTimeSinceLastSpawn = { 0.0f, 0.0f, 0.0f, 0.0f};
		const float mTimeBetweenSpawns = 0.1f;

		std::vector<Wave> mWaves;
		float mWaveTimer = 0.0f;
		float mPhaseTimer = 0.0f;
		const float mWaveStartDuration = 3.0f;
		const float mWaveClearDuration = 15.0f;
		unsigned int mCurrentWaveIndex = 0;
		unsigned int mCurrentPhaseIndex = 0;

		unsigned int mWaveEnemiesRemaining = 0;
		unsigned int mWaveEnemiesDied = 0;
		std::vector<EnemyKamikaze*> mEnemies;
	};
}