#pragma once
void ReloadConfig(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || !player->GetPlayerCharacter()->bIsServerAdminField()()) return;
	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), "Config Reloaded!.");
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/wrreload", &ReloadConfig);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/wrreload");
}
