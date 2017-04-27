#pragma once
#include "Attributed.h"
#include <SFML/Audio/Music.hpp>

namespace FieaGameEngine
{
	class KatMusic final : public Attributed
	{
		RTTI_DECLARATIONS(KatMusic, Attributed)

	public:
		static const std::string MusicFolder;

		KatMusic();
		~KatMusic();

		const std::string& GetMusicFile() const;
		void SetMusicFile(const std::string& file);

		// SFML Music class wrappers
		void Play();
		void Pause();
		void Stop();
		sf::SoundSource::Status GetStatus() const;
		float GetVolume() const;
		void SetVolume(float volume);

	protected:
		void InitializeSignatures();

	private:
		std::string mMusicFileName;
		sf::Music mMusic;
	};
}
