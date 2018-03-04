#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
#include "json.hpp"
nlohmann::json PVPDays;
bool ServerNotify, PVPEnabled, LogPvpSwitchAtConsole, ProtectCharacters, ProtectDinos, ProtectStructures;
FString ServerName;
int EndHour = -1, EndDay = -1, EnabledIndex = -1;