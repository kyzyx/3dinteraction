#include <Windows.h>
#include "timestamp.h"

static bool initialized;
static double startTime;
static double CPUFreq;

double timestamp(void) {
	if (!initialized) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		CPUFreq = double(freq.QuadPart);
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double time = now.QuadPart / CPUFreq;
	return time;
}
