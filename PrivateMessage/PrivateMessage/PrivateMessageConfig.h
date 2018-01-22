#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/PrivateMessage/config.json");
	if (!file.is_open())
	{
		ServerMessage = true;
		ServerMessageRBG[1] = 255;
		return;
	}
	nlohmann::json PrivateMessageConfig;
	file >> PrivateMessageConfig;
	file.close();
	ServerMessage = PrivateMessageConfig["PrivateMessage"]["ServerMessage"];
	nlohmann::json ServRGB = PrivateMessageConfig["PrivateMessage"]["ServerMessageRGB"];
	for (int i = 0; i < 3; i++)
	{
		ServerMessageRBG[i] = ServRGB[i];
		if (ServerMessageRBG[i] > 255 || ServerMessageRBG[i] < 0) ServerMessageRBG[i] = 255;
	}
}