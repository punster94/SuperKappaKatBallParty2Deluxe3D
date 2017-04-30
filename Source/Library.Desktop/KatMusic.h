#pragma once
#include "Entity.h"
#include <SFML/Audio/Music.hpp>

namespace FieaGameEngine
{
	class KatMusic final : public Entity
	{
		RTTI_DECLARATIONS(KatMusic, Entity)

	public:
		KatMusic();
		~KatMusic();

		KatMusic(const KatMusic& otherKatMusic);

		void Initialize(WorldState& worldState) override;
		virtual FieaGameEngine::Scope* Copy() const override;
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
		std::int32_t mPlayOnAwake;
		sf::Music mMusic;

		void CopyPrivateDataMembers(const KatMusic& otherKatMusic);

	public:
		static const std::string MusicFolder;
		static const std::string MusicFileKey;
		static const std::string PlayOnAwakeKey;
	};

	ConcreteEntityFactory(KatMusic);
}
