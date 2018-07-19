#pragma once
#include "API\ARK\Ark.h"

enum EventState
{
	WaitingForPlayers = 0,
	TeleportingPlayers,
	WaitForFight,
	Fighting,
	Rewarding,
	Finished
};

typedef std::map<int, TArray<FVector>> SpawnsMap; //Change to unordered map
typedef SpawnsMap::value_type SpawnsMapType;

class Event
{
private:
	EventState State;
	FString Name;
	SpawnsMap Spawns;
	FVector StructureProtectionPosition;
	int StructureProtectionDistance, Counter, Timer, ArkShopPointsEntryFee;
	float MovementSpeed;
	bool StructureProtection, ConfigLoaded, KillOnLogg, OverrideJoinAndLeave, FinalWarning, OnlyAllowNakedsOnJoinEventCommand;

public:
	void InitDefaults(const FString& Name, const bool OverrideJoinAndLeave = false, const bool OnlyAllowNakedsOnJoinEventCommand = true, const bool KillOnLogg = true, const bool StructureProtection = false
		, const FVector StructureProtectionPosition = FVector(0, 0, 0), const int StructureProtectionDistance = 0, const float MovementSpeed = 0.f, const int ArkShopPointsEntryFee = 0)
	{
		this->Name = Name;
		this->KillOnLogg = KillOnLogg;
		this->OverrideJoinAndLeave = OverrideJoinAndLeave;
		this->OnlyAllowNakedsOnJoinEventCommand = OnlyAllowNakedsOnJoinEventCommand;
		this->StructureProtection = StructureProtection;
		this->StructureProtectionPosition = StructureProtectionPosition;
		this->StructureProtectionDistance = StructureProtectionDistance;
		this->MovementSpeed = MovementSpeed;
		this->ArkShopPointsEntryFee = ArkShopPointsEntryFee;
		FinalWarning = ConfigLoaded = false;
	}

	void Init(const int TimerSecs)
	{
		ConfigLoaded = true;
		FinalWarning = false;
		Timer = TimerSecs;
		Counter = 0;
		SetState(EventState::WaitingForPlayers);
	}

	FString& GetName() { return Name; }

	EventState& GetState() { return State; }
	void SetState(EventState state) { State = state; }

	bool IsEventOverrideJoinAndLeave() {
		return OverrideJoinAndLeave;
	}

	bool OnlyNakeds() {
		return OnlyAllowNakedsOnJoinEventCommand;
	}

	SpawnsMap& GetSpawns()
	{
		return Spawns;
	}

	void AddSpawn(FVector Spawn, int Team = 0)
	{
		auto itr = Spawns.find(Team);
		if (itr != Spawns.end()) itr->second.Add(Spawn);
		else
		{
			TArray<FVector> Spawnz;
			Spawnz.Add(Spawn);
			Spawns.insert(SpawnsMapType(Team, Spawnz));
		}
	}

	void ClearSpawns()
	{
		Spawns.clear();
	}

	void ResetCounter() { Counter = 0; }
	bool WaitForCounter(int Count) { return Counter++ == Count; }
	int GetCounter() { return Counter;  }
	bool GetFinalWarning() { return FinalWarning; }
	void SetFinalWarning(bool FinalWarning) { this->FinalWarning = FinalWarning; }


	void ResetTimer() { Timer = 0; }
	bool WaitForTimer(int Seconds) { Timer++; return Timer > Seconds; }

	bool HasConfigLoaded() { return ConfigLoaded; }
	void ResetConfigLoaded() { ConfigLoaded = false; }

	float GetMovementSpeed() { return MovementSpeed; }

	int GetArkShopEntryFee() { return ArkShopPointsEntryFee; }

	bool IsEventProtectedStructure(const FVector& StructurePos)
	{ return StructureProtection && FVector::Distance(StructurePos, StructureProtectionPosition) < StructureProtectionDistance; }

	bool KillOnLoggout() { return KillOnLogg; }

	virtual void InitConfig(const FString& JoinEventCommand, const FString& ServerName, const FString& Map) {};
	virtual void Update() {};
};