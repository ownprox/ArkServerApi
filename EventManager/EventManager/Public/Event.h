#pragma once
#include "API\ARK\Ark.h"

enum EventState
{
	WaitingForPlayers = 0,
	TeleportingPlayers,
	WaitForFight,
	Fighting,
	Rewarding,
	Finnished
};

typedef std::map<int, TArray<FVector>> SpawnsMap;
typedef SpawnsMap::iterator SpawnsMapItr;
typedef SpawnsMap::value_type SpawnsMapType;

class Event
{
private:
	EventState State;
	FString Name;
	SpawnsMap Spawns;
	FVector StructureProtectionPosition;
	int StructureProtectionDistance, Counter;
	bool StructureProtection, ConfigLoaded, KillOnLogg;
	DWORD LastTime;

public:
	void InitDefaults(const FString& Name, const bool KillOnLogg = true, const bool StructureProtection = false
		, const FVector StructureProtectionPosition = FVector(0, 0, 0), const int StructureProtectionDistance = 0)
	{
		this->Name = Name;
		this->KillOnLogg = KillOnLogg;
		this->StructureProtection = StructureProtection;
		this->StructureProtectionPosition = StructureProtectionPosition;
		this->StructureProtectionDistance = StructureProtectionDistance;
		this->ConfigLoaded = false;
		this->LastTime = this->Counter = 0;
	}

	void Init()
	{
		ConfigLoaded = true;
		LastTime = Counter = 0;
		SetState(EventState::WaitingForPlayers);
	}

	FString GetName() { return Name; }

	EventState GetState() { return State; }
	void SetState(EventState state) { State = state; }

	SpawnsMap GetSpawns()
	{
		return Spawns;
	}

	void AddSpawn(FVector Spawn, int Team = 0)
	{
		SpawnsMapItr itr = Spawns.find(Team);
		if (itr != Spawns.end()) itr->second.Add(Spawn);
		else
		{
			TArray<FVector> Spawnz;
			Spawnz.Add(Spawn);
			Spawns.insert(SpawnsMapType(Team, Spawnz));
		}
	}

	int GetCounter() { return Counter; }
	int IncreaseCounter() { Counter++; return Counter; }
	void ResetCounter() { Counter = 0; }

	bool WaitForPassed() { return timeGetTime() > LastTime; }
	void WaitFor(int Seconds) { LastTime = timeGetTime() + (Seconds * 1000); }

	bool HasConfigLoaded() { return ConfigLoaded; }

	bool IsEventProtectedStructure(const FVector& StructurePos)
	{ return StructureProtection && FVector::Distance(StructurePos, StructureProtectionPosition) < StructureProtectionDistance; }

	bool KillOnLoggout() { return KillOnLogg; }

	
	virtual void InitConfig(const FString& JoinEventCommand, const FString& ServerName, const FString& Map) {};
	virtual void Update() {};
	virtual void OnWonEvent() {};
};