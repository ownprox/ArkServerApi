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
		HasShop = ArkApi::Tools::IsPluginLoaded("AtlasShop");
		std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/AtlasApi/Plugins/VoteRewards/config.json");
#else
		HasShop = ArkApi::Tools::IsPluginLoaded("ArkShop");
		std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/VoteRewards/config.json");
#endif
		file >> VoteConfig;
		file.close();

		TotalVoteSitesInConfig = 0;
		for (const auto& VoteSiteConfig : VoteSites)
		{
			auto VoteCnf = VoteConfig.value(VoteSiteConfig.Site, nlohmann::json::array());
			if (!VoteCnf.empty()) TotalVoteSitesInConfig++;
		}
		TotalVoteSitesInConfig--;
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