#pragma once

DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalDinoCharacter_TakeDamage, float, APrimalDinoCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructureExplosive_CanDetonateMe, bool, APrimalStructureExplosive*, AShooterCharacter*, bool);
DECLARE_HOOK(APrimalStructure_IsAllowedToBuild, int, APrimalStructure*, APlayerController*, FVector, FRotator, FPlacementData*, bool, FRotator, bool);


void PVPCheckTimer();

inline void InitHooks()
{
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage, &APrimalCharacter_TakeDamage_original);
	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage, &APrimalStructure_TakeDamage_original);
	ArkApi::GetHooks().SetHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage, &APrimalDinoCharacter_TakeDamage_original);
	ArkApi::GetHooks().SetHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild, &APrimalStructure_IsAllowedToBuild_original);

	ArkApi::GetHooks().SetHook("APrimalStructureExplosive.CanDetonateMe", &Hook_APrimalStructureExplosive_CanDetonateMe, &APrimalStructureExplosive_CanDetonateMe_original);
	ArkApi::GetCommands().AddOnTimerCallback("PVPCheckTimer", &PVPCheckTimer);
}

inline void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructureExplosive.CanDetonateMe", &Hook_APrimalStructureExplosive_CanDetonateMe);

	ArkApi::GetHooks().DisableHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild);
	ArkApi::GetCommands().RemoveOnTimerCallback("PVPCheckTimer");
}

inline time_t rawtime;
inline tm timeinfo;
inline int OneMinCounter = 60;

inline void PVPCheckTimer()
{
	if (!ArkApi::GetApiUtils().GetShooterGameMode())
		return;

	if (OneMinCounter++ >= 60)
	{
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		timeinfo.tm_min = 0;
		timeinfo.tm_hour = 0;
		timeinfo.tm_sec = 0;

		const time_t Tm = mktime(&timeinfo);

		const auto Now = std::chrono::system_clock::from_time_t(Tm);
		const auto RealNow = std::chrono::system_clock::now();

		bool IsActive = false;

		for (int i = 0; i < PVPDays.size(); i++)
		{
			const int StartDay = PVPDays[i]["StartDay"];
			const int StartHour = PVPDays[i]["StartHour"];
			const int EndHour = PVPDays[i]["EndHour"];
			const int EndDay = PVPDays[i]["EndDay"];

			const auto StartTime = Now + std::chrono::hours((StartDay - timeinfo.tm_wday + 7) % 7 * 24) + std::chrono::
				minutes(
					(StartHour - timeinfo.tm_hour + 24) % 24 * 60);
			const auto EndTime = Now + std::chrono::hours((EndDay - timeinfo.tm_wday + 7) % 7 * 24) + std::chrono::
				minutes(
					(EndHour - timeinfo.tm_hour + 24) % 24 * 60);

			if ((RealNow >= StartTime || StartTime > EndTime) && RealNow < EndTime)
			{
				IsActive = true;

				if (!PVPEnabled)
				{
					PVPEnabled = true;

					PVPMessage[0] = PVPDays[i].value("PVPEnabledMessage", "");
					PVPMessage[1] = PVPDays[i].value("PVPDisabledMessage", "");
					ProtectCharacters = PVPDays[i].value("ProtectPlayersPVPOn", false);
					ProtectDinos = PVPDays[i].value("ProtectDinosPVPOn", false);
					ProtectStructures = PVPDays[i].value("ProtectStructuresPVPOn", false);
					ProtectExplosives = PVPDays[i].value("ProtectExplosivesPVPOn", false);

					if (ServerNotify)
					{
						ArkApi::GetApiUtils().SendChatMessageToAll(
							ServerName, ArkApi::Tools::Utf8Decode(PVPMessage[0]).c_str());
					}

					if (LogPvpSwitchAtConsole)
						Log::GetLog()->info("PVP Schedule({}) Enabled!", i + 1);
				}

				break;
			}
		}

		if (PVPEnabled && !IsActive)
		{
			if (ServerNotify)
			{
				ArkApi::GetApiUtils().SendChatMessageToAll(
					ServerName, ArkApi::Tools::Utf8Decode(PVPMessage[1]).c_str());
			}

			ProtectCharacters = PVPConfig["PVPScheduler"].value("ProtectPlayersPVPOff", true);
			ProtectDinos = PVPConfig["PVPScheduler"].value("ProtectDinosPVPOff", true);
			ProtectStructures = PVPConfig["PVPScheduler"].value("ProtectStructuresPVPOff", true);
			ProtectExplosives = PVPConfig["PVPScheduler"].value("ProtectExplosivesPVPOff", true);

			PVPEnabled = false;

			if (LogPvpSwitchAtConsole)
				Log::GetLog()->info("PVP Schedule Disabled!");
		}

		OneMinCounter = 0;
	}
}

