#include <filesystem>
#include <logger.h>
#include <observer.h>
#include <process.h>
#include <save.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AccountUid get_current_user() {
	Result result;
	AccountUid uid = {0};
	if ((result = accountInitialize(AccountServiceType_Application)) != 0) {
		log("Failed to initialize account service: error %i", result);
		log("Using default UID of 0");
		goto end;
	}

	if ((result = accountGetPreselectedUser(&uid)) != 0) {
		log("Failed to get preselected user: error %i", result);
		log("Using default UID of 0");
		goto end;
	}

end:
	accountExit();
	return uid;
}

int main() {
	log("log test");

	ObserverManager observer;
	Result result;
	u64 last_sussy_app = UINT64_MAX;

	int i = 0;
	while (true) {
		log("loop iteration #%i", i++);
		observer.wait_for_events();
		log("alright, we waited for an event");
		PmProcessEventInfo event;
		int j = 0;
		while (R_SUCCEEDED(observer.get_event(&event))) {
			log("got event #%i", j++);
			// Log HBL app, so we can ignore it later.
			if (is_app_sussy(event.process_id)) {
				last_sussy_app = event.process_id;
				continue;
			}
			if (event.event != PmProcessEvent_Exit) {
				log("Process %X is not exiting, ignoring", event.process_id);
				continue;
			}
			if (event.process_id == last_sussy_app) {
				log("Exiting HBL app, ignoring");
				continue;
			}
			auto uid = get_current_user();
			auto aid = get_app_id_from_process_id(event.process_id);
			log("Process %X exited, (app id %X) checking for save data", event.process_id, aid);
			auto save_reader = SaveFileReader(event.process_id, &uid);
			auto save_count = save_reader.get_file_count();
			log("Found %i save files", save_count);
			if (save_count == 0) {
				continue;
			}
			if ((result = fsdevMountSaveDataReadOnly("gremlin", aid, uid)) != 0) {
				log("Failed to mount save data: error %i", result);
				continue;
			}
			const std::filesystem::path path("gremlin:/");
			for (auto const& dir_entry : std::filesystem::directory_iterator(path)) {
				log("Found save file %s", dir_entry.path().c_str());
			}
			fsdevUnmountDevice("gremlin");
		}
	}
}
