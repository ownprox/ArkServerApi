#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"

std::thread SaveThread;
bool CPUAffinityEnabled = false;
DWORD_PTR CPUAffinityMask;