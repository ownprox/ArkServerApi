#pragma once

DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalDinoCharacter_TakeDamage, float, APrimalDinoCharacter*, float, FDamageEvent*, AController*,
	AActor*);

void PVPCheckTimer();

inline void InitHooks()
{
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage,
	                           reinterpret_cast<LPVOID*>(&APrimalCharacter_TakeDamage_original));
	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage,
	                           reinterpret_cast<LPVOID*>(&APrimalStructure_TakeDamage_original));
	ArkApi::GetHooks().SetHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage,
	                           reinterpret_cast<LPVOID*>(&APrimalDinoCharacter_TakeDamage_original));
	ArkApi::GetCommands().AddOnTimerCallback("PVPCheckTimer", &PVPCheckTimer);
}

inline void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage);
	ArkApi::GetCommands().RemoveOnTimerCallback("PVPCheckTimer");
}

inline time_t rawtime;
inline tm timeinfo;
inline int OneMinCounter;
inline std::string PVPMessage[2];

inline void PVPCheckTimer()
{
	if (!ArkApi::GetApiUtils().GetShooterGameMode())
		return;

	if (OneMinCounter++ == 60)
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
					ProtectCharacters = PVPDays[i]["ProtectPlayers"];
					ProtectDinos = PVPDays[i]["ProtectDinos"];
					ProtectStructures = PVPDays[i]["ProtectStructures"];

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

			PVPEnabled = ProtectCharacters = ProtectDinos = ProtectStructures = false;

			if (LogPvpSwitchAtConsole)
				Log::GetLog()->info("PVP Schedule Disabled!");
		}

		OneMinCounter = 0;
	}
}

inline float Hook_APrimalCharacter_TakeDamage(APrimalCharacter* Victim, float Damage, FDamageEvent* DamageEvent,
                                              AController* Attacker, AActor* DamageCauser)
{
	return ProtectCharacters && Victim && Attacker && Attacker->TargetingTeamField() >= 50000
		       ? 0
		       : APrimalCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
}

inline float Hook_APrimalDinoCharacter_TakeDamage(APrimalDinoCharacter* Victim, float Damage, FDamageEvent* DamageEvent,
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

inline float Hook_APrimalStructure_TakeDamage(APrimalStructure* Victim, float Damage, FDamageEvent* DamageEvent,
                                              AController* Attacker, AActor* DamageCauser)
{
	return ProtectStructures
		       ? 0
		       : APrimalStructure_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser);
}
