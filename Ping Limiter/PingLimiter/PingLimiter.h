#pragma once

#include <Windows.h>
#include <API/ARK/Ark.h>
inline std::wstring Messages[5];
int PingLimit = 120, CheckPingSecondsInteval = 60000, TotalPingStrikes = 5;
bool DisplayWarningsToUsers = true;

struct PingLimitData
{
	uint64 PlayerID;
	DWORD PingStrikeTimeout;
	int Strike;

	PingLimitData(const uint64 PlayerID, const DWORD PingStrikeTimeout) : PlayerID(PlayerID), PingStrikeTimeout(PingStrikeTimeout), Strike(1) {}
	bool HasPingTimedOut(DWORD nNowTime) { return nNowTime > PingStrikeTimeout; }
};
std::vector<PingLimitData> pingData;
std::vector<PingLimitData>::iterator GetPingData(const uint64 PlayerID) { return std::find_if(pingData.begin(), pingData.end(), [PlayerID](const PingLimitData& ping) -> bool { return ping.PlayerID == PlayerID; }); }
std::vector<PingLimitData>::iterator PingItr;