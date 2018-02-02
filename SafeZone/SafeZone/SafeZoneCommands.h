#pragma once
FVector szSetPos = FVector(0, 0, 0);
void SetPos(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	szSetPos = player->DefaultActorLocationField()();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "Pos: {0:.0f}, {1:.0f}, {2:.0f}", szSetPos.X, szSetPos.Y, szSetPos.Z);
	Log::GetLog()->info("Position: {}, {}, {}", szSetPos.X, szSetPos.Y, szSetPos.Z);
}

void Dist(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	if (szSetPos.X == 0 && szSetPos.Y == 0 && szSetPos.Z == 0)
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please use /szsetpos first");
		return;
	}
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "Distance: {}", (int)FVector::Distance(player->DefaultActorLocationField()(), szSetPos));
	Log::GetLog()->info("Distance: {}", (int)FVector::Distance(player->DefaultActorLocationField()(), szSetPos));
}

void TPCoord(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);
	if (!Parsed.IsValidIndex(3))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /tpp <x> <y> <z>");
		return;
	}
	float X = 0, Y = 0, Z = 0;
	try
	{
		X = std::stof(Parsed[1].ToString().c_str());
		Y = std::stof(Parsed[2].ToString().c_str());
		Z = std::stof(Parsed[3].ToString().c_str());
	}
	catch (...) { return; }
	player->SetPlayerPos(X, Y, Z);
}

void ReloadConfig(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	if (!player->GetPlayerCharacter()->bIsServerAdminField()())
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Please login as admin to use this command");
		return;
	}
	SafeZoneDistanceMap.clear();
	InitConfig();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "Config Reloaded!");
}

void ClaimItems(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()() || !player->GetPlayerCharacter() || player->GetPlayerCharacter()->IsDead()) return;
	const FVector Position = player->DefaultActorLocationField()();
	std::vector<SafeZoneDistanceS>::iterator it = std::find_if(SafeZoneDistanceMap.begin(), SafeZoneDistanceMap.end(), [Position](SafeZoneDistanceS& SZ) -> bool { return SZ.IsInArea(Position); });
	if (it != SafeZoneDistanceMap.end() && it->Items.size() != 0)
	{
		for (ItemS items : it->Items) player->GiveItem(&items.Blueprint, items.Quantity, items.Quailty, items.IsBlueprint);
		ArkApi::GetApiUtils().SendChatMessage(player, ServerName, *it->Messages[5]);
	}
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/szsetpos", &SetPos);
	ArkApi::GetCommands().AddChatCommand("/szdist", &Dist);
	ArkApi::GetCommands().AddChatCommand("/sztp", &TPCoord);
	ArkApi::GetCommands().AddChatCommand("/szreload", &ReloadConfig);
	ArkApi::GetCommands().AddChatCommand(ClaimItemsCommand.c_str(), &ClaimItems);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/szsetpos");
	ArkApi::GetCommands().RemoveChatCommand("/szdist");
	ArkApi::GetCommands().RemoveChatCommand("/sztp");
	ArkApi::GetCommands().RemoveChatCommand("/szreload");
	ArkApi::GetCommands().RemoveChatCommand(ClaimItemsCommand.c_str());
}
