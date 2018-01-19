#pragma once
#include <fstream>
#include "json.hpp"

std::string GetCurrentDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

void InitConfig()
{
	std::ifstream file(GetCurrentDir() + "/ArkApi/Plugins/DinoColourCommand/config.json");
	if (!file.is_open())
	{
		RequiresAdmin = true;
		return;
	}
	nlohmann::json MuteConfigData;
	file >> MuteConfigData;
	file.close();
	std::string IPAddress;
	RequiresAdmin = MuteConfigData["DinoColourCommand"]["RequireAdmin"];
}