#pragma once

class TESWeatherEx : public TESWeather
{

public:
	ColorData	colorsb[TESWeather::kNumColorTypes];
	char		EditorName[40];

};

void CreateWeatherModeHook();