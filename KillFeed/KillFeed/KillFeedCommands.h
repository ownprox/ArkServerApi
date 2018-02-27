#pragma once
void ReloadConfig(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), L"Please login as a admin.");
		return;
	}
	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), L"Kill Feed Config Reloaded!");
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/kfreload", &ReloadConfig);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/kfreload");
}