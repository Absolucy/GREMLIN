// SPDX-License-Identifier: ISC
//
// This file contains from code from Atmosphere, normally licensed under GPLv2,
// however an exemption has been granted for me, to "prevent copyright horseshittery"
// and "saving you something on the order of minutes"
// Thanks, Scires :)

#include <logger.h>
#include <process.h>

typedef struct {
	u64 keys_held;
	u64 flags;
} CfgOverrideStatus;

enum OverrideStatusFlag : u64 {
	// This is the only one we care about lol
	OverrideStatusFlag_Hbl = (1u << 0),
};

bool is_app_sussy(u64 process_id) {
	struct {
		NcmProgramLocation _loc;
		CfgOverrideStatus status;
	} out;

	Result result;
	if ((result = serviceDispatchInOut(pminfoGetServiceSession(), 65002, process_id, out)) != 0) {
		log("Failed to get override status for process %X: error %i", process_id, result);
		return false;
	}
	return out.status.flags & OverrideStatusFlag_Hbl;
}

u64 get_app_id_from_process_id(u64 process_id) {
	u64 app_id;
	Result result;
	if ((result = pminfoGetProgramId(&app_id, process_id)) != 0) {
		log("Failed to get app id for process %X: error %i", process_id, result);
		return 0;
	}
	return app_id;
}
