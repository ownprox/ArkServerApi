#pragma once
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);
void CheckPingLimits();
void InitHooks()
{
	ArkApi::GetCommands().AddOnTimerCallback("CheckPingLimits", &CheckPingLimits);
	ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, &AShooterGameMode_Logout_original);
}

void RemoveHooks()
{
	ArkApi::GetCommands().RemoveOnTimerCallback("CheckPingLimits");
	ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
}

int SecCounter = 0;
DWORD nNowTime, WatchPingTill;

void CheckPingLimits()
{
	if (SecCounter++ == CheckPingSecondsInteval)
	{
		if (!ArkApi::GetApiUtils().GetWorld())
		{
			SecCounter = 0;
			return;
		}

		nNowTime = timeGetTime();
		WatchPingTill = nNowTime + (CheckPingSecondsInteval * TotalPingStrikes) + CheckPingSecondsInteval;

		const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
		for (TWeakObjectPtr<APlayerController> player_controller : player_controllers)
		{
			AShooterPlayerController* shooter_pc = static_cast<AShooterPlayerController*>(player_controller.Get());
			if (shooter_pc && shooter_pc->PlayerStateField() && (int)shooter_pc->PlayerStateField()->PingField() > PingLimit)
			{
				int Strike = 1;
				PingItr = GetPingData(shooter_pc->LinkedPlayerIDField());
				if (PingItr == pingData.end()) pingData.push_back(PingLimitData(shooter_pc->LinkedPlayerIDField(), nNowTime + (CheckPingSecondsInteval * TotalPingStrikes) + CheckPingSecondsInteval));
				else
				{
					PingItr->Strike++;
					Strike = PingItr->Strike;
				}
				
				if (Strike == TotalPingStrikes)
				{
					FString KickReason = Messages[4].c_str();
					ArkApi::GetApiUtils().GetShooterGameMode()->KickPlayerController(shooter_pc, &KickReason);
				}
				else if(DisplayWarningsToUsers)
				{
					ArkApi::GetApiUtils().SendChatMessage(shooter_pc, Messages[0].c_str(), Messages[3].c_str(), (int)shooter_pc->PlayerStateField()->PingField(), PingLimit, Strike, TotalPingStrikes);
				}
			}
		}
		std::remove_if(pingData.begin(), pingData.end(), [](PingLimitData& PLD) -> bool { return PLD.HasPingTimedOut(nNowTime); });
		SecCounter = 0;
	}
}

void _cdecl Hook_AShooterGameMode_Logout(AShooterGameMode* _this, AController* Exiting)
{
	if (Exiting && Exiting->IsA(AShooterPlayerController::GetPrivateStaticClass()))
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(Exiting);
		if (Player)
		{
			const auto& PingLeaveItr = GetPingData(Player->LinkedPlayerIDField());
			if (PingLeaveItr != pingData.end()) pingData.erase(PingLeaveItr);
		}
	}

	AShooterGameMode_Logout_original(_this, Exiting);
}