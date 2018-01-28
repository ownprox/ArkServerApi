#pragma once
const float trunc_decs(const float& f, int decs)
{
	int i1 = floor(f);
	float rmnd = f - i1;
	int i2 = static_cast<int> (rmnd * pow(10, decs));
	float f1 = i2 / pow(10, decs);
	return i1 + f1;
}

const FVector2D GetMapCoordsFromLocation(const FVector Pos)
{
	FString Map;
	ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
	int Divider = (Map.Equals("Ragnarok") ? 13100 : 8000);
	return FVector2D(Pos.X > -1 ? trunc_decs(50 + (Pos.X / Divider), 1) : trunc_decs(50 - (-Pos.X / Divider), 1), Pos.Y > -1 ? trunc_decs(50 + (Pos.Y / Divider), 1) : trunc_decs(50 - (-Pos.Y / Divider), 1));
}

void PlayerTrackDino(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /track <DinoName>");
		return;
	}
	FString DinoName = "", TempDinoName;
	for (int i = 1; i < Parsed.Num(); i++)	DinoName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AActor*> AllDinos;
	UGameplayStatics::GetAllActorsOfClass(reinterpret_cast<UObject*>(ArkApi::GetApiUtils().GetWorld()), APrimalDinoCharacter::GetPrivateStaticClass(), &AllDinos);
	const int player_team = player->TargetingTeamField()();
	APrimalDinoCharacter* Dino;
	bool Found = false;
	for (AActor* DinoActor : AllDinos)
	{
		if (!DinoActor || DinoActor->TargetingTeamField()() != player_team) continue;		
		if((Dino = static_cast<APrimalDinoCharacter*>(DinoActor)))
		{
			if (Dino->TamedNameField()().Len() > 0 && Dino->TamedNameField()().Equals(DinoName))
			{
				Found = true;
				break;
			}
			else
			{
				Dino->DinoNameTagField()().ToString(&TempDinoName);
				if (TempDinoName.Contains(DinoName))
				{
					Found = true;
					break;
				}
			}
		}
	}

	if (Found && Dino)
	{
		const FVector2D MapCoords = GetMapCoordsFromLocation(Dino->RootComponentField()()->RelativeLocationField()());
		if(Dino->TamedNameField()().Len() > 0) ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{0} Is Located At Long: {1:.1f}, Lat: {2:.1f}", Dino->TamedNameField()().ToString().c_str(), MapCoords.X, MapCoords.Y);
		else
		{
			Dino->DinoNameTagField()().ToString(&TempDinoName);
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} Is Located At Long: {1:.1f}, Lat: {2:.1f}", TempDinoName.ToString().c_str(), MapCoords.X, MapCoords.Y);
		}
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), "{} not found.", DinoName.ToString().c_str());
}

void AdminPlayerTrackDino(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(2))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /atrack <PlayerName> <DinoName>");
		return;
	}

	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(Parsed[1]);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
	{
		FString DinoName = "", TempDinoName;
		for (int i = 2; i < Parsed.Num(); i++)	DinoName += (i == 2 ? Parsed[i] : FString(" ") + Parsed[i]);
		TArray<AActor*> AllDinos;
		UGameplayStatics::GetAllActorsOfClass(reinterpret_cast<UObject*>(ArkApi::GetApiUtils().GetWorld()), APrimalDinoCharacter::GetPrivateStaticClass(), &AllDinos);
		const int player_team = Players[0]->TargetingTeamField()();
		APrimalDinoCharacter* Dino;
		bool Found = false;
		for (AActor* DinoActor : AllDinos)
		{
			if (!DinoActor || DinoActor->TargetingTeamField()() != player_team) continue;
			if ((Dino = static_cast<APrimalDinoCharacter*>(DinoActor)))
			{
				if (Dino->TamedNameField()().Len() > 0 && Dino->TamedNameField()().Equals(DinoName))
				{
					Found = true;
					break;
				}
				else
				{
					Dino->DinoNameTagField()().ToString(&TempDinoName);
					if (TempDinoName.Contains(DinoName))
					{
						Found = true;
						break;
					}
				}
			}
		}

		if (Found && Dino)
		{
			const FVector2D& MapCoords = GetMapCoordsFromLocation(Dino->RootComponentField()()->RelativeLocationField()());
			if (Dino->TamedNameField()().Len() > 0) ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{0} Is Located At Long: {1:.1f}, Lat: {2:.1f}", Dino->TamedNameField()().ToString().c_str(), MapCoords.X, MapCoords.Y);
			else
			{
				Dino->DinoNameTagField()().ToString(&TempDinoName);
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} Is Located At Long: {1:.1f}, Lat: {2:.1f}", TempDinoName.ToString().c_str(), MapCoords.X, MapCoords.Y);
			}
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), "{} not found.", DinoName.ToString().c_str());
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), "{} not found.", Parsed[1].ToString().c_str());
}

void InitCommands()
{
	if (PlayerCanTrack) ArkApi::GetCommands().AddChatCommand("/track", &PlayerTrackDino);
	ArkApi::GetCommands().AddChatCommand("/atrack", &AdminPlayerTrackDino);
}

void RemoveCommands()
{
	if (PlayerCanTrack) ArkApi::GetCommands().RemoveChatCommand("/track");
	ArkApi::GetCommands().RemoveChatCommand("/atrack");
}