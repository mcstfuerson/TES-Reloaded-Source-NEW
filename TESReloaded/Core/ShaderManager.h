#pragma once
#include <d3dx9mesh.h>

enum EffectRecordType
{
	EffectRecordType_Underwater,
	EffectRecordType_WaterLens,
	EffectRecordType_GodRays,
	EffectRecordType_DepthOfField,
	EffectRecordType_AmbientOcclusion,
	EffectRecordType_Coloring,
	EffectRecordType_Cinema,
	EffectRecordType_Bloom,
	EffectRecordType_SnowAccumulation,
	EffectRecordType_BloodLens,
	EffectRecordType_SMAA,
	EffectRecordType_MotionBlur,
	EffectRecordType_LowHF,
	EffectRecordType_WetWorld,
	EffectRecordType_Sharpening,
	EffectRecordType_VolumetricFog,
	EffectRecordType_Precipitations,
	EffectRecordType_Extra,
};

enum ShaderType
{
	ShaderType_Vertex,
	ShaderType_Pixel,
};

struct ShaderConstants {
	D3DXVECTOR4		ReciprocalResolution;
	D3DXVECTOR4		ReciprocalResolutionWater;
	D3DXVECTOR4		SunDir;
	D3DXVECTOR4		SunTiming;
	D3DXVECTOR4		SunAmount;
	D3DXVECTOR4		GameTime;
	D3DXVECTOR4		Tick;
	D3DXVECTOR4		TextureData;
	TESWeather*		pWeather;
	float			currentsunGlare;
	float			currentwindSpeed;
	UInt8			oldsunGlare;
	UInt8			oldwindSpeed;
	D3DXVECTOR4		fogColor;
	D3DXVECTOR4		oldfogColor;
	D3DXVECTOR4		sunColor;
	D3DXVECTOR4		oldsunColor;
	D3DXVECTOR4		fogData;
	float			currentfogStart;
	float			oldfogStart;
	float			currentfogEnd;
	float			oldfogEnd;
	bool			HasWater;
	D3DXMATRIXA16	ShadowWorld;
	D3DXMATRIX		ShadowViewProj;
	D3DXMATRIX		ShadowCameraToLight[3];
	D3DXVECTOR4		ShadowCubeMapLightPosition;
	D3DXVECTOR4		ShadowLightPosition[4];
	D3DXVECTOR4		ShadowCubeMapFarPlanes;
	D3DXVECTOR4		Water_waterCoefficients;
	D3DXVECTOR4		Water_waveParams;
	D3DXVECTOR4		Water_waterVolume;
	D3DXVECTOR4		Water_waterSettings;
	D3DXVECTOR4		Water_deepColor;
	D3DXVECTOR4		Water_shallowColor;
	D3DXVECTOR4		Water_shorelineParams;
	D3DXVECTOR4		HDR_ToneMapping;
	D3DXVECTOR4		Grass_Scale;
	D3DXVECTOR4		POM_ParallaxData;
	D3DXVECTOR4		Terrain_Data;
	D3DXVECTOR4		Skin_SkinData;
	D3DXVECTOR4		Skin_SkinColor;
	D3DXVECTOR4		Shadow_Data;
	D3DXVECTOR4		Rain_Data;
	D3DXVECTOR4		Snow_Data;
	D3DXVECTOR4		WaterLens_Time;
	float			WaterLens_TimeAmount;
	float			WaterLens_Percent;
	D3DXVECTOR4		GodRays_Ray;
	D3DXVECTOR4		GodRays_RayColor;
	D3DXVECTOR4		GodRays_Data;
	bool			DepthOfField_Enabled;
	D3DXVECTOR4		DepthOfField_Blur;
	D3DXVECTOR4		DepthOfField_Data;
	bool			AmbientOcclusion_Enabled;
	D3DXVECTOR4		AmbientOcclusion_AOData;
	D3DXVECTOR4		AmbientOcclusion_Data;
	D3DXVECTOR4		Coloring_ColorCurve;
	D3DXVECTOR4		Coloring_EffectGamma;
	D3DXVECTOR4		Coloring_Data;
	D3DXVECTOR4		Coloring_Values;
	D3DXVECTOR4		Cinema_Data;
	D3DXVECTOR4		Bloom_BloomData;
	D3DXVECTOR4		Bloom_BloomValues;
	D3DXVECTOR4		SnowAccumulation_Params;
	D3DXVECTOR4		BloodLens_Params;
	D3DXVECTOR4		BloodLens_BloodColor;
	D3DXVECTOR4		BloodLens_Time;
	float			BloodLens_Percent;
	D3DXVECTOR4		MotionBlur_BlurParams;
	D3DXVECTOR4		MotionBlur_Data;
	float			MotionBlur_oldAngleX;
	float			MotionBlur_oldAngleZ;
	float			MotionBlur_oldAmountX;
	float			MotionBlur_oldAmountY;
	float			MotionBlur_oldoldAmountX;
	float			MotionBlur_oldoldAmountY;
	D3DXVECTOR4		LowHF_Data;
	float			LowHF_HealthCoeff;
	float			LowHF_FatigueCoeff;
	D3DXVECTOR4		WetWorld_Coeffs;
	D3DXVECTOR4		WetWorld_Data;
	D3DXVECTOR4		Sharpening_Data;
	D3DXVECTOR4		VolumetricFog_Data;
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
	bool					LoadShader(const char* Name);
	
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
	void					BeginScene();
	void					CreateShader(const char *Name);
	void					LoadShader(NiD3DVertexShader* Shader);
	void					LoadShader(NiD3DPixelShader* Shader);
	void					DisposeShader(const char* Name);
	void					CreateEffect(EffectRecordType EffectType);
	bool					LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName);
	void					DisposeEffect(EffectRecord* TheEffect);
	void					RenderEffects(IDirect3DSurface9* RenderTarget);
	void					SwitchShaderStatus(const char* Name);
	void					SetCustomConstant(const char* Name, D3DXVECTOR4 Value);
	void					SetExtraEffectEnabled(const char* Name, bool Value);

	struct					EffectQuad { float x, y, z; float u, v; };
	LONGLONG				PerformanceFrequency;
	ShaderConstants			ShaderConst;
	CustomConstants			CustomConst;
	IDirect3DTexture9*		SourceTexture;
	IDirect3DSurface9*		SourceSurface;
	IDirect3DTexture9* 		RenderedTexture;
	IDirect3DSurface9*		RenderedSurface;
	IDirect3DTexture9*		RenderTextureSMAA;
	IDirect3DSurface9*		RenderSurfaceSMAA;
	bool					RenderedBufferFilled;
	bool					DepthBufferFilled;
	IDirect3DVertexBuffer9*	EffectVertex;
	EffectRecord*			UnderwaterEffect;
	EffectRecord*			WaterLensEffect;
	EffectRecord*			GodRaysEffect;
	EffectRecord*			DepthOfFieldEffect;
	EffectRecord*			AmbientOcclusionEffect;
	EffectRecord*			ColoringEffect;
	EffectRecord*			CinemaEffect;
	EffectRecord*			BloomEffect;
	EffectRecord*			SnowAccumulationEffect;
	EffectRecord*			BloodLensEffect;
	EffectRecord*			SMAAEffect;
	EffectRecord*			MotionBlurEffect;
	EffectRecord*			LowHFEffect;
	EffectRecord*			WetWorldEffect;
	EffectRecord*			SharpeningEffect;
	EffectRecord*			VolumetricFogEffect;
	EffectRecord*			RainEffect;
	EffectRecord*			SnowEffect;
	ExtraEffectsList		ExtraEffects;
	NiD3DVertexShader*		WaterHeightMapVertexShader;
	NiD3DPixelShader*		WaterHeightMapPixelShader;
	NiD3DVertexShader*		WaterVertexShaders[51];
	NiD3DPixelShader*		WaterPixelShaders[51];
};