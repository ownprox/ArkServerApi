#pragma once

#include <API/ARK/Ark.h>

#include "json.hpp"

namespace SafeZones
{
	inline nlohmann::json config;

	FString GetText(const std::string& str);
	void ReadConfig();
}
