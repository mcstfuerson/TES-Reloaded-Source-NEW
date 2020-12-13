#pragma once

#if defined(NEWVEGAS)
#define ScreenshotFilenamePrefix "NewVegas"
#define SettingsPath "\\Data\\Shaders\\NewVegasReloaded\\"
#define MainSettingsFile "\\Data\\NVSE\\Plugins\\NewVegasReloaded.ini"
#define WeatherSettingsFile "\\Data\\NVSE\\Plugins\\NewVegasReloaded.Weather.ini"
#define ShadersPath "Data\\Shaders\\NewVegasReloaded\\"
#define EffectsPath "Data\\Shaders\\NewVegasReloaded\\"
#define DocFile "Data\\Docs\\NewVegasReloaded\\NewVegasReloaded.html"
#define WaterDisplacementMapSize 256.0f
#define WaterHeightMapSize 256.0f
#define kSettingNearDistance 0x01203148
#define kSettingWorldFoV 0x01203160
#define kSetting1stPersonFoV 0x0120316C
#define kSettingGridsToLoad 0x011C63D0
static const char* IntroMovie = "NVRGameStudios.bik";
static const char* MainMenuMovie = "\\Data\\Video\\NVRMainMenu.bik";
static const char* MainMenuMusic = "NVRMainMenu";
#elif defined(OBLIVION)
#define ScreenshotFilenamePrefix "Oblivion"
#define SettingsPath "\\Data\\Shaders\\OblivionReloaded\\"
#define MainSettingsFile "\\Data\\OBSE\\Plugins\\OblivionReloaded.ini"
#define WeatherSettingsFile "\\Data\\OBSE\\Plugins\\OblivionReloaded.Weather.ini"
#define ShadersPath "Data\\Shaders\\OblivionReloaded\\"
#define EffectsPath "Data\\Shaders\\OblivionReloaded\\"
#define DocFile "Data\\Docs\\OblivionReloaded\\OblivionReloaded.html"
#define WaterDisplacementMapSize 256.0f
#define WaterHeightMapSize 256.0f
#define kSettingNearDistance 0x00B03134
#define kSettingWorldFoV 0x00B0313C
#define kSetting1stPersonFoV 0x00000000
#define kSettingGridsToLoad 0x00B06A2C
static const char* IntroMovie = "ORGameStudios.bik";
static const char* MainMenuMovie = "ORMainMenu.bik";
static const char* MainMenuMusic = "Special\\ORMainMenu.mp3";
#elif defined(SKYRIM)
#define ScreenshotFilenamePrefix "Skyrim"
#define SettingsPath "\\Data\\Shadersfx\\SkyrimReloaded\\"
#define MainSettingsFile "\\Data\\SKSE\\Plugins\\SkyrimReloaded.ini"
#define WeatherSettingsFile "\\Data\\SKSE\\Plugins\\SkyrimReloaded.Weather.ini"
#define ShadersPath "Data\\Shadersfx\\SkyrimReloaded\\"
#define EffectsPath "Data\\Shadersfx\\SkyrimReloaded\\"
#define DocFile "Data\\Docs\\SkyrimReloaded\\SkyrimReloaded.html"
#define WaterDisplacementMapSize 256.0f
#define WaterHeightMapSize 256.0f
#define kSettingNearDistance 0x012C8D54
#define kSettingWorldFoV 0x012C8D78
#define kSetting1stPersonFoV 0x012C8D84
#define kSettingGridsToLoad 0x01241000
#define ColorData ColorType
static const char* IntroMovie = "";
static const char* MainMenuMovie = "";
static const char* MainMenuMusic = "";
#endif
#define WeatherColorsSize TESWeather::kNumColorTypes * TESWeather::kNumTimeOfDay * 4
#define SettingStringBuffer 80

