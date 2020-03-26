#include "WeatherMode.h"

#if defined(NEWVEGAS)
static const UInt32 kSetWeatherEditorName = 0x00580B54;
static const UInt32 kSetWeatherEditorNameReturn = 0x0058130E;
#elif defined(OBLIVION)
static const UInt32 kSetWeatherEditorName = 0x004EE04E;
static const UInt32 kSetWeatherEditorNameReturn = 0x004EE0EA;
#elif defined(SKYRIM)
static const UInt32 kSetWeatherEditorName = 0x004EF358;
static const UInt32 kSetWeatherEditorNameReturn = 0x004EF35E;
#endif

void SetEditorName(TESWeatherEx* Weather, const char* Name) {

	strcpy(Weather->EditorName, Name);

}

static __declspec(naked) void SetEditorNameHook()
{

	__asm
	{
		push	ecx
		call	SetEditorName
		add		esp, 8
#if defined(SKYRIM)
		lea     ecx, [esp+0x20]
#endif
		jmp		kSetWeatherEditorNameReturn
	}

}

void CreateWeatherModeHook() {

#if defined(NEWVEGAS)
	// Extends the TESWeather allocation (for each constructor call) from size 0x36C to 0x49C to store additional data
	SafeWrite32(0x00466606, 0x49C);
	SafeWrite32(0x0046CF9B, 0x49C);
#elif defined(OBLIVION)
	// Extends the TESWeather allocation (for each constructor call) from size 0x148 to 0x220 to store additional data
	SafeWrite32(0x004486ED, 0x220);
	SafeWrite32(0x0044CBE3, 0x220);
#elif defined(SKYRIM)
	// Extends the TESWeather allocation (for each constructor call) from size 0x760 to 0x898 to store additional data
	SafeWrite32(0x00442B22, 0x898);
	SafeWrite32(0x004EF7F5, 0x898);
#endif
	WriteRelJump(kSetWeatherEditorName, (UInt32)SetEditorNameHook);

}