#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/PrivateMessage/config.json");
	if (!file.is_open())
	{
		ServerMessage = false;
		ServerMessageRGB[1] = 255;
		return;
	}
	nlohmann::json PrivateMessageConfig;
	file >> PrivateMessageConfig;
	file.close();
	ServerMessage = PrivateMessageConfig["PrivateMessage"]["ServerMessage"];

	std::string Data = PrivateMessageConfig["PrivateMessage"].value("PmPrefixStart", "[PM");		
	PmPrefixStart = ArkApi::Tools::Utf8Decode(Data).c_str();

	Data = PrivateMessageConfig["PrivateMessage"].value("PmPrefixMiddle", "->");
	PmPrefixMiddle = ArkApi::Tools::Utf8Decode(Data).c_str();

	Data = PrivateMessageConfig["PrivateMessage"].value("PmPrefixEnd", "] ");
	PmPrefixEnd = ArkApi::Tools::Utf8Decode(Data).c_str();

	nlohmann::json ServRGB = PrivateMessageConfig["PrivateMessage"]["ServerMessageRGB"];
	for (int i = 0; i < 3; i++)
		ServerMessageRGB[i] = ServRGB[i];
}