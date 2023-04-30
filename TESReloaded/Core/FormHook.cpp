#include "FormHook.h"
#include "WeatherMode.h"

#if defined(NEWVEGAS)
static const UInt32 kSetRegionEditorName = 0x004F0D29;
static const UInt32 kSetRegionEditorNameReturn = 0x004F0E1B;
#elif defined(OBLIVION)
static const UInt32 kSetRegionEditorName = 0x004A32A6;
static const UInt32 kSetRegionEditorNameReturn = 0x004A33A6;
#elif defined(SKYRIM)
#define m_data data
#define kFormType_REFR kFormType_Reference
static const UInt32 kSetRegionEditorName = 0x0048BEE4;
static const UInt32 kSetRegionEditorNameReturn = 0x0048BEEA;
#endif

bool __cdecl TrackLoadForm(TESForm* Form, UInt32 Arg2) {
	
	bool r = LoadForm(Form, Arg2);
	switch (Form->formType) {
		case TESForm::FormType::kFormType_Weather:
			{ 
				TESWeatherEx* Weather = (TESWeatherEx*)Form;
				if (TheSettingManager->SettingsMain.WeatherMode.Enabled) {
					memcpy(Weather->colorsb, Weather->colors, WeatherColorsSize);
					TheSettingManager->SetSettingsWeather(Weather);
				}

				//TODO: Add additonal setting for Moon phase luminance to toggle this
				Logger::Log("Loaded weather: %s", Weather->EditorName);
				ShaderConstants::SimpleWeatherStruct sws;
				std::copy(std::begin(Weather->colors), std::end(Weather->colors), std::begin(sws.colors));
				std::copy(std::begin(Weather->hdrInfo), std::end(Weather->hdrInfo), std::begin(sws.hdrInfo));
				//allow value overwrites to load the weathers from other plugins 
				TheShaderManager->ShaderConst.OrigWeathers.insert_or_assign(Weather->EditorName, sws);
			}
			break;
		case TESForm::FormType::kFormType_Water:
			if (TheSettingManager->SettingsMain.Main.RemoveUnderwater) {
				TESWaterForm* Water = (TESWaterForm*)Form;
				#if defined(OBLIVION)
				enum WaterType
				{
					WaterType_Blood,
					WaterType_Lava,
					WaterType_Normal,
				};
				switch (Water->waterType)
				{
					case WaterType_Blood:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 25;
						break;
					case WaterType_Lava:
						Water->texture.ddsPath.Set("Water\\alternatelavaX.dds");
						Water->textureBlend = 50;
						break;
					case WaterType_Normal:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 0;
						break;
					default:
						Water->texture.ddsPath.Set("");
						Water->textureBlend = 0;
						break;
				}
				Water->waterSimVals[TESWaterForm::kWaterVal_FogDistNear] = 163830.0f;
				Water->waterSimVals[TESWaterForm::kWaterVal_FogDistFar] = 163835.0f;
				#elif defined(SKYRIM) || defined(NEWVEGAS)
				Water->properties.fogAmountUW = 0.0f;
				Water->properties.fogNearUW = 9995.0f;
				Water->properties.fogFarUW = 10000.0f;
				#endif
			}
			break;
		default:
			break;
	}
	return r;

}

void SetEditorName(TESRegionEx* Region, const char* Name) {

	strcpy(Region->EditorName, Name);

}

static __declspec(naked) void SetEditorNameHook() {

	__asm
	{
		push	ecx
		call	SetEditorName
		add		esp, 8
#if defined(OBLIVION)
		xor		esi, esi
#elif defined(SKYRIM)
		lea     ecx, [esp+0x20]
#endif
		jmp		kSetRegionEditorNameReturn
	}

}

void CreateFormLoadHook() {
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)LoadForm,		&TrackLoadForm);
	DetourTransactionCommit();

#if defined(NEWVEGAS)
	// Extends the TESRegion allocation (for each constructor call) from size 0x38 to 0x60 to store additional data
	SafeWrite8(0x00466877, 0x60);
	SafeWrite8(0x004F1107, 0x60);
#elif defined(OBLIVION)
	// Extends the TESRegion allocation (for each constructor call) from size 0x2C to 0x54 to store additional data
	SafeWrite8(0x00448843, 0x54);
	SafeWrite8(0x004A2EFF, 0x54);
#elif defined(SKYRIM)
	// Extends the TESRegion allocation (for each constructor call) from size 0x30 to 0x58 to store additional data
	SafeWrite8(0x0048BC15, 0x58);
#endif
	WriteRelJump(kSetRegionEditorName, (UInt32)SetEditorNameHook);

}