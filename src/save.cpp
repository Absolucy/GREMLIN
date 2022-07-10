// SPDX-License-Identifier: ISC
#include <logger.h>
#include <save.h>

SaveFileReader::SaveFileReader(u64 process_id, AccountUid* uid) {
	Result result;
	if ((result = fsOpenSaveDataInfoReader(&reader, FsSaveDataSpaceId_User)) != 0) {
		log("Failed to open save data info reader: error %i", result);
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_NotInitialized));
	}
}

SaveFileReader::~SaveFileReader() {
	fsSaveDataInfoReaderClose(&reader);
}

s64 SaveFileReader::get_file_count() {
	s64 total_entries = 0;
	Result result;
	if ((result = fsSaveDataInfoReaderRead(&reader, NULL, 0, &total_entries)) != 0) {
		log("Failed to count save data info: error %i", result);
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_NotFound));
	}
	return total_entries;
}

std::vector<FsSaveDataInfo> SaveFileReader::GetFiles(FsSaveDataType type) {
	auto count = get_file_count();
	FsSaveDataInfo* infos = new FsSaveDataInfo[count];
	Result result;
	if ((result = fsSaveDataInfoReaderRead(&reader, infos, count, NULL)) != 0) {
		log("Failed to read save data info: error %i", result);
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_NotFound));
	}
	std::vector<FsSaveDataInfo> files;
	for (s64 idx = 0; idx < count; idx++) {
		auto info = infos[idx];
		if (info.save_data_type == type)
			files.push_back(info);
	}

	delete[] infos;

	return files;
}
