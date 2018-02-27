#pragma once
FString GGameUserSettingsIni = "../../../ShooterGame/Saved/Config/WindowsServer/GameUserSettings.ini";

static DataValue<FConfigCacheIniNew *> GetGConfig() { return { "Global.GConfig" }; }

void ReloadINI()
{	
	const auto SGM = ArkApi::GetApiUtils().GetShooterGameMode();

	NativeCall<void>(nullptr, "FConfigCacheIni.InitializeConfigSystem");
	NativeCall<void, AShooterGameMode*>(nullptr, "Global.PrimalGameIni_GameModeInitValues", SGM);

	float fval;
	bool bval;
	
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowThirdPersonPlayer", &bval, &GGameUserSettingsIni)) SGM->bServerAllowThirdPersonPlayerField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"Hardcore", &bval, &GGameUserSettingsIni)) SGM->bServerHardcoreField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PVE", &bval, &GGameUserSettingsIni)) SGM->bServerPVEField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"Crosshair", &bval, &GGameUserSettingsIni)) SGM->bServerCrosshairField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"MapPlayerLocation", &bval, &GGameUserSettingsIni)) SGM->bMapPlayerLocationField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowFlyerCarryPvE", &bval, &GGameUserSettingsIni)) SGM->bAllowFlyerCarryPvEField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableStructureDecayPvE", &bval, &GGameUserSettingsIni)) SGM->bDisableStructureDecayPvEField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableDinoDecayPvE", &bval, &GGameUserSettingsIni)) SGM->bDisableDinoDecayPvEField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowCaveBuildingPvE", &bval, &GGameUserSettingsIni)) SGM->bAllowCaveBuildingPvEField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvPStructureDecay", &bval, &GGameUserSettingsIni)) SGM->bPvPStructureDecayField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"OnlyAutoDestroyCoreStructures", &bval, &GGameUserSettingsIni)) SGM->bOnlyAutoDestroyCoreStructuresField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"TribeLogDestroyedEnemyStructures", &bval, &GGameUserSettingsIni)) SGM->bTribeLogDestroyedEnemyStructuresField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvEAllowStructuresAtSupplyDrops", &bval, &GGameUserSettingsIni)) SGM->bPvEAllowStructuresAtSupplyDropsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"RCONOutputTribeLogs", &bval, &GGameUserSettingsIni)) SGM->bServerRCONOutputTribeLogsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"UseOptimizedHarvestingHealth", &bval, &GGameUserSettingsIni)) SGM->bUseOptimizedHarvestingHealthField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"ClampItemSpoilingTimes", &bval, &GGameUserSettingsIni)) SGM->bClampItemSpoilingTimesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AutoDestroyDecayedDinos", &bval, &GGameUserSettingsIni)) SGM->bAutoDestroyDecayedDinosField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowMultipleAttachedC4", &bval, &GGameUserSettingsIni)) SGM->bAllowMultipleAttachedC4Field() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowFlyingStaminaRecovery", &bval, &GGameUserSettingsIni)) SGM->bAllowFlyingStaminaRecoveryField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventSpawnAnimations", &bval, &GGameUserSettingsIni)) SGM->bPreventSpawnAnimationsField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"TheMaxStructuresInRange", &fval, &GGameUserSettingsIni)) SGM->TheMaxStructuresInRangeField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DayCycleSpeedScale", &fval, &GGameUserSettingsIni)) SGM->DayCycleSpeedScaleField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"NightTimeSpeedScale", &fval, &GGameUserSettingsIni)) SGM->NightTimeSpeedScaleField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DayTimeSpeedScale", &fval, &GGameUserSettingsIni)) SGM->DayTimeSpeedScaleField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PvEStructureDecayPeriodMultiplier", &fval, &GGameUserSettingsIni)) SGM->PvEStructureDecayPeriodMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PvEDinoDecayPeriodMultiplier", &fval, &GGameUserSettingsIni)) SGM->PvEDinoDecayPeriodMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"ResourcesRespawnPeriodMultiplier", &fval, &GGameUserSettingsIni)) SGM->ResourcesRespawnPeriodMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PerPlatformMaxStructuresMultiplier", &fval, &GGameUserSettingsIni)) SGM->PerPlatformMaxStructuresMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"RCONServerGameLogBuffer", &fval, &GGameUserSettingsIni)) SGM->RCONServerGameLogBufferField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"OxygenSwimSpeedStatMultiplier", &fval, &GGameUserSettingsIni)) SGM->OxygenSwimSpeedStatMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"KickIdlePlayersPeriod", &fval, &GGameUserSettingsIni)) SGM->KickIdlePlayersPeriodField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"AutoSavePeriodMinutes", &fval, &GGameUserSettingsIni)) SGM->AutoSavePeriodMinutesField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"XPMultiplier", &fval, &GGameUserSettingsIni)) SGM->XPMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"ForceRespawnDinos", &bval, &GGameUserSettingsIni)) SGM->bForceRespawnDinosField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoDamageMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerDamageMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"StructureDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->StructureDamageMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerResistanceMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerResistanceMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoResistanceMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoResistanceMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"StructureResistanceMultiplier", &fval, &GGameUserSettingsIni)) SGM->StructureResistanceMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"TamingSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->TamingSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"HarvestAmountMultiplier", &fval, &GGameUserSettingsIni)) SGM->HarvestAmountMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"HarvestHealthMultiplier", &fval, &GGameUserSettingsIni)) SGM->HarvestHealthMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerCharacterWaterDrainMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerCharacterWaterDrainMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerCharacterFoodDrainMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerCharacterFoodDrainMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoCharacterFoodDrainMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoCharacterFoodDrainMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerCharacterStaminaDrainMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerCharacterStaminaDrainMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoCharacterStaminaDrainMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoCharacterStaminaDrainMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerCharacterHealthRecoveryMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerCharacterHealthRecoveryMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoCharacterHealthRecoveryMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoCharacterHealthRecoveryMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoCountMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoCountMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PvPZoneStructureDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->PvPZoneStructureDamageMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"OnlyAllowSpecifiedEngrams", &bval, &GGameUserSettingsIni)) SGM->bOnlyAllowSpecifiedEngramsField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"OverrideMaxExperiencePointsPlayer", &fval, &GGameUserSettingsIni)) SGM->OverrideMaxExperiencePointsPlayerField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"OverrideMaxExperiencePointsDino", &fval, &GGameUserSettingsIni)) SGM->OverrideMaxExperiencePointsDinoField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"GlobalSpoilingTimeMultiplier", &fval, &GGameUserSettingsIni)) SGM->GlobalSpoilingTimeMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"GlobalItemDecompositionTimeMultiplier", &fval, &GGameUserSettingsIni)) SGM->GlobalItemDecompositionTimeMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"GlobalCorpseDecompositionTimeMultiplier", &fval, &GGameUserSettingsIni)) SGM->GlobalCorpseDecompositionTimeMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AutoPvETimer", &bval, &GGameUserSettingsIni)) SGM->bAutoPvETimerField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AutoPvEUseSystemTime", &bval, &GGameUserSettingsIni)) SGM->bAutoPvEUseSystemTimeField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"AutoPvEStartTimeSeconds", &fval, &GGameUserSettingsIni)) SGM->AutoPvEStartTimeSecondsField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"AutoPvEStopTimeSeconds", &fval, &GGameUserSettingsIni)) SGM->AutoPvEStopTimeSecondsField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"TributeItemExpirationSeconds", &fval, &GGameUserSettingsIni)) SGM->TributeItemExpirationSecondsField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"TributeDinoExpirationSeconds", &fval, &GGameUserSettingsIni)) SGM->TributeDinoExpirationSecondsField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"TributeCharacterExpirationSeconds", &fval, &GGameUserSettingsIni)) SGM->TributeCharacterExpirationSecondsField() = (int)fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventDownloadSurvivors", &bval, &GGameUserSettingsIni)) SGM->PreventDownloadSurvivorsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventDownloadItems", &bval, &GGameUserSettingsIni)) SGM->PreventDownloadItemsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventDownloadDinos", &bval, &GGameUserSettingsIni)) SGM->PreventDownloadDinosField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventUploadSurvivors", &bval, &GGameUserSettingsIni)) SGM->bPreventUploadSurvivorsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventUploadItems", &bval, &GGameUserSettingsIni)) SGM->bPreventUploadItemsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventUploadDinos", &bval, &GGameUserSettingsIni)) SGM->bPreventUploadDinosField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"IncreasePvPRespawnInterval", &bval, &GGameUserSettingsIni)) SGM->bIncreasePvPRespawnIntervalField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"IncreasePvPRespawnIntervalCheckPeriod", &fval, &GGameUserSettingsIni)) SGM->IncreasePvPRespawnIntervalCheckPeriodField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"IncreasePvPRespawnIntervalMultiplier", &fval, &GGameUserSettingsIni)) SGM->IncreasePvPRespawnIntervalMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"IncreasePvPRespawnIntervalBaseAmount", &fval, &GGameUserSettingsIni)) SGM->IncreasePvPRespawnIntervalBaseAmountField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"ResourceNoReplenishRadiusStructures", &fval, &GGameUserSettingsIni)) SGM->ResourceNoReplenishRadiusStructuresField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"ResourceNoReplenishRadiusPlayers", &fval, &GGameUserSettingsIni)) SGM->ResourceNoReplenishRadiusPlayersField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"CropGrowthSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->CropGrowthSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"LayEggIntervalMultiplier", &fval, &GGameUserSettingsIni)) SGM->LayEggIntervalMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PoopIntervalMultiplier", &fval, &GGameUserSettingsIni)) SGM->PoopIntervalMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"CropDecaySpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->CropDecaySpeedMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvEDisableFriendlyFire", &bval, &GGameUserSettingsIni)) SGM->bPvEDisableFriendlyFireField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"FlyerPlatformAllowUnalignedDinoBasing", &bval, &GGameUserSettingsIni)) SGM->bFlyerPlatformAllowUnalignedDinoBasingField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MaxPlatformSaddleStructureLimit", &fval, &GGameUserSettingsIni)) SGM->MaxPlatformSaddleStructureLimitField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MaxNumberOfPlayersInTribe", &fval, &GGameUserSettingsIni)) SGM->MaxNumberOfPlayersInTribeField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MatingIntervalMultiplier", &fval, &GGameUserSettingsIni)) SGM->MatingIntervalMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"EggHatchSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->EggHatchSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyMatureSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyMatureSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyFoodConsumptionSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyFoodConsumptionSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"StructureDamageRepairCooldown", &fval, &GGameUserSettingsIni)) SGM->StructureDamageRepairCooldownField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"CustomRecipeEffectivenessMultiplier", &fval, &GGameUserSettingsIni)) SGM->CustomRecipeEffectivenessMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"CustomRecipeSkillMultiplier", &fval, &GGameUserSettingsIni)) SGM->CustomRecipeSkillMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvEAllowTribeWar", &bval, &GGameUserSettingsIni)) SGM->bPvEAllowTribeWarField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvEAllowTribeWarCancel", &bval, &GGameUserSettingsIni)) SGM->bPvEAllowTribeWarCancelField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowCustomRecipes", &bval, &GGameUserSettingsIni)) SGM->bAllowCustomRecipesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PassiveDefensesDamageRiderlessDinos", &bval, &GGameUserSettingsIni)) SGM->bPassiveDefensesDamageRiderlessDinosField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MaxTribeLogs", &fval, &GGameUserSettingsIni)) SGM->MaxTribeLogsField() = (int)fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MaxPersonalTamedDinos", &fval, &GGameUserSettingsIni)) SGM->MaxPersonalTamedDinosField() = (int)fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableFriendlyFire", &bval, &GGameUserSettingsIni)) SGM->bDisableFriendlyFireField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoHarvestingDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoHarvestingDamageMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PlayerHarvestingDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->PlayerHarvestingDamageMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"DinoTurretDamageMultiplier", &fval, &GGameUserSettingsIni)) SGM->DinoTurretDamageMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableLootCrates", &bval, &GGameUserSettingsIni)) SGM->bDisableLootCratesField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"ExtinctionEventTimeInterval", &fval, &GGameUserSettingsIni)) SGM->ExtinctionEventTimeIntervalField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"EnableExtraStructurePreventionVolumes", &bval, &GGameUserSettingsIni)) SGM->bEnableExtraStructurePreventionVolumesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"ForceAllowCaveFlyers", &bval, &GGameUserSettingsIni)) SGM->bForceAllowCaveFlyersField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventOfflinePvP", &bval, &GGameUserSettingsIni)) SGM->bPreventOfflinePvPField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PvPDinoDecay", &bval, &GGameUserSettingsIni)) SGM->bPvPDinoDecayField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowAnyoneBabyImprintCuddle", &bval, &GGameUserSettingsIni)) SGM->bAllowAnyoneBabyImprintCuddleField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableImprintDinoBuff", &bval, &GGameUserSettingsIni)) SGM->bDisableImprintDinoBuffField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"ShowFloatingDamageText", &bval, &GGameUserSettingsIni)) SGM->bShowFloatingDamageTextField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"OnlyDecayUnsnappedCoreStructures", &bval, &GGameUserSettingsIni)) SGM->bOnlyDecayUnsnappedCoreStructuresField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"FastDecayUnsnappedCoreStructures", &bval, &GGameUserSettingsIni)) SGM->bFastDecayUnsnappedCoreStructuresField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DestroyUnconnectedWaterPipes", &bval, &GGameUserSettingsIni)) SGM->bDestroyUnconnectedWaterPipesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowCrateSpawnsOnTopOfStructures", &bval, &GGameUserSettingsIni)) SGM->bAllowCrateSpawnsOnTopOfStructuresField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"NotifyAdminCommandsInChat", &bval, &GGameUserSettingsIni)) SGM->bNotifyAdminCommandsInChatField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"PreventOfflinePvPInterval", &fval, &GGameUserSettingsIni)) SGM->PreventOfflinePvPIntervalField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyImprintingStatScaleMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyImprintingStatScaleMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyCuddleIntervalMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyCuddleIntervalMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyCuddleGracePeriodMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyCuddleGracePeriodMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"BabyCuddleLoseImprintQualitySpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->BabyCuddleLoseImprintQualitySpeedMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"HairGrowthSpeedMultiplier", &fval, &GGameUserSettingsIni)) SGM->HairGrowthSpeedMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"PreventDiseases", &bval, &GGameUserSettingsIni)) SGM->bPreventDiseasesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"NonPermanentDiseases", &bval, &GGameUserSettingsIni)) SGM->bNonPermanentDiseasesField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableDinoTaming", &bval, &GGameUserSettingsIni)) SGM->bDisableDinoTamingField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableDinoRiding", &bval, &GGameUserSettingsIni)) SGM->bDisableDinoRidingField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"MinimumDinoReuploadInterval", &fval, &GGameUserSettingsIni)) SGM->MinimumDinoReuploadIntervalField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"OverrideOfficialDifficulty", &fval, &GGameUserSettingsIni)) SGM->OverrideOfficialDifficultyField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"UseSingleplayerSettings", &bval, &GGameUserSettingsIni)) SGM->bUseSingleplayerSettingsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"UseCorpseLocator", &bval, &GGameUserSettingsIni)) SGM->bUseCorpseLocatorField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"DisableStructurePlacementCollision", &bval, &GGameUserSettingsIni)) SGM->bDisableStructurePlacementCollisionField() = bval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"SupplyCrateLootQualityMultiplier", &fval, &GGameUserSettingsIni)) SGM->SupplyCrateLootQualityMultiplierField() = fval;
	if (GetGConfig()()->GetFloat(L"ServerSettings", L"FishingLootQualityMultiplier", &fval, &GGameUserSettingsIni)) SGM->FishingLootQualityMultiplierField() = fval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowPlatformSaddleMultiFloors", &bval, &GGameUserSettingsIni)) SGM->bAllowPlatformSaddleMultiFloorsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AllowUnlimitedRespecs", &bval, &GGameUserSettingsIni)) SGM->bAllowUnlimitedRespecsField() = bval;
	if (GetGConfig()()->GetBool(L"ServerSettings", L"AutoUnlockAllEngrams", &bval, &GGameUserSettingsIni)) SGM->bAutoUnlockAllEngramsField() = bval;
}

void ReloadSettings(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), L"Please login as a admin.");
		return;
	}
	ReloadINI();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), L"Settings Reloaded!.");
}

void RconReloadSettings(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
{
	ReloadINI();
	FString Msg = "Settings Reloaded!.";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &Msg);
}

void ConsoleReloadSettings(APlayerController* player, FString* message, bool mode)
{
	if (!player || !player->PlayerStateField()()) return;
	AShooterPlayerController* aPlayer = static_cast<AShooterPlayerController*>(player);
	if (!aPlayer || !aPlayer->GetPlayerCharacter()) return;
	if (!aPlayer->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(aPlayer, FLinearColor(1, 0, 0), L"Please login as a admin.");
		return;
	}
	ReloadINI();
	ArkApi::GetApiUtils().SendServerMessage(aPlayer, FLinearColor(0, 1, 0), L"Settings Reloaded!.");
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/sreload", &ReloadSettings);
	ArkApi::GetCommands().AddRconCommand("sreload", &RconReloadSettings);
	ArkApi::GetCommands().AddConsoleCommand("sreload", &ConsoleReloadSettings);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/sreload");
	ArkApi::GetCommands().RemoveRconCommand("sreload");
	ArkApi::GetCommands().RemoveConsoleCommand("sreload");
}
