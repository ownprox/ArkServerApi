#pragma once
void PM(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr)
	{
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (!Parsed.IsValidIndex(2))
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /pm \"<PlayerName>\" <Msg>");
			return;
		}
		FString Msg = "";
		for (int i = 2; i < Parsed.Num(); i++)	Msg += (i == 2 ? Parsed[i] : FString(" ") + Parsed[i]);
		TArray<AShooterPlayerController*> Players = ArkApi::GetApiUtils().FindPlayerFromCharacterName(Parsed[1]);
		if (Players.Num() > 0 && Players[0] != nullptr && Players[0]->PlayerStateField()())
		{
			PlayerS* o;
			if ((o = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(Players[0]))) != NULL)
			{
				o->LastPmSteamID = p->SteamID;
				p->LastPmSteamID = o->SteamID;
				if (ServerMessage)
				{
					ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(ServerMessageRBG[0], ServerMessageRBG[1], ServerMessageRBG[2]), L"[{}->{}] {}", *ArkApi::GetApiUtils().GetCharacterName(player), *ArkApi::GetApiUtils().GetCharacterName(Players[0]), *Msg);
					ArkApi::GetApiUtils().SendServerMessage(Players[0], FLinearColor(ServerMessageRBG[0], ServerMessageRBG[1], ServerMessageRBG[2]), L"[{}->{}] {}", *ArkApi::GetApiUtils().GetCharacterName(player), *ArkApi::GetApiUtils().GetCharacterName(Players[0]), *Msg);
				}
				else
				{
					ArkApi::GetApiUtils().SendChatMessage(player, ArkApi::GetApiUtils().GetCharacterName(player), L"[PM:{}] {}", *ArkApi::GetApiUtils().GetCharacterName(Players[0]), *Msg);
					ArkApi::GetApiUtils().SendChatMessage(Players[0], ArkApi::GetApiUtils().GetCharacterName(player), L"[PM] {}", *Msg);
				}
			}
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), L"Cant Find: {}", *Parsed[1]);
	}
}

void Reply(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField()()) return;
	PlayerS* p;
	if ((p = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(player))) != nullptr)
	{
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (!Parsed.IsValidIndex(1))
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /r <Msg>");
			return;
		}

		FString Msg = "";
		for (int i = 1; i < Parsed.Num(); i++)	Msg += (i == 1 ? Parsed[i] : FString(" ") + Parsed[i]);
		AShooterPlayerController* other = nullptr;
		if ((other = ArkApi::GetApiUtils().FindPlayerFromSteamId(p->LastPmSteamID)) != nullptr && other->PlayerStateField()())
		{
			PlayerS* o;
			if ((o = GetPlayer(ArkApi::GetApiUtils().GetSteamIdFromController(other))) != nullptr)
			{
				o->LastPmSteamID = p->SteamID;
				p->LastPmSteamID = o->SteamID;
				if(ServerMessage)
				{
					ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(ServerMessageRBG[0], ServerMessageRBG[1], ServerMessageRBG[2]), L"[{}->{}] {}", *ArkApi::GetApiUtils().GetCharacterName(player), *ArkApi::GetApiUtils().GetCharacterName(other), *Msg);
					ArkApi::GetApiUtils().SendServerMessage(other, FLinearColor(ServerMessageRBG[0], ServerMessageRBG[1], ServerMessageRBG[2]), L"[{}->{}] {}", *ArkApi::GetApiUtils().GetCharacterName(player), *ArkApi::GetApiUtils().GetCharacterName(other), *Msg);
				}
				else
				{
					ArkApi::GetApiUtils().SendChatMessage(player, ArkApi::GetApiUtils().GetCharacterName(player), L"[PM:{}] {}", *ArkApi::GetApiUtils().GetCharacterName(other), *Msg);
					ArkApi::GetApiUtils().SendChatMessage(other, ArkApi::GetApiUtils().GetCharacterName(player), L"[PM] {}", *Msg);
				}
			}
		}
		else ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Player seems to be offline");
	}
}

void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/pm", &PM);
	ArkApi::GetCommands().AddChatCommand("/r", &Reply);
}

void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/pm");
	ArkApi::GetCommands().RemoveChatCommand("/r");
}
