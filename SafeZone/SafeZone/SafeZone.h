#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
int NotificationCheckSeconds;
std::string SafeZoneEnterNotifcation, SafeZoneLeaveNotification;
bool DestroyAggressiveDinos, SafeZoneNotifications;

struct SafeZoneDistanceS
{
	FVector Postion;
	bool isPVP, isBuilding;
	std::string NoPvPMessage, NoBuildingMessage;
	int Distance;
	SafeZoneDistanceS(FVector Postion, bool isPVP, bool isBuilding, std::string NoPvPMessage, std::string NoBuildingMessage, int Distance)
	{
		this->Postion = Postion;
		this->isPVP = isPVP;
		this->isBuilding = isBuilding;
		this->NoPvPMessage = NoPvPMessage;
		this->NoBuildingMessage = NoBuildingMessage;
		this->Distance = Distance;
	}
	const bool IsInArea(const FVector& Position) { return FVector::Distance(this->Postion, Position) < this->Distance; }
};

std::vector<SafeZoneDistanceS> SafeZoneDistanceMap;

struct SafeZonePlayerData
{
	bool IsInSafeArea;
	long long PlayerID;
	SafeZonePlayerData(long long PlayerID, bool IsInSafeArea)
	{
		this->PlayerID = PlayerID;
		this->IsInSafeArea = IsInSafeArea;
	}
	const bool IsSafeArea() { return IsInSafeArea; }
	const bool IsOfPlayerID(const long long PlayerID) { return PlayerID == this->PlayerID; }
	void SetSafeArea(bool IsInSafeArea) { this->IsInSafeArea = IsInSafeArea; }
};

std::vector<SafeZonePlayerData> SafeZonePlayerDataMap;

std::vector<SafeZonePlayerData>::iterator FindPlayer(const long long PlayerID, bool IsInSafeArea = false)
{
	std::vector<SafeZonePlayerData>::iterator it = std::find_if(SafeZonePlayerDataMap.begin(), SafeZonePlayerDataMap.end(), [PlayerID](SafeZonePlayerData& SZD) -> bool { return SZD.IsOfPlayerID(PlayerID); });
	if (it == SafeZonePlayerDataMap.end())
	{
		SafeZonePlayerDataMap.push_back(SafeZonePlayerData(PlayerID, IsInSafeArea));
		it = --SafeZonePlayerDataMap.end();
	}
	return it;
}

void RemovePlayer(const long long PlayerID)
{
	SafeZonePlayerDataMap.erase(std::remove_if(SafeZonePlayerDataMap.begin(), SafeZonePlayerDataMap.end(), [PlayerID](SafeZonePlayerData& SZD) -> bool { return SZD.IsOfPlayerID(PlayerID); }));
}