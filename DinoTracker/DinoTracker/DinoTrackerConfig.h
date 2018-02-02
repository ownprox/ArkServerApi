#pragma once

#include "DinoTracker.h"

#include <fstream>
#include "json.hpp"

inline void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DinoTracker/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->error("Failed to load config.json");
		throw;
	}

	nlohmann::json Config;

	try
	{
		file >> Config;
	}
	catch (const std::exception& error)
	{
		Log::GetLog()->error(error.what());
		throw;
	}

	file.close();

	PlayerCanTrack = Config["DinoTracker"]["PlayersCanTrack"];
	MaxDistance = Config["DinoTracker"]["MaxDistance"];

	int i = 0;

	auto MessagesConfig = Config["Messages"];
	for (nlohmann::json::iterator it = MessagesConfig.begin(); it != MessagesConfig.end(); ++it)
	{
		Messages[i++] = ArkApi::Tools::Utf8Decode(*it);
	}
}
