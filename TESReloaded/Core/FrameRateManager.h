#pragma once

class FrameRateManager { // Never disposed
public:
	FrameRateManager();

	double			GetPerformance();

	LONGLONG		PerformanceFrequency;
	LONGLONG		PerformanceCounterStart;
	double			Time;
	double			LastTime;
	double			ElapsedTime;
	double			SmartControlMPF;
	double			LastPerformance;
};