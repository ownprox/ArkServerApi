#pragma once
DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalDinoCharacter_TakeDamage, float, APrimalDinoCharacter*, float, FDamageEvent*, AController*, AActor*);
void PVPCheckTimer();

void InitHooks()
{
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalCharacter_TakeDamage_original));
	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalStructure_TakeDamage_original));
	ArkApi::GetHooks().SetHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalDinoCharacter_TakeDamage_original));
	ArkApi::GetCommands().AddOnTimerCallback("PVPCheckTimer", &PVPCheckTimer);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalDinoCharacter.TakeDamage", &Hook_APrimalDinoCharacter_TakeDamage);
	ArkApi::GetCommands().RemoveOnTimerCallback("PVPCheckTimer");
}

time_t rawtime;
tm timeinfo;
int OneMinCounter = 0;
std::string PVPMessage[2];

void PVPCheckTimer()
{	
	if (!ArkApi::GetApiUtils().GetShooterGameMode()) return;
	if (OneMinCounter++ == 60)
	{
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		if (!PVPEnabled)
		{
			for (int i = 0; i < PVPDays.size(); i++)
			{
				if (timeinfo.tm_wday == PVPDays[i]["StartDay"]  && timeinfo.tm_hour >= PVPDays[i]["StartHour"] && PVPDays[i]["EndHour"] > timeinfo.tm_hour)
				{
					PVPMessage[0] = PVPDays[i]["PVPEnabledMessage"];
					PVPMessage[1] = PVPDays[i]["PVPDisabledMessage"];
					ProtectCharacters = PVPDays[i]["ProtectPlayers"];
					ProtectDinos = PVPDays[i]["ProtectDinos"];
					ProtectStructures = PVPDays[i]["ProtectStructures"];
					if (ServerNotify) ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, ArkApi::Tools::Utf8Decode(PVPMessage[0]).c_str());
					EndHour = PVPDays[i]["EndHour"];
					EndDay = PVPDays[i]["EndDay"];
					PVPEnabled = true;
					EnabledIndex = (i + 1);
					if (LogPvpSwitchAtConsole) Log::GetLog()->info("PVP Schedule({}) Enabled!.", EnabledIndex);
					break;
				}
			}
		}
		else if (timeinfo.tm_wday == EndDay && timeinfo.tm_hour == EndHour)
		{
			if (ServerNotify) ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, ArkApi::Tools::Utf8Decode(PVPMessage[1]).c_str());
			PVPEnabled = ProtectCharacters = ProtectDinos = ProtectStructures = false;
			if (LogPvpSwitchAtConsole) Log::GetLog()->info("PVP Schedule({}) Disabled!.", EnabledIndex);
		}
		OneMinCounter = 0;
	}
}

float _cdecl Hook_APrimalCharacter_TakeDamage(APrimalCharacter* Victim, float Damage, FDamageEvent* DamageEvent, AController* Attacker, AActor* DamageCauser)
{
	return ((ProtectCharacters && !PVPEnabled && Victim && Attacker && (!Attacker->IsLocalController() || Attacker->TargetingTeamField()() != 0)) ? 0 : APrimalCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser));
}

float _cdecl Hook_APrimalDinoCharacter_TakeDamage(APrimalDinoCharacter* Victim, float Damage, FDamageEvent* DamageEvent, AController* Attacker, AActor* DamageCauser)
{
	return ((ProtectDinos && !PVPEnabled && Victim && Victim->TargetingTeamField()() != 0 && Attacker && (!Attacker->IsLocalController() || Attacker->TargetingTeamField()() != 0)) ? 0 : APrimalDinoCharacter_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser));
}

float _cdecl Hook_APrimalStructure_TakeDamage(APrimalStructure* Victim, float Damage, FDamageEvent* DamageEvent, AController* Attacker, AActor* DamageCauser)
{	
	return ((ProtectStructures && !PVPEnabled && Victim && Attacker && (!Attacker->IsLocalController() || Attacker->TargetingTeamField()() != 0)) ? 0 : APrimalStructure_TakeDamage_original(Victim, Damage, DamageEvent, Attacker, DamageCauser));
}