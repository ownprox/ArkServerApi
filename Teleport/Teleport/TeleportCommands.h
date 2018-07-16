#pragma once
void TeleportRequest(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr)
	{
		if (p->IsTping)
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[18].c_str());
			return;
		}
		if (TPRCoolDownEnabled && IsOnCoolDown(player))	return;
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (Parsed.Num() < 2)
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[0].c_str());
			return;
		}
		FString PlayerName = "";
		for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
		TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
		if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField())
		{
			if (!Players[0]->GetPlayerCharacter() || Players[0]->GetPlayerCharacter()->IsDead())
			{
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[1].c_str(), *ArkApi::GetApiUtils().GetCharacterName(Players[0]));
				return;
			}

			if (AllowDinoTeleport && ArkApi::GetApiUtils().IsRidingDino(Players[0]))
			{
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[15].c_str(), *ArkApi::GetApiUtils().GetCharacterName(Players[0]));
				ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(1, 0, 0), Messages[16].c_str());
				return;
			}

			if (AllowDinoTeleport && ArkApi::GetApiUtils().IsRidingDino(player))
			{
				ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(1, 0, 0), Messages[15].c_str(), *ArkApi::GetApiUtils().GetCharacterName(player));
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[16].c_str());
				return;
			}

			if (MaxDistance != -1 && FVector::Distance(ArkApi::GetApiUtils().GetPosition(player), ArkApi::GetApiUtils().GetPosition(Players[0])) > MaxDistance)
			{
				ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(1, 0, 0), Messages[17].c_str());
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[17].c_str());
				return;
			}

			PlayerS* o;
			if ((o = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]))) != NULL)
			{
				if (o->IsTping)
				{
					ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[19].c_str(), *ArkApi::GetApiUtils().GetCharacterName(Players[0]));
					return;
				}
				o->LastTPRID = p->SteamID;
				ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(0, 1, 0), Messages[2].c_str(), *ArkApi::GetApiUtils().GetCharacterName(player));
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[3].c_str(), *ArkApi::GetApiUtils().GetCharacterName(Players[0]));
			}
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[4].c_str(), *PlayerName);
	}
}

void TeleportAccept(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr && p->LastTPRID != -1)
	{
		AShooterPlayerController* other = nullptr;
		if ((other = ArkApi::GetApiUtils().FindPlayerFromSteamId(p->LastTPRID)) != nullptr && other->PlayerStateField())
		{
			if (!other->GetPlayerCharacter() || other->GetPlayerCharacter()->IsDead())
			{
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[1].c_str(), *ArkApi::GetApiUtils().GetCharacterName(other));
				return;
			}
			p->LastTPRID = -1;
			PlayerS* op;
			if ((op = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(other))) != NULL) op->IsTping = true;
			p->IsTping = true;
			ArkApi::GetApiUtils().SendServerMessage(other, FLinearColor(0, 1, 0), Messages[5].c_str(), *ArkApi::GetApiUtils().GetCharacterName(player), TeleportDelay);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[6].c_str(), *ArkApi::GetApiUtils().GetCharacterName(other), TeleportDelay);
			Timer((TeleportDelay * 1000), true, TeleportToPlayer, other, op, player, p, AllowDinoTeleport, MaxDistance);
		}
	}
}

void AdminTP(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[8].c_str());
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* OtherPlayer = FindPlayerFromCharacterName(PlayerName);
	if (OtherPlayer != nullptr && OtherPlayer->PlayerStateField()) ArkApi::GetApiUtils().TeleportToPlayer(player, OtherPlayer, false);
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[4].c_str(), *Parsed[1]);
}

void AdminTPM(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[8].c_str());
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* OtherPlayer = FindPlayerFromCharacterName(PlayerName);
	if (OtherPlayer != nullptr && OtherPlayer->PlayerStateField()) ArkApi::GetApiUtils().TeleportToPlayer(OtherPlayer, player, false);
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[4].c_str(), *Parsed[1]);
}


void AdminTeleTamedToMe(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	float TamedDist = 500;
	if (Parsed.IsValidIndex(1))
	{
		try
		{
			TamedDist = std::stof(Parsed[1].ToString().c_str());
		}
		catch (...) {}
	}
	if (TamedDist <= 0 || TamedDist > 8000) TamedDist = 8000;
	TArray<TWeakObjectPtr<APrimalDinoCharacter>> Dinos;
	player->GetTamedDinosNearBy(&Dinos, TamedDist);
	APrimalDinoCharacter* Dino;
	FVector Pos;
	FRotator Rot{ 0, 0, 0 };
	for (int i = 0; i < Dinos.Num(); i++)
		if ((Dino = Dinos[i].Get()) != nullptr)
		{
			Pos = ArkApi::GetApiUtils().GetPosition(player);
			Dino->TeleportTo(&Pos, &Rot, true, false);
			FString DinoName = Dino->TamedNameField();
			if (DinoName.Len() == 0) Dino->DinoNameTagField().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[9].c_str(), *DinoName);
		}
}

