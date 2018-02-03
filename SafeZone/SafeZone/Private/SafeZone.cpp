#pragma once
#include "..\Public\SafeZone.h"
#include "SZManager.h"

namespace SafeZone
{
	bool IsInsafeZome(const FVector Position)
	{
		return SafeZoneManager::SZManager().Get().IsInsafeZome(Position);
	}
};