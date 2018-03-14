#pragma once
#include "API\ARK\Ark.h"
#include "Event.h"
#include "../Private/EventMan.h"

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

namespace EventManager
{
	ARK_API EventMan* GetEventManager();
	ARK_API void AddEvent(Event* event);
	ARK_API void RemoveEvent(Event* event);
	ARK_API bool StartEvent(const int EventID = -1);
	ARK_API void TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap Spawns, const int StartTeam = 0);
	ARK_API void TeleportWinningEventPlayersToStart();
	ARK_API void ArkShopAddPoints(int amount, uint64 steam_id);
}