#pragma once
void WeekendRatesCheck();

void InitHooks()
{
	ArkApi::GetCommands().AddOnTimerCallback("WeekendRatesCheck", &WeekendRatesCheck);
}

void RemoveHooks()
{
	ArkApi::GetCommands().RemoveOnTimerCallback("WeekendRatesCheck");
}

time_t rawtime;
tm timeinfo;
int TenMinCounter = 0;
float OldValues[14];

void WeekendRatesCheck()
{	
	if (!ArkApi::GetApiUtils().GetShooterGameMode()) return;
	if (TenMinCounter++ == 600)
	{
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		if (!WeekendRatesEnabled && (timeinfo.tm_wday == 0 || timeinfo.tm_wday == 6))
		{
			WeekendRatesEnabled = true;

			OldValues[0] = ArkApi::GetApiUtils().GetShooterGameMode()->XPMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->XPMultiplierField() = (float)WeekendConfig["WeekendRates"]["XPMultiplier"];

			OldValues[1] = ArkApi::GetApiUtils().GetShooterGameMode()->TamingSpeedMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->TamingSpeedMultiplierField() = (float)WeekendConfig["WeekendRates"]["TamingSpeedMultiplier"];

			OldValues[2] = ArkApi::GetApiUtils().GetShooterGameMode()->HarvestAmountMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->HarvestAmountMultiplierField() = (float)WeekendConfig["WeekendRates"]["HarvestAmountMultiplier"];

			OldValues[3] = ArkApi::GetApiUtils().GetShooterGameMode()->HarvestHealthMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->HarvestHealthMultiplierField() = (float)WeekendConfig["WeekendRates"]["HarvestHealthMultiplier"];

			OldValues[4] = ArkApi::GetApiUtils().GetShooterGameMode()->CraftXPMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->CraftXPMultiplierField() = (float)WeekendConfig["WeekendRates"]["CraftXPMultiplier"];

			OldValues[5] = ArkApi::GetApiUtils().GetShooterGameMode()->FishingLootQualityMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->FishingLootQualityMultiplierField() = (float)WeekendConfig["WeekendRates"]["FishingLootQualityMultiplier"];

			OldValues[6] = ArkApi::GetApiUtils().GetShooterGameMode()->BabyMatureSpeedMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->BabyMatureSpeedMultiplierField() = (float)WeekendConfig["WeekendRates"]["BabyMatureSpeedMultiplier"];

			OldValues[7] = ArkApi::GetApiUtils().GetShooterGameMode()->MateBoostEffectMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->MateBoostEffectMultiplierField() = (float)WeekendConfig["WeekendRates"]["MateBoostEffectMultiplier"];

			OldValues[8] = ArkApi::GetApiUtils().GetShooterGameMode()->MatingIntervalMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->MatingIntervalMultiplierField() = (float)WeekendConfig["WeekendRates"]["MatingIntervalMultiplier"];

			OldValues[9] = ArkApi::GetApiUtils().GetShooterGameMode()->MatingSpeedMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->MatingSpeedMultiplierField() = (float)WeekendConfig["WeekendRates"]["MatingSpeedMultiplier"];

			OldValues[10] = ArkApi::GetApiUtils().GetShooterGameMode()->EggHatchSpeedMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->EggHatchSpeedMultiplierField() = (float)WeekendConfig["WeekendRates"]["EggHatchSpeedMultiplier"];

			OldValues[11] = ArkApi::GetApiUtils().GetShooterGameMode()->LayEggIntervalMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->LayEggIntervalMultiplierField() = (float)WeekendConfig["WeekendRates"]["LayEggIntervalMultiplier"];

			OldValues[12] = ArkApi::GetApiUtils().GetShooterGameMode()->ResourcesRespawnPeriodMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->ResourcesRespawnPeriodMultiplierField() = (float)WeekendConfig["WeekendRates"]["ResourcesRespawnPeriodMultiplier"];

			OldValues[13] = ArkApi::GetApiUtils().GetShooterGameMode()->SupplyCrateLootQualityMultiplierField()();
			ArkApi::GetApiUtils().GetShooterGameMode()->SupplyCrateLootQualityMultiplierField() = (float)WeekendConfig["WeekendRates"]["SupplyCrateLootQualityMultiplier"];

			if (MOTDChangeEnabled) ArkApi::GetApiUtils().GetShooterGameMode()->SetMessageOfTheDay(&WeekendMOTD);
			if (Notification) ArkApi::GetApiUtils().SendNotificationToAll(WeekendRatesEnabledRGB, 1.2f, 8, nullptr, *WeekendRatesEnabledMessage);
			if (Chatbox) ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"<RichColor Color=\"{}, {}, {}, {}\">{}</>", WeekendRatesEnabledRGB.R, WeekendRatesEnabledRGB.G, WeekendRatesEnabledRGB.B, WeekendRatesEnabledRGB.A, *WeekendRatesEnabledMessage);
		}
		else if (WeekendRatesEnabled && timeinfo.tm_wday != 0 && timeinfo.tm_wday != 6)
		{
			WeekendRatesEnabled = false;

			ArkApi::GetApiUtils().GetShooterGameMode()->XPMultiplierField() = OldValues[0];

			ArkApi::GetApiUtils().GetShooterGameMode()->TamingSpeedMultiplierField() = OldValues[1];

			ArkApi::GetApiUtils().GetShooterGameMode()->HarvestAmountMultiplierField() = OldValues[2];

			ArkApi::GetApiUtils().GetShooterGameMode()->HarvestHealthMultiplierField() = OldValues[3];

			ArkApi::GetApiUtils().GetShooterGameMode()->CraftXPMultiplierField() = OldValues[4];

			ArkApi::GetApiUtils().GetShooterGameMode()->FishingLootQualityMultiplierField() = OldValues[5];

			ArkApi::GetApiUtils().GetShooterGameMode()->BabyMatureSpeedMultiplierField() = OldValues[6];

			ArkApi::GetApiUtils().GetShooterGameMode()->MateBoostEffectMultiplierField() = OldValues[7];

			ArkApi::GetApiUtils().GetShooterGameMode()->MatingIntervalMultiplierField() = OldValues[8];

			ArkApi::GetApiUtils().GetShooterGameMode()->MatingSpeedMultiplierField() = OldValues[9];

			ArkApi::GetApiUtils().GetShooterGameMode()->EggHatchSpeedMultiplierField() = OldValues[10];

			ArkApi::GetApiUtils().GetShooterGameMode()->LayEggIntervalMultiplierField() = OldValues[11];

			ArkApi::GetApiUtils().GetShooterGameMode()->ResourcesRespawnPeriodMultiplierField() = OldValues[12];

			ArkApi::GetApiUtils().GetShooterGameMode()->SupplyCrateLootQualityMultiplierField() = OldValues[13];

			if (MOTDChangeEnabled) ArkApi::GetApiUtils().GetShooterGameMode()->SetMessageOfTheDay(&NormalMOTD);
			if (Notification) ArkApi::GetApiUtils().SendNotificationToAll(WeekendRatesDisabledRGB, 1.2f, 8, nullptr, *WeekendRatesDisabledMessage);
			if (Chatbox) ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"<RichColor Color=\"{}, {}, {}, {}\">{}</>", WeekendRatesDisabledRGB.R, WeekendRatesDisabledRGB.G, WeekendRatesDisabledRGB.B, WeekendRatesDisabledRGB.A, *WeekendRatesDisabledMessage);
		}
		TenMinCounter = 0;
	}
}