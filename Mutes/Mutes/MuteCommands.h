#pragma once
void AdminMute(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		if (RconConnID != -1)
		{
			FString Msg = L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>";
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		int Min = 0, Hour = 0;
		try
		{
			Min = std::stoi(Parsed[1].ToString().c_str());
			Hour = std::stoi(Parsed[2].ToString().c_str());
		}
		catch (...) 
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>");
			return; 
		}
		if (Min == 0 && Hour == 0)
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat mute <min> <hour> <PlayerName>");
			return;
		}
		
		Log::GetLog()->info("[CMD] {} Muted: {}, Hours: {}, Minutes: {}", ArkApi::GetApiUtils().GetCharacterName(player).ToString(),
							ArkApi::GetApiUtils().GetCharacterName(Player).ToString(), Min, Hour);

		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, false, false);

		if (RconConnID != -1)
		{
			FString Msg = FString::Format(L"{} Has been muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
	}
}

void AdminIPMute(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		if (RconConnID != -1)
		{
			FString Msg = L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>";
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		int Min = 0, Hour = 0;
		try
		{
			Min = std::stoi(Parsed[1].ToString().c_str());
			Hour = std::stoi(Parsed[2].ToString().c_str());
		}
		catch (...) 
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>");
			return; 
		}
		if (Min == 0 && Hour == 0)
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipmute <min> <hour> <PlayerName>");
			return;
		}

		const FString& IPAddress = ArkApi::GetApiUtils().GetIPAddress(Player);

		Log::GetLog()->info("[CMD] {} IP Muted: {} ({}), Hours: {}, Minutes: {}", ArkApi::GetApiUtils().GetCharacterName(player).ToString(),
			ArkApi::GetApiUtils().GetCharacterName(Player).ToString(), IPAddress.ToString(), Min, Hour);

		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, true, false, IPAddress);

		if (RconConnID != -1)
		{
			FString Msg = FString::Format(L"{} ({}) Has been ip muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} ({}) Has been ip muted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
	}
}

void AdminUnMute(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		if (RconConnID != -1)
		{
			FString Msg = L"Incorrect Syntax: cheat unmute <PlayerName>";
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat unmute <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		Log::GetLog()->info("[CMD] {} UnMuted: {} ({})", ArkApi::GetApiUtils().GetCharacterName(player).ToString(),
			ArkApi::GetApiUtils().GetCharacterName(Player).ToString());

		RemoveMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player));
		if (RconConnID != -1)
		{
			FString Msg = FString::Format(L"{} Has been unmuted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been unmuted from global.", *ArkApi::GetApiUtils().GetCharacterName(Player));
	}
}

void AdminIPBan(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		if (RconConnID != -1)
		{
			FString Msg = L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>";
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 3; i < Parsed.Num(); i++)	PlayerName += (i == 3 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		int Min = 0, Hour = 0;
		try
		{
			Min = std::stoi(Parsed[1].ToString().c_str());
			Hour = std::stoi(Parsed[2].ToString().c_str());
		}
		catch (...)
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>");
		}
		if (Min == 0 && Hour == 0)
		{
			if (RconConnID != -1)
			{
				FString Msg = L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>";
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipban <min> <hour> <PlayerName>");
			return;
		}

		const FString& IPAddress = ArkApi::GetApiUtils().GetIPAddress(Player);

		Log::GetLog()->info("[CMD] {} IP Banned: {} ({}), Hours: {}, Minutes: {}", ArkApi::GetApiUtils().GetCharacterName(player).ToString(),
			ArkApi::GetApiUtils().GetCharacterName(Player).ToString(), IPAddress.ToString(), Min, Hour);

		AddMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player), Min, Hour, true, true, IPAddress);

		if (RconConnID != -1)
		{
			FString Msg = FString::Format(L"{} ({}) Has been ip banned.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} ({}) Has been ip banned.", *ArkApi::GetApiUtils().GetCharacterName(Player), *IPAddress);
	}
}