long double LastTime;

FString GetBlueprint(UObjectBase* object)
{
	if (object != nullptr && object->ClassField() != nullptr)
	{
		FString path_name;
		object->ClassField()->GetDefaultObject(true)->GetFullName(&path_name, nullptr);

		if (int find_index = 0; path_name.FindChar(' ', find_index))
		{
			path_name = "Blueprint'" + path_name.Mid(find_index + 1,
				path_name.Len() - (find_index + (path_name.EndsWith(
					"_C", ESearchCase::
					CaseSensitive)
					? 3
					: 1))) + "'";
			return path_name.Replace(L"Default__", L"", ESearchCase::CaseSensitive);
		}
	}

	return FString("");
}

int  Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation, FPlacementData* OutPlacementData, bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement)
{


	if (bFinalPlacement && PC != nullptr)
	{
		if (!PVPEnabled && BlockC4PlacementInPVPOff) {
			
			const FString path_name = GetBlueprint(_this);
		if (path_name.ToString() == "Blueprint'/Game/PrimalEarth/Structures/C4Charge.C4Charge'")
		{
			ArkApi::GetApiUtils().SendNotification(static_cast<AShooterPlayerController*>(PC), FColorList::Green, 1.3f, 15.0f, nullptr, "C4 is Blocked in PVP OFF");

					return 0;
		}
		
		}
		
		
	}

	return APrimalStructure_IsAllowedToBuild_original(_this, PC, AtLocation, AtRotation, OutPlacementData, bDontAdjustForMaxRange, PlayerViewRotation, bFinalPlacement);
}


bool Hook_APrimalStructureExplosive_CanDetonateMe(APrimalStructureExplosive* _this, AShooterCharacter* Character, bool bUsingRemote)
{
	if (ProtectExplosives)
	{
		const auto& NowTime = ArkApi::GetApiUtils().GetWorld()->GetTimeSeconds();
		if (NowTime >= LastTime && Character)
		{
			AShooterPlayerController* aspc = ArkApi::GetApiUtils().FindControllerFromCharacter(Character);
			if (aspc)
			{
				LastTime = NowTime + 1;
				ArkApi::GetApiUtils().SendChatMessage(aspc, ServerName, ArkApi::Tools::Utf8Decode(PVPMessage[2]).c_str());
			}
		}
		return false;
	}
	return APrimalStructureExplosive_CanDetonateMe_original(_this, Character, bUsingRemote);
}

float Hook_APrimalCharacter_TakeDamage(APrimalCharacter* Victim, float Damage, FDamageEvent* DamageEvent,
	AController* Attacker, AActor* DamageCauser)
{
	return ProtectCharacters && Victim && Attacker && Victim->TargetingTeamField() >= 50000 && Attacker->TargetingTeamField() >= 50000
		? 0
		: APrimalCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
}

float Hook_APrimalDinoCharacter_TakeDamage(APrimalDinoCharacter* Victim, float Damage, FDamageEvent* DamageEvent,
	AController* Attacker, AActor* DamageCauser)
{

	if (ProtectDinos && Victim && Victim->TargetingTeamField() >= 50000)
	{
		if (Attacker && Attacker->TargetingTeamField() < 50000)
			return APrimalDinoCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
		return 0;
	}

	return APrimalDinoCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
}

float Hook_APrimalStructure_TakeDamage(APrimalStructure* Victim, float Damage, FDamageEvent* DamageEvent,
	AController* Attacker, AActor* DamageCauser)
{//bImmuneToAutoDemolish
	//bUseHarvestingComponent 	
	return ProtectStructures
		? 0
		: APrimalStructure_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
}
