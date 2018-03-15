#pragma once
#include "API/ARK/Ark.h"
#include "../Public/Event.h"
#include "../Public/IEventManager.h"

namespace EventManager
{
	class EventManager : public IEventManager
	{
	private:
		bool EventRunning;
		TArray<EventPlayer> Players;
		TArray<Event*> Events;
		Event* CurrentEvent;
		DWORD NextEventTime;
		FString JoinEventCommand, ServerName, Map;
		~EventManager() = default;
		EventManager() : EventRunning(false), CurrentEvent(nullptr), NextEventTime(timeGetTime() + 120000) {};
	public:
		static EventManager& Get();

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(EventManager&&) = delete;

		bool IsEventRunning() { return EventRunning; }
		FString& GetCurrentEventName();

		void AddEvent(Event* event);
		void RemoveEvent(Event* event);
		bool StartEvent(const int EventID = -1);

		EventPlayer* FindPlayer(long long SteamID);
		bool AddPlayer(long long PlayerID, AShooterPlayerController* player);
		bool RemovePlayer(long long PlayerID, bool ByCommand = true);
		TArray<EventPlayer>& GetEventPlayers() { return Players; }
		int GetEventPlayersCount() { return (int)Players.Num(); }

		void Update();

		void TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam = 0);
		void TeleportWinningEventPlayersToStart();

		void SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg);
		void SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, FString& msg);


		//Hooks
		bool CanTakeDamage(long long AttackerID, long long VictimID);
		void OnPlayerDied(long long AttackerID, long long VictimID);
		void OnPlayerLogg(AShooterPlayerController* Player);
		bool IsEventProtectedStructure(const FVector& StructurePos);
	};
}