static const UInt32*	SettingGridsToLoad				= (UInt32*)kSettingGridsToLoad;
static const UInt32*	SettingGridDistantCount			= (UInt32*)0x00B06A90; // Only Oblivion
static float*			SettingWorldFoV					= (float*)kSettingWorldFoV;
static float*			Setting1stPersonFoV				= (float*)kSetting1stPersonFoV;
static float*			SettingNearDistance				= (float*)kSettingNearDistance;
static float*			SettingLODFadeOutMultActors		= (float*)0x00B0762C; // Only Oblivion
static float*			SettingLODFadeOutMultItems		= (float*)0x00B07624; // Only Oblivion
static float*			SettingLODFadeOutMultObjects	= (float*)0x00B0761C; // Only Oblivion
static UInt32*			SettingMinGrassSize				= (UInt32*)0x00B09B20; // Only Oblivion
static float*			SettingGrassEndDistance			= (float*)0x00B09B18; // Only Oblivion
static float*			SettingGrassStartFadeDistance	= (float*)0x00B09B10; // Only Oblivion
static float*			SettingGrassWindMagnitudeMax	= (float*)0x00B09B30; // Only Oblivion
static float*			SettingGrassWindMagnitudeMin	= (float*)0x00B09B28; // Only Oblivion
static float*			SettingTexturePctThreshold		= (float*)0x00B08B6C; // Only Oblivion
static UInt32*			SettingMultiSample				= (UInt32*)0x00B06D0C; // Only Oblivion
static UInt8*			SettingHDR						= (UInt8*)0x00B06DE4; // Only Oblivion

struct SettingsMainStruct {

	struct MainStruct {
		char	MainFile[MAX_PATH];
		char	WeatherFile[MAX_PATH];
		char	ScreenshotPath[MAX_PATH];
		bool	RemoveUnderwater;
		bool	RemovePrecipitations;
		bool	MemoryManagement;
		bool	FPSOverlay;
		bool	SaveSettings;
		bool	ReplaceIntro;
		UInt8	ScreenshotType;
		UInt8	AnisotropicFilter;
		UInt16	ScreenshotKey;
		int		WaterReflectionMapSize;
		float	FarPlaneDistance;
		float	FoV;
	};
	
	struct CameraModeStruct {
		bool		Enabled;
		bool		ChasingFirst;
		bool		ChasingThird;
		UInt8		Crosshair;
		float		NearDistanceFirst;
		float		NearDistanceThird;
		NiPoint3	Offset;
		NiPoint3	DialogOffset;
	};

	struct EquipmentModeStruct {
		bool		Enabled;
		bool		SleepingEquipment;
		bool		SwimmingEquipment;
		UInt16		TorchKey;
		UInt16		CombatEquipmentKey;
		float		DualBlockDelay;
		NiPoint3	ShieldOnBackPos;
		NiPoint3	ShieldOnBackRot;
		NiPoint3	WeaponOnBackPos;
		NiPoint3	WeaponOnBackRot;
		NiPoint3	TwoHandWeaponOnBackPos;
		NiPoint3	TwoHandWeaponOnBackRot;
		NiPoint3	BowOnBackPos;
		NiPoint3	BowOnBackRot;
		NiPoint3	StaffOnBackPos;
		NiPoint3	StaffOnBackRot;
		NiPoint3	TorchOnBeltPos;
		NiPoint3	TorchOnBeltRot;
	};
	
	struct FrameRateStruct {
		bool	Enabled;
		int		Average;
		int		Min;
		int		Critical;
		int		Gap;
		int		Delay;
		int		GridStep;
		int		FadeMinObjects;
		int		FadeMinActors;
		int		GridMin;
		float	FadeStep;
	};

	struct GrassModeStruct {
		bool	Enabled;
	};

	struct MountedCombatStruct {
		bool		Enabled;
		NiPoint3	WeaponOnBackPos;
		NiPoint3	WeaponOnBackRot;
		NiPoint3	TwoHandWeaponOnBackPos;
		NiPoint3	TwoHandWeaponOnBackRot;
		NiPoint3	BowOnBackPos;
		NiPoint3	BowOnBackRot;
		NiPoint3	StaffOnBackPos;
		NiPoint3	StaffOnBackRot;
	};
	
