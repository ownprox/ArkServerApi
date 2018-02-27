#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/KillFeed/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->error("Failed to load config.json");
		DisplayType = 0;
		DisplayDelay = 5;
		DisplayDistance = DinoKillsEnabled = PlayerKillsEnabled = true;
		return;
	}
	nlohmann::json TeleportConfig, TempConfig;
	file >> TeleportConfig;
	file.close();
	ServerName = FString(ArkApi::Tools::Utf8Decode(TeleportConfig["KillFeed"]["ServerName"]).c_str());
	PlayerKillsEnabled = TeleportConfig["KillFeed"]["PlayerKillsEnabled"];
	DinoKillsEnabled = TeleportConfig["KillFeed"]["TamedDinoKillsEnabled"];
	DisplaySameTribeKills = TeleportConfig["KillFeed"]["DisplaySameTribeKills"];
	DisplayType = TeleportConfig["KillFeed"]["DisplayType"];
	DisplayScale = TeleportConfig["KillFeed"]["DisplayScale"];
	DisplayDelay = TeleportConfig["KillFeed"]["DisplayDelay"];
	TempConfig = TeleportConfig["KillFeed"]["DisplayColour"];
	DisplayColour = FLinearColor(TempConfig[0], TempConfig[1], TempConfig[2], TempConfig[3]);
	DisplayDistance = TeleportConfig["KillFeed"]["DisplayDistance"];
	DisplayInChatbox = TeleportConfig["KillFeed"]["DisplayInChatbox"];

	TempConfig = TeleportConfig["KillFeed"]["Messages"];
	int j = 0;
	std::string Data;
	for (nlohmann::json::iterator it = TempConfig.begin(); it != TempConfig.end(); it++)
	{		
		Data = *it;
		Messages[j++] = ArkApi::Tools::Utf8Decode(Data).c_str();
	}
	return;
}