void AdminIPUnBan(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		if (RconConnID != -1)
		{
			FString Msg = L"Incorrect Syntax: cheat ipunban <PlayerName>";
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Incorrect Syntax: cheat ipunban <PlayerName>");
		return;
	}
	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		Log::GetLog()->info("[CMD] {} IP UnBanned: {}", ArkApi::GetApiUtils().GetCharacterName(player).ToString(),
			ArkApi::GetApiUtils().GetCharacterName(Player).ToString());

		if (RemoveMute(ArkApi::GetApiUtils().GetSteamIdFromController(Player)))
		{
			if (RconConnID != -1)
			{
				FString Msg = FString::Format(L"{} Has been unbanned.", *ArkApi::GetApiUtils().GetCharacterName(Player));
				rcon_connection->SendMessageW(RconConnID, 0, &Msg);
			}
			else
				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"{} Has been unbanned.", *ArkApi::GetApiUtils().GetCharacterName(Player));
		}
	}
}

void AdminListMutes(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	FString MutedData;
	try
	{
		auto& db = GetDB();
		const long long nNowTime = timeGetTime();
		db << "SELECT SteamId, IP, MutedTill, IsIPBan FROM Mutes;"
			>> [&](const uint64 SteamID, const std::string& IP, long long MutedTill, const int IsIPBan)
		{
			MutedTill = (MutedTill - nNowTime) / 1000;
			if (MutedTill > 0)
			{
				const int64 hours = (MutedTill > 3600 ? (MutedTill / 3600) : 0), mins((hours > 0 && (MutedTill - (hours * 3600)) > 60) ? ((MutedTill - (hours * 3600)) / 60) : (MutedTill > 60 ? (MutedTill / 60) : 0));

				MutedData += FString::Format(L"SteamID: {}, {} for: {} hours, {} mins{}\n", SteamID, (IsIPBan == 1 ? L"IP Banned" : L"Muted"), hours, mins, (IP.empty() ? "" : ", IP: " + IP));
			}
		};
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}

	if (RconConnID != -1)
		rcon_connection->SendMessageW(RconConnID, 0, &MutedData);
	else 
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), *MutedData);
}

void AdminGetIP(const FString& message, int RconConnID, AShooterPlayerController* player, RCONClientConnection* rcon_connection)
{
	TArray<FString> Parsed;
	message.ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(1))
	{
		FString Msg = FString::Format(L"Your IP: {}", *ArkApi::GetApiUtils().GetIPAddress(player));

		if (RconConnID != -1)
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		else
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), *Msg);
		return;
	}

	FString PlayerName = "";
	for (int i = 1; i < Parsed.Num(); i++)	PlayerName += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
	AShooterPlayerController* Player = FindPlayerFromCharacterName(PlayerName);
	if (Player != nullptr && Player->PlayerStateField())
	{
		FString Msg = FString::Format(L"{} IP: {}", *ArkApi::GetApiUtils().GetCharacterName(Player), *ArkApi::GetApiUtils().GetIPAddress(Player));
		
		if (RconConnID != -1)
			rcon_connection->SendMessageW(RconConnID, 0, &Msg);
		else
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), *Msg);
	}
}

/*
*	COMMANDS
*/

void CMDAdminMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminMute(*message, -1, player, nullptr);
}

void CMDAdminIPMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminIPMute(*message, -1, player, nullptr);
}

void CMDAdminUnMute(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminUnMute(*message, -1, player, nullptr);
}

void CMDAdminClearALL(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	ClearMutes();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"All mutes & ip bans removed.");
}

void CMDAdminIPBan(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminIPBan(*message, -1, player, nullptr);
}

void CMDAdminIPUnBan(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminIPUnBan(*message, -1, player, nullptr);
}

void CMDAdminListMutes(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminListMutes(*message, -1, player, nullptr);
}

void CMDReloadConfig(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	InitConfig(true);
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"Config Reloaded!");
}

