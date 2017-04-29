#pragma once
#include "Attributed.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace FieaGameEngine
{
	class KatSound : public Attributed
	{
		RTTI_DECLARATIONS(KatSound, Attributed)

	public:
		static const std::string SoundFolder;

		KatSound();
		~KatSound();

		const std::string& GetSoundFile() const;
		void SetSoundFile(const std::string& file);

		// SFML Music class wrappers
		void Play();
		void Pause();
		void Stop();
		sf::SoundSource::Status GetStatus() const;
		float GetVolume() const;
		void SetVolume(float volume);

		// static methods for caching sound buffers
		static sf::SoundBuffer& LoadSoundBuffer(const std::string& file);
		static void UnloadSoundBuffer(const std::string& file);
		static void UnloadAllSoundBuffers();

	protected:
		void InitializeSignatures();

	private:
		std::string mSoundFileName;
		sf::Sound mSound;

		static HashMap<std::string, std::shared_ptr<sf::SoundBuffer>> sBufferMap;
	};
}