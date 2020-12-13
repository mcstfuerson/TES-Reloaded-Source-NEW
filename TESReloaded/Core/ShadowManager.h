#pragma once

class ShadowManager { // Never disposed
public:
	ShadowManager();
	
	enum ShadowMapTypeEnum {
		MapNear		= 0,
		MapFar		= 1,
		MapOrtho	= 2,
	};
	enum ShadowCubeMapStateEnum {
		None		= 0,
		Exterior	= 1,
		Interior	= 2,
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
	void					RenderShadowCubeMap(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowsInteriors);
	void					RenderShadowMaps();
	void					ClearShadowCubeMaps(IDirect3DDevice9* Device, int From, ShadowCubeMapStateEnum NewState);
	void					CalculateBlend(NiPointLight** Lights, int LightIndex);

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
	IDirect3DCubeTexture9*	ShadowCubeMapTexture[4];
	IDirect3DSurface9*		ShadowCubeMapSurface[4][6];
	IDirect3DSurface9*		ShadowCubeMapDepthSurface;
	ShaderRecord*			ShadowCubeMapVertex;
	ShaderRecord*			ShadowCubeMapPixel;
	IDirect3DVertexShader9* ShadowCubeMapVertexShader;
	IDirect3DPixelShader9*	ShadowCubeMapPixelShader;
	D3DVIEWPORT9			ShadowCubeMapViewPort;
	NiPointLight*			ShadowCubeMapLights[4];
	ShaderRecord*			CurrentVertex;
	ShaderRecord*			CurrentPixel;
	TESObjectCELL*			CurrentCell;
	ShadowCubeMapStateEnum	ShadowCubeMapState;
	bool					AlphaEnabled;
};

void CreateShadowsHook();
void CreateEditorShadowsHook();