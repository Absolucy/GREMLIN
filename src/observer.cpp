// SPDX-License-Identifier: ISC
#include <logger.h>
#include <observer.h>
#include <stdlib.h>

ObserverManager::ObserverManager() {
	Result result;
	log("ObserverManager()");
	if ((result = pglGetEventObserver(&observer)) != 0) {
		log("Failed to get event observer: error %i", result);
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_IoError));
	}
	log("ObserverManager() after pglGetEventObserver");
	if ((result = pglEventObserverGetProcessEvent(&observer, &process_event)) != 0) {
		log("Failed to get event observer: error %i", result);
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_IoError));
	}
	log("ObserverManager() after pglEventObserverGetProcessEvent");
}

ObserverManager::~ObserverManager() {
	log("~ObserverManager()");
	pglEventObserverClose(&observer);
	log("~ObserverManager() after pglEventObserverClose");
}

void ObserverManager::wait_for_events() {
	log("ObserverManager::wait_for_events()");
	eventWait(&process_event, UINT64_MAX);
	log("ObserverManager::wait_for_events() after eventWait");
	eventClear(&process_event);
	log("ObserverManager::wait_for_events() after eventClear");
}

Result ObserverManager::get_event(PmProcessEventInfo* event_out) {
	log("ObserverManager::get_event()");
	Result ret = pglEventObserverGetProcessEventInfo(&observer, event_out);
	log("ObserverManager::get_event() after pglEventObserverGetProcessEventInfo");
	return ret;
}
