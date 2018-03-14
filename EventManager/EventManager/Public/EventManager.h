#pragma once
#include "API\ARK\Ark.h"
#include "Event.h"
#include "../Private/EventMan.h"

#ifdef EVMGR_EXPORTS
#define EM_API __declspec(dllexport)
#else
#define EM_API __declspec(dllimport)
#endif

namespace EventManager
{
	EM_API class EventMgr
	{
	public:
		virtual ~EventMgr() = default;
		bool IsEventRunning()
		{
			return EventMan::Get().IsEventRunning();
		}

		FString& GetCurrentEventName()
		{
			return EventMan::Get().GetCurrentEventName();
		}

		void AddEvent(Event* event)
		{
			EventMan::Get().AddEvent(event);
		}

		void RemoveEvent(Event* event)
		{
			EventMan::Get().RemoveEvent(event);
		}

		bool AddPlayer(const long long PlayerID, AShooterPlayerController* player)
		{
			return EventMan::Get().AddPlayer(PlayerID, player);
		}

		bool RemovePlayer(const long long PlayerID, const bool ByCommand)
		{
			return EventMan::Get().RemovePlayer(PlayerID, ByCommand);
		}

		EventPlayerArray& GetEventPlayers()
		{
			return EventMan::Get().GetEventPlayers();
		}

		int GetEventPlayersCount()
		{
			return EventMan::Get().GetEventPlayersCount();
		}

		bool StartEvent(const int EventID)
		{
			return EventMan::Get().StartEvent(EventID);
		}

		bool IsEventProtectedStructure(const FVector& StructurePos)
		{
			return EventMan::Get().IsEventProtectedStructure(StructurePos);
		}

		void TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam)
		{
			EventMan::Get().TeleportEventPlayers(TeamBased, WipeInventory, PreventDinos, Spawns, StartTeam);
		}

		void TeleportWinningEventPlayersToStart()
		{
			EventMan::Get().TeleportWinningEventPlayersToStart();
		}

		template <typename T, typename... Args>
		void SendChatMessageToAllEventPlayers(const FString& sender_name, const T* msg, Args&&... args)
		{
			const FString text(FString::Format(msg, std::forward<Args>(args)...));
			EventMan::Get().SendChatMessageToAllEventPlayers(sender_name, text);
		}

		template <typename T, typename... Args>
		void SendNotificationToAllEventPlayers(FLinearColor color, float display_scale,
			float display_time, UTexture2D* icon, const T* msg, Args&&... args)
		{
			const FString text(FString::Format(msg, std::forward<Args>(args)...));
			EventMan::Get().SendNotificationToAllEventPlayers(color, display_scale, display_time, icon, text);
		}
	};


	EM_API EventMgr& APIENTRY Get()
	{
		static EventMgr Instance;
		return Instance;
	}
}