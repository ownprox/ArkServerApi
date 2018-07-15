#pragma once
#include <fstream>
#include "json.hpp"

inline void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/PingLimiter/config.json");
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
	PingLimit = Config["PingLimit"];
	TotalPingStrikes = Config["TotalPingStrikes"];
	CheckPingSecondsInteval = Config["CheckPingSecondsInteval"];
	DisplayWarningsToUsers = Config["DisplayWarningsToUsers"];
	int i = 0;
	auto MessagesConfig = Config["Messages"];
	for (nlohmann::json::iterator it = MessagesConfig.begin(); it != MessagesConfig.end(); ++it)
	{
		Messages[i++] = ArkApi::Tools::Utf8Decode(*it);
	}
}
