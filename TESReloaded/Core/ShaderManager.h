#pragma once
#include <d3dx9mesh.h>

enum EffectRecordType
{
	EffectRecordType_Underwater,
	EffectRecordType_WaterLens,
	EffectRecordType_GodRays,
	EffectRecordType_KhajiitRays,
	EffectRecordType_MasserRays,
	EffectRecordType_SecundaRays,
	EffectRecordType_DepthOfField,
	EffectRecordType_AmbientOcclusion,
	EffectRecordType_Coloring,
	EffectRecordType_Cinema,
	EffectRecordType_Bloom,
	EffectRecordType_SnowAccumulation,
	EffectRecordType_SMAA,
	EffectRecordType_MotionBlur,
	EffectRecordType_WetWorld,
	EffectRecordType_Sharpening,
	EffectRecordType_VolumetricFog,
	EffectRecordType_VolumetricLight,
	EffectRecordType_Precipitations,
	EffectRecordType_ShadowsExteriors,
	EffectRecordType_ShadowsExteriorsPoint,
	EffectRecordType_ShadowsExteriorsPointDialog,
	EffectRecordType_ShadowsInteriors,
	EffectRecordType_Extra,
};

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Pixel,
};

enum DayPhase
{
	Dawn,
	Sunrise,
	Day,
	Sunset,
	Dusk,
	Night
};

enum CellLocation
{
	Unk,
	Interior,
	Exterior,
	Fake_Exterior
};

struct ShaderConstants {
	
	struct ShadowMapStruct {
		D3DXMATRIX	    ShadowWorld;
		D3DXMATRIX		ShadowViewProj;
		D3DXMATRIX		ShadowCameraToLight[4];
		D3DXVECTOR4		ShadowCubeMapLightPosition;
		D3DXVECTOR4		ShadowCastLightPosition[12];
		D3DXVECTOR4		ShadowCullLightPosition[24];
		D3DXVECTOR4		ShadowCubeMapFarPlanes;
		D3DXVECTOR4		ShadowCubeMapBlend;
		D3DXVECTOR4		ShadowCubeMapBlend2;
		D3DXVECTOR4		ShadowCubeMapBlend3;
		D3DXVECTOR4		ShadowLightDir;
		D3DXVECTOR4		ShadowBiasForward;
		D3DXVECTOR4		ShadowBiasDeferred;
	};

	struct PointLightStruct {
		D3DXVECTOR4		LightPosition[2];
		D3DXVECTOR4		LightColor[2];
	};

	struct WaterStruct {
		D3DXVECTOR4		waterCoefficients;
		D3DXVECTOR4		waveParams;
		D3DXVECTOR4		waterVolume;
		D3DXVECTOR4		waterSettings;
		D3DXVECTOR4		deepColor;
		D3DXVECTOR4		shallowColor;
		D3DXVECTOR4		shorelineParams;
	};
	struct HDRStruct {
		D3DXVECTOR4		ToneMapping;
	};
	struct GrassStruct {
		D3DXVECTOR4		Scale;
	};
	struct POMStruct {
		D3DXVECTOR4		ParallaxData;
	};
	struct TerrainStruct {
		D3DXVECTOR4		Data;
	};
	struct SkinStruct {
		D3DXVECTOR4		SkinData;
		D3DXVECTOR4		SkinColor;
	};
	struct ShadowStruct {
		D3DXVECTOR4		Data;
		D3DXVECTOR4		ShadowSkinData;
		D3DXVECTOR4		OrthoData;
	};
	struct PrecipitationsStruct {
		D3DXVECTOR4		RainData;
		D3DXVECTOR4		SnowData;
	};
	struct WaterLensStruct {
		D3DXVECTOR4		Time;
		float			TimeAmount;
		float			Percent;
	};
	struct GodRaysStruct {
		D3DXVECTOR4		Ray;
		D3DXVECTOR4		RayColor;
		D3DXVECTOR4		Data;
	};
	struct DepthOfFieldStruct {
		bool			Enabled;
		D3DXVECTOR4		Blur;
		D3DXVECTOR4		Data;
	};
	struct AmbientOcclusionStruct {
		bool			Enabled;
		D3DXVECTOR4		AOData;
		D3DXVECTOR4		Data;
	};
	struct ColoringStruct {
		D3DXVECTOR4		ColorCurve;
		D3DXVECTOR4		EffectGamma;
		D3DXVECTOR4		Data;
		D3DXVECTOR4		Values;
	};
	struct CinemaStruct {
		D3DXVECTOR4		Data;
	};
	struct BloomStruct {
		D3DXVECTOR4		BloomData;
		D3DXVECTOR4		BloomValues;
	};
	struct SnowAccumulationStruct {
		D3DXVECTOR4		Params;
	};
	struct MotionBlurStruct {
		D3DXVECTOR4		BlurParams;
		D3DXVECTOR4		Data;
		float			oldAngleX;
		float			oldAngleZ;
		float			oldAmountX;
		float			oldAmountY;
		float			oldoldAmountX;
		float			oldoldAmountY;
	};
	struct WetWorldStruct {
		D3DXVECTOR4		Coeffs;
		D3DXVECTOR4		Data;
	};
	struct SharpeningStruct {
		D3DXVECTOR4		Data;
	};
	struct VolumetricFogStruct {
		D3DXVECTOR4		Data;
	};
	struct VolumetricLightStruct {

