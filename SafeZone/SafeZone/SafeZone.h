#pragma once
#include "API\ARK\Ark.h"
int NotificationCheckSeconds;
bool DestroyAggressiveDinos, SafeZoneNotifications, AdminIgnoreChecks;
FString ServerName;
std::wstring ClaimItemsCommand;

struct ItemS
{
	FString Blueprint;
	int Quantity;
	float Quailty;
	bool IsBlueprint;

	ItemS(const FString Blueprint, const int Quantity, const float Quailty, const bool IsBlueprint)
	{
		this->Blueprint = Blueprint;
		this->Quantity = Quantity;
		this->Quailty = Quailty;
		this->IsBlueprint = IsBlueprint;
	}
};

typedef std::vector<ItemS> ItemArray;
typedef ItemArray::iterator ItemArrayItr;

struct SafeZoneDistanceS
{
	FString Name;
	FVector Postion;
	bool isPVP, PreventStructureDamage, isBuilding;
	FString Messages[6];
	int Distance;
	ItemArray Items;

	SafeZoneDistanceS(const FString Name, const FVector Postion, const bool isPVP, const bool PreventStructureDamage, const bool isBuilding, const int Distance, FString Messages[3], ItemArray Items)
	{
		this->Name = Name;
		this->Postion = Postion;
		this->isPVP = isPVP;
		this->PreventStructureDamage = PreventStructureDamage;
		this->isBuilding = isBuilding;
		this->Distance = Distance;
		for (int i = 0; i < 6; i++) this->Messages[i] = Messages[i];
		this->Items = Items;
	}
	const bool IsInArea(const FVector& Position) { return FVector::Distance(this->Postion, Position) < this->Distance; }
};

std::vector<SafeZoneDistanceS> SafeZoneDistanceMap;

struct SafeZonePlayerData
{
	bool IsInSafeArea;
	long long PlayerID;
	FString Name, LeaveMessage;
	SafeZonePlayerData(const long long PlayerID, const bool IsInSafeArea)
	{
		this->PlayerID = PlayerID;
		this->IsInSafeArea = IsInSafeArea;
	}
	const bool IsSafeArea() { return IsInSafeArea; }
	const bool IsOfPlayerID(const long long PlayerID) { return PlayerID == this->PlayerID; }
	void SetSafeArea(const bool IsInSafeArea) { this->IsInSafeArea = IsInSafeArea; }
	void SetSafeArea(const bool IsInSafeArea, const FString Name, const FString LeaveMessage)
	{ 
		this->IsInSafeArea = IsInSafeArea;
		this->Name = Name;
		this->LeaveMessage = LeaveMessage;
	}
};

std::vector<SafeZonePlayerData> SafeZonePlayerDataMap;

std::vector<SafeZonePlayerData>::iterator FindPlayer(const long long PlayerID, const bool IsInSafeArea = false)
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
	std::vector<SafeZonePlayerData>::iterator it = std::find_if(SafeZonePlayerDataMap.begin(), SafeZonePlayerDataMap.end(), [PlayerID](SafeZonePlayerData& SZD) -> bool { return SZD.IsOfPlayerID(PlayerID); });
	if (it != SafeZonePlayerDataMap.end()) SafeZonePlayerDataMap.erase(it);
}