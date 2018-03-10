#pragma once
DECLARE_HOOK(AShooterPlayerController_ServerSendChatMessage_Impl, void, AShooterPlayerController*, FString*, EChatSendMode::Type);
void CleanUpLoggedOutPlayerPassedMutes();
void InitHooks()
{
	ArkApi::GetHooks().SetHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl, reinterpret_cast<LPVOID*>(&AShooterPlayerController_ServerSendChatMessage_Impl_original));
    ArkApi::GetCommands().AddOnTimerCallback("CleanPassedMutes", &CleanUpLoggedOutPlayerPassedMutes);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("AShooterPlayerController.ServerSendChatMessage_Implementation", &Hook_AShooterPlayerController_ServerSendChatMessage_Impl);
	ArkApi::GetCommands().RemoveOnTimerCallback("CleanPassedMutes");
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
	if (_this && Mode == EChatSendMode::Type::GlobalChat && !Message->IsEmpty())
	{
		const uint64 SteamID = ArkApi::GetApiUtils().GetSteamIdFromController(_this);
		const int Muted = IsMuted(SteamID, ArkApi::GetApiUtils().GetIPAddress(_this));
		if (Muted != 0)
		{
			const int hours = (Muted > 3600 ? (Muted / 3600) : 0), mins((hours > 0 && (Muted - (hours * 3600)) > 60) ? ((Muted - (hours * 3600)) / 60) : (Muted > 60 ? (Muted / 60) : 0));
			if (_this && (mins != 0 || hours != 0)) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), L"Muted For: {} hours, {} mins.", hours, mins);
			else if(_this) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), L"Muted For: {} seconds.", Muted);
			return;
		}

		if (BadWord.size() > 0)
		{
			BadWordItr Itr = std::find_if(BadWord.begin(), BadWord.end(), [Message](const BadWords& bw) -> bool { return Message->Contains(bw.Word); });
			if (Itr != BadWord.end())
			{
				if (Itr->Minutes > 0)
				{
					if(_this) ArkApi::GetApiUtils().SendServerMessage(_this, FLinearColor(255, 0, 0), L"Muted For: {} Minutes for saying {}.", Itr->Minutes, *Itr->Word);
					AddMute(SteamID, Itr->Minutes, 0, false);
					SaveConfig();
				}
				return;
			}
		}
	}
	AShooterPlayerController_ServerSendChatMessage_Impl_original(_this, Message, Mode);
}