		D3DXVECTOR4 baseData;
		D3DXVECTOR4 accumData;
		D3DXVECTOR4 miscData;
		D3DXVECTOR4 miscData2;

		SettingsVolumetricLightStruct pleasantSettings = { 0.18,	0.15,	0.13,	5000.0,		0.18,	0.15,	0.13,	2000.0,		3.5,	8500.0,		20000.0,	500000.0 };
		SettingsVolumetricLightStruct rainySettings =    { 0.07,	0.07,	0.07,	5000.0,		0.12,	0.12,	0.09,	2000.0,		0.5,	8500.0,		20000.0,	500000.0 };
		SettingsVolumetricLightStruct cloudySettings =   { 0.18,	0.15,	0.13,	5000.0,		0.18,	0.15,	0.13,	2000.0,		1.5,	8500.0,		20000.0,	500000.0 };
		SettingsVolumetricLightStruct nightSettings =    { 0.05,	0.07,	0.1,	5000.0,		0.03,	0.05,	0.08,	2000.0,		0.75,	8500.0,		20000.0,	500000.0 };
		SettingsVolumetricLightStruct middaySettings =   { 0.10,	0.08,	0.07,	5000.0,		0.13,	0.11,	0.09,	2000.0,		1.25,	8500.0,		20000.0,	500000.0 };

		const std::map<UInt8, SettingsVolumetricLightStruct> weatherTypeMap = {
			{TESWeather::WeatherType::kType_None,pleasantSettings },
			{TESWeather::WeatherType::kType_Pleasant,pleasantSettings },
			{TESWeather::WeatherType::kType_Cloudy,cloudySettings },
			{TESWeather::WeatherType::kType_Rainy,rainySettings },
			{TESWeather::WeatherType::kType_Snow,rainySettings }
		};
	};

	struct SimpleWeatherStruct {
		TESWeather::ColorData		colors[10];
		float			hdrInfo[14];
	};

	struct SimpleLightingStruct {
		UInt8	r;
		UInt8	g;
		UInt8	b;
		UInt8	a;
	};

	typedef std::map<std::string, SimpleWeatherStruct> WeatherMap;
	typedef std::map<std::string, SimpleLightingStruct> InteriorLightingMap;

