#pragma once
DECLARE_HOOK(APrimalStructure_IsAllowedToBuild, int, APrimalStructure*, APlayerController*, FVector, FRotator, FPlacementData*, bool, FRotator, bool);
DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
//DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalTargetableActor*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);
void OnTimer();

void InitHooks()
{
	ArkApi::GetHooks().SetHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild, reinterpret_cast<LPVOID*>(&APrimalStructure_IsAllowedToBuild_original));
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalCharacter_TakeDamage_original));
	//ArkApi::GetHooks().SetHook("APrimalTargetableActor.TakeDamage", &Hook_APrimalStructure_TakeDamage, reinterpret_cast<LPVOID*>(&APrimalStructure_TakeDamage_original));
	ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, reinterpret_cast<LPVOID*>(&AShooterGameMode_Logout_original));
	if (SafeZoneNotifications) ArkApi::GetCommands().AddOnTimerCallback("0", &OnTimer);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild);
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	//ArkApi::GetHooks().DisableHook("APrimalTargetableActor.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
	if(SafeZoneNotifications) ArkApi::GetCommands().RemoveOnTimerCallback("0");
}

bool CheckSafeZone(const FVector& Position, APlayerController* PC, bool PVP, bool NoMessage)
{
	bool InSafeZone = false;
	std::vector<SafeZoneDistanceS>::iterator it = std::find_if(SafeZoneDistanceMap.begin(), SafeZoneDistanceMap.end(), [Position](SafeZoneDistanceS& SZ) -> bool { return SZ.IsInArea(Position); });
	if (it != SafeZoneDistanceMap.end())
	{
		InSafeZone = true;
		if (!NoMessage && PC != nullptr)
		{
			AShooterPlayerController* ASPC = static_cast<AShooterPlayerController*>(PC);
			if (ASPC && ASPC->GetPlayerCharacter())
			{
				if(!PVP) InSafeZone = !ASPC->GetPlayerCharacter()->bIsServerAdminField()();
				ArkApi::GetApiUtils().SendServerMessage(ASPC, FLinearColor(255, 0, 0), (PVP ? it->NoPvPMessage : it->NoBuildingMessage).c_str());
			}
		}
		return InSafeZone;
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
				if (CheckSafeZone(Player->DefaultActorLocationField()(), nullptr, false, true))
				{
					if (!SafeZoneItr->IsSafeArea())
					{
						ArkApi::GetApiUtils().SendServerMessage(Player, FLinearColor(0, 255, 0), SafeZoneEnterNotifcation.c_str());
						SafeZoneItr->SetSafeArea(true);
					}
				}
				else if (SafeZoneItr->IsSafeArea())
				{
					ArkApi::GetApiUtils().SendServerMessage(Player, FLinearColor(255, 0, 0), SafeZoneLeaveNotification.c_str());
					SafeZoneItr->SetSafeArea(false);
				}
			}
		}
		SecCounter = 0;
	}
}

int _cdecl Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation, FPlacementData * OutPlacementData, bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement)
{
	return ((bFinalPlacement && PC && PC->IsA(AShooterPlayerController::GetPrivateStaticClass()) && CheckSafeZone(AtLocation, PC, false, false)) ? 0 : APrimalStructure_IsAllowedToBuild_original(_this, PC, AtLocation, AtRotation, OutPlacementData, bDontAdjustForMaxRange, PlayerViewRotation, bFinalPlacement));
}

bool PrimalCharacterDamageCheckSafeZone(APrimalCharacter* Victim, AController* Attacker, AActor* DamageCauser)
{
	if (Victim && Victim->IsA(AShooterCharacter::GetPrivateStaticClass()))
	{
		AShooterCharacter* shooterCharacter = static_cast<AShooterCharacter*>(Victim);
		if (shooterCharacter && shooterCharacter->GetSpawnedForController() && CheckSafeZone(shooterCharacter->GetSpawnedForController()->DefaultActorLocationField()(), shooterCharacter->GetSpawnedForController(), true, false))
		{
			if (DestroyAggressiveDinos && Attacker && Attacker->IsLocalController() && DamageCauser && Attacker->IsA(AAIController::GetPrivateStaticClass()))	DamageCauser->Destroy(true, false);
			return true;
		}
	}
	return false;
}

float _cdecl Hook_APrimalCharacter_TakeDamage(APrimalCharacter* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return (PrimalCharacterDamageCheckSafeZone(_this, EventInstigator, DamageCauser) ? 0 : APrimalCharacter_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser));
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
/*
bool PrimalStructureDamageCheckSafeZone(APrimalTargetableActor* structure, AController* Attacker, AActor* DamageCauser)
{
	if (structure)
	{
		FVector Pos = structure->DefaultActorLocationField()();
		printf("%0.f %0.f %0.f\n", Pos.X, Pos.Y, Pos.Z);
	}
	return false;
}

float _cdecl Hook_APrimalStructure_TakeDamage(APrimalTargetableActor* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_this)
	{
		FVector Pos = _this->DefaultActorLocationField()();
		printf("%0.f %0.f %0.f\n", Pos.X, Pos.Y, Pos.Z);
	}
	return (PrimalStructureDamageCheckSafeZone(_this, EventInstigator, DamageCauser) ? 0 : APrimalStructure_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser));
}*/