#pragma once
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);
void CleanUpLoggedOutCoolDownPlayers();

void InitHooks()
{
	if(TPREnabled) ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, reinterpret_cast<LPVOID*>(&AShooterGameMode_Logout_original));
	ArkApi::GetCommands().AddOnTimerCallback("CleanUpLoggedOutCoolDownPlayers", &CleanUpLoggedOutCoolDownPlayers);
}

void RemoveHooks()
{
	if (TPREnabled) ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
	ArkApi::GetCommands().RemoveOnTimerCallback("CleanUpLoggedOutCoolDownPlayers");
}

void _cdecl Hook_AShooterGameMode_Logout(AShooterGameMode* _this, AController* Exiting)
{
	if (Exiting) RemovePlayer(ArkApi::GetApiUtils().GetSteamIdFromController(Exiting));
	AShooterGameMode_Logout_original(_this, Exiting);
}


std::vector<CoolDownData>::iterator CoolDownItr;
int SecCounter = 0;
DWORD64 CoolDownNowTime;
void CleanUpLoggedOutCoolDownPlayers()
{
	if (SecCounter++ == 7200)
	{
		if (!ArkApi::GetApiUtils().GetWorld())
		{
			SecCounter = 0;
			return;
		}
		CoolDownNowTime = timeGetTime();
		for (CoolDownItr = CoolData.begin(); CoolDownItr != CoolData.end(); CoolDownItr++) if (CoolDownNowTime > CoolDownItr->CoolDown) CoolDownItr = CoolData.erase(CoolDownItr);
		SecCounter = 0;
	}
}