	D3DXVECTOR4				ReciprocalResolution;
	D3DXVECTOR4				ReciprocalResolutionWater;
	D3DXVECTOR4				DirectionalLight; //currently only used for moon lighting
	bool					OverrideVanillaDirectionalLight;
	DayPhase				DayPhase;
	D3DXVECTOR4				ReflectionLightDir;
	D3DXVECTOR4				SunDir;
	D3DXVECTOR4				SunTiming;
	D3DXVECTOR4				SunAmount;
	D3DXVECTOR4				MasserDir;
	D3DXVECTOR4				MasserAmount;
	float					MasserFade;
	D3DXVECTOR4				SecundaDir;
	D3DXVECTOR4				SecundaAmount;
	float					SecundaFade;
	bool					MoonsExist;
	WeatherMap				OrigWeathers;
	InteriorLightingMap		InteriorLighting;
	float					MoonPhaseCoeff;
	D3DXVECTOR4				RaysPhaseCoeff;
	D3DXVECTOR4				GameTime;
	D3DXVECTOR4				Tick;
	D3DXVECTOR4				InteriorDimmer;
	float					InteriorDimmerStart;
	float					InteriorDimmerEnd;
	D3DXVECTOR4				TextureData;
	TESWeather*				pWeather;
	float					currentsunGlare;
	float					currentwindSpeed;
	UInt8					oldsunGlare;
	UInt8					oldwindSpeed;
	D3DXVECTOR4				fogColor;
	D3DXVECTOR4				oldfogColor;
	D3DXVECTOR4				sunColor;
	D3DXVECTOR4				oldsunColor;
	D3DXVECTOR4				fogData;
	float					currentfogStart;
	float					oldfogStart;
	float					currentfogEnd;
	float					oldfogEnd;
	bool					HasWater;
	ShadowMapStruct			ShadowMap;
	PointLightStruct		PointLights;
	WaterStruct				Water;
	HDRStruct				HDR;
	GrassStruct				Grass;
	POMStruct				POM;
	TerrainStruct			Terrain;
	SkinStruct				Skin;
	ShadowStruct			Shadow;
	ShadowStruct			ShadowCube;
	PrecipitationsStruct	Precipitations;
	WaterLensStruct			WaterLens;
	GodRaysStruct			GodRays;
	GodRaysStruct			KhajiitRaysMasser;
	GodRaysStruct			KhajiitRaysSecunda;
	DepthOfFieldStruct		DepthOfField;
	AmbientOcclusionStruct	AmbientOcclusion;
	ColoringStruct			Coloring;
	CinemaStruct			Cinema;
	BloomStruct				Bloom;
	SnowAccumulationStruct	SnowAccumulation;
	MotionBlurStruct		MotionBlur;
	WetWorldStruct			WetWorld;
	SharpeningStruct		Sharpening;
	VolumetricFogStruct		VolumetricFog;
	VolumetricLightStruct	VolumetricLight;
	bool					EveningTransLightDirSet;
	D3DXVECTOR4				EveningTransLightDir;
	bool					MorningTransLightDirSet;
	D3DXVECTOR4				MorningTransLightDir;
};

struct ShaderValue {
	UInt32				RegisterIndex;
	UInt32				RegisterCount;
	union {
	D3DXVECTOR4*		Value;
	TextureRecord*		Texture;
	};
};

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	void					SetConstantTableValue(LPCSTR Name, UInt32 Index);

	ShaderValue*			FloatShaderValues;
	UInt32					FloatShaderValuesCount;
	ShaderValue*			TextureShaderValues;
	UInt32					TextureShaderValuesCount;
};

class ShaderRecord : public ShaderProgram {
public:
	ShaderRecord();
	~ShaderRecord();

	void					CreateCT();
	void					SetCT();
	bool					LoadShader(const char* Name, const char* DirPostFix = "");
	
	ShaderType				Type;
	bool					Enabled;
	bool					HasCT;
	bool					HasRB; 
	bool					HasDB;
	void*					Function;
	char* 					Source;
	ID3DXBuffer*			Errors;
	ID3DXBuffer*			Shader;
	ID3DXConstantTable*		Table;
};

class EffectRecord : public ShaderProgram {
public:
	EffectRecord();
	~EffectRecord();

	bool						LoadEffect(const char* Name);
	void						CreateCT();
	void						SetCT();
	void						Render(IDirect3DDevice9* Device, IDirect3DSurface9* RenderTarget, IDirect3DSurface9* RenderedSurface, bool ClearRenderTarget);

	bool						Enabled;
	char*	 					Source;
	ID3DXBuffer*				Errors;
	ID3DXEffect*				Effect;
};

