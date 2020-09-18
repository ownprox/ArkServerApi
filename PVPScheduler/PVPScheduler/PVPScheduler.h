#pragma once

#include <Windows.h>
#include "API/ARK/Ark.h"
#include "json.hpp"

nlohmann::json PVPDays;
bool ServerNotify, BlockC4PlacementInPVPOff, PVPEnabled, LogPvpSwitchAtConsole, ProtectCharacters = true, ProtectDinos = true, ProtectStructures = true, ProtectExplosives = true;
FString ServerName;
inline std::string PVPMessage[3];