void CMDAdminGetIP(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;
	AdminGetIP(*message, -1, player, nullptr);
}

/*
*	RCON COMMANDS
*/

void RconAdminMute(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminMute(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconAdminIPMute(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminIPMute(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconAdminUnMute(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminUnMute(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconAdminClearALL(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	ClearMutes();
	FString Msg = "All mutes & ip bans removed.";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &Msg);
}

void RconAdminIPBan(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminIPBan(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconAdminIPUnBan(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminIPUnBan(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconAdminListMutes(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminListMutes(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void RconReloadConfig(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	InitConfig(true);
	FString Msg = "Config Reloaded";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &Msg);
}

void RconAdminGetIP(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	AdminGetIP(rcon_packet->Body, rcon_packet->Id, nullptr, rcon_connection);
}

void InitCommands()
{
	ArkApi::GetCommands().AddConsoleCommand("mute", &CMDAdminMute);
	ArkApi::GetCommands().AddConsoleCommand("ipmute", &CMDAdminIPMute);
	ArkApi::GetCommands().AddConsoleCommand("unmute", &CMDAdminUnMute);
	ArkApi::GetCommands().AddConsoleCommand("ipban", &CMDAdminIPBan);
	ArkApi::GetCommands().AddConsoleCommand("ipunban", &CMDAdminIPUnBan);
	ArkApi::GetCommands().AddConsoleCommand("listmutes", &CMDAdminListMutes);
	ArkApi::GetCommands().AddConsoleCommand("clearall", &CMDAdminClearALL);
	ArkApi::GetCommands().AddConsoleCommand("mreload", &CMDReloadConfig);
	ArkApi::GetCommands().AddConsoleCommand("getip", &CMDAdminGetIP);

	//RCON
	ArkApi::GetCommands().AddRconCommand("mute", &RconAdminMute);
	ArkApi::GetCommands().AddRconCommand("ipmute", &RconAdminIPMute);
	ArkApi::GetCommands().AddRconCommand("unmute", &RconAdminUnMute);
	ArkApi::GetCommands().AddRconCommand("ipban", &RconAdminIPBan);
	ArkApi::GetCommands().AddRconCommand("ipunban", &RconAdminIPUnBan);
	ArkApi::GetCommands().AddRconCommand("listmutes", &RconAdminListMutes);
	ArkApi::GetCommands().AddRconCommand("clearall", &RconAdminClearALL);
	ArkApi::GetCommands().AddRconCommand("mreload", &RconReloadConfig);
	ArkApi::GetCommands().AddRconCommand("getip", &RconAdminGetIP);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveConsoleCommand("mute");
	ArkApi::GetCommands().RemoveConsoleCommand("ipmute");
	ArkApi::GetCommands().RemoveConsoleCommand("unmute");
	ArkApi::GetCommands().RemoveConsoleCommand("ipban");
	ArkApi::GetCommands().RemoveConsoleCommand("ipunban");
	ArkApi::GetCommands().RemoveConsoleCommand("listmutes");
	ArkApi::GetCommands().RemoveConsoleCommand("clearall");
	ArkApi::GetCommands().RemoveConsoleCommand("mreload");
	ArkApi::GetCommands().RemoveConsoleCommand("getip");

	//RCON
	ArkApi::GetCommands().RemoveRconCommand("mute");
	ArkApi::GetCommands().RemoveRconCommand("ipmute");
	ArkApi::GetCommands().RemoveRconCommand("unmute");
	ArkApi::GetCommands().RemoveRconCommand("ipban");
	ArkApi::GetCommands().RemoveRconCommand("ipunban");
	ArkApi::GetCommands().RemoveRconCommand("listmutes");
	ArkApi::GetCommands().RemoveRconCommand("clearall");
	ArkApi::GetCommands().RemoveRconCommand("mreload");
	ArkApi::GetCommands().RemoveRconCommand("getip");
}