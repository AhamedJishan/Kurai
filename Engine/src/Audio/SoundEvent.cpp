#include "SoundEvent.h"

#include <fmod/fmod_studio.hpp>
#include "AudioSystem.h"

namespace Dawn
{
	SoundEvent::SoundEvent(AudioSystem* system, unsigned int id)
		:mSystem(system)
		,mId(id)
	{
	}

	SoundEvent::SoundEvent()
		:mSystem(nullptr)
		,mId(0)
	{
	}

	bool SoundEvent::IsValid() const
	{
		return (mSystem && mSystem->GetEventInstance(mId) != nullptr);
	}

	bool SoundEvent::IsOneshot() const
	{
		bool isOneshot = false;
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
		{
			FMOD::Studio::EventDescription* ed = nullptr;
			event->getDescription(&ed);
			if (ed)
				ed->isOneshot(&isOneshot);
		}
		return isOneshot;
	}

	bool SoundEvent::Is3D() const
	{
		bool is3D = false;
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
		{
			FMOD::Studio::EventDescription* ed = nullptr;
			event->getDescription(&ed);
			if (ed)
				ed->is3D(&is3D);
		}
		return is3D;
	}

	void SoundEvent::Restart() const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
			event->start();
	}

	void SoundEvent::Stop(bool allowFadeOut) const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;

		if (event)
			event->stop(mode);
	}

	void SoundEvent::Set3DAttributes(const glm::mat4& worldTranform) const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
		{
			FMOD_3D_ATTRIBUTES attribute;

			attribute.position  = VecToFmodVec(worldTranform[3][0], worldTranform[3][1], worldTranform[3][2]);
			attribute.forward	= VecToFmodVec(worldTranform[2][0], worldTranform[2][1], worldTranform[2][2]);
			attribute.up		= VecToFmodVec(worldTranform[1][0], worldTranform[1][1], worldTranform[1][2]);
			attribute.velocity	= { 0, 0, 0 };

			event->set3DAttributes(&attribute);
		}
	}

	void SoundEvent::SetPaused(bool pause)
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
			event->setPaused(pause);
	}

	void SoundEvent::SetVolume(float value)
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
			event->setVolume(value);
	}

	void SoundEvent::SetPitch(float value)
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
			event->setPitch(value);
	}

	void SoundEvent::SetParameter(const std::string& name, float value)
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		if (event)
			event->setParameterByName(name.c_str(), value);
	}

	bool SoundEvent::GetPaused() const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		bool paused = false;
		if (event)
			event->getPaused(&paused);

		return paused;
	}

	float SoundEvent::GetVolume() const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		float volume = 0.0f;
		if (event)
			event->getVolume(&volume);

		return volume;
	}

	float SoundEvent::GetPitch() const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		float pitch = 0.0f;
		if (event)
			event->getPitch(&pitch);

		return pitch;
	}

	float SoundEvent::GetParameter(const std::string& name) const
	{
		FMOD::Studio::EventInstance* event = mSystem ? mSystem->GetEventInstance(mId) : nullptr;

		float value = 0.0f;
		if (event)
			event->getParameterByName(name.c_str(), &value);

		return value;
	}
}