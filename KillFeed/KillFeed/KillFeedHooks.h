#pragma once
DECLARE_HOOK(APrimalDinoCharacter_Die, bool, APrimalDinoCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(AShooterCharacter_Die, bool, AShooterCharacter*, float, FDamageEvent*, AController*, AActor*);

void InitHooks()
{
	if (DinoKillsEnabled) ArkApi::GetHooks().SetHook("APrimalDinoCharacter.Die", &Hook_APrimalDinoCharacter_Die, reinterpret_cast<LPVOID*>(&APrimalDinoCharacter_Die_original));
	if (PlayerKillsEnabled) ArkApi::GetHooks().SetHook("AShooterCharacter.Die", &Hook_AShooterCharacter_Die, reinterpret_cast<LPVOID*>(&AShooterCharacter_Die_original));
}

void RemoveHooks()
{
	if (DinoKillsEnabled) ArkApi::GetHooks().DisableHook("APrimalDinoCharacter.Die", &Hook_APrimalDinoCharacter_Die);
	if (PlayerKillsEnabled) ArkApi::GetHooks().DisableHook("AShooterCharacter.Die", &Hook_AShooterCharacter_Die);
}

void NotifyDeath(const FString& KillerName, const FString& KillerTribe, const FString& KillerDinoName, const FString& VictimName, const FString& VictimTribe, const FString& WeaponName, UTexture2D* WeaponIcon, float Distance, bool IsPlayer)
{
	const bool isKillerDino = !KillerDinoName.IsEmpty();
	switch (DisplayType)
	{
	case 0:
		if(isKillerDino) ArkApi::GetApiUtils().SendNotificationToAll(DisplayColour, DisplayScale, DisplayDelay, WeaponIcon, (IsPlayer ? Messages[2].c_str() : Messages[3].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *FString("(" + KillerDinoName + ")"), *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, trunc_decs(Distance, 2));
		else ArkApi::GetApiUtils().SendNotificationToAll(DisplayColour, DisplayScale, DisplayDelay, WeaponIcon, (IsPlayer ? Messages[0].c_str() : Messages[1].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, *WeaponName, trunc_decs(Distance, 2));
		break;
	case 1:
		ArkApi::GetApiUtils().SendNotificationToAll(DisplayColour, DisplayScale, DisplayDelay, WeaponIcon, (IsPlayer ? Messages[2].c_str() : Messages[3].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *(isKillerDino ? FString("(" + KillerDinoName + ")") : FString("")), *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, trunc_decs(Distance, 2));
		break;
	case 2:
		if (isKillerDino)ArkApi::GetApiUtils().SendNotificationToAll(DisplayColour, DisplayScale, DisplayDelay, WeaponIcon, (IsPlayer ? Messages[2].c_str() : Messages[3].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *FString("(" + KillerDinoName + ")"), *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, trunc_decs(Distance, 2));
		else ArkApi::GetApiUtils().SendNotificationToAll(DisplayColour, DisplayScale, DisplayDelay, WeaponIcon, (IsPlayer ? Messages[0].c_str() : Messages[1].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, *WeaponName, trunc_decs(Distance, 2));
		break;
	}
	if (DisplayInChatbox)
	{
		if(isKillerDino) ArkApi::GetApiUtils().SendServerMessageToAll(DisplayColour, (IsPlayer ? Messages[2].c_str() : Messages[3].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *FString("(" + KillerDinoName + ")"), *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, trunc_decs(Distance, 2));
		else ArkApi::GetApiUtils().SendServerMessageToAll(DisplayColour, (IsPlayer ? Messages[0].c_str() : Messages[1].c_str()), *(KillerTribe.IsEmpty() ? FString("") : FString("[" + KillerTribe + "]")), *KillerName, *(VictimTribe.IsEmpty() ? FString("") : FString("[" + VictimTribe + "]")), *VictimName, *WeaponName, trunc_decs(Distance, 2));
	}
}

bool _cdecl Hook_AShooterCharacter_Die(AShooterCharacter* _this, float KillingDamage, FDamageEvent* DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (_this && !Killer->IsLocalController() && Killer->IsA(AShooterPlayerController::StaticClass()))
	{
		AShooterPlayerController* KillerShooterController = static_cast<AShooterPlayerController*>(Killer);
		if (KillerShooterController && KillerShooterController->GetPlayerCharacter())
		{ 
			FString KillerDinoName, WeaponName = "Punch";
			UTexture2D* WeaponIcon = nullptr;

			if (KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()() && KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->AssociatedPrimalItemField()())
			{
				if (DisplayType != 2)
				{
					KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->GetHumanReadableName(&WeaponName);
					WeaponIcon = GetWeaponIcon(WeaponName);
				}

				KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->AssociatedPrimalItemField()()->GetItemName(&WeaponName, false, true, nullptr);
			}
			APrimalDinoCharacter* KillerDino = KillerShooterController->GetPlayerCharacter()->GetRidingDino();
			if (KillerDino != nullptr) KillerDino->DinoNameTagField()().ToString(&KillerDinoName);

			float Distance = (FVector::Distance((_this->RootComponentField()() ? _this->RootComponentField()()->RelativeLocationField()() : FVector(0, 0, 0)), KillerShooterController->DefaultActorLocationField()()) / 220);
			NotifyDeath(ArkApi::GetApiUtils().GetCharacterName(KillerShooterController), KillerShooterController->GetPlayerCharacter()->TribeNameField()(), KillerDinoName, _this->PlayerNameField()(), _this->TribeNameField()(), WeaponName, WeaponIcon, Distance, true);
		}
	}
	return AShooterCharacter_Die_original(_this, KillingDamage, DamageEvent, Killer, DamageCauser);
}

bool _cdecl Hook_APrimalDinoCharacter_Die(APrimalDinoCharacter* Dino, float KillingDamage, FDamageEvent* DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (Dino->TamingTeamIDField()() != 0 && !Killer->IsLocalController() && Killer->IsA(AShooterPlayerController::StaticClass()))
	{
		AShooterPlayerController* KillerShooterController = static_cast<AShooterPlayerController*>(Killer);
		if (KillerShooterController && KillerShooterController->GetPlayerCharacter())
		{
			FString DinoName, KillerDinoName, WeaponName = "Punch";
			UTexture2D* WeaponIcon = nullptr;

			if (Dino->TamedNameField()().Len() > 0) DinoName = Dino->TamedNameField()();
			else Dino->DinoNameTagField()().ToString(&DinoName);

			if (KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()() && KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->AssociatedPrimalItemField()())
			{
				if (DisplayType != 2)
				{
					KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->GetHumanReadableName(&WeaponName);
					WeaponIcon = GetWeaponIcon(WeaponName);
				}

				KillerShooterController->GetPlayerCharacter()->CurrentWeaponField()()->AssociatedPrimalItemField()()->GetItemName(&WeaponName, false, true, nullptr);
			}

			APrimalDinoCharacter* KillerDino = KillerShooterController->GetPlayerCharacter()->GetRidingDino();
			if (KillerDino != nullptr) KillerDino->DinoNameTagField()().ToString(&KillerDinoName);

			float Distance = (FVector::Distance((Dino->RootComponentField()() ? Dino->RootComponentField()()->RelativeLocationField()() : FVector(0, 0, 0)), KillerShooterController->DefaultActorLocationField()()) / 220);
			NotifyDeath(ArkApi::GetApiUtils().GetCharacterName(KillerShooterController), KillerShooterController->GetPlayerCharacter()->TribeNameField()(), KillerDinoName, DinoName, Dino->TribeNameField()(), WeaponName, WeaponIcon, Distance, false);
		}
	}
	return APrimalDinoCharacter_Die_original(Dino, KillingDamage, DamageEvent, Killer, DamageCauser);
}