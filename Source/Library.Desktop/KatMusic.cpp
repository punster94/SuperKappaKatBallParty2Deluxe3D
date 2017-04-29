#include "pch.h"
#include "KatMusic.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(KatMusic)

	const string KatMusic::MusicFolder = "music/";
	const string KatMusic::MusicFileKey = "MusicFile";
	const string KatMusic::PlayOnAwakeKey = "PlayOnAwake";

	KatMusic::KatMusic() :
		mPlayOnAwake(0)
	{
		InitializeSignatures();

		// Always loop the music
		mMusic.setLoop(true);
	}

	KatMusic::~KatMusic()
	{
	}

	void KatMusic::Initialize(WorldState& worldState)
	{
		if (mPlayOnAwake)
		{
			Play();
		}
	}

	const string& KatMusic::GetMusicFile() const
	{
		return mMusicFileName;
	}

	void KatMusic::SetMusicFile(const string& file)
	{
		mMusicFileName = file;
	}

	void KatMusic::Play()
	{
		mMusic.openFromFile(MusicFolder + mMusicFileName);
		mMusic.play();
	}

	void KatMusic::Pause()
	{
		mMusic.pause();
	}

	void KatMusic::Stop()
	{
		mMusic.stop();
	}

	sf::SoundSource::Status KatMusic::GetStatus() const
	{
		return mMusic.getStatus();
	}

	float KatMusic::GetVolume() const
	{
		return mMusic.getVolume();
	}

	void KatMusic::SetVolume(float volume)
	{
		mMusic.setVolume(volume);
	}

	void KatMusic::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(MusicFileKey, &mMusicFileName);
		AddExternalAttribute(PlayOnAwakeKey, &mPlayOnAwake);
	}
}
