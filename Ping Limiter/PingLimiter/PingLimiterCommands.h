#pragma once
inline void Ping(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter())
		return;

	ArkApi::GetApiUtils().SendChatMessage(player, Messages[0].c_str(), Messages[1].c_str(), (uint8)player->PlayerStateField()->PingField());
}

void ReloadConfig(APlayerController* player_controller, FString* message, bool WriteToLog)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField()) return;
	InitConfig();
	SecCounter = 0;
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), L"Config Reloaded!");
}

void RconReloadConfig(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* world)
{
	InitConfig();
	SecCounter = 0;
	FString Msg = "Config Reloaded";
	rcon_connection->SendMessageW(rcon_packet->Id, 0, &Msg);
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand(Messages[2].c_str(), &Ping);
	ArkApi::GetCommands().AddConsoleCommand("pingreload", &ReloadConfig);

	//RCON
	ArkApi::GetCommands().AddRconCommand("pingreload", &RconReloadConfig);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand(Messages[2].c_str());
	ArkApi::GetCommands().RemoveConsoleCommand("pingreload");

	//RCON
	ArkApi::GetCommands().RemoveRconCommand("pingreload");
}