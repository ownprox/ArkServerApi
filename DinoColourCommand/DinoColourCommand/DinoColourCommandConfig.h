#pragma once

#include <fstream>
#include "json.hpp"

inline void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DinoColourCommand/config.json");
	if (!file.is_open())
	{
		RequiresAdmin = true;
		return;
	}

	nlohmann::json configData;
	file >> configData;
	file.close();

	RequiresAdmin = configData["DinoColourCommand"]["RequireAdmin"];
	RequiresPermission = configData["DinoColourCommand"].value("RequirePermission", false);
}
