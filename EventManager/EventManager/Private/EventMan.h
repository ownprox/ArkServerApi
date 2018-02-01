#pragma once
#include "API\ARK\Ark.h"
#include "Event.h"

enum EventTeam
{
	All = 0,
	Red = 1,
	Blue = 2
};

struct EventPlayer
{
	long long PlayerID;
	EventTeam Team;
	int Kills;
	AShooterPlayerController* ASPC;
	bool Alive;
	FVector StartPos;

	EventPlayer(const long long PlayerID, AShooterPlayerController* ASPC)
	{
		this->PlayerID = PlayerID;
		this->ASPC = ASPC;
		this->Team = EventTeam::All;
		this->Kills = 0;
		this->Alive = true;
		this->StartPos = ArkApi::GetApiUtils().GetPosition(ASPC);
	}
};

typedef std::vector<EventPlayer> EventPlayerArray;
typedef EventPlayerArray::iterator EventPlayerArrayItr;
typedef std::vector<Event*> EventList;
typedef EventList::iterator EventItr;

class EventMan
{
private:
	bool EventRunning;
	EventPlayerArray Players;
	EventList Events;
	Event* CurrentEvent;
	HANDLE ThreadHandle;
	DWORD NextEventTime;
	ServerType ServType;
	static EventMan* Instance;

public:
	EventMan();

	static EventMan* GetInstance();

	void AddEvent(Event* event);
	void RemoveEvent(Event* event);
	void Update();
	void TeleportEventPlayers(const FVector* Positions, const bool TeamBased = false, const bool IsBet = false);

	EventPlayer* FindPlayer(long long SteamID);
	bool AddPlayer(long long SteamID, AShooterPlayerController* player);
	bool RemovePlayer(long long SteamID, bool ByCommand = true);
	bool IsEventRunning() { return EventRunning; }
	bool StartEvent(const int EventID = -1);

	int GetEventPlayerCount() { return (int)Players.size(); }

	//Hooked
	bool CanTakeDamage(long long AttackerID, long long VictimID);
	void OnPlayerDied(long long AttackerID, long long VictimID);
};