	struct SleepingModeStruct {
		bool	Enabled;
		UInt8	Mode;
	};
	
	struct WeatherModeStruct {
		bool		Enabled;
		NiPoint3	CoeffNight;
		NiPoint3	CoeffFog;
		NiPoint3	CoeffSun;
	};
	
	struct ShadowModeStruct {
		bool	MultiPointLighting;
		float	NearQuality;
	};

	struct ShadersStruct {
		bool Blood;
		bool Grass;
		bool HDR;
		bool NightEye;
		bool POM;
		bool Skin;
		bool Terrain;
		bool Water;
	};

	struct EffectsStruct {
		bool AmbientOcclusion;
		bool BloodLens;
		bool Bloom;
		bool Cinema;
		bool Coloring;
		bool DepthOfField;
		bool GodRays;
		bool LowHF;
		bool MotionBlur;
		bool Precipitations;
		bool Sharpening;
		bool SMAA;
		bool SnowAccumulation;
		bool Underwater;
		bool WaterLens;
		bool WetWorld;
		bool VolumetricFog;
		bool Extra;
	};

	struct MenuStruct {
		char	TextFont[40];
		char	TextFontStatus[40];
		char	ValueFormat[5];
		bool	InfoEnabled;
		UInt8	TextColorNormal[3];
		UInt8	TextShadowColorNormal[3];
		UInt8	TextColorSelected[3];
		UInt8	TextShadowColorSelected[3];
		UInt8	TextColorEditing[3];
		UInt8	TextShadowColorEditing[3];
		UInt8	TextSize;
		UInt8	TextSizeStatus;
		UInt8	KeyEnable;
		UInt8	KeyUp;
		UInt8	KeyDown;
		UInt8	KeyLeft;
		UInt8	KeyRight;
		UInt8	KeyPageUp;
		UInt8	KeyPageDown;
		UInt8	KeyAdd;
		UInt8	KeySubtract;
		UInt8	KeySave;
		UInt8	KeyEditing;
		int		PositionX;
		int		PositionY;
		int		InfoPositionX;
		int		InfoPositionY;
		int		TitleColumnSize;
		int		MainItemColumnSize;
		int		ItemColumnSize;
		int		InfoColumnSize;
		int		RowSpace;
		int		RowsPerPage;
		float	StepValue;
	};

	struct LowHFSoundStruct {
		bool	HealthEnabled;
		bool	FatigueEnabled;
		float	HealthCoeff;
		float	FatigueCoeff;
	};

	struct PurgerStruct {
		bool	Enabled;
		bool	PurgeTextures;
		bool	PurgeCells;
		UInt16	Key;
		int		Time;
	};

	struct GravityStruct {
		bool	Enabled;
		float	Value;
	};

	struct DodgeStruct {
		bool	Enabled;
		bool	DoubleTap;
		UInt8	AcrobaticsLevel;
		float	DoubleTapTime;
	};
	
	struct FlyCamStruct {
		bool	Enabled;
		UInt8	KeyAdd;
		UInt8	KeySubtract;
		float	ScrollMultiplier;
		float	StepValue;
	};

	struct DevelopStruct {
		bool	CompileShaders;
		bool	CompileEffects;
		UInt8	TraceShaders;
		UInt8	LogShaders;
	};

	MainStruct					Main;
	CameraModeStruct			CameraMode;
	EquipmentModeStruct			EquipmentMode;
	FrameRateStruct				FrameRate;
	GrassModeStruct				GrassMode;
	MountedCombatStruct			MountedCombat;
	SleepingModeStruct			SleepingMode;
	WeatherModeStruct			WeatherMode;
	ShadowModeStruct			ShadowMode;
	ShadersStruct				Shaders;
	EffectsStruct				Effects;
	MenuStruct					Menu;
	LowHFSoundStruct			LowHFSound;
	PurgerStruct				Purger;
	GravityStruct				Gravity;
	DodgeStruct					Dodge;
	FlyCamStruct				FlyCam;
	DevelopStruct				Develop;
};

