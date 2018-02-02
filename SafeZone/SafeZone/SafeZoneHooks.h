#pragma once
DECLARE_HOOK(APrimalStructure_IsAllowedToBuild, int, APrimalStructure*, APlayerController*, FVector, FRotator, FPlacementData*, bool, FRotator, bool);
DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);
void OnTimer();

void InitHooks()
{
	ArkApi::GetHooks().SetHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild, reinterpret_cast<LPVOID*>(&APrimalStructure_IsAllowedToBuild_original));
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalCharacter_TakeDamage_original));
	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalStructure_TakeDamage_original));
	ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, reinterpret_cast<LPVOID*>(&AShooterGameMode_Logout_original));
	if (SafeZoneNotifications) ArkApi::GetCommands().AddOnTimerCallback("OnSafeZoneTimer", &OnTimer);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild);
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
	if(SafeZoneNotifications) ArkApi::GetCommands().RemoveOnTimerCallback("OnSafeZoneTimer");
}

bool CheckSafeZone(const FVector& Position, AShooterPlayerController* ASPC, const int Type)
{
	if (AdminIgnoreChecks && ASPC && ASPC->GetPlayerCharacter() && ASPC->GetPlayerCharacter()->bIsServerAdminField()()) return false;
	std::vector<SafeZoneDistanceS>::iterator it = std::find_if(SafeZoneDistanceMap.begin(), SafeZoneDistanceMap.end(), [Position](SafeZoneDistanceS& SZ) -> bool { return SZ.IsInArea(Position); });
	if (it != SafeZoneDistanceMap.end())
	{
		bool IsProtected = true;
		switch (Type)
		{
		case 0:
			IsProtected = it->isBuilding;
			break;
		case 1:
			IsProtected = it->isPVP;
			break;
		case 2:
			IsProtected = it->PreventStructureDamage;
			break;
		}
		if (Type != -1 && ASPC != nullptr && it->Messages[Type].Len() != 0)ArkApi::GetApiUtils().SendChatMessage(ASPC, ServerName, *it->Messages[Type]);
		return IsProtected;
	}
	return false;
}

int SecCounter = 0;
std::vector<SafeZonePlayerData>::iterator SafeZoneItr;
void OnTimer()
{
	if (SecCounter++ == NotificationCheckSeconds)
	{
		if (!ArkApi::GetApiUtils().GetWorld())
		{
			SecCounter = 0;
			return;
		}
		const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField()();
		for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
		{
			AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
			if (Player && Player->GetPlayerCharacter() && !Player->GetPlayerCharacter()->IsDead() && Player->LinkedPlayerIDField()() != 0 && (SafeZoneItr = FindPlayer(Player->LinkedPlayerIDField()())) != SafeZonePlayerDataMap.end())
			{
				const FVector Position = Player->DefaultActorLocationField()();
				std::vector<SafeZoneDistanceS>::iterator it = std::find_if(SafeZoneDistanceMap.begin(), SafeZoneDistanceMap.end(), [Position](SafeZoneDistanceS& SZ) -> bool { return SZ.IsInArea(Position); });
				if (it != SafeZoneDistanceMap.end())
				{
					if (!SafeZoneItr->IsSafeArea())
					{

						ArkApi::GetApiUtils().SendChatMessage(Player, ServerName, *it->Messages[3], *it->Name);
						SafeZoneItr->SetSafeArea(true, it->Name, it->Messages[4]);
					}
				}
				else if (SafeZoneItr->IsSafeArea())
				{
					if(SafeZoneItr->LeaveMessage.Len() != 0) ArkApi::GetApiUtils().SendChatMessage(Player, ServerName, *SafeZoneItr->LeaveMessage, *SafeZoneItr->Name);
					SafeZoneItr->SetSafeArea(false);
				}
			}
		}
		SecCounter = 0;
	}
}

