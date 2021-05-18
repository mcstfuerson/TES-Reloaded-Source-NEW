#include "FrameRateManager.h"

FrameRateManager::FrameRateManager() {

	Logger::Log("Starting the framerate manager...");
	TheFrameRateManager = this;

	LARGE_INTEGER Frequency;
	LARGE_INTEGER PerformanceCounter;

	Time = 0.0;
	LastTime = 0.0;
	ElapsedTime = 0.0;
	LastPerformance = 0;
	QueryPerformanceFrequency(&Frequency); PerformanceFrequency = Frequency.QuadPart;
	QueryPerformanceCounter(&PerformanceCounter); PerformanceCounterStart = PerformanceCounter.QuadPart;

}
double FrameRateManager::GetPerformance() {

	LARGE_INTEGER PerformanceCounterEnd;

	QueryPerformanceCounter(&PerformanceCounterEnd);
	return (double)(PerformanceCounterEnd.QuadPart - TheFrameRateManager->PerformanceCounterStart) * 1000.0 / (double)TheFrameRateManager->PerformanceFrequency;

}