struct SettingsShadowStruct {
	typedef std::vector<UInt32> ExcludedFormsList;

	struct FormsStruct {
		bool				Activators;
		bool				Actors;
		bool				Apparatus;
		bool				Books;
		bool				Containers;
		bool				Doors;
		bool				Furniture;
		bool				Misc;
		bool				Statics;
		bool				Trees;
	};

	struct ExteriorsStruct {
		FormsStruct			Forms[3];
		bool				Enabled[3];
		bool				AlphaEnabled[3];
		int					Quality;
		int					ShadowMapSize[3];
		float				Darkness;
		float				ShadowMapRadius[3];
		float				ShadowMapFarPlane;
		ExcludedFormsList	ExcludedForms;
	};
	
	struct InteriorsStruct {
		FormsStruct			Forms;
		bool				Enabled;
		bool				AlphaEnabled;
		bool				TorchesCastShadows;
		int					LightPoints;
		int					Quality;
		int					ShadowCubeMapSize;
		float				Darkness;
		ExcludedFormsList	ExcludedForms;
	};

	ExteriorsStruct		Exteriors;
	InteriorsStruct		Interiors;
};

struct SettingsWaterStruct {
	float choppiness;
	float waveWidth;
	float waveSpeed;
	float reflectivity;
	float causticsStrength;
	float causticsStrengthS;
	float shoreFactor;
	float shoreMovement;
	float turbidity;
	float inScattCoeff;
	float inExtCoeff_R;
	float inExtCoeff_G;
	float inExtCoeff_B;
	float depthDarkness;
	float LODdistance;
	float MinLOD;
	float LensTimeMultA;
	float LensTimeMultB;
	float LensTime;
	float LensAmount;
	float LensViscosity;
};

struct SettingsGrassStruct {
	bool WindEnabled;
	int GrassDensity;
	float WindCoefficient;
	float ScaleX;
	float ScaleY;
	float ScaleZ;
	float MinHeight;
	float MinDistance;
	float MaxDistance;
};

struct SettingsHDRStruct {
	float ToneMapping;
	float ToneMappingBlur;
	float ToneMappingColor;
	float Linearization;
};

struct SettingsPOMStruct {
	float HeightMapScale;
	float ShadowSoftening;
	float MinSamples;
	float MaxSamples;
};

struct SettingsTerrainStruct {
	float DistantSpecular;
	float DistantNoise;
	float NearSpecular;
	float MiddleSpecular;
};

struct SettingsSkinStruct {
	float Attenuation;
	float SpecularPower;
	float MaterialThickness;
	float RimScalar;
	float CoeffRed;
	float CoeffGreen;
	float CoeffBlue;
};

struct SettingsGodRaysStruct {
	bool TimeEnabled;
	bool SunGlareEnabled;
	int LightShaftPasses;
	float RayIntensity;
	float RayLength;
	float RayDensity;
	float RayVisibility;
	float Luminance;
	float GlobalMultiplier;
	float RayR;
	float RayG;
	float RayB;
	float Saturate;
};

struct SettingsDepthOfFieldStruct {
	bool Enabled;
	bool DistantBlur;
	UInt8 Mode;
	float DistantBlurStartRange;
	float DistantBlurEndRange;
	float BaseBlurRadius;
	float BlurFallOff;
	float Radius;
	float DiameterRange;
	float NearBlurCutOff;
};

struct SettingsAmbientOcclusionStruct {
	bool Enabled;
	float RadiusMultiplier;
	float StrengthMultiplier;
	float ClampStrength;
	float AngleBias;
	float Range;
	float LumThreshold;
	float BlurDropThreshold;
	float BlurRadiusMultiplier;
};

struct SettingsColoringStruct {
	float Strength;
	float BaseGamma;
	float Fade;
	float Contrast;
	float Saturation;
	float Bleach;
	float BleachLuma;
	float ColorCurve;
	float ColorCurveR;
	float ColorCurveG;
	float ColorCurveB;
	float EffectGamma;
	float EffectGammaR;
	float EffectGammaG;
	float EffectGammaB;
	float Linearization;
};

