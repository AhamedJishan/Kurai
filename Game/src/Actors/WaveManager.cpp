#include "WaveManager.h"
#include "Utils/Log.h"

#include "Core/Scene.h"
#include "Player.h"
#include "Arena.h"
#include "EnemyKamikaze.h"

namespace Dawn
{
	WaveManager::WaveManager(Scene* scene, Player* player, Arena* arena)
		:Actor(scene)
		,mPlayer(player)
		,mArena(arena)
	{
		Init();
	}
	WaveManager::~WaveManager()
	{
	}

	void WaveManager::Init()
	{
		// === WAVE 1 ===
		Wave wave1;
		wave1.phases.push_back(Phase(0.0f, 3, 2));
		wave1.phases.push_back(Phase(5.0f, 5, 0));

		// === WAVE 2 ===
		Wave wave2;
		wave2.phases.push_back(Phase(0.0f, 5, 1));
		wave2.phases.push_back(Phase(3.0f, 5, 2));
		wave2.phases.push_back(Phase(0.0f, 5, 3));

		// === WAVE 3 ===
		Wave wave3;
		wave3.phases.push_back(Phase(0.0f, 7, 0));
		wave3.phases.push_back(Phase(3.0f, 5, 1));
		wave3.phases.push_back(Phase(0.0f, 5, 2));
		wave3.phases.push_back(Phase(0.0f, 5, 3));
		wave3.phases.push_back(Phase(3.0f, 10, 0));

		// === WAVE 4 ===
		Wave wave4;
		wave4.phases.push_back(Phase(0.0f, 10, 0));
		wave4.phases.push_back(Phase(4.0f, 10, 1));
		wave4.phases.push_back(Phase(0.0f, 5, 2));
		wave4.phases.push_back(Phase(0.0f, 5, 3));
		wave4.phases.push_back(Phase(3.0f, 10, 0));
		wave4.phases.push_back(Phase(0.0f, 10, 1));
		wave4.phases.push_back(Phase(0.0f, 10, 2));

		mWaves.push_back(wave1);
		mWaves.push_back(wave2);
		mWaves.push_back(wave3);
		mWaves.push_back(wave4);

		mWaveState = WaveState::Begin;
	}
	
	void WaveManager::Update(float deltaTime)
	{
		switch (mWaveState)
		{
			case Dawn::WaveState::Begin:
			{
				Begin();
				break;
			}
			case Dawn::WaveState::WaveStarting:
			{
				mWaveTimer += deltaTime;
				if (mWaveTimer >= mWaveStartDuration)
				{
					mWaveState = WaveState::WaveActive;

					mWaveTimer = 0.0f;
					mPhaseTimer = 0.0f;

					mCurrentPhaseIndex = 0;
					mWaveEnemiesRemaining = GetTotalEnemyCountForWave(mCurrentWaveIndex);
					mWaveEnemiesDied = 0;
					mEnemies.clear();
				}
				break;
			}
			case Dawn::WaveState::WaveActive:
			{
				mPhaseTimer += deltaTime;

				UpdateWaveEnemyState();

				const Wave& currentWave = mWaves[mCurrentWaveIndex];

				if (mCurrentPhaseIndex < currentWave.phases.size())
				{
					const Phase& currentPhase = currentWave.phases[mCurrentPhaseIndex];

					if (mPhaseTimer >= currentPhase.triggerTime)
					{
						mPhaseTimer = 0.0f;
						QueueSpawn(currentPhase.enemyCount, currentPhase.spawnLocationIndex);
						mCurrentPhaseIndex++;
					}
				}

				for (unsigned int spawnLocationIndex = 0; spawnLocationIndex < NUM_SPAWN_LOCATIONS; spawnLocationIndex++)
				{
					mTimeSinceLastSpawn[spawnLocationIndex] += deltaTime;

					if ((mSpawnQueue[spawnLocationIndex] > 0) && (mTimeSinceLastSpawn[spawnLocationIndex] >= mTimeBetweenSpawns))
					{
						mSpawnQueue[spawnLocationIndex] -= 1;
						mTimeSinceLastSpawn[spawnLocationIndex] = 0.0f;
						EnemyKamikaze* enemy = new EnemyKamikaze(mScene, mPlayer, this, mArena);
						enemy->SetPosition(mSpawnLocations[spawnLocationIndex]);
						mEnemies.push_back(enemy);
					}
				}
				
				if (mCurrentPhaseIndex >= currentWave.phases.size() && mWaveEnemiesRemaining == 0)
				{
					mWaveTimer = 0;
					mWaveState = WaveState::WaveClear;
				}

				break;
			}
			case Dawn::WaveState::WaveClear:
			{
				if ((mCurrentWaveIndex + 1) >= mWaves.size())
					mWaveState = WaveState::End;

				mWaveTimer += deltaTime;
				if (mWaveTimer >= mWaveClearDuration)
				{
					mCurrentWaveIndex++;
					mCurrentPhaseIndex = 0;

					mWaveState = WaveState::WaveStarting;
					mSpawnQueue.fill(0);
					mTimeSinceLastSpawn.fill(0.0f);
					mWaveTimer = 0.0f;
				}

				break;
			}
			case Dawn::WaveState::End:
			{
				// TODO: Victory
				break;
			}
			default:
			{
				// Do nothing
				break;
			}
		}
	}

	void WaveManager::QueueSpawn(unsigned int enemyCount, unsigned int spawnLocationIndex)
	{
		if (spawnLocationIndex >= NUM_SPAWN_LOCATIONS)
		{
			LOG_WARN("Spawn location index '%d', is out of range!", spawnLocationIndex);
			return;
		}

		mSpawnQueue[spawnLocationIndex] += enemyCount;
		mTimeSinceLastSpawn[spawnLocationIndex] = 0.0f;
	}

	unsigned int WaveManager::GetTotalEnemyCountForWave(unsigned int waveIndex)
	{
		if (waveIndex >= mWaves.size())
		{
			LOG_WARN("Wave index '%d', is out of range!", waveIndex);
			return 0;
		}

		unsigned int enemyCount = 0;

		for (const Phase& phase : mWaves[waveIndex].phases)
			enemyCount += phase.enemyCount;

		return enemyCount;
	}

	void WaveManager::Begin()
	{
		mWaveState = WaveState::WaveStarting;

		mSpawnQueue.fill(0);
		mTimeSinceLastSpawn.fill(0.0f);

		mWaveTimer = 0.0f;
		mPhaseTimer = 0.0f;

		mCurrentWaveIndex = 0;
		mCurrentPhaseIndex = 0;

		mWaveEnemiesRemaining = 0;
		mWaveEnemiesDied = 0;

		mEnemies.clear();
	}

	void WaveManager::UpdateWaveEnemyState()
	{
		auto newEnd = std::remove_if(mEnemies.begin(), mEnemies.end(),
			[this](EnemyKamikaze* e)
			{
				return !mScene->ContainsActor(e);
			});
		unsigned int enemiesKilledThisFrame = std::distance(newEnd, mEnemies.end());
		mEnemies.erase(newEnd, mEnemies.end());

		mWaveEnemiesDied += enemiesKilledThisFrame;
		mWaveEnemiesRemaining = GetTotalEnemyCountForWave(mCurrentWaveIndex) - mWaveEnemiesDied;
	}
}