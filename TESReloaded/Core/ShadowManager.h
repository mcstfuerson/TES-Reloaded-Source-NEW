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
	void					GetFrustum(ShadowMapTypeEnum ShadowMapType, D3DMATRIX* Matrix);
	bool					InFrustum(ShadowMapTypeEnum ShadowMapType, NiAVObject* Object);
	TESObjectREFR*			GetRef(TESObjectREFR* Ref, SettingsShadowStruct::FormsStruct* Forms, SettingsShadowStruct::ExcludedFormsList* ExcludedForms);
	TESObjectREFR*			GetRefO(TESObjectREFR* Ref);
	void					RenderObject(NiAVObject* Node, D3DXVECTOR4* ShadowData, bool HasWater);
	void					RenderObjectPoint(NiAVObject* Node, D3DXVECTOR4* ShadowData, bool HasWater);
	void					RenderTerrain(NiAVObject* Object, ShadowMapTypeEnum ShadowMapType, D3DXVECTOR4* ShadowData);
	void					Render(NiGeometry* Geo, D3DXVECTOR4* ShadowData);
	void					RenderShadowMap(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXVECTOR3* At, D3DXVECTOR4* SunDir, D3DXVECTOR4* ShadowData);
	void					RenderShadowCubeMapExt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsExteriors, D3DXVECTOR4* ShadowData);
	void					RenderShadowCubeMapInt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors, D3DXVECTOR4* ShadowData);
	void                    RenderShadowCubeMapFakeInt(int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors, D3DXVECTOR4* ShadowData);
	void                    RenderShadowCubeMap(int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, D3DXVECTOR4* ShadowData, bool enabled);
	void					RenderExteriorShadows();
	void					RenderInteriorShadows();
	void					RenderShadowMaps();
	void					ClearShadowMap(IDirect3DDevice9* Device);
	void					ClearShadowCubeMaps(IDirect3DDevice9* Device, int From, ShadowCubeMapStateEnum NewState);
	void					ClearShadowCubeMaps(IDirect3DDevice9* Device, int LightIndex);
	void					CalculateBlend(NiPointLight** Lights, int LightIndex);
	void                    AddSceneLight(NiPointLight* Light, int Key, std::map<int, NiPointLight*>& SceneLights);
	int                     GetExtSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** Lights, int LightIndex);
	int                     GetShadowSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings);
	void                    SetAllShadowMapLightPos(NiPointLight** Lights, int LightIndex);
	void                    SetShadowMapLightPos(NiPointLight** Lights, int index);
	void                    SetShadowCubeMapRegisters(int index);
	void					ResetIntervals();



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
	IDirect3DCubeTexture9*	ShadowCubeMapTexture[12];
	IDirect3DSurface9*		ShadowCubeMapSurface[12][6];
	IDirect3DSurface9*		ShadowCubeMapDepthSurface;
	ShaderRecord*			ShadowCubeMapVertex;
	ShaderRecord*			ShadowCubeMapPixel;
	IDirect3DVertexShader9* ShadowCubeMapVertexShader;
	IDirect3DPixelShader9*	ShadowCubeMapPixelShader;
	D3DVIEWPORT9			ShadowCubeMapViewPort;
	NiPointLight*			ShadowCubeMapLights[12];
	ShaderRecord*			CurrentVertex;
	ShaderRecord*			CurrentPixel;
	TESObjectCELL*			CurrentCell;
	ShadowCubeMapStateEnum	ShadowCubeMapState;
	bool					AlphaEnabled;
	bool					FakeExtShadowLightDirSet;
	D3DXVECTOR4				FakeExtShadowLightDir;
	D3DXVECTOR4				ShadowLightDirOld;
	D3DXVECTOR4				ShadowLightDirNew;
	bool					UpdateShadowLightDir;
	float					UpdateTargetTime;
	D3DXVECTOR3				LookAtPosition;
	int                     ShadowCubeLightCount;
	int						GameHour;
	float					GameTime;
};

void CreateShadowsHook();
void CreateEditorShadowsHook();