struct SettingsCinemaStruct {
	UInt8 Mode;
	float AspectRatio;
	float VignetteRadius;
	float VignetteDarkness;
};

struct SettingsBloomStruct {
	float BloomIntensity;
	float OriginalIntensity;
	float BloomSaturation;
	float OriginalSaturation;
	float Luminance;
	float MiddleGray;
	float WhiteCutOff;
};

struct SettingsPrecipitationsStruct {
	struct RainStruct {

	};
	
	struct SnowStruct {
		float DepthStep;
		float Flakes;
		float Speed;
	};
	
	struct WetWorldStruct {
		float Amount;
		float Increase;
		float Decrease;
		float PuddleCoeff_R;
		float PuddleCoeff_G;
		float PuddleCoeff_B;
		float PuddleSpecularMultiplier;
	};

	struct SnowAccumulationStruct {
		float Amount;
		float Increase;
		float Decrease;
		float SunPower;
		float BlurNormDropThreshhold;
		float BlurRadiusMultiplier;
	};
	
	RainStruct				Rain;
	SnowStruct				Snow;
	WetWorldStruct			WetWorld;
	SnowAccumulationStruct	SnowAccumulation;
};

struct SettingsBloodStruct {
	float LensChance;
	float LensColorR;
	float LensColorG;
	float LensColorB;
	float LensIntensity;
	float LensTime;
};

struct SettingsMotionBlurStruct {
	bool Enabled;
	float GaussianWeight;
	float BlurScale;
	float BlurOffsetMax;
	float BlurCutOff;
};

struct SettingsLowHFStruct {
	float HealthLimit;
	float FatigueLimit;
	float LumaMultiplier;
	float BlurMultiplier;
	float VignetteMultiplier;
	float DarknessMultiplier;
};

struct SettingsSharpeningStruct {
	float Strength;
	float Clamp;
	float Offset;
};

struct SettingsVolumetricFogStruct {
	float Exponent;
	float ColorCoeff;
	float Amount;
	float MaxDistance;
};

struct SettingsWeatherStruct {
	char					LowerLayer[80];
	char					UpperLayer[80];
	UInt8					WindSpeed;
	UInt8					CloudSpeedLower;
	UInt8					CloudSpeedUpper;
	UInt8					TransDelta;
	UInt8					SunGlare;
	UInt8					SunDamage;
	float					FogNearDay;
	float					FogNearNight;
	float					FogFarDay;
	float					FogFarNight;
	TESWeather::ColorData	Colors[TESWeather::kNumColorTypes];
	float					HDR[14];
};

typedef std::map<std::string, SettingsWaterStruct> SettingsWaterList;
typedef std::map<std::string, SettingsDepthOfFieldStruct> SettingsDepthOfFieldList;
typedef std::map<std::string, SettingsAmbientOcclusionStruct> SettingsAmbientOcclusionList;
typedef std::map<std::string, SettingsColoringStruct> SettingsColoringList;
typedef std::map<std::string, SettingsBloomStruct> SettingsBloomList;
typedef std::map<std::string, SettingsMotionBlurStruct> SettingsMotionBlurList;
typedef std::map<std::string, SettingsWeatherStruct> SettingsWeatherList;
typedef std::map<std::string, std::string> DefinitionsList;
typedef std::map<UInt32, std::string> SectionsList;
typedef std::map<std::string, float> SettingsList;

class SettingManager {
public:
	SettingManager();

