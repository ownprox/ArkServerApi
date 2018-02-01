#pragma once
enum ServerType
{
	Island = 0,
	Ragnarok,
	Center,
	Scorched
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
	FVector StartPos;
	int TravelDistance, Counter, MinimumPlayersNeeded, BetAmount, StartPlayerCount;
	FString Name;
	DWORD LastTime;
	FVector* Spawns;
	bool SpawnsSet;
public:
	EventState GetState() { return State; }
	bool Finnished() { return State == EventState::Finnished; }
	bool TimePassed() { return timeGetTime() > LastTime; }
	int GetCount() { return Counter; }
	int GetBetAmount() { return BetAmount; }
	FString GetName() { return Name; }
	bool GetSpawnsSet() { return SpawnsSet; }
	FVector* GetSpawns() { return Spawns; }
	int GetStartedPlayerCount() { return StartPlayerCount; }
	void SetStartPlayerCount(int StartPlayerCount) { this->StartPlayerCount = StartPlayerCount; }
	void SetSpawns(FVector* spawns) { Spawns = spawns; SpawnsSet = true; }
	void AddTime(int Seconds) { LastTime = timeGetTime() + (Seconds * 1000); }
	int UpCount() { Counter++; return Counter; }
	void SetState(EventState state) { State = state; }
	void ResetCount() { Counter = 0;  }
	void InitDefaults(FString Name, FVector StartPos, int TravelDistance, int MinimumPlayersNeeded, int BetAmount = 0) { this->Name = Name; this->StartPos = StartPos; this->TravelDistance = TravelDistance; this->MinimumPlayersNeeded = MinimumPlayersNeeded; this->Counter = 0; this->LastTime = 0; this->SpawnsSet = false; this->BetAmount = BetAmount; }
	void Reset() { ResetCount(); AddTime(0); SetState(EventState::WaitingForPlayers); }
	virtual void Init(ServerType ServType) {};
	virtual void Update() {};
	virtual void OnWonEvent();
};