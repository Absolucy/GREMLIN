#include <cstdio>
#include <fcntl.h>
#include <logger.h>
#include <switch.h>
#include <unistd.h>

Logger::Logger() {
	if ((file = fopen("sdmc:/switch/GREMLIN/log.txt", "w")) == NULL) {
		diagAbortWithResult(MAKERESULT(Module_Libnx, LibnxError_IoError));
	}
}

Logger::~Logger() {
	fclose(file);
}

Logger& Logger::instance() {
	static Logger logger;
	return logger;
}
