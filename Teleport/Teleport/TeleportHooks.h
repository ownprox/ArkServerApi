#pragma once
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);

void InitHooks()
{
	if(TPREnabled) ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, reinterpret_cast<LPVOID*>(&AShooterGameMode_Logout_original));
}

void RemoveHooks()
{
	if (TPREnabled) ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
}

void _cdecl Hook_AShooterGameMode_Logout(AShooterGameMode* _this, AController* Exiting)
{
	if (Exiting) RemovePlayer(ArkApi::GetApiUtils().GetSteamIdFromController(Exiting));
	AShooterGameMode_Logout_original(_this, Exiting);
}