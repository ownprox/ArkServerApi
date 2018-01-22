#pragma once
DECLARE_HOOK(AShooterPlayerController_ServerSendChatMessage_Impl, void, AShooterPlayerController*, FString*, EChatSendMode::Type);
void CleanUpLoggedOutPlayerPassedMutes();

void InitHooks()
{
	ArkApi::GetHooks().SetHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl, reinterpret_cast<LPVOID*>(&AShooterPlayerController_ServerSendChatMessage_Impl_original));
	ArkApi::GetCommands().AddOnTimerCallback("0", &CleanUpLoggedOutPlayerPassedMutes);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl);
	ArkApi::GetCommands().RemoveOnTimerCallback("0");
}

std::vector<MuteData>::iterator MuteItr;
int SecCounter = 0;
DWORD64 MuteTimePasted;
void CleanUpLoggedOutPlayerPassedMutes()
{
	if (SecCounter++ == 14400)
	{
		if (!ArkApi::GetApiUtils().GetWorld())
		{
			SecCounter = 0;
			return;
		}
		MuteTimePasted = timeGetTime();
		for (MuteItr = muteData.begin(); MuteItr != muteData.end(); MuteItr++) if (MuteTimePasted > MuteItr->MutedTill) MuteItr = muteData.erase(MuteItr);
		SecCounter = 0;
	}
}

void _cdecl Hook_AShooterPlayerController_ServerSendChatMessage_Impl(AShooterPlayerController* _this, FString* Message, EChatSendMode::Type Mode)
{
	const uint64 SteamID = ArkApi::GetApiUtils().GetSteamIdFromController(_this);
	const int Muted = IsMuted(SteamID, ArkApi::GetApiUtils().GetIPAddress(_this));
	if (Mode == EChatSendMode::Type::GlobalChat && Muted != 0)
	{
		const int hours = (Muted > 3600 ? (Muted / 3600) : 0), mins((hours > 0 && (Muted - (hours * 3600)) > 60) ? ((Muted - (hours * 3600)) / 60) : (Muted > 60 ? (Muted / 60) : 0));
		if (mins != 0 || hours != 0) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), "Muted For: {} hours, {} mins.", hours, mins);
		else ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), "Muted For: {Muted} seconds.");
		return;
	}
	AShooterPlayerController_ServerSendChatMessage_Impl_original(_this, Message, Mode);
}