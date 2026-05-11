#include "AudioSystem.h"
#include "Utils/Log.h"

#include <vector>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>
#include "SoundEvent.h"

namespace Dawn
{
	unsigned int AudioSystem::sNextId = 0;

	FMOD_VECTOR VecToFmodVec(glm::vec3 vec)
	{
		FMOD_VECTOR retVec;

		retVec.x = vec.x;
		retVec.y = vec.y;
		retVec.z = vec.z;

		return retVec;
	}

	FMOD_VECTOR VecToFmodVec(float x, float y, float z)
	{
		FMOD_VECTOR retVec;

		retVec.x = x;
		retVec.y = y;
		retVec.z = z;

		return retVec;
	}

	AudioSystem::AudioSystem()
	{
		if (!Init())
		{
			LOG_ERROR("Failed to initialise AudioSystem!");
			Shutdown();
		}
	}

	AudioSystem::~AudioSystem()
	{
		Shutdown();
	}
	
	bool AudioSystem::Init()
	{
		FMOD_RESULT result;

		result = FMOD::Studio::System::create(&mSystem);
		if (result != FMOD_OK)
		{
			LOG_ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			return false;
		}

		result = mSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK)
		{
			LOG_ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			return false;
		}

		// Load master and string banks
		LoadBank("Assets/Audio/Master.strings.bank");
		LoadBank("Assets/Audio/Master.bank");

		return true;
	}

	void AudioSystem::Shutdown()
	{
		UnloadAllBanks();

		if (mSystem)
			mSystem->release();
	}

	void AudioSystem::LoadBank(const std::string& name)
	{
		auto it = mBanks.find(name);
		if (it != mBanks.end())
		{
			LOG_WARN("Bank '%s' is already loaded", name.c_str());
			return;
		}

		FMOD_RESULT result;
		// Load the bank
		FMOD::Studio::Bank* bank = nullptr;
		result = mSystem->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);

		if (result != FMOD_OK)
		{
			LOG_WARN("Failed to load bank '%s'", name.c_str());
			return;
		}

		bank->loadSampleData();
		mBanks.emplace(name, bank);

		const int maxPathLength = 512;

		// Load all EventDescriptions
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);

			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* event = events[i];
				event->getPath(eventName, maxPathLength, nullptr);
				mEvents.emplace(eventName, event);
			}
		}

		// Load all Buses
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);

			char busName[maxPathLength];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				bus->getPath(busName, maxPathLength, nullptr);
				mBuses.emplace(busName, bus);
			}
		}
	}

	void AudioSystem::UnloadBank(const std::string& name)
	{
		auto it = mBanks.find(name);
		if (it == mBanks.end())
		{
			LOG_WARN("Bank '%s' isn't present", name.c_str());
			return;
		}

		FMOD::Studio::Bank* bank = it->second;

		const int maxPathLength = 512;

		// Unload all EventDescriptions
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);

			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* event = events[i];
				event->getPath(eventName, maxPathLength, nullptr);
				
				auto eventIt = mEvents.find(eventName);
				if (eventIt != mEvents.end())
					mEvents.erase(eventIt);
			}
		}

		// Unload all Buses
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);

			char busName[maxPathLength];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				bus->getPath(busName, maxPathLength, nullptr);
				
				auto busIt = mBuses.find(busName);
				if (busIt != mBuses.end())
					mBuses.erase(busIt);
			}
		}

		bank->unloadSampleData();
		bank->unload();

		mBanks.erase(it);
	}

	void AudioSystem::UnloadAllBanks()
	{
		for (auto& it : mBanks)
		{
			it.second->unloadSampleData();
			it.second->unload();
		}
		mBanks.clear();

		mEvents.clear();
		mBuses.clear();
	}

	void AudioSystem::Update()
	{
		// Remove invalid event instances
		for (auto it = mEventInstances.begin(); it != mEventInstances.end(); )
		{
			FMOD::Studio::EventInstance* event = it->second;

			FMOD_STUDIO_PLAYBACK_STATE state;
			event->getPlaybackState(&state);

			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				event->release();
				it = mEventInstances.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Update FMOD
		if (mSystem) mSystem->update();
	}

	SoundEvent AudioSystem::PlayEvent(const std::string& name, const glm::vec3 position)
	{
		unsigned int eventId = 0;

		auto iter = mEvents.find(name);
		if (iter != mEvents.end())
		{
			FMOD::Studio::EventInstance* event = nullptr;
			iter->second->createInstance(&event);

			if (event)
			{
				bool is3D = false;
				iter->second->is3D(&is3D);

				if (is3D)
				{
					FMOD_3D_ATTRIBUTES attr;
					attr.position = VecToFmodVec(position);
					event->set3DAttributes(&attr);
				}

				event->start();

				eventId = ++sNextId;
				mEventInstances.emplace(eventId, event);
			}

			return SoundEvent(this, eventId);
		}

		LOG_WARN("Event '%s' is not a part of any loaded audio banks", name.c_str());
		return SoundEvent();
	}

	void AudioSystem::SetListener(const glm::mat4& viewMatrix)
	{
		glm::mat4 invViewMat = glm::inverse(viewMatrix);

		FMOD_3D_ATTRIBUTES listener;

		listener.position = VecToFmodVec(invViewMat[3][0], invViewMat[3][1], invViewMat[3][2]);
		listener.forward = VecToFmodVec(invViewMat[2][0], invViewMat[2][1], invViewMat[2][2]);
		listener.up = VecToFmodVec(invViewMat[1][0], invViewMat[1][1], invViewMat[1][2]);

		listener.velocity = { 0, 0, 0 };

		mSystem->setListenerAttributes(0, &listener);
	}

	void AudioSystem::SetBusVolume(const std::string& name, float value)
	{
		auto it = mBuses.find(name);
		if (it == mBuses.end())
		{
			LOG_ERROR("Bus '%s' doesn't exist", name.c_str());
			return;
		}

		it->second->setVolume(value);
	}
	
	void AudioSystem::SetBusPaused(const std::string& name, bool value)
	{
		auto it = mBuses.find(name);
		if (it == mBuses.end())
		{
			LOG_ERROR("Bus '%s' doesn't exist", name.c_str());
			return;
		}

		it->second->setPaused(value);
	}
	
	float AudioSystem::GetBusVolume(const std::string& name)
	{
		float volume = 0.0f;

		auto it = mBuses.find(name);
		if (it == mBuses.end())
		{
			LOG_ERROR("Bus '%s' doesn't exist", name.c_str());
			return volume;
		}

		it->second->getVolume(&volume);

		return volume;
	}
	
	bool AudioSystem::GetBusPaused(const std::string& name)
	{
		bool paused = false;

		auto it = mBuses.find(name);
		if (it == mBuses.end())
		{
			LOG_ERROR("Bus '%s' doesn't exist", name.c_str());
			return paused;
		}

		it->second->getPaused(&paused);

		return paused;
	}

	FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
	{
		auto it = mEventInstances.find(id);
		if (it != mEventInstances.end())
			return it->second;

		return nullptr;
	}
}