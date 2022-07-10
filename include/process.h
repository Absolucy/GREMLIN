// SPDX-License-Identifier: ISC
#pragma once
#include <switch.h>

/// Does the process id have the Homebrew Launcher injected?
/// Yes, the name is stupid. Deal with it.
///
/// @param process_id The process id to check.
/// @returns true if the process has the HBL injected.
bool is_app_sussy(u64 process_id);
u64 get_app_id_from_process_id(u64 process_id);
