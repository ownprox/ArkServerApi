#pragma once
#include <fstream>
#include "json.hpp"

nlohmann::json VoteConfig;

FString GetConfig(const std::string& Element)
{
	const auto& Vote = VoteConfig.value("VoteRewards", nlohmann::json::array());
	if (!Vote.empty())
		return FString(ArkApi::Tools::Utf8Decode(Vote.value(Element, "")).c_str());
	return L"";
}

FString GetMsg(const std::string& VoteSite, const int ID)
{
	const auto& Vote = VoteConfig.value(VoteSite, nlohmann::json::array());
	if (!Vote.empty())
	{
		const auto& Msgs = Vote.value("Messages", nlohmann::json::array());
		if (Msgs.size() > ID)
			return FString(ArkApi::Tools::Utf8Decode(Msgs[ID]).c_str());
	}
	return L"";
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