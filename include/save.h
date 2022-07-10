// SPDX-License-Identifier: ISC
#pragma once
#include <switch.h>
#include <vector>

class SaveFileReader
{
private:
	FsSaveDataInfoReader reader;

public:
	SaveFileReader(u64 process_id, AccountUid* uid);
	~SaveFileReader();

	s64 get_file_count();
	std::vector<FsSaveDataInfo> GetFiles(FsSaveDataType type);
};
