#pragma once

#include <API/UE/Math/ColorList.h>

inline void ReloadConfig(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() ||
		!player->GetPlayerCharacter()->bIsServerAdminField()())
		return;

	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FColorList::Green, "Config Reloaded!");
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/wrreload", &ReloadConfig);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/wrreload");
}
