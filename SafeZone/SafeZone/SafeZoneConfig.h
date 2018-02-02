#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	Log::GetLog()->info("Loading SafeZones.");
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/SafeZone/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->info("Could not open file config.json");
		throw;
	}
	nlohmann::json SafeZoneConfig;
	file >> SafeZoneConfig;
	file.close();
	AdminIgnoreChecks = SafeZoneConfig["SafeZones"]["AdminIgnoreChecks"];
	SafeZoneNotifications = SafeZoneConfig["SafeZones"]["SafeZoneNotifications"];
	NotificationCheckSeconds = SafeZoneConfig["SafeZones"]["SafeZoneNotificationCheckSeconds"];
	if (NotificationCheckSeconds < 2) NotificationCheckSeconds = 2;
	else NotificationCheckSeconds = NotificationCheckSeconds - 1;
	DestroyAggressiveDinos = SafeZoneConfig["SafeZones"]["SafeZoneKillAgressiveDino"];
	nlohmann::json Messages;
	FString Msgs[6];
	int j = 0;
	std::string Data;
	Data = SafeZoneConfig["SafeZones"]["ServerName"];
	ServerName = FString(ArkApi::Tools::ConvertToWideStr(Data).c_str());
	Data = SafeZoneConfig["SafeZones"]["ClaimItemsCommand"];
	ClaimItemsCommand = ArkApi::Tools::ConvertToWideStr(Data);
	auto SafeZonesDistanceMap = SafeZoneConfig["SafeZones"]["DistanceSafeZones"];
	for (const auto& safeZone : SafeZonesDistanceMap)
	{
		auto Pos = safeZone["Position"];

		Messages = safeZone["Messages"];
		j = 0;
		for (nlohmann::json::iterator it = Messages.begin(); it != Messages.end(); it++)
		{
			Data = *it;
			Msgs[j++] = FString(ArkApi::Tools::ConvertToWideStr(Data).c_str());
			if (j == 6) break;
		}

		ItemArray Items;
		auto SafeZonesItemMap = safeZone["Items"];
		for (const auto& szitem : SafeZonesItemMap)
		{
			Data = szitem["Blueprint"];
			Items.push_back(ItemS(FString(Data.c_str()), szitem["Quantity"], szitem["Quality"], szitem["IsBlueprint"]));
		}

		Data = safeZone["Name"];

		SafeZoneDistanceMap.push_back(SafeZoneDistanceS(FString(ArkApi::Tools::ConvertToWideStr(Data).c_str()), FVector(Pos[0], Pos[1], Pos[2]), safeZone["PreventPVP"], safeZone["PreventStructureDamage"], safeZone["PreventBuilding"], safeZone["Distance"], Msgs, Items));
		
		Log::GetLog()->info("SafeZone: {} ({}, {}, {}) Added", Data.c_str(), (float)Pos[0], (float)Pos[1], (float)Pos[2]);
	}
}