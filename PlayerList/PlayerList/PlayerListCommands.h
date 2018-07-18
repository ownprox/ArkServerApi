#pragma once
inline void PlayersNameList(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || StaffOnly && !player->bIsAdmin()())
		return;

	FString Players;
	int Counter = 0;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
		if (Player) Players += Counter++ == 0 ? ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L"") : (FString(", ") + ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""));
	}
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *Players);
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), Counter);
}

inline void PlayersSteamIDList(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()())
		return;
	FString Players;
	int Counter = 0;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
		if (Player) Players += Counter++ == 0 ? FString::Format(L"{} ({})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), ArkApi::GetApiUtils().GetShooterGameMode()->GetSteamIDForPlayerID(Player->LinkedPlayerIDField())) : FString::Format(L", {} ({})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), ArkApi::GetApiUtils().GetShooterGameMode()->GetSteamIDForPlayerID(Player->LinkedPlayerIDField()));
	}
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *Players);
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), Counter);
}

inline void PlayerIDList(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()())
		return;
	FString Players;
	int Counter = 0;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
		if (Player) Players += Counter++ == 0 ? FString::Format(L"{} ({})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), Player->LinkedPlayerIDField()) : FString::Format(L", {} ({})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), Player->LinkedPlayerIDField());
	}
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *Players);
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), Counter);
}

AShooterPlayerController* FindPlayerFromPlayerID(int PlayerID)
{

	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> player_controller : player_controllers)
	{
		if (player_controller->PlayerStateField()->PlayerIdField() == PlayerID)
		{
			return static_cast<AShooterPlayerController*>(player_controller.Get());
		}
	}

	return nullptr;
}

inline void TribeIDList(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()())
		return;
	FString Players;
	int Counter = 0, TribeID = 0;
	FString TribeName;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
		if (Player)
		{
			AShooterPlayerState* player_state = static_cast<AShooterPlayerState*>(Player->PlayerStateField());
			if (player_state && player_state->MyTribeDataField())
			{
				TribeID = player_state->MyTribeDataField()->TribeIDField();
				TribeName = player_state->MyTribeDataField()->TribeNameField();
			}
			Players += TribeName.IsEmpty() ? (Counter++ == 0 ? FString::Format(L"{}", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L"")) : FString::Format(L", {}", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""))) 
				: (Counter++ == 0 ? FString::Format(L"{} ({}:{})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), *TribeName, TribeID) : FString::Format(L", {} ({}:{})", *ArkApi::GetApiUtils().GetCharacterName(Player).Replace(L"{}", L""), *TribeName, TribeID));
		}
	}
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *Players);
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), Counter);
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand(Messages[3].c_str(), &PlayersNameList);
	ArkApi::GetCommands().AddChatCommand(Messages[4].c_str(), &PlayersSteamIDList);
	ArkApi::GetCommands().AddChatCommand(Messages[5].c_str(), &PlayerIDList);
	ArkApi::GetCommands().AddChatCommand(Messages[6].c_str(), &TribeIDList);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand(Messages[3].c_str());
	ArkApi::GetCommands().RemoveChatCommand(Messages[4].c_str());
	ArkApi::GetCommands().RemoveChatCommand(Messages[5].c_str());
	ArkApi::GetCommands().RemoveChatCommand(Messages[6].c_str());
}