#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
#include "json.hpp"
nlohmann::json WeekendConfig;
bool WeekendRatesEnabled = false, MOTDChangeEnabled = false, Notification = true, Chatbox = false;
FString ServerName, WeekendMOTD, NormalMOTD, WeekendRatesEnabledMessage, WeekendRatesDisabledMessage;
FLinearColor WeekendRatesEnabledRGB{0.f, 1.f, 0.f, 1.f}, WeekendRatesDisabledRGB{ 1.f, 0.f, 0.f, 1.f };