#pragma once
#include <API/ARK/Ark.h>

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

namespace SafeZone
{
	ARK_API bool IsInsafeZome(const FVector Position);
};