#pragma once

#include <API/UE/Math/ColorList.h>

inline void ReloadConfig(APlayerController* player_controller, FString* cmd, bool)
{
	AShooterPlayerController* shooter_controller = static_cast<AShooterPlayerController*>(player_controller);

	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::Green, "Config Reloaded!");
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddConsoleCommand("/wrreload", &ReloadConfig);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveConsoleCommand("/wrreload");
}