void AdminTeleTamedToPlayer(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	float TamedDist = 500;
	if (!Parsed.IsValidIndex(2))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[10].c_str());
		return;
	}
	else
	{
		try
		{
			TamedDist = std::stof(Parsed[1].ToString().c_str());
		}
		catch (...) {}
	}
	if (TamedDist == 0 || TamedDist > 8000) TamedDist = 1000;

	FString PlayerName = "";
	for (int i = 2; i < Parsed.Num(); i++)	PlayerName += (i == 2 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* OtherPlayer = FindPlayerFromCharacterName(PlayerName);
	if (OtherPlayer != nullptr && OtherPlayer->PlayerStateField())
	{
		TArray<TWeakObjectPtr<APrimalDinoCharacter>> Dinos;
		OtherPlayer->GetTamedDinosNearBy(&Dinos, TamedDist);
		APrimalDinoCharacter* Dino;
		FVector Pos;
		FRotator Rot{ 0, 0, 0 };
		for (int i = 0; i < Dinos.Num(); i++) if ((Dino = Dinos[i].Get()) != nullptr)
		{
			Pos = ArkApi::GetApiUtils().GetPosition(player);
			Dino->TeleportTo(&Pos, &Rot, true, false);
			FString DinoName = Dino->TamedNameField();
			if (DinoName.Len() == 0) Dino->DinoNameTagField().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[9].c_str(), *DinoName);
		}
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[4].c_str(), *Parsed[1]);
}

void AdminTPCoord(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), Messages[11].c_str());
		return;
	}
	float X = 0, Y = 0, Z = 0;
	try
	{
		X = std::stof(Parsed[1].ToString().c_str());
		Y = std::stof(Parsed[2].ToString().c_str());
		Z = std::stof(Parsed[3].ToString().c_str());
	}
	catch (...) { return; }
	player->SetPlayerPos(X, Y, Z);
}

void AdminPos(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	FVector Pos = player->DefaultActorLocationField();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[12].c_str(), Pos.X, Pos.Y, Pos.Z);
	Log::GetLog()->warn(ArkApi::Tools::ConvertToAnsiStr(Messages[12]).c_str(), Pos.X, Pos.Y, Pos.Z);
}

void AdminTPTarget(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	AActor* Actor = player->GetPlayerCharacter()->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel1, nullptr, 0.0f, 0.0f, nullptr, nullptr, false, false);
	if (!Actor) return;
	if (Actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
	{
		APrimalDinoCharacter* Dino = static_cast<APrimalDinoCharacter*>(Actor);
		if (Dino && !Dino->IsDead())
		{
			FVector Pos = ArkApi::GetApiUtils().GetPosition(player);
			FRotator Rot{ 0, 0, 0 };
			Dino->TeleportTo(&Pos, &Rot, true, false);
			FString DinoName = Dino->TamedNameField();
			if (DinoName.Len() == 0) Dino->DinoNameTagField().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), Messages[9].c_str(), *DinoName);
		}
	}
}

void ReloadConfig(APlayerController* Player_Controller, FString* message, bool write_to_log)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(Player_Controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), L"Teleport Config Reloaded!");
}

void InitCommands()
{
	if (TPREnabled)
	{
		ArkApi::GetCommands().AddChatCommand(TPR, &TeleportRequest);
		ArkApi::GetCommands().AddChatCommand(TPA, &TeleportAccept);
	}
	ArkApi::GetCommands().AddConsoleCommand(TP, &AdminTP);
	ArkApi::GetCommands().AddConsoleCommand(TPM, &AdminTPM);
	ArkApi::GetCommands().AddConsoleCommand(TPD, &AdminTeleTamedToMe);
	ArkApi::GetCommands().AddConsoleCommand(TPDP, &AdminTeleTamedToPlayer);
	ArkApi::GetCommands().AddConsoleCommand(TPP, &AdminTPCoord);
	ArkApi::GetCommands().AddConsoleCommand(TT, &AdminTPTarget);
	ArkApi::GetCommands().AddConsoleCommand(POS, &AdminPos);
	ArkApi::GetCommands().AddConsoleCommand("treload", &ReloadConfig);
}

void RemoveCommands()
{
	if (TPREnabled)
	{
		ArkApi::GetCommands().RemoveChatCommand(TPR);
		ArkApi::GetCommands().RemoveChatCommand(TPA);
	}
	ArkApi::GetCommands().RemoveConsoleCommand(TP);
	ArkApi::GetCommands().RemoveConsoleCommand(TPM);
	ArkApi::GetCommands().RemoveConsoleCommand(TPD);
	ArkApi::GetCommands().RemoveConsoleCommand(TPDP);
	ArkApi::GetCommands().RemoveConsoleCommand(TPP);
	ArkApi::GetCommands().RemoveConsoleCommand(TT);
	ArkApi::GetCommands().RemoveConsoleCommand(POS);
	ArkApi::GetCommands().RemoveConsoleCommand("treload");
}