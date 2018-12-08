#pragma once
struct MuteData
{
	uint64 SteamID;
	DWORD MutedTill;
	bool IsIP, IPBan;
	FString IPAddress;

	MuteData(const uint64 SteamID, const DWORD MutedTill, const FString& IPAddress, const bool IPBan) : SteamID(SteamID), MutedTill(MutedTill), IPAddress(IPAddress), IPBan(IPBan), IsIP(!IPAddress.IsEmpty()) {}
	bool HasMutePassed() { return timeGetTime() > MutedTill; }
};
TArray<MuteData> muteData;
MuteData* GetMuteData(const uint64 SteamID, const FString& IPAddress = FString(), const bool CheckForIPBan = false)
{
	return muteData.FindByPredicate([SteamID, IPAddress, CheckForIPBan](const MuteData& mute) -> bool { return CheckForIPBan ? (mute.IPBan && mute.IPAddress == IPAddress) : (mute.IsIP && !IPAddress.IsEmpty() ? (mute.IPAddress == IPAddress || mute.SteamID == SteamID) : mute.SteamID == SteamID); });
}

struct BadWords
{
	const FString Word;
	const int Minutes;
	BadWords(const std::wstring Word, const int Minutes) : Word(FString(Word.c_str())), Minutes(Minutes) {}
};

TArray<BadWords> BadWord;

void InitMutes()
{
	try
	{
		auto& db = GetDB();
		db << "create table if not exists Mutes ("
			"SteamId integer primary key not null,"
			"IP text not null,"
			"IsIPBan int not null,"
			"MutedTill integer not null"
			");";

		const long long nNowTime = timeGetTime();
		db << "SELECT SteamId, IP, MutedTill, IsIPBan FROM Mutes;"
			>> [&](const uint64 SteamID, const std::string& IP, const long long MutedTill, const int IsIPBan)
		{
			if (MutedTill > nNowTime)
			{
				const FString& FIP = IP.c_str();
				muteData.Add(MuteData(SteamID, (DWORD)MutedTill, FIP, IsIPBan == 1));
			} else db << "DELETE FROM Mutes WHERE SteamId = ?;" << SteamID;
		};
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}
}

void AddMute(const uint64 SteamID, const int MuteMins, const int MuteHours, const bool IPMute, const bool IsIpBan, const FString& IPAddress = FString(""))
{
	const DWORD MutedTill = timeGetTime() + ((MuteMins * 60000) + (MuteHours * 3600000));
	auto muteddata = GetMuteData(SteamID, IPAddress);
	if (!muteddata) muteData.Add(MuteData(SteamID, MutedTill, IPAddress, IsIpBan));
	else
	{
		muteddata->MutedTill = MutedTill;
		muteddata->IsIP = IPMute;
		muteddata->IPBan = IsIpBan;
		muteddata->IPAddress = IPAddress;
	}

	if (IsIpBan)
	{
		FString BanMsg = FString::Format(*Messages[3], MuteHours, MuteMins);
		if (AShooterPlayerController* player; player = ArkApi::GetApiUtils().FindPlayerFromSteamId(SteamID))
		{
			ArkApi::GetApiUtils().GetShooterGameMode()->KickPlayerController(player, &BanMsg);
		}
	}

	try
	{
		int count = 0;
		auto& db = GetDB();
		db << "SELECT count(1) FROM Mutes WHERE SteamId = ?;" << SteamID >> count;
		if (count == 0)
		{
			db << "INSERT INTO Mutes (SteamId, IP, IsIPBan, MutedTill) VALUES (?, ?, ?, ?);" << SteamID << IPAddress.ToString() << (IsIpBan ? 1 : 0) << (long long)MutedTill;
		}
		else
		{
			db << "UPDATE Mutes SET IP = ?, IsIPBan = ?,  MutedTill = ? WHERE SteamId = ?;" << IPAddress.ToString() << (IsIpBan ? 1 : 0) << (long long)MutedTill << SteamID;
		}
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}
}

bool RemoveMute(const uint64 SteamID)
{
	auto muteddata = GetMuteData(SteamID);
	if (muteddata)
	{
		try
		{
			auto& db = GetDB();
			db << "DELETE FROM Mutes WHERE SteamId = ?;" << SteamID;
		}
		catch (const sqlite::sqlite_exception& exception)
		{
			Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
		}
		muteData.RemoveAll([SteamID](const MuteData& MD) { return MD.SteamID == SteamID; });
		return true;
	}
	return false;
}

void ClearMutes()
{
	try
	{
		auto& db = GetDB();
		db << "DELETE FROM Mutes;";
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}
	muteData.Empty();
}

int IsMuted(const uint64 SteamID, const FString& IPAddress, const bool CheckForIPBan)
{
	auto muteddata = GetMuteData(SteamID, IPAddress, CheckForIPBan);
	if (muteddata)
	{
		if (muteddata->HasMutePassed())
		{
			muteData.RemoveAll([SteamID](const MuteData& MD) { return MD.SteamID == SteamID; });
			return 0;
		}
		return (int)((muteddata->MutedTill - timeGetTime()) / 1000);
	}
	return 0;
}