#pragma once
#include <fstream>
#include "json.hpp"

bool InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/Teleport/config.json");
	if (!file.is_open())
	{
		Log::GetLog()->error("Failed to load config.json");
		return false;
	}
	nlohmann::json TeleportConfig, Commands;
	file >> TeleportConfig;
	file.close();
	TPREnabled = TeleportConfig["Teleport"]["TPREnabled"];
	TPRCoolDownEnabled = TeleportConfig["Teleport"]["TPRCoolDownEnabled"];
	TPRCoolDownForPlayerTeleportedTo = TeleportConfig["Teleport"]["TPRCoolDownForPlayerTeleportedTo"];
	TPRCoolDownSeconds = TeleportConfig["Teleport"]["TPRCoolDownSeconds"];
	TeleportDelay = TeleportConfig["Teleport"]["TPRDelaySeconds"];
	MaxDistance = TeleportConfig["Teleport"]["TPRMaxDistance"];
	AllowDinoTeleport = !(bool)TeleportConfig["Teleport"]["TPRAllowDinoTeleport"];
	Commands = TeleportConfig["Teleport"]["Commands"];
	std::string Data = Commands["tpr"];
	TPR = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tpa"];
	TPA = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tp"];
	TP = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tpm"];
	TPM = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tpd"];
	TPD = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tpdp"];
	TPDP = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tpp"];
	TPP = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["tt"];
	TT = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Data = Commands["pos"];
	POS = FString(ArkApi::Tools::Utf8Decode(Data).c_str());
	Commands = TeleportConfig["Teleport"]["Messages"];
	int j = 0;
	for (nlohmann::json::iterator it = Commands.begin(); it != Commands.end(); it++)
	{		
		Data = *it;
		Messages[j++] = ArkApi::Tools::Utf8Decode(Data).c_str();
	}
	return true;
}