#pragma once
inline void PlayersList(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter())
		return;

	FString Players;
	int Counter = 0;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField()();
	for (TWeakObjectPtr<APlayerController> PlayerCon : player_controllers)
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(PlayerCon.Get());
		if (Player) Players += Counter++ == 0 ? ArkApi::GetApiUtils().GetCharacterName(Player) : (FString(", ") + ArkApi::GetApiUtils().GetCharacterName(Player));
	}
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), *Players);
	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[2].c_str(), *Players);
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand(Messages[3].c_str(), &PlayersList);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand(Messages[3].c_str());
}