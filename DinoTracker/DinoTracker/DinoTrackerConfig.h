#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DinoTracker/config.json");
	if (!file.is_open())
	{
		PlayerCanTrack = true;
		return;
	}
	nlohmann::json Config;
	file >> Config;
	file.close();
	PlayerCanTrack = Config["DinoTracker"]["PlayersCanTrack"];
}