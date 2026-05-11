#pragma once

#include <string>
#include <glm/mat4x4.hpp>

namespace Dawn
{
	// forward declarations
	class AudioSystem;

	class SoundEvent
	{
	public:
		SoundEvent();

		bool IsValid() const;

		bool IsOneshot() const;
		bool Is3D() const;
		void Set3DAttributes(const glm::mat4& worldTranform) const;

		void Restart() const;
		void Stop(bool allowFadeout = true) const;

		void SetPaused(bool value);
		void SetPitch(float value);
		void SetVolume(float value);
		void SetParameter(const std::string& name, float value);

		bool GetPaused() const;
		float GetPitch() const;
		float GetVolume() const;
		float GetParameter(const std::string& name) const;

		const unsigned int GetId() const { return mId; }

	private:
		friend class AudioSystem;
		SoundEvent(AudioSystem* system, unsigned int id);

	private:
		AudioSystem* mSystem = nullptr;
		unsigned int mId;
	};
}