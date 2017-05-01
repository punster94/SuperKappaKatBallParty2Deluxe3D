#include "pch.h"
#include "PowerupSpawner.h"
#include <random>
#include "Powerup.h"


namespace KatBall
{
	PowerupSpawner::PowerupSpawner() :
		mSpawnChance(0.0f), mElapsedTime(0.0f), mLongBoiLengthIncrease(0.0f), mBigBoiScaleIncrease(0.0f), mVortexBoiRotationSpeed(0.0f),
		mLongBoi(nullptr), mBigBoi(nullptr), mVortexBoi(nullptr)
	{
		PowerupSpawner::InitializeSignatures();
		unsigned randomSeed = std::chrono::system_clock::now().time_since_epoch().count();
		mGenerator.seed(randomSeed);
		//(*this)[sPowerupKey].SetType(FieaGameEngine::Datum::DatumType::Table);
	}

	PowerupSpawner::PowerupSpawner(const PowerupSpawner& rhs) :
		Entity(rhs)
	{
		CopyPrivateDataMembers(rhs);
	}

	void PowerupSpawner::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);

		mLongBoi = FindChildEntityByName(sLongBoiKey)->As<Powerup>();
		mBigBoi = FindChildEntityByName(sBigBoiKey)->As<Powerup>();
		mVortexBoi = FindChildEntityByName(sVortexBoiKey)->As<Powerup>();

		InitializePowerups();

		Adopt(*(FindChildEntityByName(sLongBoiKey)->As<Powerup>()), sPowerupKey);
		Adopt(*(FindChildEntityByName(sBigBoiKey)->As<Powerup>()), sPowerupKey);
		Adopt(*(FindChildEntityByName(sVortexBoiKey)->As<Powerup>()), sPowerupKey);

		//Entity::Initialize(worldState);

		//mLongBoiMesh = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();
	}

	void PowerupSpawner::InitializePowerups()
	{
		mLongBoi->SetType(Powerup::PowerupType::LongBoi);
		mBigBoi->SetType(Powerup::PowerupType::BigBoi);
		mVortexBoi->SetType(Powerup::PowerupType::VortexBoi);

		mLongBoi->SetLengthIncrease(mLongBoiLengthIncrease);
		mBigBoi->SetScaleIncrease(mBigBoiScaleIncrease);
		mVortexBoi->SetRotationSpeed(mVortexBoiRotationSpeed);

		mLongBoi->SetRelativePosition(mPosition);
		mBigBoi->SetRelativePosition(mPosition);
		mVortexBoi->SetRelativePosition(mPosition);
	}

	void PowerupSpawner::Update(FieaGameEngine::WorldState& worldState)
	{
		mElapsedTime += worldState.DeltaTime();
		if (mElapsedTime >= mSpawnAttemptInterval)
		{	// Every 5 seconds, attempt to spawn this thing
			mElapsedTime = 0.0f;
			AttemptSpawn();
		}

		auto& datum = (*this)[FieaGameEngine::Sector::sSectorEntitiesKey];
		for (std::uint32_t i = 0; i < datum.Size(); i++)
		{
			datum.Get<Scope&>(i).As<Entity>()->Update(worldState);
		}
	}

	void PowerupSpawner::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute("Spawn Chance", &mSpawnChance, 1);

		AddExternalAttribute(sLongBoiSpawnWeight, &mLongBoiSpawnWeight, 1);
		AddExternalAttribute(sBigBoiSpawnWeight, &mBigBoiSpawnWeight, 1);
		AddExternalAttribute(sVortexBoiSpawnWeight, &mVortexBoiSpawnWeight, 1);

		AddExternalAttribute(sLongBoiStat, &mLongBoiLengthIncrease, 1);
		AddExternalAttribute(sBigBoiStat, &mBigBoiScaleIncrease, 1);
		AddExternalAttribute(sVortexBoiStat, &mVortexBoiRotationSpeed, 1);
	}

	float PowerupSpawner::GetSpawnChance() const
	{
		return mSpawnChance;
	}

	void PowerupSpawner::SetSpawnChance(const float& spawnChance)
	{
		mSpawnChance = spawnChance;
	}

	float PowerupSpawner::GetLongBoiLengthIncrease() const
	{
		return mLongBoiLengthIncrease;
	}

	void PowerupSpawner::SetLongBoiLengthIncrease(const float& lengthIncrease)
	{
		mLongBoiLengthIncrease = lengthIncrease;
	}

	float PowerupSpawner::GetBigBoiScaleIncrease() const
	{
		return mBigBoiScaleIncrease;
	}

	void PowerupSpawner::SetBigBoiScaleIncrase(const float& scaleIncrease)
	{
		mBigBoiScaleIncrease = scaleIncrease;
	}

	float PowerupSpawner::getVortexBoiRotationSpeed() const
	{
		return mVortexBoiRotationSpeed;
	}

	void PowerupSpawner::SetVortexBoiRotationSpeed(const float& rotationSpeed)
	{
		mVortexBoiRotationSpeed = rotationSpeed;
	}

	std::int32_t PowerupSpawner::GetLongBoiSpawnWeight() const
	{
		return mLongBoiSpawnWeight;
	}

	void PowerupSpawner::SetLongBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mLongBoiSpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetBigBoiSpawnWeight() const
	{
		return mBigBoiSpawnWeight;
	}

	void PowerupSpawner::SetBigBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mBigBoiSpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetVortexBoiSpawnWeight() const
	{
		return mVortexBoiSpawnWeight;
	}

	void PowerupSpawner::SetVortexBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mVortexBoiSpawnWeight = spawnWeight;
	}

	void PowerupSpawner::AttemptSpawn()
	{
		std::uniform_int_distribution<std::uint32_t> distribution(0, 100);
		std::uint32_t roll = distribution(mGenerator);

		if (roll <= mSpawnChance)
		{
			//mScale.x = (mScale.x == 2) ? 0.5 : 1;
			//mScale.y = (mScale.y == 2) ? 0.5 : 1;
			//mScale.z = (mScale.z == 2) ? 0.5 : 1;
			std::uint32_t totalSpawnWeight = mLongBoiSpawnWeight + mBigBoiSpawnWeight + mVortexBoiSpawnWeight;
			std::uniform_int_distribution<std::uint32_t> weightedDistribution(0, totalSpawnWeight);
			std::uint32_t weightedRoll = distribution(mGenerator);

			if (weightedRoll <= mLongBoiSpawnWeight)
			{
				//(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(*mLongBoi));
				Adopt(*new Powerup(*mLongBoi), FieaGameEngine::Sector::sSectorEntitiesKey);
			}
			else if (weightedRoll <= mLongBoiSpawnWeight + mBigBoiSpawnWeight)
			{
				//(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(*mBigBoi));
				Adopt(*new Powerup(*mBigBoi), FieaGameEngine::Sector::sSectorEntitiesKey);
			}
			else if (weightedRoll <= mLongBoiSpawnWeight + mBigBoiSpawnWeight + mVortexBoiSpawnWeight)
			{
				//(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(*mVortexBoi));
				Adopt(*new Powerup(*mVortexBoi), FieaGameEngine::Sector::sSectorEntitiesKey);
			}
		}
	}

	void PowerupSpawner::CopyPrivateDataMembers(const PowerupSpawner& rhs)
	{
		mLongBoiMesh = rhs.mLongBoiMesh;
		mLongBoi = rhs.mLongBoi;
		mBigBoi = rhs.mBigBoi;
		mVortexBoi = rhs.mVortexBoi;

		mSpawnChance = rhs.mSpawnChance;	
		mElapsedTime = rhs.mElapsedTime;

		mLongBoiSpawnWeight = rhs.mLongBoiSpawnWeight;	
		mBigBoiSpawnWeight = rhs.mBigBoiSpawnWeight;	
		mVortexBoiSpawnWeight = mVortexBoiSpawnWeight;	

		mLongBoiLengthIncrease = rhs.mLongBoiLengthIncrease;	
		mBigBoiScaleIncrease = rhs.mBigBoiScaleIncrease;		
		mVortexBoiRotationSpeed = rhs.mVortexBoiRotationSpeed;	

		FixExternalAttributes();
	}

	void PowerupSpawner::FixExternalAttributes()
	{
		(*this)[sSpawnChance].SetStorage(&mSpawnChance, 1);

		(*this)[sLongBoiSpawnWeight].SetStorage(&mLongBoiSpawnWeight, 1);
		(*this)[sBigBoiSpawnWeight].SetStorage(&mBigBoiSpawnWeight, 1);
		(*this)[sVortexBoiSpawnWeight].SetStorage(&mVortexBoiSpawnWeight, 1);

		(*this)[sLongBoiStat].SetStorage(&mLongBoiLengthIncrease, 1);
		(*this)[sBigBoiStat].SetStorage(&mBigBoiScaleIncrease, 1);
		(*this)[sLongBoiStat].SetStorage(&mLongBoiLengthIncrease, 1);
	}

	const std::string PowerupSpawner::sSpawnChance = "Spawn Chance";
	const std::string PowerupSpawner::sLongBoiSpawnWeight = "Long Boi Spawn Weight";
	const std::string PowerupSpawner::sBigBoiSpawnWeight = "Big Boi Spawn Weight";
	const std::string PowerupSpawner::sVortexBoiSpawnWeight = "Vortex Boi Spawn Weight";
	const std::string PowerupSpawner::sPowerupKey = "powerups";

	const std::string PowerupSpawner::sLongBoiStat = "Long Boi Length Increase";
	const std::string PowerupSpawner::sBigBoiStat = "Big Boi Scale Increase";
	const std::string PowerupSpawner::sVortexBoiStat = "Vortex Boi Rotation Speed";

	const std::string PowerupSpawner::sRigidBodyKey = "rigidbody";
	const std::string PowerupSpawner::sBallColliderKey = "ball collider";
	const std::string PowerupSpawner::sBallMeshKey = "ball mesh";

	const std::string PowerupSpawner::sLongBoiKey = "long boi";
	const std::string PowerupSpawner::sBigBoiKey = "big boi";
	const std::string PowerupSpawner::sVortexBoiKey = "vortex boi";
}
