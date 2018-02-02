#pragma once

inline float trunc_decs(float f, int decs)
{
	float i1 = floor(f);
	float rmnd = f - i1;
	float i2 = static_cast<float>(rmnd * pow(10, decs));
	float f1 = static_cast<float>(i2 / pow(10, decs));
	return i1 + f1;
}

inline FVector2D GetMapCoordsFromLocation(const FVector& Pos)
{
	FString Map;
	ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
	int Divider = Map.Equals("Ragnarok") ? 13100 : 8000;
	return FVector2D(Pos.X > -1 ? trunc_decs(50 + Pos.X / Divider, 1) : trunc_decs(50 + Pos.X / Divider, 1),
	                 Pos.Y > -1 ? trunc_decs(50 + Pos.Y / Divider, 1) : trunc_decs(50 + Pos.Y / Divider, 1));
}

/**
 * \brief Finds all dinos owned by team
 */
inline TArray<APrimalDinoCharacter*> FindDinos(const int team, const FString& DinoName,
                                               AShooterPlayerController* player = nullptr)
{
	TArray<AActor*> AllDinos;
	UGameplayStatics::GetAllActorsOfClass(reinterpret_cast<UObject*>(ArkApi::GetApiUtils().GetWorld()), APrimalDinoCharacter::GetPrivateStaticClass(), &AllDinos);

	TArray<APrimalDinoCharacter*> FoundDinos;
	FVector PlayerPos = ArkApi::IApiUtils::GetPosition(player);
	APrimalDinoCharacter* Dino;
	FString DinoTag;

	for (AActor* DinoActor : AllDinos)
	{
		if (!DinoActor || DinoActor->TargetingTeamField()() != team) continue;
		Dino = static_cast<APrimalDinoCharacter*>(DinoActor);
		if (MaxDistance != -1 && FVector::Distance(PlayerPos, Dino->RootComponentField()()->RelativeLocationField()()) < MaxDistance) continue;
		DinoTag = Dino->TamedNameField()();
		if (DinoTag == DinoName) FoundDinos.Add(Dino);
		else
		{
			Dino->DinoNameTagField()().ToString(&DinoTag);
			if (DinoTag == DinoName) FoundDinos.Add(Dino);
		}
	}

	return FoundDinos;
}

inline void PlayerTrackDino(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter())
		return;

	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", Messages[1]);
		return;
	}

	FString DinoName;
	for (int i = 1; i < Parsed.Num(); i++)
		DinoName += i == 1 ? Parsed[i] : FString(" ") + Parsed[i];

	TArray<APrimalDinoCharacter*> FoundDinos = FindDinos(player->TargetingTeamField()(), DinoName, player);
	if (FoundDinos.Num() < 1)
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *DinoName);
		return;
	}

	FString Text;

	int i = 0;

	for (APrimalDinoCharacter* Dino : FoundDinos)
	{
		const FVector2D MapCoords = GetMapCoordsFromLocation(Dino->RootComponentField()()->RelativeLocationField()());

		FString CurrentDinoName = Dino->TamedNameField()();
		if (CurrentDinoName.IsEmpty()) Dino->DinoNameTagField()().ToString(&CurrentDinoName);

		Text += (i++ > 0 ? L"\n" : L"") + FString::Format(Messages[3].c_str(), *CurrentDinoName, MapCoords.X, MapCoords.Y);
	}

	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", *Text);
}

inline void AdminPlayerTrackDino(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter())
		return;

	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", Messages[6]);
		return;
	}

	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(2))
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", Messages[7]);
		return;
	}

	int TeamId;

	try
	{
		TeamId = std::stoi(*Parsed[1]);
	}
	catch (const std::exception&)
	{
		return;
	}

	FString DinoName;
	for (int i = 2; i < Parsed.Num(); i++)
		DinoName += i == 2 ? Parsed[i] : FString(" ") + Parsed[i];

	TArray<APrimalDinoCharacter*> FoundDinos = FindDinos(TeamId, DinoName);
	if (FoundDinos.Num() < 1)
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *DinoName);
		return;
	}

	FString Text;

	int i = 0;

	for (APrimalDinoCharacter* Dino : FoundDinos)
	{
		const FVector2D MapCoords = GetMapCoordsFromLocation(Dino->RootComponentField()()->RelativeLocationField()());

		FString CurrentDinoName = Dino->TamedNameField()();
		if (CurrentDinoName.IsEmpty()) Dino->DinoNameTagField()().ToString(&CurrentDinoName);
		Text += (i++ > 0 ? L"\n" : L"") + FString::Format(Messages[3].c_str(), *CurrentDinoName, MapCoords.X, MapCoords.Y);
	}

	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", *Text);
}

inline void AdminGetPlayerTeamID(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", Messages[6]);
		return;
	}

	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}", Messages[9]);
		return;
	}

	FString PlayerName;
	for (int i = 1; i < Parsed.Num(); i++)
		PlayerName += i == 1 ? Parsed[i] : FString(" ") + Parsed[i];
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	for(auto player : Players)
		ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), L"{}->TeamID: {}", *ArkApi::GetApiUtils().GetCharacterName(player), player->TargetingTeamField()());
}

inline void InitCommands()
{
	if (PlayerCanTrack) ArkApi::GetCommands().AddChatCommand(Messages[4].c_str(), &PlayerTrackDino);
	ArkApi::GetCommands().AddChatCommand(Messages[5].c_str(), &AdminPlayerTrackDino);
	ArkApi::GetCommands().AddChatCommand(Messages[8].c_str(), &AdminGetPlayerTeamID);
}

inline void RemoveCommands()
{
	if (PlayerCanTrack) ArkApi::GetCommands().RemoveChatCommand(Messages[4].c_str());
	ArkApi::GetCommands().RemoveChatCommand(Messages[5].c_str());
	ArkApi::GetCommands().RemoveChatCommand(Messages[8].c_str());
}