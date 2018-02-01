#pragma once
#include "API\ARK\Ark.h"
#include "Event.h"

#ifdef ARK_EXPORTS
#define ARK_API __declspec(dllexport)
#else
#define ARK_API __declspec(dllimport)
#endif

namespace EventManager
{
	ARK_API void AddEvent(Event* event);
	ARK_API void RemoveEvent(Event* event);
	ARK_API bool IsEventRunning();
	ARK_API int GetEventPlayerCount();
	ARK_API bool StartEvent(const int EventID = -1);
}