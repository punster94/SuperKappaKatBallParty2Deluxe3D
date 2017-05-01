#include "pch.h"
#include "KatSound.h"

using namespace std;
using namespace sf;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(KatSound)

	const string KatSound::SoundFolder = "sfx/";
	const string KatSound::SoundFileKey = "SoundFile";
	const string KatSound::PlayOnAwakeKey = "PlayOnAwake";
	const string KatSound::VolumeKey = "Volume";
	HashMap<string, shared_ptr<SoundBuffer>> KatSound::sBufferMap;

	KatSound::KatSound() :
		mPlayOnAwake(0), mVolume(100.0f)
	{
		InitializeSignatures();
	}

	KatSound::KatSound(const KatSound& other)
	{
		CopyPrivateDataMembers(other);
	}

	KatSound::~KatSound()
	{
	}

	void KatSound::Initialize(WorldState& worldState)
	{
		Entity::Initialize(worldState);

		if (mPlayOnAwake)
		{
			Play();
		}
	}

	const string& KatSound::GetSoundFile() const
	{
		return mSoundFileName;
	}

	void KatSound::SetSoundFile(const string& file)
	{
		mSoundFileName = file;
	}

	SoundBuffer& KatSound::LoadSoundBuffer(const string& file)
	{
		// since the SoundBuffers will store the whole file in memory, we don't want to load it multiple times
		// Instead, cache the buffer the first time it is loaded
		HashMap<string, shared_ptr<SoundBuffer>>::Iterator it = sBufferMap.Find(file);
		if (it != sBufferMap.end())
		{
			return *it->second;
		}
		else
		{
			shared_ptr<SoundBuffer> buffer = make_shared<SoundBuffer>();
			buffer->loadFromFile(file);
			sBufferMap.Insert(pair<string, shared_ptr<SoundBuffer>>(file, buffer));
			return *buffer;
		}
	}

	void KatSound::UnloadSoundBuffer(const string& file)
	{
		sBufferMap.Remove(file);
	}

	void KatSound::UnloadAllSoundBuffers()
	{
		sBufferMap.Clear();
	}

	void KatSound::Play()
	{
		mSound.setBuffer(LoadSoundBuffer(SoundFolder + mSoundFileName));
		mSound.setVolume(mVolume);
		mSound.play();
	}

	void KatSound::Pause()
	{
		mSound.pause();
	}

	void KatSound::Stop()
	{
		mSound.stop();
	}

	SoundSource::Status KatSound::GetStatus() const
	{
		return mSound.getStatus();
	}

	float KatSound::GetVolume() const
	{
		return mVolume;
	}

	void KatSound::SetVolume(float volume)
	{
		mVolume = volume;
	}

	void KatSound::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(SoundFileKey, &mSoundFileName);
		AddExternalAttribute(PlayOnAwakeKey, &mPlayOnAwake);
		AddExternalAttribute(VolumeKey, &mVolume);
	}

	void KatSound::CopyPrivateDataMembers(const KatSound& other)
	{
		mSoundFileName = other.mSoundFileName;
		mPlayOnAwake = other.mPlayOnAwake;
		mVolume = other.mVolume;
	}

	Scope* KatSound::Copy() const
	{
		return new KatSound(*this);
	}
}
