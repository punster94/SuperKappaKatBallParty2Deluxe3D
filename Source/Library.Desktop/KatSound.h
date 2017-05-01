#pragma once
#include "Entity.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace FieaGameEngine
{
	class KatSound : public Entity
	{
		RTTI_DECLARATIONS(KatSound, Entity)

	public:
		KatSound();
		KatSound(const KatSound& other);
		~KatSound();

		void Initialize(WorldState& worldState) override;
		Scope* Copy() const override;
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
		void InitializeSignatures() override;

	private:
		std::string mSoundFileName;
		std::float_t mVolume;
		sf::Sound mSound;

		void CopyPrivateDataMembers(const KatSound& other);

		static HashMap<std::string, std::shared_ptr<sf::SoundBuffer>> sBufferMap;

	public:
		static const std::string SoundFolder;
		static const std::string SoundFileKey;
		static const std::string VolumeKey;
	};

	ConcreteEntityFactory(KatSound);
}
