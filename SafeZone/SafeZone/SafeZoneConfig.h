#pragma once
#include <fstream>

void InitConfig()
{
	Log::GetLog()->info("Loading SafeZones.");
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/SafeZone/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->info("Could not open file config.json");
		throw;
	}
	file >> SafeZoneConfig;
	file.close();
	std::string Name;
	SafeZoneNotifications = SafeZoneConfig["SafeZones"]["SafeZoneNotifications"];
	NotificationCheckSeconds = SafeZoneConfig["SafeZones"]["SafeZoneNotificationCheckSeconds"];
	if (NotificationCheckSeconds < 2) NotificationCheckSeconds = 2;
	else NotificationCheckSeconds = NotificationCheckSeconds - 1;
	SafeZoneEnterNotifcation = SafeZoneConfig["SafeZones"]["SafeZoneEnterNotifcation"];
	SafeZoneLeaveNotification = SafeZoneConfig["SafeZones"]["SafeZoneLeaveNotification"];
	DestroyAggressiveDinos = SafeZoneConfig["SafeZones"]["SafeZoneKillAgressiveDino"];
	auto SafeZonesDistanceMap = SafeZoneConfig["SafeZones"]["DistanceSafeZones"];
	for (const auto& safeZone : SafeZonesDistanceMap) 
	{
		auto Pos = safeZone["Position"];
		SafeZoneDistanceMap.push_back(SafeZoneDistanceS(FVector(Pos[0], Pos[1], Pos[2]), safeZone["PreventPVP"], safeZone["PreventBuilding"], safeZone["NoPVPMessage"], safeZone["NoBuildMessage"], safeZone["Distance"]));
		Name = safeZone["Name"];
		Log::GetLog()->info("SafeZone: {} ({}, {}, {}) Added", Name.c_str(), (float)Pos[0], (float)Pos[1], (float)Pos[2]);
	}
}