#pragma once
#include "API/ARK/Ark.h"
#include "../Public/Event.h"
#include "../Public/IEventManager.h"

namespace EventManager
{
	class EventManager : public IEventManager
	{
	private:
		bool LogToConsole, EventQueueNotifications, UseSchedule;
		TArray<EventPlayer> Players;
		TArray<Event*> Events;
		Event* CurrentEvent;
		DWORD NextEventTime;
		FString JoinCommand, ServerName, Map, PlayerDeadMsg, InventoryNotFoundMsg, MustBeNakedMsg;
		int32 MinStartEvent, MaxStartEvent;
		~EventManager() = default;
		EventManager() : LogToConsole(true), EventQueueNotifications(true), CurrentEvent(nullptr), UseSchedule(false), NextEventTime(timeGetTime() + 120000),
			MinStartEvent(7200000), MaxStartEvent(21600000) {};
	public:
		static EventManager& Get();

		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;
		EventManager& operator=(const EventManager&) = delete;
		EventManager& operator=(EventManager&&) = delete;

		FString& GetServerName();

		bool IsEventRunning() { return CurrentEvent != nullptr; }
		bool OnlyNakeds() { return CurrentEvent->OnlyNakeds(); }
		FString GetCurrentEventName();
		EventState GetEventState();
		bool IsEventOverrideJoinAndLeave();

		void AddEvent(Event* event);
		void RemoveEvent(Event* event);
		int GetEventsCount();
		bool StartEvent(const int EventID = -1);

		EventPlayer* FindPlayer(long long SteamID);
		bool AddPlayer(AShooterPlayerController* player);
		bool RemovePlayer(AShooterPlayerController* player);
		TArray<EventPlayer>& GetEventPlayers() { return Players; }
		int GetEventPlayersCount() { return (int)Players.Num(); }

		void Update();

		void TeleportEventPlayers(const bool ApplyFairHp, const bool ApplyFairMovementSpeed, const bool ApplyFairMeleeDamage, const bool DisableInputs, const bool WipeInventoryOrCheckIsNaked, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam = 0);
		void TeleportWinningEventPlayersToStart(const bool WipeInventory);
		void EnableEventPlayersInputs();

		std::optional<FString> CheckIfPlayersNaked(AShooterPlayerController* Player);

		void GiveEventPlayersEquipment(const EventEquipment& Equipment);

		void SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg);
		void SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, FString& msg);

		bool GetEventQueueNotifications();

		void InitConfigs(const FString& ServerName, const FString& JoinCommand, int EventStartMinuteMin, int EventStartMinuteMax, bool DebugLogToConsole
			, const FString& PlayerDeadMsg, const FString& InventoryNotFoundMsg, const FString& MustBeNakedMsg);

		//Hooks
		bool CanTakeDamage(long long AttackerID, long long VictimID);
		bool OnPlayerDied(long long AttackerID, long long VictimID);
		void OnPlayerLogg(AShooterPlayerController* Player);
		bool IsEventProtectedStructure(const FVector& StructurePos);
	};
}