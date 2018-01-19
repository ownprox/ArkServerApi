#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DinoColourCommand/config.json");
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