#include "SZManager.h"

namespace SafeZoneManager
{
	SZManager& SZManager::Get()
	{
		static SZManager instance;
		return instance;
	}

	SafeZonePlayerDataItr SZManager::FindPlayer(const long long PlayerID)
	{
		SafeZonePlayerDataItr it = std::find_if(begin(), end(), [PlayerID](SafeZonePlayerData& SZD) -> bool { return SZD.IsOfPlayerID(PlayerID); });
		if (it == end())
		{
			push_back(SafeZonePlayerData(PlayerID));
			it = --end();
		}
		return it;
	}

	void SZManager::RemovePlayer(const long long PlayerID)
	{
		SafeZonePlayerDataItr it = std::find_if(begin(), end(), [PlayerID](SafeZonePlayerData& SZD) -> bool { return SZD.IsOfPlayerID(PlayerID); });
		if (it != end()) erase(it);
	}

	SafeZoneDistanceSItr SZManager::FindSafeZone(const FVector Position)
	{
		SafeZoneDistanceSItr it = std::find_if(SafeZoneDistanceMap.begin(), SafeZoneDistanceMap.end(), [Position](SafeZoneDistanceS& SZ) -> bool { return SZ.IsInArea(Position); });
		return it;
	}

	const bool SZManager::IsInsafeZome(const FVector Position)
	{
		if (FindSafeZone(Position) != SafeZoneDistanceMap.end()) return true;
		return false;
	}
	
	void SZManager::AddOnPlayerJoinLeaveCallBack(const FString& id, const std::function<void(AShooterPlayerController*, bool)>& callback)
	{
		PlayerJoinLeaveCallBacks.push_back(std::make_shared<OnPlayerJoinLeaveCallBack>(id, callback));
	}

	bool SZManager::RemoveOnPlayerJoinLeaveCallBack(const FString& command)
	{
		return RemoveCommand<OnPlayerJoinLeaveCallBack>(command, PlayerJoinLeaveCallBacks);
	}
	
	void SZManager::ExecuteCallBacks(AShooterPlayerController* Player, const bool IsSafe)
	{
		for (const auto& data : PlayerJoinLeaveCallBacks) data->callback(Player, IsSafe);
	}
}