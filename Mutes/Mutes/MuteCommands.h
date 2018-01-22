#pragma once
void AdminMute(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command.");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /mute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
	{
		int Min = 0, Hour = 0;
		try
		{
			Min = std::stoi(Parsed[1].ToString().c_str());
			Hour = std::stoi(Parsed[2].ToString().c_str());
		}
		catch (...) { return; }
		if (Min == 0 && Hour == 0)
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /mute <min> <hour> <PlayerName>");
			return;
		}
		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]), Min, Hour, false);
		SaveConfig();
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} Has been muted from global.", ArkApi::GetApiUtils().GetCharacterName(Players[0]).ToString().c_str());
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: {}", Parsed[1].ToString().c_str());
}

void AdminIPMute(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command.");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /ipmute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
	{
		int Min = 0, Hour = 0;
		try
		{
			Min = std::stoi(Parsed[1].ToString().c_str());
			Hour = std::stoi(Parsed[2].ToString().c_str());
		}
		catch (...) { return; }
		if (Min == 0 && Hour == 0)
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /ipmute <min> <hour> <PlayerName>");
			return;
		}
		const FString& IPAddress = ArkApi::GetApiUtils().GetIPAddress(player);
		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]), Min, Hour, true, IPAddress);
		SaveConfig();
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} ({}) Has been ip muted from global.", ArkApi::GetApiUtils().GetCharacterName(Players[0]).ToString().c_str(), IPAddress.ToString().c_str());
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: {}", Parsed[1].ToString().c_str());
}

void AdminUnMute(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command.");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /unmute <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(PlayerName);
	if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
	{
		RemoveMute(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]));
		SaveConfig();
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} Has been unmuted from global.", ArkApi::GetApiUtils().GetCharacterName(Players[0]).ToString().c_str());
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Cant Find: {}", Parsed[1].ToString().c_str());
}

void AdminUnMuteALL(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command.");
		return;
	}
	ClearMutes();
	SaveConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "All mutes removed.");
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/mute", &AdminMute);
	ArkApi::GetCommands().AddChatCommand("/ipmute", &AdminIPMute);
	ArkApi::GetCommands().AddChatCommand("/unmute", &AdminUnMute);
	ArkApi::GetCommands().AddChatCommand("/unmuteall", &AdminUnMuteALL);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/mute");
	ArkApi::GetCommands().RemoveChatCommand("/ipmute");
	ArkApi::GetCommands().RemoveChatCommand("/unmute");
	ArkApi::GetCommands().RemoveChatCommand("/unmuteall");
}
