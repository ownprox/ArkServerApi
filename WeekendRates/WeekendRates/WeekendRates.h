#pragma once

#include "API/ARK/Ark.h"
#include "json.hpp"

inline nlohmann::json WeekendConfig;
inline bool WeekendRatesEnabled = false, MOTDChangeEnabled = false, Notification = true, Chatbox = false;
inline FString ServerName, WeekendMOTD, NormalMOTD, WeekendRatesEnabledMessage, WeekendRatesDisabledMessage;
inline FLinearColor WeekendRatesEnabledRGB{0.f, 1.f, 0.f, 1.f}, WeekendRatesDisabledRGB{1.f, 0.f, 0.f, 1.f};
inline int StartDay, EndDay;