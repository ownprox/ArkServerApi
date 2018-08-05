#pragma once

void ToggleWeekendRates(AShooterPlayerController* player, FString* message, int mode);
void WeekendRatesCheck();

void EnableRates();
void DisableRates();

inline void InitHooks()
{
	ArkApi::GetCommands().AddOnTimerCallback("WeekendRatesCheck", &WeekendRatesCheck);

	ArkApi::GetCommands().AddChatCommand("/ToggleWeekendRates", &ToggleWeekendRates);
}

inline void RemoveHooks()
{
	ArkApi::GetCommands().RemoveOnTimerCallback("WeekendRatesCheck");

	ArkApi::GetCommands().RemoveChatCommand("/ToggleWeekendRates");
}

inline time_t RawTime;
inline tm TimeInfo;
inline int TenMinCounter;
inline float OldValues[15];

inline bool RatesEnabledManually, HasStarted = false;

inline void ToggleWeekendRates(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player->GetPlayerCharacter() || !player->GetPlayerCharacter()->bIsServerAdminField() || WeekendRatesEnabled)
		return;

	RatesEnabledManually = !RatesEnabledManually;

	if (RatesEnabledManually)
	{
		EnableRates();
	}
	else
	{
		DisableRates();
	}
}

inline void EnableRates()
{
	AShooterGameMode* GameMode = ArkApi::GetApiUtils().GetShooterGameMode();

	OldValues[0] = GameMode->XPMultiplierField();
	const float XPMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["XPMultiplier"]);
	if (XPMultiplier != 0)
		GameMode->XPMultiplierField() = XPMultiplier;

	OldValues[1] = GameMode->TamingSpeedMultiplierField();
	const float TamingSpeedMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["TamingSpeedMultiplier"]);
	if (TamingSpeedMultiplier != 0)
		GameMode->TamingSpeedMultiplierField() = TamingSpeedMultiplier;

	OldValues[2] = GameMode->HarvestAmountMultiplierField();
	const float HarvestAmountMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["HarvestAmountMultiplier"]);
	if (HarvestAmountMultiplier != 0)
		GameMode->HarvestAmountMultiplierField() = HarvestAmountMultiplier;

	OldValues[3] = GameMode->HarvestHealthMultiplierField();
	const float HarvestHealthMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["HarvestHealthMultiplier"]);
	if (HarvestHealthMultiplier != 0)
		GameMode->HarvestHealthMultiplierField() = HarvestHealthMultiplier;

	OldValues[4] = GameMode->CraftXPMultiplierField();
	const float CraftXPMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["CraftXPMultiplier"]);
	if (CraftXPMultiplier != 0)
		GameMode->CraftXPMultiplierField() = CraftXPMultiplier;

	OldValues[5] = GameMode->FishingLootQualityMultiplierField();
	const float FishingLootQuality = static_cast<float>(WeekendConfig["WeekendRates"]["FishingLootQualityMultiplier"]);
	if (FishingLootQuality != 0)
		GameMode->FishingLootQualityMultiplierField() = FishingLootQuality;

	OldValues[6] = GameMode->BabyMatureSpeedMultiplierField();
	const float BabyMatureSpeed = static_cast<float>(WeekendConfig["WeekendRates"]["BabyMatureSpeedMultiplier"]);
	if (BabyMatureSpeed != 0)
		GameMode->BabyMatureSpeedMultiplierField() = BabyMatureSpeed;

	OldValues[7] = GameMode->MateBoostEffectMultiplierField();
	const float MateBoostEffect = static_cast<float>(WeekendConfig["WeekendRates"]["MateBoostEffectMultiplier"]);
	if (MateBoostEffect != 0)
		GameMode->MateBoostEffectMultiplierField() = MateBoostEffect;

	OldValues[8] = GameMode->MatingIntervalMultiplierField();
	const float MatingIntervalMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["MatingIntervalMultiplier"]);
	if (MatingIntervalMultiplier != 0)
		GameMode->MatingIntervalMultiplierField() = MatingIntervalMultiplier;

	OldValues[9] = GameMode->MatingSpeedMultiplierField();
	const float MatingSpeedMultiplier = static_cast<float>(WeekendConfig["WeekendRates"]["MatingSpeedMultiplier"]);
	if (MatingSpeedMultiplier != 0)
		GameMode->MatingSpeedMultiplierField() = MatingSpeedMultiplier;

	OldValues[10] = GameMode->EggHatchSpeedMultiplierField();
	const float EggHatchSpeed = static_cast<float>(WeekendConfig["WeekendRates"]["EggHatchSpeedMultiplier"]);
	if (EggHatchSpeed != 0)
		GameMode->EggHatchSpeedMultiplierField() = EggHatchSpeed;

	OldValues[11] = GameMode->LayEggIntervalMultiplierField();
	const float LayEggInterval = static_cast<float>(WeekendConfig["WeekendRates"]["LayEggIntervalMultiplier"]);
	if (LayEggInterval != 0)
		GameMode->LayEggIntervalMultiplierField() = LayEggInterval;

	OldValues[12] = GameMode->ResourcesRespawnPeriodMultiplierField();
	const float ResourcesRespawnPeriod = static_cast<float>(WeekendConfig["WeekendRates"][
		"ResourcesRespawnPeriodMultiplier"]);
	if (ResourcesRespawnPeriod != 0)
		GameMode->ResourcesRespawnPeriodMultiplierField() = ResourcesRespawnPeriod;

	OldValues[13] = GameMode->SupplyCrateLootQualityMultiplierField();
	const float SupplyCrateLootQuality = static_cast<float>(WeekendConfig["WeekendRates"][
		"SupplyCrateLootQualityMultiplier"]);
	if (SupplyCrateLootQuality != 0)
		GameMode->SupplyCrateLootQualityMultiplierField() = SupplyCrateLootQuality;

	OldValues[14] = GameMode->StructureResistanceMultiplierField();
	const float StructureResistance = static_cast<float>(WeekendConfig["WeekendRates"].value(
		"StructureResistanceMultiplier", 0));
	if (StructureResistance != 0)
		GameMode->StructureResistanceMultiplierField() = StructureResistance;

	if (MOTDChangeEnabled) GameMode->SetMessageOfTheDay(&WeekendMOTD);

	if (Notification)
		ArkApi::GetApiUtils().SendNotificationToAll(WeekendRatesEnabledRGB, 1.2f, 8, nullptr,
		                                            *WeekendRatesEnabledMessage);

	if (Chatbox)
		ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"<RichColor Color=\"{}, {}, {}, {}\">{}</>",
		                                           WeekendRatesEnabledRGB.R, WeekendRatesEnabledRGB.G,
		                                           WeekendRatesEnabledRGB.B, WeekendRatesEnabledRGB.A,
		                                           *WeekendRatesEnabledMessage);
}

