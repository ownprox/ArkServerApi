#pragma once
#include "API/ARK/Ark.h"
#include "../Public/Event.h"
#include "../Public/IEventManager.h"

namespace EventManager
{
	class EventManager : public IEventManager
	{
	private:
		bool LogToConsole, EventQueueNotifications, DefaultRunningSpeed, UseSchedule;
		TArray<EventPlayer> Players;
		TArray<Event*> Events;
		Event* CurrentEvent;
		DWORD NextEventTime;
		FString JoinEventCommand, ServerName, Map;
		int32 MinStartEvent, MaxStartEvent;
		~EventManager() = default;
		EventManager() : LogToConsole(true), EventQueueNotifications(true), DefaultRunningSpeed(false), CurrentEvent(nullptr), UseSchedule(false), NextEventTime(timeGetTime() + 120000),
			MinStartEvent(7200000), MaxStartEvent(21600000) {};
	public:
		static EventManager& Get();

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(EventManager&&) = delete;

		FString& GetServerName();

		bool IsEventRunning() { return CurrentEvent != nullptr; }
		FString GetCurrentEventName();
		EventState GetEventState();
		bool IsEventOverrideJoinAndLeave();

		void AddEvent(Event* event);
		void RemoveEvent(Event* event);
		bool StartEvent(const int EventID = -1);

		EventPlayer* FindPlayer(long long SteamID);
		bool AddPlayer(AShooterPlayerController* player);
		bool RemovePlayer(AShooterPlayerController* player);
		TArray<EventPlayer>& GetEventPlayers() { return Players; }
		int GetEventPlayersCount() { return (int)Players.Num(); }

		void Update();

		void TeleportEventPlayers(const bool TeamBased, const bool DefaultRunningSpeed, const bool DisableInputs, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam = 0);
		void TeleportWinningEventPlayersToStart(const bool WipeInventory);
		void EnableEventPlayersInputs();

		void SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg);
		void SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, FString& msg);

		bool GetEventQueueNotifications();

		//Hooks
		bool CanTakeDamage(long long AttackerID, long long VictimID);
		bool OnPlayerDied(long long AttackerID, long long VictimID);
		void OnPlayerLogg(AShooterPlayerController* Player);
		bool IsEventProtectedStructure(const FVector& StructurePos);
	};
}