bool APrimalStructureIsAllowedBuildCheckSafeZone(APlayerController* PC, FVector AtLocation)
{
	if (PC && PC->IsA(AShooterPlayerController::GetPrivateStaticClass()))
	{
		AShooterPlayerController* ASPC = static_cast<AShooterPlayerController*>(PC);
		if (ASPC && CheckSafeZone(AtLocation, ASPC, 0))  return true;
	}
	return false;
}

int _cdecl Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation, FPlacementData * OutPlacementData, bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement)
{
	return ((bFinalPlacement && APrimalStructureIsAllowedBuildCheckSafeZone(PC, AtLocation)) ? 0 : APrimalStructure_IsAllowedToBuild_original(_this, PC, AtLocation, AtRotation, OutPlacementData, bDontAdjustForMaxRange, PlayerViewRotation, bFinalPlacement));
}

long long GetPlayerID(APrimalCharacter* _this)
{
	AShooterCharacter* shooterCharacter = static_cast<AShooterCharacter*>(_this);
	return (shooterCharacter && shooterCharacter->GetPlayerData()) ? shooterCharacter->GetPlayerData()->MyDataField()()->PlayerDataIDField()() : -1;
}

bool PrimalCharacterDamageCheckSafeZone(APrimalCharacter* Victim, AController* Attacker, AActor* DamageCauser)
{
	if (Victim && Victim->IsA(AShooterCharacter::GetPrivateStaticClass()))
	{
		if (!Attacker->IsLocalController() && Attacker->IsA(AShooterPlayerController::StaticClass()))
		{
			AShooterPlayerController* AttackerShooterController = static_cast<AShooterPlayerController*>(Attacker);
			if (AttackerShooterController && AttackerShooterController->LinkedPlayerIDField()() != GetPlayerID(Victim) && CheckSafeZone(Victim->RootComponentField()()->RelativeLocationField()(), AttackerShooterController, 1)) return true;
		}
		else
		{
			if (DestroyAggressiveDinos && CheckSafeZone(Victim->RootComponentField()()->RelativeLocationField()(), nullptr, 1) && Attacker && Attacker->IsLocalController() && DamageCauser) DamageCauser->Destroy(true, false);
			return true;
		}
	}
	return false;
}

float _cdecl Hook_APrimalCharacter_TakeDamage(APrimalCharacter* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return (PrimalCharacterDamageCheckSafeZone(_this, EventInstigator, DamageCauser) ? 0 : APrimalCharacter_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser));
}

bool PrimalStructureDamageCheckSafeZone(APrimalStructure* structure, AController* Attacker, AActor* DamageCauser)
{
	if (structure)
	{
		FVector Pos = structure->RootComponentField()()->RelativeLocationField()();
		if(!Attacker->IsLocalController() && Attacker->IsA(AShooterPlayerController::StaticClass()))
		{
			AShooterPlayerController* AttackerShooterController = static_cast<AShooterPlayerController*>(Attacker);
			if (AttackerShooterController && CheckSafeZone(structure->RootComponentField()()->RelativeLocationField()(), AttackerShooterController, 2)) return true;
		}
		else
		{
			if (DestroyAggressiveDinos&& CheckSafeZone(structure->RootComponentField()()->RelativeLocationField()(), nullptr, 2) && Attacker && Attacker->IsLocalController() && DamageCauser) DamageCauser->Destroy(true, false);
			return true;
		}
	}
	return false;
}

float _cdecl Hook_APrimalStructure_TakeDamage(APrimalStructure* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return (PrimalStructureDamageCheckSafeZone(_this, EventInstigator, DamageCauser) ? 0 : APrimalStructure_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser));
}

void _cdecl Hook_AShooterGameMode_Logout(AShooterGameMode* _this, AController* Exiting)
{
	if (Exiting && Exiting->IsA(AShooterPlayerController::GetPrivateStaticClass()))
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(Exiting);
		if (Player && Player->LinkedPlayerIDField()() != 0) RemovePlayer(Player->LinkedPlayerIDField()());
	}
	AShooterGameMode_Logout_original(_this, Exiting);
}