inline void DisableRates()
{
	AShooterGameMode* GameMode = ArkApi::GetApiUtils().GetShooterGameMode();

	GameMode->XPMultiplierField() = OldValues[0];
	GameMode->TamingSpeedMultiplierField() = OldValues[1];
	GameMode->HarvestAmountMultiplierField() = OldValues[2];
	GameMode->HarvestHealthMultiplierField() = OldValues[3];
	GameMode->CraftXPMultiplierField() = OldValues[4];
	GameMode->FishingLootQualityMultiplierField() = OldValues[5];
	GameMode->BabyMatureSpeedMultiplierField() = OldValues[6];
	GameMode->MateBoostEffectMultiplierField() = OldValues[7];
	GameMode->MatingIntervalMultiplierField() = OldValues[8];
	GameMode->MatingSpeedMultiplierField() = OldValues[9];
	GameMode->EggHatchSpeedMultiplierField() = OldValues[10];
	GameMode->LayEggIntervalMultiplierField() = OldValues[11];
	GameMode->ResourcesRespawnPeriodMultiplierField() = OldValues[12];
	GameMode->SupplyCrateLootQualityMultiplierField() = OldValues[13];
	GameMode->StructureResistanceMultiplierField() = OldValues[14];

	if (MOTDChangeEnabled) GameMode->SetMessageOfTheDay(&NormalMOTD);

	if (Notification)
		ArkApi::GetApiUtils().SendNotificationToAll(WeekendRatesDisabledRGB, 1.2f, 8, nullptr,
		                                            *WeekendRatesDisabledMessage);

	if (Chatbox)
		ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"<RichColor Color=\"{}, {}, {}, {}\">{}</>",
		                                           WeekendRatesDisabledRGB.R, WeekendRatesDisabledRGB.G,
		                                           WeekendRatesDisabledRGB.B, WeekendRatesDisabledRGB.A,
		                                           *WeekendRatesDisabledMessage);
}

inline void WeekendRatesCheck()
{
	AShooterGameMode* GameMode = ArkApi::GetApiUtils().GetShooterGameMode();
	if (!GameMode) return;

	if (TenMinCounter++ == (HasStarted ? 600 : 30))
	{
		HasStarted = true;
		time(&RawTime);
		localtime_s(&TimeInfo, &RawTime);

		TimeInfo.tm_min = 0;
		TimeInfo.tm_hour = 0;
		TimeInfo.tm_sec = 0;

		const time_t Tm = mktime(&TimeInfo);

		const auto Now = std::chrono::system_clock::from_time_t(Tm);

		const auto StartTime = Now + std::chrono::hours((StartDay - TimeInfo.tm_wday + 7) % 7 * 24);
		const auto EndTime = Now + std::chrono::hours((EndDay - TimeInfo.tm_wday + 7) % 7 * 24);

		if ((Now >= StartTime || StartTime > EndTime) && Now < EndTime)
		{
			if (!WeekendRatesEnabled)
			{
				WeekendRatesEnabled = true;

				EnableRates();
			}
		}
		else if (WeekendRatesEnabled)
		{
			WeekendRatesEnabled = false;

			DisableRates();
		}

		TenMinCounter = 0;
	}
}
