#pragma once
DECLARE_HOOK(AShooterPlayerController_ServerSendChatMessage_Impl, void, AShooterPlayerController*, FString*, EChatSendMode::Type);
DECLARE_HOOK(AShooterGameMode_HandleNewPlayer, bool, AShooterGameMode*, AShooterPlayerController*, UPrimalPlayerData*, AShooterCharacter*, bool);
void CleanUpLoggedOutPlayerPassedMutes();
void InitHooks()
{
	ArkApi::GetHooks().SetHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl, &AShooterPlayerController_ServerSendChatMessage_Impl_original);
	ArkApi::GetHooks().SetHook("AShooterGameMode.HandleNewPlayer_Implementation", &Hook_AShooterGameMode_HandleNewPlayer, &AShooterGameMode_HandleNewPlayer_original);
	ArkApi::GetCommands().AddOnTimerCallback("CleanPassedMutes", &CleanUpLoggedOutPlayerPassedMutes);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl);
	ArkApi::GetHooks().DisableHook("AShooterGameMode.HandleNewPlayer_Implementation", &Hook_AShooterGameMode_HandleNewPlayer);
	ArkApi::GetCommands().RemoveOnTimerCallback("CleanPassedMutes");
}

std::vector<MuteData>::iterator MuteItr;
int SecCounter = 0;
DWORD MuteTimePasted;
void CleanUpLoggedOutPlayerPassedMutes()
{
	if (SecCounter++ == 10/*14400*/)
	{
		if (!ArkApi::GetApiUtils().GetWorld())
		{
			SecCounter = 0;
			return;
		}
		MuteTimePasted = timeGetTime();
		muteData.RemoveAll([](const MuteData& muteddata) { return MuteTimePasted > muteddata.MutedTill; });
		SecCounter = 0;
	}
}

bool _cdecl Hook_AShooterGameMode_HandleNewPlayer(AShooterGameMode* _this, AShooterPlayerController* NewPlayer, UPrimalPlayerData* PlayerData, AShooterCharacter* PlayerCharacter, bool bIsFromLogin)
{
	if (NewPlayer)
	{
		const int Muted = IsMuted(ArkApi::GetApiUtils().GetSteamIdFromController(NewPlayer), ArkApi::GetApiUtils().GetIPAddress(NewPlayer), true);
		if (Muted != 0)
		{
			const int hours = (Muted > 3600 ? (Muted / 3600) : 0), mins((hours > 0 && (Muted - (hours * 3600)) > 60) ? ((Muted - (hours * 3600)) / 60) : (Muted > 60 ? (Muted / 60) : 0));
			FString BanMsg;
			if ((mins != 0 || hours != 0)) BanMsg = FString::Format(*Messages[3], hours, mins);
			else BanMsg = FString::Format(*Messages[4], Muted);
			ArkApi::GetApiUtils().GetShooterGameMode()->KickPlayerController(NewPlayer, &BanMsg);
			return false;
		}
	} 
	return AShooterGameMode_HandleNewPlayer_original(_this, NewPlayer, PlayerData, PlayerCharacter, bIsFromLogin);
}

void _cdecl Hook_AShooterPlayerController_ServerSendChatMessage_Impl(AShooterPlayerController* _this, FString* Message, EChatSendMode::Type Mode)
{
	if (_this && Mode == EChatSendMode::Type::GlobalChat && !Message->IsEmpty())
	{
		const uint64 SteamID = ArkApi::GetApiUtils().GetSteamIdFromController(_this);
		const int Muted = IsMuted(SteamID, ArkApi::GetApiUtils().GetIPAddress(_this), false);
		if (Muted != 0)
		{
			const int hours = (Muted > 3600 ? (Muted / 3600) : 0), mins((hours > 0 && (Muted - (hours * 3600)) > 60) ? ((Muted - (hours * 3600)) / 60) : (Muted > 60 ? (Muted / 60) : 0));
			if (_this && (mins != 0 || hours != 0)) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), *Messages[0], hours, mins);
			else if(_this) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), *Messages[1], Muted);
			return;
		}

		if (BadWord.Num() > 0)
		{
			const auto& Itr = BadWord.FindByPredicate([Message](const BadWords& bw) -> bool { return Message->Contains(bw.Word); });
			if (Itr)
			{
				if (Itr->Minutes > 0)
				{
					if(_this) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), *Messages[2], Itr->Minutes, *Itr->Word);
					AddMute(SteamID, Itr->Minutes, 0, false, false);
				}
				return;
			}
		}
	}
	AShooterPlayerController_ServerSendChatMessage_Impl_original(_this, Message, Mode);
}