typedef std::map<std::string, EffectRecord*> ExtraEffectsList;
typedef std::map<std::string, D3DXVECTOR4> CustomConstants;

class ShaderManager { // Never disposed
public:
	ShaderManager();

	void					CreateEffects();
	void					InitializeConstants();
	void					UpdateConstants();
	void					UpdateShaderStates();
	void					BeginScene();
	void					CreateShader(const char *Name);
	void					LoadShader(NiD3DVertexShader* Shader, const char* DirPostFix = "");
	void					LoadShader(NiD3DPixelShader* Shader, const char* DirPostFix = "");
	void					DisposeShader(const char* Name);
	void					CreateEffect(EffectRecordType EffectType);
	bool					LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName);
	void					LoadEffectSettings();
	void					DisposeEffect(EffectRecord* TheEffect);
	void					RenderEffects(IDirect3DSurface9* RenderTarget);
	void					SwitchShaderStatus(const char* Name);
	void					SetCustomConstant(const char* Name, D3DXVECTOR4 Value);
	void					SetExtraEffectEnabled(const char* Name, bool Value);
	void					SetPhaseLumCoeff(int phaseLength, int phaseDay);


	int						GameDay;
	int						InitFrameCount;
	int						InitFrameTarget;
	struct					EffectQuad { float x, y, z; float u, v; };
	ShaderConstants			ShaderConst;
	CustomConstants			CustomConst;
	CellLocation			LocationState;
	bool					DialogState;
	IDirect3DTexture9*		SourceTexture;
	IDirect3DSurface9*		SourceSurface;
	IDirect3DTexture9* 		RenderedTexture;
	IDirect3DSurface9*		RenderedSurface;
	IDirect3DTexture9*		RenderTextureSMAA;
	IDirect3DSurface9*		RenderSurfaceSMAA;
	bool					RenderedBufferFilled;
	bool					DepthBufferFilled;
	bool					isFullyInitialized;
	bool					UseIntervalUpdate;
	TESObjectCELL*			previousCell;
	float					previousBlend;
	//Begin Volume Light
	float					previousModifier = 1.0f;
	float					currentModifier = 1.0f;
	bool					modifiersSet = false;
	//End Volume Light
	SettingsWaterStruct*	sws;
	SettingsAmbientOcclusionStruct* sas;
	SettingsBloomStruct* sbs;
	SettingsColoringStruct* scs;
	ShaderConstants::SimpleLightingStruct	InteriorLighting;
	IDirect3DVertexBuffer9*	EffectVertex;
	EffectRecord*			UnderwaterEffect;
	EffectRecord*			WaterLensEffect;
	EffectRecord*			GodRaysEffect;
	EffectRecord*			MasserRaysEffect;
	EffectRecord*			SecundaRaysEffect;
	EffectRecord*			DepthOfFieldEffect;
	EffectRecord*			AmbientOcclusionEffect;
	EffectRecord*			ColoringEffect;
	EffectRecord*			CinemaEffect;
	EffectRecord*			BloomEffect;
	EffectRecord*			SnowAccumulationEffect;
	EffectRecord*			SMAAEffect;
	EffectRecord*			MotionBlurEffect;
	EffectRecord*			WetWorldEffect;
	EffectRecord*			SharpeningEffect;
	EffectRecord*			VolumetricFogEffect;
	EffectRecord*			VolumetricLightEffect;
	EffectRecord*			RainEffect;
	EffectRecord*			SnowEffect;
	EffectRecord*			ShadowsExteriorsEffect;
	EffectRecord*           ShadowsExteriorsPointEffect;
	EffectRecord*			ShadowsExteriorsPointDialogEffect;
	EffectRecord*			ShadowsInteriorsEffect;
	ExtraEffectsList		ExtraEffects;
	NiD3DVertexShader*		WaterHeightMapVertexShader;
	NiD3DPixelShader*		WaterHeightMapPixelShader;
	NiD3DVertexShader*		WaterVertexShaders[51];
	NiD3DPixelShader*		WaterPixelShaders[51];
};