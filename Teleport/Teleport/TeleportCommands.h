#pragma once
void TeleportRequest(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr)
	{
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (Parsed.Num() < 2)
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tpr <PlayerName>");
			return;
		}
		FString PlayerName = "";
		for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
		TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
		if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
		{
			if (!Players[0]->GetPlayerCharacter() || Players[0]->GetPlayerCharacter()->IsDead())
			{
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "%s Is Dead.", ArkApi::GetApiUtils().GetCharacterName(Players[0]).ToString().c_str());
				return;
			}
			PlayerS* o;
			if ((o = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]))) != NULL)
			{
				o->LastTPRID = p->SteamID;
				ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(0, 255, 0), "%s Would Like to teleport to you, /tpa", ArkApi::GetApiUtils().GetCharacterName(player).ToString().c_str());
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "Teleport Request sent to: %s", ArkApi::GetApiUtils().GetCharacterName(Players[0]).ToString().c_str());
			}
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: %s", PlayerName.ToString().c_str());
	}
}

void TeleportAccept(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr && p->LastTPRID != -1)
	{
		AShooterPlayerController* other = nullptr;
		if ((other = ArkApi::GetApiUtils().FindPlayerFromSteamId(p->LastTPRID)) != nullptr && other->PlayerStateField()())
		{
			if (!other->GetPlayerCharacter() || other->GetPlayerCharacter()->IsDead())
			{
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "%s Is Dead.", ArkApi::GetApiUtils().GetCharacterName(other).ToString().c_str());
				return;
			}
			p->LastTPRID = -1;
			ArkApi::GetApiUtils().SendServerMessage(other, FLinearColor(0, 255, 0), "You will Teleport to %s in %d Seconds.", ArkApi::GetApiUtils().GetCharacterName(player).ToString().c_str(), TeleportDelay);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "%s Is Teleporting to you in %d Seconds.", ArkApi::GetApiUtils().GetCharacterName(other).ToString().c_str(), TeleportDelay);
			Timer(TeleportDelay, true, ArkApi::GetApiUtils().TeleportToPlayer, other, player, AllowDinoTeleport, MaxDistance);
		}
	}
}

void AdminTP(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tp <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())	ArkApi::GetApiUtils().TeleportToPlayer(player, Players[0], false);
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: %s", Parsed[1].ToString().c_str());
}

void AdminTPM(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tp <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()()) ArkApi::GetApiUtils().TeleportToPlayer(Players[0], player, false);
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: %s", Parsed[1].ToString().c_str());
}


void AdminTeleTamedToMe(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	int TamedDist = 500;
	if (Parsed.IsValidIndex(1))
	{
		try
		{
			TamedDist = std::stoi(Parsed[1].ToString().c_str());
		}
		catch (...) {}
	}
	if (TamedDist <= 0 || TamedDist > 8000) TamedDist = 1000;
	TArray<TWeakObjectPtr<APrimalDinoCharacter>> Dinos;
	player->GetTamedDinosNearBy(&Dinos, TamedDist);
	APrimalDinoCharacter* Dino;
	for (int i = 0; i < Dinos.Num(); i++)
		if ((Dino = Dinos[i].Get()) != nullptr)
		{
			Dino->TeleportTo(&ArkApi::GetApiUtils().GetPosition(player), &FRotator(0, 0, 0), true, false);
			FString DinoName = Dino->TamedNameField()();
			if (DinoName.Len() == 0) Dino->DinoNameTagField()().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "%s Teleported to you.", DinoName.ToString().c_str());
		}
}

void AdminTeleTamedToPlayer(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	int TamedDist = 500;
	if (!Parsed.IsValidIndex(2))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tpdp <Dist> <PlayerName>");
		return;
	}
	else
	{
		try
		{
			TamedDist = std::stoi(Parsed[1].ToString().c_str());
		}
		catch (...) {}
	}
	if (TamedDist == 0 || TamedDist > 8000) TamedDist = 1000;

	FString PlayerName = "";
	for (int i = 2; i < Parsed.Num(); i++)	PlayerName += (i == 2 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
	{
		TArray<TWeakObjectPtr<APrimalDinoCharacter>> Dinos;
		Players[0]->GetTamedDinosNearBy(&Dinos, TamedDist);
		APrimalDinoCharacter* Dino;
		for (int i = 0; i < Dinos.Num(); i++) if ((Dino = Dinos[i].Get()) != nullptr)
		{
			Dino->TeleportTo(&ArkApi::GetApiUtils().GetPosition(player), &FRotator(0, 0, 0), true, false);
			FString DinoName = Dino->TamedNameField()();
			if (DinoName.Len() == 0) Dino->DinoNameTagField()().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "%s Teleported to you.", DinoName.ToString().c_str());
		}
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: %s", Parsed[1].ToString().c_str());
}

void AdminTPCoord(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tpp <x> <y> <z>");
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

void AdminPos(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	FVector Pos = player->DefaultActorLocationField()();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "%.0f %.0f %.0f", Pos.X, Pos.Y, Pos.Z);
}

void AdminTPTarget(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	AActor* Actor = player->GetPlayerCharacter()->GetAimedActor(ECollisionChannel::ECC_GameTraceChannel1, nullptr, 0.0f, 0.0f, nullptr, nullptr, false, false);
	if (!Actor) return;
	if (Actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
	{
		APrimalDinoCharacter* Dino = static_cast<APrimalDinoCharacter*>(Actor);
		if (Dino && !Dino->IsDead())
		{
			Dino->TeleportTo(&ArkApi::GetApiUtils().GetPosition(player), &FRotator(0, 0, 0), true, false);
			FString DinoName = Dino->TamedNameField()();
			if (DinoName.Len() == 0) Dino->DinoNameTagField()().ToString(&DinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "%s Teleported to you.", DinoName.ToString().c_str());
		}
	}
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/tpr", &TeleportRequest);
	ArkApi::GetCommands().AddChatCommand("/tpa", &TeleportAccept);
	ArkApi::GetCommands().AddChatCommand("/tp", &AdminTP);
	ArkApi::GetCommands().AddChatCommand("/tpm", &AdminTPM);
	ArkApi::GetCommands().AddChatCommand("/tpd", &AdminTeleTamedToMe);
	ArkApi::GetCommands().AddChatCommand("/tpdp", &AdminTeleTamedToPlayer);
	ArkApi::GetCommands().AddChatCommand("/tpp", &AdminTPCoord);
	ArkApi::GetCommands().AddChatCommand("/tt", &AdminTPTarget);
	ArkApi::GetCommands().AddChatCommand("/pos", &AdminPos);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/tpr");
	ArkApi::GetCommands().RemoveChatCommand("/tpa");
	ArkApi::GetCommands().RemoveChatCommand("/tp");
	ArkApi::GetCommands().RemoveChatCommand("/tpm");
	ArkApi::GetCommands().RemoveChatCommand("/tpd");
	ArkApi::GetCommands().RemoveChatCommand("/tpdp");
	ArkApi::GetCommands().RemoveChatCommand("/tpp");
	ArkApi::GetCommands().RemoveChatCommand("/tt");
	ArkApi::GetCommands().RemoveChatCommand("/pos");
}