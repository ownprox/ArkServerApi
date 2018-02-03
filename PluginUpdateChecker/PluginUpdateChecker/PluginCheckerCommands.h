#pragma once

void ChangeDinoColour(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendChatMessage(player, PName, L"<RichColor Color=\"1, 0, 0, 1\">Please login as admin to use this command.</>");
		return;
	}
	Timer(0, true, CheckLatestPluginsVersions, player);
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/checkplugins", &ChangeDinoColour);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/checkplugins");
}
