#pragma once
#include <API/ARK/Ark.h>

namespace SafeZoneManager
{
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
		FString Messages[8];
		int Distance;
		ItemArray Items;
		FLinearColor EnterNotificationColour, LeaveNotificationColour;

		SafeZoneDistanceS(const FString Name, const FVector Postion, const bool isPVP, const bool PreventStructureDamage, const bool isBuilding, const int Distance, const FString Messages[6], const ItemArray Items, const FLinearColor EnterNotificationColour, const FLinearColor LeaveNotificationColour)
		{
			this->Name = Name;
			this->Postion = Postion;
			this->isPVP = isPVP;
			this->PreventStructureDamage = PreventStructureDamage;
			this->isBuilding = isBuilding;
			this->Distance = Distance;
			for (int i = 0; i < 6; i++) this->Messages[i] = Messages[i];
			this->Items = Items;
			this->EnterNotificationColour = EnterNotificationColour;
			this->LeaveNotificationColour = LeaveNotificationColour;
		}
		const bool IsInArea(const FVector& Position) { return FVector::Distance(this->Postion, Position) < this->Distance; }
	};
	
	struct SafeZonePlayerData
	{
		bool IsInSafeArea;
		long long PlayerID;
		FString Name, LeaveMessage;
		FLinearColor LeaveNotificationColour;
		SafeZonePlayerData(const long long PlayerID)
		{
			this->PlayerID = PlayerID;
			this->IsInSafeArea = false;
		}
		const bool IsSafeArea() { return IsInSafeArea; }
		const bool IsOfPlayerID(const long long PlayerID) { return PlayerID == this->PlayerID; }
		void SetSafeArea(const bool IsInSafeArea) { this->IsInSafeArea = IsInSafeArea; }
		void SetSafeArea(const bool IsInSafeArea, const FString Name, const FString LeaveMessage, const FLinearColor LeaveNotificationColour)
		{
			this->IsInSafeArea = IsInSafeArea;
			this->Name = Name;
			this->LeaveMessage = LeaveMessage;
			this->LeaveNotificationColour = LeaveNotificationColour;
		}
	};

	typedef std::vector<SafeZonePlayerData>::iterator SafeZonePlayerDataItr;
	typedef std::vector<SafeZoneDistanceS>::iterator SafeZoneDistanceSItr;
	class SZManager : public std::vector<SafeZonePlayerData>
	{
	private:
		template <typename T>
		bool RemoveCommand(const FString& command, std::vector<std::shared_ptr<T>>& commands)
		{
			auto iter = std::find_if(commands.begin(), commands.end(),
				[&command](const std::shared_ptr<T>& data) -> bool
			{
				return data->command == command;
			});

			if (iter != commands.end())
			{
				commands.erase(std::remove(commands.begin(), commands.end(), *iter), commands.end());

				return true;
			}

			return false;
		}
		template <typename T>
		struct Command
		{
			Command(FString command, std::function<T> callback)
				: command(std::move(command)),
				callback(std::move(callback))
			{
			}

			FString command;
			std::function<T> callback;
		};
		using OnPlayerJoinLeaveCallBack = Command<void(AShooterPlayerController*, bool)>;
		std::vector<std::shared_ptr<OnPlayerJoinLeaveCallBack>> PlayerJoinLeaveCallBacks;
	public:
		void AddOnPlayerJoinLeaveCallBack(const FString& id, const std::function<void(AShooterPlayerController*, bool)>& callback);
		bool RemoveOnPlayerJoinLeaveCallBack(const FString& id);
		std::vector<SafeZoneDistanceS> SafeZoneDistanceMap;
		static SZManager& Get();
		SafeZonePlayerDataItr FindPlayer(const long long PlayerID);
		SafeZoneDistanceSItr FindSafeZone(const FVector Position);
		void RemovePlayer(const long long PlayerID);
		const bool IsInsafeZome(const FVector Position);
		void ExecuteCallBacks(AShooterPlayerController* Player, const bool IsSafe);
	};
}