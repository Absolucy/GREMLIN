// This is for the C garbage.
#define INNER_HEAP_SIZE 0x80000

#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 __nx_applet_type = AppletType_None;

void __libnx_initheap(void) {
	static u8 inner_heap[INNER_HEAP_SIZE];
	extern void* fake_heap_start;
	extern void* fake_heap_end;

	// Configure the newlib heap.
	fake_heap_start = inner_heap;
	fake_heap_end = inner_heap + sizeof(inner_heap);
}

void __appInit(void) {
	Result result;

	// Initialize the Service manager
	if ((result = smInitialize()) != 0)
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

	// Initialize system firmware stuff
	if ((result = setsysInitialize()) == 0) {
		SetSysFirmwareVersion fw;
		if ((result = setsysGetFirmwareVersion(&fw)) == 0)
			hosversionSet(MAKEHOSVERSION(fw.major, fw.minor, fw.micro));
		setsysExit();
	}

	// Initialize the filesystem
	if ((result = fsInitialize()) != 0)
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));

	// Mount SD card filesystem
	if ((result = fsdevMountSdmc()) != 0) // how r u even running this lol
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));

	// Connect to pgl
	if ((result = pglInitialize()) != 0)
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_NotInitialized));

	// Connect to pm:info
	if ((result = pminfoInitialize()) != 0)
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_NotInitialized));

	// Close service manager session
	smExit();
}

void __appExit(void) {
	// Deinitialize filesystem
	fsdevUnmountAll();
	fsExit();
	// Disconnect from pgl
	pglExit();
	// Disconnect from pm:info
	pminfoExit();
}

#ifdef __cplusplus
}
#endif
