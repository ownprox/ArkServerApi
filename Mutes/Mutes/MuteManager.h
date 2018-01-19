#pragma once
FString GetIPAddress(AShooterPlayerController* player)
{
	FString IPAddress = "";
	if (player->PlayerStateField()()->IsA(AShooterPlayerState::StaticClass()))
	{
		AShooterPlayerState* ASPS = static_cast<AShooterPlayerState*>(player->PlayerStateField()());
		if (ASPS && ASPS->MyPlayerDataStructField()()) IPAddress = ASPS->MyPlayerDataStructField()()->SavedNetworkAddressField()();
	}
	return IPAddress;
}

struct MuteData
{
	uint64 SteamID;
	DWORD64 MutedTill;
	bool IsIP;
	FString IPAddress;

	MuteData(const uint64 SteamID, const int MuteMins, const int MuteHours, const bool IPBan, const FString& IPAddress)
	{
		this->SteamID = SteamID;
		this->IsIP = IsIP;
		this->IPAddress = IPAddress;
		UpdateMuteTime(MuteMins, MuteHours);
	}

	MuteData(const uint64 SteamID, const DWORD64 MutedTill, const bool IPBan, const FString& IPAddress)
	{
		this->SteamID = SteamID;
		this->MutedTill = MutedTill;
		this->IsIP = IsIP;
		this->IPAddress = IPAddress;
	}
	void UpdateMuteTime(const int& MuteMins, const int& MuteHours) { MutedTill = timeGetTime() + ((MuteMins * 60000) + (MuteHours * 3600000)); }
	bool HasMutePassed() { return timeGetTime() > MutedTill; }
};
std::vector<MuteData> muteData;
std::vector<MuteData>::iterator GetMuteData(const uint64 SteamID, const FString& IPAddress = FString("")) { return std::find_if(muteData.begin(), muteData.end(), [SteamID, IPAddress](const MuteData& mute) -> bool { return (mute.IsIP ? mute.IPAddress == IPAddress : mute.SteamID == SteamID); }); }

void LoadMute(const uint64 SteamID, const DWORD64 MutedTill, const bool IPMute, const FString& IPAddress)
{
	muteData.push_back(MuteData(SteamID, MutedTill, IPMute, IPAddress));
}

void AddMute(const uint64 SteamID, const int MuteMins, const int MuteHours, const bool IPMute, const FString& IPAddress = FString(""))
{
	auto iter = GetMuteData(SteamID, IPAddress);
	if (iter == muteData.end()) muteData.push_back(MuteData(SteamID, MuteMins, MuteHours, IPMute, IPAddress));
	else iter->UpdateMuteTime(MuteMins, MuteHours);
}

void RemoveMute(const uint64 SteamID)
{
	auto iter = GetMuteData(SteamID);
	if (iter != muteData.end()) muteData.erase(iter);
}

void ClearMutes()
{
	muteData.clear();
}

int IsMuted(const uint64 SteamID, const FString& IPAddress)
{
	auto iter = GetMuteData(SteamID, IPAddress);
	if (iter != muteData.end())
	{
		printf("Mute DAta Found!");
		if (iter->HasMutePassed())
		{
			printf("Mute Time Passed!");
			muteData.erase(iter);
			return 0;
		}
		return ((iter->MutedTill - timeGetTime()) / 1000);
	}
	return 0;
}