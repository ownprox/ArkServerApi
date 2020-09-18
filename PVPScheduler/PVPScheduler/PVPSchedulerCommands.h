#pragma once

#include <fstream>

std::string Days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
nlohmann::json PVPConfig;

inline void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/PVPScheduler/config.json");
	if (!file.is_open())
		return;

	file >> PVPConfig;
	file.close();

	std::string TempData;
	TempData = PVPConfig["PVPScheduler"]["ServerName"];
	ServerName = FString(ArkApi::Tools::Utf8Decode(TempData).c_str());
	ServerNotify = PVPConfig["PVPScheduler"]["NotifyServer"];
	BlockC4PlacementInPVPOff = PVPConfig["PVPScheduler"]["BlockC4PlacementInPVPOff"];
	LogPvpSwitchAtConsole = PVPConfig["PVPScheduler"]["LogPvpSwitchAtConsole"];
	PVPMessage[2] = PVPConfig["PVPScheduler"].value("ProtectExplosiveMessage", "");

	Log::GetLog()->info("Loading PVP Schedule's");

	PVPDays = PVPConfig["PVPScheduler"]["Days"];
	for (int i = 0; i < PVPDays.size(); i++)
	{
		if (PVPDays[i]["StartDay"] > 6 || PVPDays[i]["StartDay"] < 0)
			PVPDays[i]["StartDay"] = 0;
		if (PVPDays[i]["EndDay"] > 6 || PVPDays[i]["EndDay"] < 0)
			PVPDays[i]["EndDay"] = 0;
		if (PVPDays[i]["StartHour"] > 23 || PVPDays[i]["StartHour"] < 0)
			PVPDays[i]["StartHour"] = 0;
		if (PVPDays[i]["EndHour"] > 23 || PVPDays[i]["EndHour"] < 0)
			PVPDays[i]["EndHour"] = 0;

		Log::GetLog()->info("Loaded PVP Schedule {} Start Day: {}, Hour: {} - End Day: {}, End Hour {}", (i + 1),
		                    Days[(int)PVPDays[i]["StartDay"]], (int)PVPDays[i]["StartHour"], Days[(int)PVPDays[i]["EndDay"]],
		                    (int)PVPDays[i]["EndHour"]);
	}

	ProtectCharacters = PVPConfig["PVPScheduler"].value("ProtectPlayersPVPOff", true);
	ProtectDinos = PVPConfig["PVPScheduler"].value("ProtectDinosPVPOff", true);
	ProtectStructures = PVPConfig["PVPScheduler"].value("ProtectStructuresPVPOff", true);
	ProtectExplosives = PVPConfig["PVPScheduler"].value("ProtectExplosivesPVPOff", true);
}
