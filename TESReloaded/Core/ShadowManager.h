#pragma once
#include <list>

class ShadowManager { // Never disposed
public:
	ShadowManager();

	enum ShadowMapTypeEnum {
		MapNear		= 0,
		MapFar		= 1,
		MapOrtho	= 2,
		MapSkin     = 3,
	};
	enum ShadowCubeMapStateEnum {
		None		   = 0,
		Exterior	   = 1,
		Exterior_Night = 2,
		Interior	   = 3
	};
	enum PlaneEnum {
		PlaneNear	= 0,
		PlaneFar	= 1,
		PlaneLeft	= 2,
		PlaneRight	= 3,
		PlaneTop	= 4,
		PlaneBottom	= 5,
	};

	void					CreateD3DMatrix(D3DMATRIX* Matrix, NiTransform* Transform);
	void					GetShadowFrustum(ShadowMapTypeEnum ShadowMapType, D3DMATRIX* Matrix);
	bool					InShadowFrustum(ShadowMapTypeEnum ShadowMapType, NiAVObject* Object);
	TESObjectREFR*			GetRef(TESObjectREFR* Ref, SettingsShadowStruct::FormsStruct* Forms, SettingsShadowStruct::ExcludedFormsList* ExcludedForms);
	TESObjectREFR*			GetRefO(TESObjectREFR* Ref);
	void					RenderObject(NiAVObject* Node, D3DXVECTOR4* ShadowData, bool HasWater, float MinRadius);
	void					RenderObjectPoint(NiAVObject* Node, D3DXVECTOR4* ShadowData, bool HasWater);
	void					RenderObjectPointActor(NiAVObject* Node, D3DXVECTOR4* ShadowData, bool HasWater, int lightIndex);
	void					RenderTerrain(NiAVObject* Object, ShadowMapTypeEnum ShadowMapType, D3DXVECTOR4* ShadowData);
	void					Render(NiGeometry* Geo, D3DXVECTOR4* ShadowData);
	void					RenderActor(NiGeometry* Geo, D3DXVECTOR4* ShadowData, int lightIndex);
	void					RenderShadowMap(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXVECTOR3* At, D3DXVECTOR4* SunDir, D3DXVECTOR4* ShadowData);
	void					RenderShadowCubeMapExt(NiPointLight** Lights, int LightIndex, float radiusLimit, SettingsShadowStruct::InteriorsStruct* ShadowsExteriors, D3DXVECTOR4* ShadowData);
	void					RenderShadowCubeMapInt(NiPointLight** Lights, int LightIndex, float radiusLimit, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors, D3DXVECTOR4* ShadowData);
	void                    RenderShadowCubeMapFakeInt(int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors, D3DXVECTOR4* ShadowData);
	void                    RenderShadowCubeMap(int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, D3DXVECTOR4* ShadowData, bool enabled);
	void                    RenderShadowCubeMapActor(int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, D3DXVECTOR4* ShadowData, bool enabled);
	void					RenderExteriorShadows();
	void					RenderInteriorShadows();
	void					RenderShadowMaps();
	void					ClearShadowMap(IDirect3DDevice9* Device);
	void					ClearShadowCubeMaps(IDirect3DDevice9* Device, int From, ShadowCubeMapStateEnum NewState);
	void					ClearShadowCubeMaps(IDirect3DDevice9* Device, int LightIndex);
	void					ClearShadowCubeLightRegister(int From);
	void					ClearShadowCubeLightCullRegister(int From);
	void					ClearGeneralPointLightRegister(int From);
	void                    AddSceneLight(NiPointLight* Light, int Key, std::map<int, NiPointLight*>& SceneLights);
	int                     GetShadowSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** ShadowCastLights, NiPointLight** ShadowCullLights, NiPointLight** GeneralPointLights, int& ShadowCastLightIndex, int& ShadowCullLightIndex, int& GeneralPointLightIndex, SettingsShadowPointLightsStruct* ShadowSettings);
	void                    SetAllShadowCastLightPos(NiPointLight** Lights, int LightIndex);
	void                    SetShadowCastLightPos(NiPointLight** Lights, int index);
	void                    SetAllShadowCullLightPos(NiPointLight** Lights, int LightIndex);
	void                    SetShadowCullLightPos(NiPointLight** Lights, int index);
	void					SetAllGeneralLightPos(NiPointLight** Lights, int LightIndex);
	void					SetGeneralLightPos(NiPointLight** Lights, int index);
	void                    SetShadowCubeMapRegisters(int index);
	void					ResetIntervals();
	void					LoadShadowLightPointSettings();
	bool					IsLightFromMagic(NiPointLight* Light);

	IDirect3DTexture9*		ShadowMapTexture[4];
	IDirect3DSurface9*		ShadowMapSurface[4];
	IDirect3DSurface9*		ShadowMapDepthSurface[4];
	ShaderRecord*			ShadowMapVertex;
	ShaderRecord*			ShadowMapPixel;
	IDirect3DVertexShader9* ShadowMapVertexShader;
	IDirect3DPixelShader9*  ShadowMapPixelShader;
	D3DVIEWPORT9			ShadowMapViewPort[4];
	D3DXPLANE				ShadowMapFrustum[4][6];
	NiVector4				BillboardRight;
	NiVector4				BillboardUp;

	//-------SHADOW CUBE MAP--------

	//TEXTURES
	IDirect3DCubeTexture9*	ShadowCubeMapTexture[12];

	//SURFACES
	IDirect3DSurface9*		ShadowCubeMapSurface[12][6];
	IDirect3DSurface9*		ShadowCubeMapDepthSurface[12][6];

	//SHADERS
	ShaderRecord*			ShadowCubeMapVertex;
	ShaderRecord*			ShadowCubeMapPixel;
	IDirect3DVertexShader9* ShadowCubeMapVertexShader;
	IDirect3DPixelShader9*	ShadowCubeMapPixelShader;
	ShaderRecord*			ShadowCubeMapExteriorPixel;
	IDirect3DPixelShader9*	ShadowCubeMapExteriorPixelShader;

	//MISC
	D3DVIEWPORT9			ShadowCubeMapViewPort;
	NiPointLight*			ShadowCubeMapLights[12];
	int                     ShadowCubeLightCount;
	int						ShadowCubeCullLightCount;
	double					ShadowCubeMapStaticValue[12];
	bool					ShadowCubeMapStaticTracker[12];
	bool					EnableStaticMaps;
	int						EnableStaticMapsFrameCount;
	int						EnableStaticMapsFrameThreshold = 30;

	//-------END SHADOW CUBE--------

	ShaderRecord*			CurrentVertex;
	ShaderRecord*			CurrentPixel;
	TESObjectCELL*			CurrentCell;
	ShadowCubeMapStateEnum	ShadowCubeMapState;
	bool					AlphaEnabled;
	bool					FakeExtShadowLightDirSet;
	int						FakeExtShadowLightDirCnt;
	D3DXVECTOR4				FakeExtShadowLightDir;
	D3DXVECTOR4				ShadowLightDirOld;
	D3DXVECTOR4				ShadowLightDirNew;
	bool					UpdateShadowLightDir;
	float					UpdateTargetTime;
	D3DXVECTOR3				LookAtPosition;
	int						GeneralPointLightCount;
	float					GameTime;
	SettingsShadowPointLightsStruct* ShadowLightPointSettings;
};

void CreateShadowsHook();
void CreateEditorShadowsHook();