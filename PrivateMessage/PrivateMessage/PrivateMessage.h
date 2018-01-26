#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
bool ServerMessage;
int ServerMessageRBG[3] = { 0, 0, 0 };

struct PlayerS
{
	__int64 SteamID, LastPmSteamID;
	PlayerS(__int64 SteamID)
	{
		this->SteamID = SteamID;
		this->LastPmSteamID = -1;
	}
};

typedef std::vector<PlayerS> PlayerArray;
typedef PlayerArray::iterator PlayerArrayItr;
PlayerArray Players;

PlayerS* GetPlayer(__int64 SteamID)
{
	PlayerArrayItr player = std::find_if(Players.begin(), Players.end(), [SteamID](const PlayerS & ps) -> bool { return ps.SteamID == SteamID; });
	if (player != Players.end()) return &(*player);
	else
	{
		Players.push_back(PlayerS(SteamID));
		return &(*(Players.end()-1));
	}
}

void RemovePlayer(__int64 SteamID)
{
	PlayerArrayItr player = std::find_if(Players.begin(), Players.end(), [SteamID](const PlayerS & ps) -> bool { return ps.SteamID == SteamID; });
	if (player != Players.end()) Players.erase(player);
}