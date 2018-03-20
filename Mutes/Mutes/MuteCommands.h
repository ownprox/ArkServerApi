#pragma once
void AdminMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField()())
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
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>");
			return;
		}
		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, false, false);
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", PlayerName.ToString().c_str());
}

void AdminIPMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField()())
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
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>");
			return;
		}
		const FString& IPAddress = ArkApi::GetApiUtils().GetIPAddress(Player);
		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, true, false, IPAddress);
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} ({}) Has been ip muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", PlayerName.ToString().c_str());
}

void AdminUnMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat unmute <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField()())
	{
		RemoveMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player));
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been unmuted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", PlayerName.ToString().c_str());
}

void AdminClearALL(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	ClearMutes();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"All mutes & ip bans removed.");
}

void AdminIPBan(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField()())
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
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>");
			return;
		}
		const FString& IPAddress = ArkApi::GetApiUtils().GetIPAddress(Player);
		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, true, true, IPAddress);
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} ({}) Has been ip banned.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", PlayerName.ToString().c_str());
}

void AdminIPUnBan(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipunban <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField()())
	{
		RemoveMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player));
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been unbanned.", *ArkApi::GetApiUtils().GetCharacterName(Player));
	}
	else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", PlayerName.ToString().c_str());
}

void ReloadConfig(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	InitConfig(true);
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"Config Reloaded!");
}

void RconReloadConfig(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	InitConfig(true);
	FString Msg = "Config Reloaded";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &Msg);
}

void InitCommands()
{
	ArkApi::GetCommands().AddConsoleCommand("mute", &AdminMute);
	ArkApi::GetCommands().AddConsoleCommand("ipmute", &AdminIPMute);
	ArkApi::GetCommands().AddConsoleCommand("unmute", &AdminUnMute);
	ArkApi::GetCommands().AddConsoleCommand("ipban", &AdminIPBan);
	ArkApi::GetCommands().AddConsoleCommand("ipunban", &AdminIPUnBan);
	ArkApi::GetCommands().AddConsoleCommand("clearall", &AdminClearALL);
	ArkApi::GetCommands().AddConsoleCommand("mreload", &ReloadConfig);

	//RCON
	ArkApi::GetCommands().AddRconCommand("mreload", &RconReloadConfig);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveConsoleCommand("mute");
	ArkApi::GetCommands().RemoveConsoleCommand("ipmute");
	ArkApi::GetCommands().RemoveConsoleCommand("unmute");
	ArkApi::GetCommands().RemoveConsoleCommand("ipban");
	ArkApi::GetCommands().RemoveConsoleCommand("ipunban");
	ArkApi::GetCommands().RemoveConsoleCommand("clearall");
	ArkApi::GetCommands().RemoveConsoleCommand("mreload");

	//RCON
	ArkApi::GetCommands().RemoveRconCommand("mreload");
}