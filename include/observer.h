// SPDX-License-Identifier: ISC
#pragma once
#include <optional>
#include <switch.h>
#include <vector>

class ObserverManager
{
private:
	PglEventObserver observer;
	Event process_event;

public:
	ObserverManager();
	~ObserverManager();

	void wait_for_events();
	Result get_event(PmProcessEventInfo* event_out);
};
