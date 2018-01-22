#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/Teleport/config.json");
	if (!file.is_open())
	{
		TeleportDelay = 60;
		MaxDistance = -1.f;
		AllowDinoTeleport = false;
		TPREnabled = true;
		return;
	}
	nlohmann::json TeleportConfig;
	file >> TeleportConfig;
	file.close();
	TPREnabled = TeleportConfig["Teleport"]["TPREnabled"];
	TeleportDelay = TeleportConfig["Teleport"]["TPRDelaySeconds"];
	MaxDistance = TeleportConfig["Teleport"]["TPRMaxDistance"];
	AllowDinoTeleport = TeleportConfig["Teleport"]["TPRAllowDinoTeleport"];
}