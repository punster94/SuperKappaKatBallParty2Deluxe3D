#include "pch.h"
#include "KatMusic.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(KatMusic)

	const string KatMusic::MusicFolder = "music/";
	const string KatMusic::MusicFileKey = "MusicFile";
	const string KatMusic::PlayOnAwakeKey = "PlayOnAwake";
	const string KatMusic::VolumeKey = "Volume";

	KatMusic::KatMusic() :
		mPlayOnAwake(0), mVolume(100.0f)
	{
		InitializeSignatures();

		// Always loop the music
		mMusic.setLoop(true);
	}

	KatMusic::KatMusic(const KatMusic& other):
		Entity(other)
	{
		CopyPrivateDataMembers(other);
	}

	KatMusic::~KatMusic()
	{
	}

	void KatMusic::Initialize(WorldState& worldState)
	{
		Entity::Initialize(worldState);

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
		mMusic.setVolume(mVolume);
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
		return mVolume;
	}

	void KatMusic::SetVolume(float volume)
	{
		mVolume = volume;
	}

	void KatMusic::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(MusicFileKey, &mMusicFileName);
		AddExternalAttribute(PlayOnAwakeKey, &mPlayOnAwake);
		AddExternalAttribute(VolumeKey, &mVolume);
	}

	void KatMusic::CopyPrivateDataMembers(const KatMusic& other)
	{
		mMusicFileName = other.mMusicFileName;
		mPlayOnAwake = other.mPlayOnAwake;
	}

	Scope* KatMusic::Copy() const
	{
		return new KatMusic(*this);
	}
}
