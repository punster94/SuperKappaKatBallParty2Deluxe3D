#include "pch.h"
#include "KatMusic.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(KatMusic)

	const string KatMusic::MusicFolder = "music/";

	KatMusic::KatMusic()
	{
		InitializeSignatures();

		// Always loop the music
		mMusic.setLoop(true);
	}

	KatMusic::~KatMusic()
	{
	}

	const string& KatMusic::GetMusicFile() const
	{
		return mMusicFileName;
	}

	void KatMusic::SetMusicFile(const string& file)
	{
		mMusicFileName = file;
		mMusic.openFromFile(MusicFolder + mMusicFileName);
	}

	void KatMusic::Play()
	{
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

		AddExternalAttribute("ExternalString", &mMusicFileName);
	}
}
