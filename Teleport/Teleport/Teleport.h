#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
int TeleportDelay;
float MaxDistance;
bool AllowDinoTeleport, TPREnabled;

struct PlayerS
{
	__int64 SteamID, LastTPRID;
	PlayerS(__int64 SteamID)
	{
		this->SteamID = SteamID;
		this->LastTPRID = -1;
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

class Timer
{
public:
	template <class callable, class... arguments>
	Timer(int after, bool async, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments ...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		if (async)
		{
			std::thread([after, task]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(after));
				task();
			}).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}
	}
};