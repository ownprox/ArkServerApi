#pragma once

#include <API/UE/Math/ColorList.h>

inline void ReloadConfig(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() ||
		!player->GetPlayerCharacter()->bIsServerAdminField())
		return;

	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FColorList::Green, "Config Reloaded!");
}

inline void ReloadConfigCon(APlayerController* player_controller, FString* message, bool lol)
{
	AShooterPlayerController* shooter_controller = static_cast<AShooterPlayerController*>(player_controller);
	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Green, "Config Reloaded!");
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/wrreload", &ReloadConfig);
	ArkApi::GetCommands().AddConsoleCommand("wrreload", &ReloadConfigCon);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/wrreload");
	ArkApi::GetCommands().RemoveChatCommand("wrreload");
}