	void							LoadSettings();
	void							SaveSettings(const char* Item, const char* Definition);
	DefinitionsList					GetMenuItems();
	DefinitionsList					GetMenuDefinitions(const char* Item);
	SectionsList					GetMenuSections(const char* Item, const char* Definition);
	SettingsList					GetMenuSettings(const char* Item, const char* Definition, const char* Section, UInt32 SelectedSectionKey);
	void							SetMenuSetting(const char* Item, const char* Definition, const char* Section, UInt32 SelectedSectionKey, const char* Setting, float Value);
	bool							GetMenuShaderEnabled(const char* Name);
	SettingsWaterStruct*			GetSettingsWater(const char* PlayerLocation);
	SettingsDepthOfFieldStruct*		GetSettingsDepthOfField(const char* Section);
	SettingsAmbientOcclusionStruct*	GetSettingsAmbientOcclusion(const char* Section);
	SettingsColoringStruct*			GetSettingsColoring(const char* PlayerLocation);
	SettingsBloomStruct*			GetSettingsBloom(const char* PlayerLocation);
	SettingsMotionBlurStruct*		GetSettingsMotionBlur(const char* Section);
	SettingsWeatherStruct*			GetSettingsWeather(const char* WeatherName);
	void							SetSettingsWeather(TESWeather* Weather);
	SettingsWeatherStruct*			CreateSettingsWeather(const char* WeatherName);

	char							CurrentPath[MAX_PATH];
	bool							GameLoading;
	SettingsMainStruct				SettingsMain;
	SettingsGrassStruct				SettingsGrass;
	SettingsHDRStruct				SettingsHDR;
	SettingsPOMStruct				SettingsPOM;
	SettingsTerrainStruct			SettingsTerrain;
	SettingsSkinStruct				SettingsSkin;
	SettingsGodRaysStruct			SettingsGodRays;
	SettingsCinemaStruct			SettingsCinema;
	SettingsPrecipitationsStruct	SettingsPrecipitations;
	SettingsBloodStruct				SettingsBlood;
	SettingsLowHFStruct				SettingsLowHF;
	SettingsSharpeningStruct		SettingsSharpening;
	SettingsVolumetricFogStruct		SettingsVolumetricFog;
	SettingsShadowStruct			SettingsShadows;

private:
	char Water_choppiness[8];
	char Water_waveWidth[8];
	char Water_waveSpeed[8];
	char Water_reflectivity[8];
	char Water_causticsStrength[8];
	char Water_causticsStrengthS[8];
	char Water_shoreFactor[8];
	char Water_shoreMovement[8];
	char Water_turbidity[8];
	char Water_inScattCoeff[8];
	char Water_inExtCoeff_R[8];
	char Water_inExtCoeff_G[8];
	char Water_inExtCoeff_B[8];
	char Water_depthDarkness[8];
	char Water_LODdistance[8];
	char Water_MinLOD[8];
	char Water_LensTimeMultA[8];
	char Water_LensTimeMultB[8];
	char Water_LensTime[8];
	char Water_LensAmount[8];
	char Water_LensViscosity[8];

	char Coloring_Strength[8];
	char Coloring_BaseGamma[8];
	char Coloring_Fade[8];
	char Coloring_Contrast[8];
	char Coloring_Saturation[8];
	char Coloring_Bleach[8];
	char Coloring_BleachLuma[8];
	char Coloring_ColorCurve[8];
	char Coloring_ColorCurveR[8];
	char Coloring_ColorCurveG[8];
	char Coloring_ColorCurveB[8];
	char Coloring_EffectGamma[8];
	char Coloring_EffectGammaR[8];
	char Coloring_EffectGammaG[8];
	char Coloring_EffectGammaB[8];
	char Coloring_Linearization[8];

	char Bloom_BloomIntensity[8];
	char Bloom_OriginalIntensity[8];
	char Bloom_BloomSaturation[8];
	char Bloom_OriginalSaturation[8];
	char Bloom_Luminance[8];
	char Bloom_MiddleGray[8];
	char Bloom_WhiteCutOff[8];

	SettingsWaterList				SettingsWater;
	SettingsDepthOfFieldList		SettingsDepthOfField;
	SettingsAmbientOcclusionList	SettingsAmbientOcclusion;
	SettingsColoringList			SettingsColoring;
	SettingsBloomList				SettingsBloom;
	SettingsMotionBlurList			SettingsMotionBlur;
	SettingsWeatherList				SettingsWeather;

};

void CreateSettingsHook();