#pragma once

#include "WeekendRates.h"

#include <fstream>

inline void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/WeekendRates/config.json");
	if (!file.is_open()) return;
	file >> WeekendConfig;
	file.close();

	std::string Data = WeekendConfig["WeekendRates"]["ServerName"];
	ServerName = FString(ArkApi::Tools::Utf8Decode(Data).c_str());

	Notification = WeekendConfig["WeekendRates"]["Notification"];

	Chatbox = WeekendConfig["WeekendRates"]["Chatbox"];

	nlohmann::json ServRGB = WeekendConfig["WeekendRates"]["WeekendRatesEnabledRGB"];
	WeekendRatesEnabledRGB = FLinearColor(ServRGB[0], ServRGB[1], ServRGB[2], ServRGB[3]);

	Data = WeekendConfig["WeekendRates"]["WeekendRatesEnabledMessage"];
	WeekendRatesEnabledMessage = FString(ArkApi::Tools::Utf8Decode(Data).c_str());

	ServRGB = WeekendConfig["WeekendRates"]["WeekendRatesDisabledRGB"];
	WeekendRatesDisabledRGB = FLinearColor(ServRGB[0], ServRGB[1], ServRGB[2], ServRGB[3]);

	Data = WeekendConfig["WeekendRates"]["WeekendRatesDisabledMessage"];
	WeekendRatesDisabledMessage = FString(ArkApi::Tools::Utf8Decode(Data).c_str());

	MOTDChangeEnabled = WeekendConfig["WeekendRates"]["MOTDChangeEnabled"];

	Data = WeekendConfig["WeekendRates"]["WeekendMOTD"];
	WeekendMOTD = FString(ArkApi::Tools::Utf8Decode(Data).c_str());

	Data = WeekendConfig["WeekendRates"]["NormalMOTD"];
	NormalMOTD = FString(ArkApi::Tools::Utf8Decode(Data).c_str());

	StartDay = WeekendConfig["WeekendRates"]["StartDay"];
	EndDay = WeekendConfig["WeekendRates"]["EndDay"];
}
