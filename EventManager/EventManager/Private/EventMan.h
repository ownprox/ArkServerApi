#pragma once
#include "API\ARK\Ark.h"
#include "..\Public\Event.h"

struct EventPlayer
{
	long long PlayerID;
	int Kills, Team;
	AShooterPlayerController* ASPC;
	bool Alive;
	FVector StartPos;

	EventPlayer(const long long PlayerID, AShooterPlayerController* ASPC)
	{
		this->PlayerID = PlayerID;
		this->ASPC = ASPC;
		this->Team = 0;
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
	DWORD NextEventTime;
	FString JoinEventCommand, ServerName, Map;

public:
	EventMan() : EventRunning(false), CurrentEvent(nullptr), NextEventTime(timeGetTime() + 120000) {};	
	static EventMan& Get();
	void Update();


	//API Functions
	bool IsEventRunning() { return EventRunning; }
	FString& GetCurrentEventName();

	void AddEvent(Event* event);
	void RemoveEvent(Event* event);
	bool StartEvent(const int EventID = -1);

	EventPlayer* FindPlayer(long long SteamID);
	bool AddPlayer(long long PlayerID, AShooterPlayerController* player);
	bool RemovePlayer(long long PlayerID, bool ByCommand = true);
	EventPlayerArray& GetEventPlayers() { return Players; }
	int GetEventPlayersCount() { return (int)Players.size(); }

	void TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam = 0);
	void TeleportWinningEventPlayersToStart();

	void SendChatMessageToAllEventPlayers(const FString& sender_name, const FString& msg)
	{
		FChatMessage chat_message = FChatMessage();
		chat_message.SenderName = sender_name;
		chat_message.Message = msg;
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientChatMessage(chat_message);
	}

	void SendNotificationToAllEventPlayers(FLinearColor color, float display_scale,
		float display_time, UTexture2D* icon, FString& msg)
	{
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientServerSOTFNotificationCustom(&msg, color, display_scale, display_time, icon, nullptr);
	}


	//Hooks
	bool CanTakeDamage(long long AttackerID, long long VictimID);
	void OnPlayerDied(long long AttackerID, long long VictimID);
	void OnPlayerLogg(AShooterPlayerController* Player);
	bool IsEventProtectedStructure(const FVector& StructurePos);
};