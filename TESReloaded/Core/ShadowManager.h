#pragma once
#include <list>

class ShadowManager { // Never disposed
public:
	ShadowManager();
	
	enum ShadowMapTypeEnum {
		MapNear		= 0,
		MapFar		= 1,
		MapOrtho	= 2,
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
	void					RenderObject(NiAVObject* Node, bool HasWater);
	void					Render(NiGeometry* Geo);
	void					RenderShadowMap(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXVECTOR3* At, D3DXVECTOR4* SunDir);
	void					RenderShadowCubeMapExt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsExteriors);
	void					RenderShadowCubeMapInt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors);
	void                    RenderShadowCubeMap(NiPointLight** Lights, int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, bool enabled);
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



	IDirect3DTexture9*		ShadowMapTexture[3];
	IDirect3DSurface9*		ShadowMapSurface[3];
	IDirect3DSurface9*		ShadowMapDepthSurface[3];
	ShaderRecord*			ShadowMapVertex;
	ShaderRecord*			ShadowMapPixel;
	IDirect3DVertexShader9* ShadowMapVertexShader;
	IDirect3DPixelShader9*  ShadowMapPixelShader;
	D3DVIEWPORT9			ShadowMapViewPort[3];
	D3DXPLANE				ShadowMapFrustum[3][6];
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
	int                     ShadowCubeLightCount;
};

void CreateShadowsHook();
void CreateEditorShadowsHook();