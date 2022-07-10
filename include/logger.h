// SPDX-License-Identifier: ISC
#pragma once
#include <stdio.h>

class Logger
{
private:
	FILE* file;

public:
	static Logger& instance();

	Logger();
	~Logger();

	template <typename... Args> void log(const char* f, Args... args) {
		fprintf(file, f, args...);
		fputc('\n', file);
		fflush(file);
	}
};

template <typename... Args> void log(const char* f, Args... args) {
	Logger::instance().log(f, args...);
}
