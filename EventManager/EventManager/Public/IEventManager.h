#pragma once
#include "API\ARK\Ark.h"

#ifdef EVMGR_EXPORTS
#define EM_API __declspec(dllexport)
#else
#define EM_API __declspec(dllimport)
#endif

namespace EventManager
{
	struct EventPlayer
	{
		const long long PlayerID;
		FVector StartPos;
		int Kills, Team;
		AShooterPlayerController* ASPC;

		EventPlayer(const long long PlayerID, AShooterPlayerController* ASPC) : PlayerID(PlayerID), ASPC(ASPC), StartPos(ArkApi::GetApiUtils().GetPosition(ASPC))
			, Kills(0), Team(0) {}
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
		const FString BP;
		const int Quantity;
		const float Quality;
		EventItem(const FString BP, const int Quantity, const float Quality) : BP(BP), Quantity(Quantity), Quality(Quality) {}
	};

	struct EventEquipment
	{
		const EventItem* Armour;
		const TArray<EventItem> Items;
		EventEquipment(const TArray<EventItem>& Items, const EventItem Armour[(int)EventArmourType::Max]) : Items(Items), Armour(Armour) {}
	};

	class EM_API IEventManager
	{
	public:
		virtual ~IEventManager() = default;

		virtual void Update() = 0;

		virtual FString& GetServerName() = 0;

		virtual bool IsEventRunning() = 0;
		virtual FString GetCurrentEventName() = 0;
		virtual EventState GetEventState() = 0;
		virtual bool IsEventOverrideJoinAndLeave() = 0;

		virtual void AddEvent(Event* event) = 0;
		virtual void RemoveEvent(Event* event) = 0;

		virtual bool AddPlayer(AShooterPlayerController* player) = 0;
		virtual bool RemovePlayer(AShooterPlayerController* player) = 0;

		virtual TArray<EventPlayer>& GetEventPlayers() = 0;
		virtual int GetEventPlayersCount() = 0;

		virtual bool StartEvent(const int EventID) = 0;
		virtual	bool IsEventProtectedStructure(const FVector& StructurePos) = 0;
		
		virtual	void TeleportEventPlayers(const bool TeamBased, const bool DefaultRunningSpeed, const bool DisableInputs, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam) = 0;
		virtual void TeleportWinningEventPlayersToStart(const bool WipeInventory) = 0;
		virtual void EnableEventPlayersInputs() = 0;

		virtual void GiveEventPlayersEquipment(const EventEquipment& Equipment) = 0;

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
			const FString text(FString::Format(msg, std::forward<Args>(args)...));
			SendNotificationToAllEventPlayersInternal(color, display_scale, display_time, icon, text);
		}

		virtual bool GetEventQueueNotifications() = 0;

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