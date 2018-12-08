#pragma once
DECLARE_HOOK(AShooterGameMode_SaveWorld, void, AShooterGameMode *);
bool Saving = false;

void InitHooks()
{
	ArkApi::GetHooks().SetHook("AShooterGameMode.SaveWorld", &Hook_AShooterGameMode_SaveWorld, &AShooterGameMode_SaveWorld_original);
}

void RemoveHooks()
{
	ArkApi::GetHooks().DisableHook("AShooterGameMode.SaveWorld", &Hook_AShooterGameMode_SaveWorld);
}

void Hook_AShooterGameMode_SaveWorld(AShooterGameMode* aShooterGameMode)
{
	if (Saving)
	{
		Log::GetLog()->warn("World Already Saving!");
		return;
	}
	Saving = true;
	SaveThread = std::thread([aShooterGameMode]()
	{
		__try
		{
			AShooterGameMode_SaveWorld_original(aShooterGameMode);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{ Log::GetLog()->error("Error failed to save!"); }
		Saving = false;
	});
	
	if (CPUAffinityEnabled) SetThreadAffinityMask(SaveThread.native_handle(), CPUAffinityMask);
	SaveThread.detach();
}