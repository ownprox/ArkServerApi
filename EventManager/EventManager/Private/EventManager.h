#pragma once
#include "API/ARK/Ark.h"
#include "../Public/Event.h"
#include "../Public/IEventManager.h"

namespace EventManager
{
	class EventManager : public IEventManager
	{
	private:
		bool LogToConsole, EventQueueNotifications, UseSchedule, EventStartAuto, TeamBased, bCanRewardWinner;
		TArray<EventPlayer> Players;
		TArray<Event*> Events;
		Event* CurrentEvent;
		DWORD NextEventTime;
		FString JoinCommand, ServerName, Map, PlayerDeadMsg, InventoryNotFoundMsg, MustBeNakedMsg;
		int32 MinStartEvent, MaxStartEvent;
		TArray<EventTeam> EventTeamData;
		int LastEquipmentIndex, CurrentEventID, ScheduleCheckCounter;
		time_t rawtime;
		tm timeinfo;
		TArray<Schedule> Schedules;
		~EventManager() = default;
		EventManager() : LogToConsole(true), EventQueueNotifications(true), CurrentEvent(nullptr), UseSchedule(false), NextEventTime(timeGetTime() + 120000),
			MinStartEvent(7200000), MaxStartEvent(21600000), EventStartAuto(true), TeamBased(false), LastEquipmentIndex(-1), bCanRewardWinner(false),
			CurrentEventID(0), ScheduleCheckCounter(0) {};
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
		int GetArkShopEntryFee();

		void AddEvent(Event* event);
		void RemoveEvent(Event* event);
		int GetEventsCount();
		bool StartEvent(const int EventID = -1);
		int GetEventID(const FString& EventName)
		{
			for(int EventID = 0; EventID < Events.Num(); EventID++)
			for (const auto& Event : Events)
				if (Event->GetName() == EventName)
					return EventID;
			return -1;
		}

		void AddSchedule(const FString& EventName, int StartDay, int StartHour)
		{ 
			Schedules.Add(Schedule(EventName, StartDay, StartHour));
		}

		void RemoveSchedules() { Schedules.Empty(); }

		EventPlayer* FindPlayer(long long SteamID);
		bool AddPlayer(AShooterPlayerController* player);
		bool HasPlayer(const int PlayerID);
		bool RemovePlayer(AShooterPlayerController* player);
		TArray<EventPlayer>& GetEventPlayers() { return Players; }
		int GetEventPlayersCount() { return (int)Players.Num(); }
		int GetTeamAliveCount(const int Team) { return EventTeamData[Team].Alive; }
		int GetTeamScore(const int Team) { return EventTeamData[Team].Score; }
		bool CanRewardWinner() { return bCanRewardWinner;  }

		void Update();

		bool TeleportEventPlayers(const bool ApplyFairHp, const bool ApplyFairMovementSpeed, const bool ApplyFairMeleeDamage, const bool DisableInputs, const bool WipeInventoryOrCheckIsNaked, const bool PreventDinos, SpawnsMap& Spawns);
		void TeleportHome(const EventPlayer& player, const bool WipeInventory, const bool PlayerDied);
		void TeleportWinningEventPlayersToStart(const bool WipeInventory);
		void EnableInputs();

		std::optional<FString> CheckIfPlayersNaked(AShooterPlayerController* Player);

		int GetRandomIndexNonRecurr(int TotalSize);
		void UpdateItemColours(const short ItemColour, const TArray<UPrimalItem*>& Items);
		void GiveEventPlayersEquipment(const EventEquipment& Equipment);
		void ResetPlayerStats(EventPlayer Player, const bool PlayerDied);

		void SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg);
		void SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, FString& msg);

		bool GetEventQueueNotifications();

		void InitConfigs(const FString& ServerName, const FString& JoinCommand, int EventStartMinuteMin, int EventStartMinuteMax, bool DebugLogToConsole,
			const FString& PlayerDeadMsg, const FString& InventoryNotFoundMsg, const FString& MustBeNakedMsg, bool StartEventOnServerStart, bool EventStartAuto);
		bool ArkShopSpendPoints(int amount, int PlayerID);
		int ArkShopGetPoints(int PlayerID);
		void ArkShopAddPoints(int amount, int PlayerID);

		//Hooks
		bool CanTakeDamage(long long AttackerID, long long VictimID);
		bool OnPlayerDied(long long AttackerID, long long VictimID);
		void OnPlayerLogg(AShooterPlayerController* Player);
		bool IsEventProtectedStructure(const FVector& StructurePos);

	};
}