#pragma once
#include <fstream>
#include "json.hpp"

nlohmann::json VoteConfig;

FString GetConfig(const std::string& Element)
{
	return FString(ArkApi::Tools::Utf8Decode(VoteConfig["VoteRewards"][Element]).c_str());
}

FString GetMsg(const std::string& VoteSite, const int ID)
{
	return FString(ArkApi::Tools::Utf8Decode(VoteConfig[VoteSite]["Messages"][ID]).c_str());
}

void LoadConfig()
{
	try
	{
	#ifdef ATLAS
		std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/VoteRewards/config.json");
	#else
		std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/VoteRewards/config.json");
	#endif
		file >> VoteConfig;
		file.close();
	}
	catch (nlohmann::json::exception ex)
	{
		Log::GetLog()->error("Json Error: {}", ex.what());
	}
	catch (std::exception ex)
	{
		Log::GetLog()->error("Config Read Error: {}", ex.what());
	}
}