#pragma once
#include "API\ARK\Ark.h"

#ifdef EVMGR_EXPORTS
#define EM_API __declspec(dllexport)
#else
#define EM_API __declspec(dllimport)
#endif

namespace EventManager
{
	struct EventPlayerStats_s
	{
		float health, melee, speed;
		EventPlayerStats_s() : health(-1.f), melee(-1.f), speed(-1.f) {}
	};

	struct EventPlayer
	{
		const long long PlayerID;
		FVector StartPos, TeledPos;
		int Kills, Team;
		AShooterPlayerController* ASPC;
		bool Delete;
		EventPlayerStats_s EventPlayerStats;
		unsigned int DinoID1, DinoID2;
		int DinoSeat;

		EventPlayer(const long long PlayerID, AShooterPlayerController* ASPC) : PlayerID(PlayerID), ASPC(ASPC), StartPos(ArkApi::GetApiUtils().GetPosition(ASPC))
			, Kills(0), Team(0), Delete(false), EventPlayerStats(EventPlayerStats_s()), TeledPos(FVector(0.f, 0.f, 0.f)), DinoID1(-1), DinoID2(-1), DinoSeat(-1) {}
	};

	struct EventTeam
	{
		int Alive, Score;
		EventTeam() : Alive(0), Score(0) {}
	};

	enum EventArmourType
	{
		Head = 0,
		Torso,
		Gloves,
		Offhand,
		Legs,
		Feet,
		Max
	};

	struct EventItem
	{
		FString BP;
		int Quantity;
		float Quality, DMG, Armour, Dura, AmmoClip;
		EventItem(const FString BP, const int Quantity, const float Quality, const float DMG, const float Armour, const float Dura, const float AmmoClip) : BP(BP), Quantity(Quantity), Quality(Quality), DMG(DMG), Armour(Armour), Dura(Dura), AmmoClip(AmmoClip) {}
		EventItem() : BP(), Quantity(0), Quality(0), DMG(0), Armour(0), Dura(0), AmmoClip(0) {}
	};

	struct EventEquipment
	{
		EventItem Armour[(int)EventArmourType::Max];
		const TArray<EventItem> Items;
		EventEquipment(const TArray<EventItem>& Items, const EventItem Armour[(int)EventArmourType::Max]) : Items(Items)
		{
			for(int i = 0; i < (int)EventArmourType::Max; i++) this->Armour[i] = Armour[i];
		}
	};

	struct Schedule
	{
		FString EventName;
		int EventID, StartDay, StartHour;
		Schedule(const FString& EventName, int StartDay, int StartHour) : EventName(EventName), StartDay(StartDay), StartHour(StartHour), EventID(-1){}
	};

	class EM_API IEventManager
	{
	public:
		virtual ~IEventManager() = default;

		virtual void Update() = 0;

		virtual FString& GetServerName() = 0;

		virtual bool IsEventRunning() = 0;
		virtual bool OnlyNakeds() = 0;
		virtual FString GetCurrentEventName() = 0;
		virtual EventState GetEventState() = 0;
		virtual bool IsEventOverrideJoinAndLeave() = 0;
		virtual int GetArkShopEntryFee() = 0;

		virtual void AddEvent(Event* event) = 0;
		virtual void RemoveEvent(Event* event) = 0;
		virtual int GetEventsCount() = 0;
		virtual bool StartEvent(const int EventID = -1) = 0;

		virtual void AddSchedule(const FString& EventName, int StartDay, int StartHour) = 0;
		virtual void RemoveSchedules() = 0;

		virtual bool AddPlayer(AShooterPlayerController* player) = 0;
		virtual bool HasPlayer(const int PlayerID) = 0;
		virtual bool RemovePlayer(AShooterPlayerController* player) = 0;

		virtual TArray<EventPlayer>& GetEventPlayers() = 0;
		virtual int GetEventPlayersCount() = 0;
		virtual int GetTeamAliveCount(const int Team) = 0;
		virtual int GetTeamScore(const int Team) = 0;
		virtual bool CanRewardWinner() = 0;

		virtual	bool IsEventProtectedStructure(const FVector& StructurePos) = 0;
		
		virtual bool TeleportEventPlayers(const bool ApplyFairHp, const bool ApplyFairMovementSpeed, const bool ApplyFairMeleeDamage, const bool DisableInputs, const bool WipeInventoryOrCheckIsNaked, const bool PreventDinos, SpawnsMap& Spawns) = 0;
		virtual void TeleportHome(const EventPlayer& player, const bool WipeInventory, const bool PlayerDied) = 0;
		virtual void TeleportWinningEventPlayersToStart(const bool WipeInventory) = 0;
		virtual void EnableInputs() = 0;

		virtual std::optional<FString> CheckIfPlayersNaked(AShooterPlayerController* Player) = 0;
		virtual void SetItemStatValue(UPrimalItem* item, EPrimalItemStat::Type item_stat_type, const float new_value) = 0;

		virtual int GetRandomIndexNonRecurr(int TotalSize) = 0;
		virtual void GiveEventPlayersEquipment(const EventEquipment& Equipment) = 0;
		virtual void ResetPlayerStats(EventPlayer Player, const bool PlayerDied) = 0;

		template <typename T, typename... Args>
		void SendChatMessageToAllEventPlayers(const FString& sender_name, const T* msg, Args&&... args)
		{
			const FString text(FString::Format(msg, std::forward<Args>(args)...));
			SendChatMessageToAllEventPlayersInternal(sender_name, text);
		}

		template <typename T, typename... Args>
		void SendNotificationToAllEventPlayers(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, const T* msg, Args&&... args)
		{
			SendNotificationToAllEventPlayersInternal(color, display_scale, display_time, icon, FString::Format(msg, std::forward<Args>(args)...));
		}

		virtual bool GetEventQueueNotifications() = 0;

		virtual void InitConfigs(const FString& ServerName, const FString& JoinCommand, int EventStartMinuteMin, int EventStartMinuteMax, bool DebugLogToConsole,
			const FString& PlayerDeadMsg, const FString& InventoryNotFoundMsg, const FString& MustBeNakedMsg, bool StartEventOnServerStart, bool EventStartAuto) = 0;

		virtual bool ArkShopSpendPoints(int amount, int PlayerID) = 0;
		virtual int ArkShopGetPoints(int PlayerID) = 0;
		virtual void ArkShopAddPoints(int amount, int PlayerID) = 0;

		virtual bool CanTakeDamage(long long AttackerID, long long VictimID) = 0;
		virtual bool OnPlayerDied(long long AttackerID, long long VictimID) = 0;
		virtual void OnPlayerLogg(AShooterPlayerController* Player) = 0;

	private:
		virtual void SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg) = 0;
		virtual void SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, FString& msg) = 0;
	};

	EM_API IEventManager& APIENTRY Get();
}