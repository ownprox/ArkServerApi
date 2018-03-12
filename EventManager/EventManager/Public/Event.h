#pragma once
#include "API\ARK\Ark.h"

enum EventTeam
{
	All = 0,
	Red = 1,
	Blue = 2
};

enum EventState
{
	WaitingForPlayers = 0,
	TeleportingPlayers,
	WaitForFight,
	Fighting,
	Rewarding,
	Finnished
};

class Event
{
private:
	EventState State;
	FString Name, ServerName;
	TArray<FVector> SpawnsA, SpawnsB;
	FVector EventPosition;
	int EventDistance, Counter, MinimumPlayersNeeded, StartPlayerCount;
	bool StructureProtection, SpawnsSet;
	DWORD LastTime;

public:

	void InitDefaults(const FString Name, const FString ServerName, const int MinimumPlayersNeeded, const bool StructureProtection, const FVector EventPosition, const int EventDistance)
	{
		this->Name = Name;
		this->ServerName = ServerName;
		this->StructureProtection = StructureProtection;
		this->EventPosition = EventPosition;
		this->EventDistance = EventDistance;
		this->MinimumPlayersNeeded = MinimumPlayersNeeded;
		this->Counter = 0;
		this->LastTime = 0;
		this->SpawnsSet = false;
	}

	void Reset()
	{
		SpawnsSet = true;
		ResetCount();
		AddTime(0);
		SetState(EventState::WaitingForPlayers);
	}

	const FString GetName() { return Name; }
	const FString GetServerName() { return ServerName; }

	const EventState GetState() { return State; }

	const TArray<FVector> GetSpawns(EventTeam Team = EventTeam::All) { return Team == EventTeam::Blue ? SpawnsB : SpawnsA; }
	void AddSpawn(FVector Spawn, EventTeam Team = EventTeam::All) { Team == EventTeam::Blue ? SpawnsB.Add(Spawn) : SpawnsA.Add(Spawn); }

	const int GetCount() { return Counter; }
	const int UpCount() { Counter++; return Counter; }
	const int GetStartedPlayerCount() { return StartPlayerCount; }

	const bool GetSpawnsSet() { return SpawnsSet; }
	const bool TimePassed() { return timeGetTime() > LastTime; }
	const bool IsEventProtectedStructure(const FVector& StructurePos) { return StructureProtection && FVector::Distance(StructurePos, EventPosition) < EventDistance; }

	void SetStartPlayerCount(int StartPlayerCount) { this->StartPlayerCount = StartPlayerCount; }
	void AddTime(int Seconds) { LastTime = timeGetTime() + (Seconds * 1000); }
	void SetState(EventState state) { State = state; }
	void ResetCount() { Counter = 0;  }

	virtual void Init(const FString& Map) {};
	virtual void Update() {};
	virtual void OnWonEvent() {};
};