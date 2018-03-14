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
	EventMan();
	static EventMan* Get();
	void AddEvent(Event* event);
	void RemoveEvent(Event* event);
	void Update();

	void TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap Spawns, const int StartTeam = 0);
	void TeleportWinningEventPlayersToStart();

	EventPlayer* FindPlayer(long long SteamID);
	bool AddPlayer(long long SteamID, AShooterPlayerController* player);
	bool RemovePlayer(long long SteamID, bool ByCommand = true);
	bool IsEventRunning() { return EventRunning; }
	bool StartEvent(const int EventID = -1);

	EventPlayerArray& GetEventPlayers() { return Players; }

	int GetPlayersAlive() { return (int)Players.size(); }

	//Hooked
	bool CanTakeDamage(long long AttackerID, long long VictimID);
	void OnPlayerDied(long long AttackerID, long long VictimID);
	void OnPlayerLogg(AShooterPlayerController* Player);
	bool IsEventProtectedStructure(const FVector& StructurePos);

	//Event Player Messages
	template <typename T, typename... Args>
	void SendChatMessageToAllEventPlayers(const FString& sender_name, const T* msg, Args&&... args)
	{
		const FString text(FString::Format(msg, std::forward<Args>(args)...));
		FChatMessage chat_message = FChatMessage();
		chat_message.SenderName = sender_name;
		chat_message.Message = text;
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientChatMessage(chat_message);
	}

	template <typename T, typename... Args>
	void SendNotificationToAllEventPlayers(FLinearColor color, float display_scale,
		float display_time, UTexture2D* icon, const T* msg, Args&&... args)
	{
		const FString text(FString::Format(msg, std::forward<Args>(args)...));
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer->ClientServerSOTFNotificationCustom(&text, color, display_scale, display_time, icon, nullptr);
	}
};