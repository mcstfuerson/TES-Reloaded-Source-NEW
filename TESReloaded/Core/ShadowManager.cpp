#include <algorithm>
#include <cmath>
#include <list>
#include <string>
#include <thread>
#include <sstream>
#if defined(NEWVEGAS)
#define RenderStateArgs 0, 0
#define kRockParams 0x01200658
#define kRustleParams 0x01200668
#define kWindMatrixes 0x01200688
#define kShadowSceneNode 0x011F91C8
static const UInt32 kRenderShadowMapHook = 0x00870C39;
static const UInt32 kRenderShadowMapReturn = 0x00870C41;
static const UInt32 kAddCastShadowFlagHook = 0x0050DD06;
static const UInt32 kAddCastShadowFlagReturn = 0x0050DD0B;
static const UInt32 kEditorCastShadowFlagHook = 0x00000000;
static const UInt32 kEditorCastShadowFlagReturn = 0x00000000;
static const UInt32 kLeavesNodeName = 0x0066A115;
static const UInt32 kLeavesNodeNameReturn = 0x0066A11E;
static const void* VFTNiNode = (void*)0x0109B5AC;
static const void* VFTBSFadeNode = (void*)0x010A8F90;
static const void* VFTBSFaceGenNiNode = (void*)0x010660DC;
static const void* VFTBSTreeNode = (void*)0x010668E4;
static const void* VFTNiTriShape = (void*)0x0109D454;
static const void* VFTNiTriStrips = (void*)0x0109CD44;
#elif defined(OBLIVION)
#define RenderStateArgs 0
#define kRockParams 0x00B46778
#define kRustleParams 0x00B46788
#define kWindMatrixes 0x00B467B8
#define kShadowSceneNode 0x00B42F54
static const UInt32 kRenderShadowMapHook = 0x0040C919;
static const UInt32 kRenderShadowMapReturn = 0x0040C920;
static const UInt32 kAddCastShadowFlagHook = 0x004B1A25;
static const UInt32 kAddCastShadowFlagReturn = 0x004B1A2A;
static const UInt32 kEditorCastShadowFlagHook = 0x005498DD;
static const UInt32 kEditorCastShadowFlagReturn = 0x005498E3;
static const void* VFTNiNode = (void*)0x00A7E38C;
static const void* VFTBSFadeNode = (void*)0x00A3F944;
static const void* VFTBSFaceGenNiNode = (void*)0x00A64F5C;
static const void* VFTBSTreeNode = (void*)0x00A65854;
static const void* VFTNiTriShape = (void*)0x00A7ED5C;
static const void* VFTNiTriStrips = (void*)0x00A7F27C;
#endif
#define ShadowMapObjectMinBound 10.0f

#if defined(NEWVEGAS) || defined(OBLIVION)
ShadowManager::ShadowManager() {

	Logger::Log("Starting the shadows manager...");
	TheShadowManager = this;

	IDirect3DDevice9* Device = TheRenderManager->device;
	SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors = &TheSettingManager->SettingsShadows.Exteriors;
	SettingsShadowStruct::InteriorsStruct* ShadowsInteriors = &TheSettingManager->SettingsShadows.Interiors;
	SettingsShadowStruct::InteriorsStruct* ShadowsExteriorsPoint = &TheSettingManager->SettingsShadows.ExteriorsPoint;

	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.x = TheSettingManager->SettingsShadows.Exteriors.forwardNormBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.y = TheSettingManager->SettingsShadows.Exteriors.forwardFarNormBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.z = TheSettingManager->SettingsShadows.Exteriors.forwardConstBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.w = TheSettingManager->SettingsShadows.Exteriors.forwardFarConstBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.x = TheSettingManager->SettingsShadows.Exteriors.deferredNormBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.y = TheSettingManager->SettingsShadows.Exteriors.deferredFarNormBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.z = TheSettingManager->SettingsShadows.Exteriors.deferredConstBias;
	TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.w = TheSettingManager->SettingsShadows.Exteriors.deferredFarConstBias;

	UINT ShadowMapSize = 0;

	//TODO: should this setting be on it's own? choose smaller of two for now
	UINT ShadowCubeMapSize = min(ShadowsInteriors->ShadowCubeMapSize, ShadowsExteriorsPoint->ShadowCubeMapSize);

	CurrentCell = NULL;
	ShadowCubeMapState = ShadowCubeMapStateEnum::None;
	int ShadowCubeLightCount = 0;
	int ShadowCubeCullLightCount = 0;

	ShadowMapVertex = new ShaderRecord();
	if (ShadowMapVertex->LoadShader("ShadowMap.vso")) Device->CreateVertexShader((const DWORD*)ShadowMapVertex->Function, &ShadowMapVertexShader);
	ShadowMapPixel = new ShaderRecord();
	if (ShadowMapPixel->LoadShader("ShadowMap.pso")) Device->CreatePixelShader((const DWORD*)ShadowMapPixel->Function, &ShadowMapPixelShader);

	ShadowCubeMapVertex = new ShaderRecord();
	if (ShadowCubeMapVertex->LoadShader("ShadowCubeMap.vso")) Device->CreateVertexShader((const DWORD*)ShadowCubeMapVertex->Function, &ShadowCubeMapVertexShader);
	ShadowCubeMapPixel = new ShaderRecord();
	if (ShadowCubeMapPixel->LoadShader("ShadowCubeMap.pso")) Device->CreatePixelShader((const DWORD*)ShadowCubeMapPixel->Function, &ShadowCubeMapPixelShader);
	ShadowCubeMapExteriorPixel = new ShaderRecord();
	if (ShadowCubeMapExteriorPixel->LoadShader("ShadowCubeMapExterior.pso")) Device->CreatePixelShader((const DWORD*)ShadowCubeMapExteriorPixel->Function, &ShadowCubeMapExteriorPixelShader);

	for (int i = 0; i < 4; i++) {
		UINT ShadowMapSize = ShadowsExteriors->ShadowMapSize[i];
		Device->CreateTexture(ShadowMapSize, ShadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &ShadowMapTexture[i], NULL);
		ShadowMapTexture[i]->GetSurfaceLevel(0, &ShadowMapSurface[i]);
		Device->CreateDepthStencilSurface(ShadowMapSize, ShadowMapSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &ShadowMapDepthSurface[i], NULL);
		ShadowMapViewPort[i] = { 0, 0, ShadowMapSize, ShadowMapSize, 0.0f, 1.0f };
	}
	for (int i = 0; i < 12; i++) {
		Device->CreateCubeTexture(ShadowCubeMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &ShadowCubeMapTexture[i], NULL);
		for (int j = 0; j < 6; j++) {
			ShadowCubeMapTexture[i]->GetCubeMapSurface((D3DCUBEMAP_FACES)j, 0, &ShadowCubeMapSurface[i][j]);
			Device->CreateDepthStencilSurface(ShadowCubeMapSize, ShadowCubeMapSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &ShadowCubeMapDepthSurface[i][j], NULL);
		}
	}

	ShadowCubeMapViewPort = { 0, 0, ShadowCubeMapSize, ShadowCubeMapSize, 0.0f, 1.0f };

	ShadowCubeMapLights[12] = { NULL };

	ResetIntervals();

}

void ShadowManager::CreateD3DMatrix(D3DMATRIX* Matrix, NiTransform* Transform) {

	NiMatrix33* Rot = &Transform->rot;
	NiPoint3* Pos = &Transform->pos;
	float Scale = Transform->scale;

	Matrix->_11 = Rot->data[0][0] * Scale;
	Matrix->_12 = Rot->data[1][0] * Scale;
	Matrix->_13 = Rot->data[2][0] * Scale;
	Matrix->_14 = 0.0f;
	Matrix->_21 = Rot->data[0][1] * Scale;
	Matrix->_22 = Rot->data[1][1] * Scale;
	Matrix->_23 = Rot->data[2][1] * Scale;
	Matrix->_24 = 0.0f;
	Matrix->_31 = Rot->data[0][2] * Scale;
	Matrix->_32 = Rot->data[1][2] * Scale;
	Matrix->_33 = Rot->data[2][2] * Scale;
	Matrix->_34 = 0.0f;
	Matrix->_41 = Pos->x - TheRenderManager->CameraPosition.x;
	Matrix->_42 = Pos->y - TheRenderManager->CameraPosition.y;
	Matrix->_43 = Pos->z - TheRenderManager->CameraPosition.z;
	Matrix->_44 = 1.0f;

}

void ShadowManager::GetShadowFrustum(ShadowMapTypeEnum ShadowMapType, D3DMATRIX* Matrix) {

	ShadowMapFrustum[ShadowMapType][PlaneNear].a = Matrix->_13;
	ShadowMapFrustum[ShadowMapType][PlaneNear].b = Matrix->_23;
	ShadowMapFrustum[ShadowMapType][PlaneNear].c = Matrix->_33;
	ShadowMapFrustum[ShadowMapType][PlaneNear].d = Matrix->_43;
	ShadowMapFrustum[ShadowMapType][PlaneFar].a = Matrix->_14 - Matrix->_13;
	ShadowMapFrustum[ShadowMapType][PlaneFar].b = Matrix->_24 - Matrix->_23;
	ShadowMapFrustum[ShadowMapType][PlaneFar].c = Matrix->_34 - Matrix->_33;
	ShadowMapFrustum[ShadowMapType][PlaneFar].d = Matrix->_44 - Matrix->_43;
	ShadowMapFrustum[ShadowMapType][PlaneLeft].a = Matrix->_14 + Matrix->_11;
	ShadowMapFrustum[ShadowMapType][PlaneLeft].b = Matrix->_24 + Matrix->_21;
	ShadowMapFrustum[ShadowMapType][PlaneLeft].c = Matrix->_34 + Matrix->_31;
	ShadowMapFrustum[ShadowMapType][PlaneLeft].d = Matrix->_44 + Matrix->_41;
	ShadowMapFrustum[ShadowMapType][PlaneRight].a = Matrix->_14 - Matrix->_11;
	ShadowMapFrustum[ShadowMapType][PlaneRight].b = Matrix->_24 - Matrix->_21;
	ShadowMapFrustum[ShadowMapType][PlaneRight].c = Matrix->_34 - Matrix->_31;
	ShadowMapFrustum[ShadowMapType][PlaneRight].d = Matrix->_44 - Matrix->_41;
	ShadowMapFrustum[ShadowMapType][PlaneTop].a = Matrix->_14 - Matrix->_12;
	ShadowMapFrustum[ShadowMapType][PlaneTop].b = Matrix->_24 - Matrix->_22;
	ShadowMapFrustum[ShadowMapType][PlaneTop].c = Matrix->_34 - Matrix->_32;
	ShadowMapFrustum[ShadowMapType][PlaneTop].d = Matrix->_44 - Matrix->_42;
	ShadowMapFrustum[ShadowMapType][PlaneBottom].a = Matrix->_14 + Matrix->_12;
	ShadowMapFrustum[ShadowMapType][PlaneBottom].b = Matrix->_24 + Matrix->_22;
	ShadowMapFrustum[ShadowMapType][PlaneBottom].c = Matrix->_34 + Matrix->_32;
	ShadowMapFrustum[ShadowMapType][PlaneBottom].d = Matrix->_44 + Matrix->_42;
	for (int i = 0; i < 6; ++i) {
		D3DXPLANE Plane(ShadowMapFrustum[ShadowMapType][i]);
		D3DXPlaneNormalize(&ShadowMapFrustum[ShadowMapType][i], &Plane);
	}

}

TESObjectREFR* ShadowManager::GetRef(TESObjectREFR* Ref, SettingsShadowStruct::FormsStruct* Forms, SettingsShadowStruct::ExcludedFormsList* ExcludedForms) {

	TESObjectREFR* r = NULL;

	if (Ref && Ref->GetNode()) {
		TESForm* Form = Ref->baseForm;
		if (!(Ref->flags & TESForm::FormFlags::kFormFlags_NotCastShadows)) {
			UInt8 TypeID = Form->formType;
			if ((TypeID == TESForm::FormType::kFormType_Activator && Forms->Activators) ||
				(TypeID == TESForm::FormType::kFormType_Apparatus && Forms->Apparatus) ||
				(TypeID == TESForm::FormType::kFormType_Book && Forms->Books) ||
				(TypeID == TESForm::FormType::kFormType_Container && Forms->Containers) ||
				(TypeID == TESForm::FormType::kFormType_Door && Forms->Doors) ||
				(TypeID == TESForm::FormType::kFormType_Misc && Forms->Misc) ||
				(TypeID == TESForm::FormType::kFormType_Stat && Forms->Statics) ||
				(TypeID == TESForm::FormType::kFormType_Tree && Forms->Trees) ||
				(TypeID == TESForm::FormType::kFormType_Furniture && Forms->Furniture) ||
				(TypeID >= TESForm::FormType::kFormType_NPC && TypeID <= TESForm::FormType::kFormType_LeveledCreature && Forms->Actors))
				r = Ref;
			if (r && ExcludedForms->size() > 0 && std::binary_search(ExcludedForms->begin(), ExcludedForms->end(), Form->refID)) r = NULL;
		}
	}
	return r;

}

TESObjectREFR* ShadowManager::GetRefO(TESObjectREFR* Ref) {

	TESObjectREFR* r = NULL;

	if (Ref && Ref->GetNode()) {
		UInt8 TypeID = Ref->baseForm->formType;
		if (TypeID == TESForm::FormType::kFormType_Stat) r = Ref;
	}
	return r;

}

bool ShadowManager::InShadowFrustum(ShadowMapTypeEnum ShadowMapType, NiAVObject* Object) {

	float Distance = 0.0f;
	bool R = false;
	NiBound* Bound = Object->GetWorldBound();

	if (Bound) {
		D3DXVECTOR3 Position = { Bound->Center.x - TheRenderManager->CameraPosition.x, Bound->Center.y - TheRenderManager->CameraPosition.y, Bound->Center.z - TheRenderManager->CameraPosition.z };

		R = true;
		for (int i = 0; i < 6; ++i) {
			Distance = D3DXPlaneDotCoord(&ShadowMapFrustum[ShadowMapType][i], &Position);
			if (Distance <= -Bound->Radius) {
				R = false;
				break;
			}
		}
		if (ShadowMapType == MapFar && R) { // Ensures to not be fully in the near frustum
			for (int i = 0; i < 6; ++i) {
				Distance = D3DXPlaneDotCoord(&ShadowMapFrustum[MapNear][i], &Position);
				if (Distance <= -Bound->Radius || std::fabs(Distance) < Bound->Radius) {
					R = false;
					break;
				}
			}
			R = !R;
		}
	}
	return R;

}
void ShadowManager::RenderObject(NiAVObject* Object, D3DXVECTOR4* ShadowData, bool HasWater, float MinRadius) {

	float Radius = Object->GetWorldBoundRadius();


	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled) && Radius >= MinRadius) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode || VFT == VFTBSFadeNode || VFT == VFTBSFaceGenNiNode || VFT == VFTBSTreeNode) {
			NiNode* Node = (NiNode*)Object;
			for (int i = 0; i < Node->m_children.end; i++) {
				RenderObject(Node->m_children.data[i], ShadowData, HasWater, MinRadius);
			}
		}
		else if (VFT == VFTNiTriShape || VFT == VFTNiTriStrips) {
			NiGeometry* Geo = (NiGeometry*)Object;
			if (Geo->shader) {
				if (!HasWater || (HasWater && Geo->GetWorldBound()->Center.z > 0.0f)) {
					NiGeometryBufferData* GeoData = Geo->geomData->BuffData;
					if (GeoData) {
						Render(Geo, ShadowData);
					}
					else if (Geo->skinInstance && Geo->skinInstance->SkinPartition && Geo->skinInstance->SkinPartition->Partitions) {
						GeoData = Geo->skinInstance->SkinPartition->Partitions[0].BuffData;
						if (GeoData) Render(Geo, ShadowData);
					}
				}
			}
		}
	}

}

void ShadowManager::RenderObjectPoint(NiAVObject* Object, D3DXVECTOR4* ShadowData, bool HasWater) {

	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled)) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode || VFT == VFTBSFadeNode || VFT == VFTBSFaceGenNiNode || VFT == VFTBSTreeNode) {
			NiNode* Node = (NiNode*)Object;
			for (int i = 0; i < Node->m_children.end; i++) {
				RenderObjectPoint(Node->m_children.data[i], ShadowData, HasWater);
			}
		}
		else if (VFT == VFTNiTriShape || VFT == VFTNiTriStrips) {
			NiGeometry* Geo = (NiGeometry*)Object;
			if (Geo->shader) {
				if (!HasWater || (HasWater && Geo->GetWorldBound()->Center.z > TheShaderManager->ShaderConst.Water.waterSettings.x)) {
					NiGeometryBufferData* GeoData = Geo->geomData->BuffData;
					if (GeoData) {
						Render(Geo, ShadowData);
					}
					else if (Geo->skinInstance && Geo->skinInstance->SkinPartition && Geo->skinInstance->SkinPartition->Partitions) {
						GeoData = Geo->skinInstance->SkinPartition->Partitions[0].BuffData;
						if (GeoData) Render(Geo, ShadowData);
					}
				}
			}
		}
	}

}

void ShadowManager::RenderObjectPointActor(NiAVObject* Object, D3DXVECTOR4* ShadowData, bool HasWater, int lightIndex) {

	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled)) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode || VFT == VFTBSFadeNode || VFT == VFTBSFaceGenNiNode || VFT == VFTBSTreeNode) {
			NiNode* Node = (NiNode*)Object;
			for (int i = 0; i < Node->m_children.end; i++) {
				RenderObjectPointActor(Node->m_children.data[i], ShadowData, HasWater, lightIndex);
			}
		}
		else if (VFT == VFTNiTriShape || VFT == VFTNiTriStrips) {
			NiGeometry* Geo = (NiGeometry*)Object;
			if (Geo->shader) {
				if (!HasWater || (HasWater && Geo->GetWorldBound()->Center.z > TheShaderManager->ShaderConst.Water.waterSettings.x)) {
					NiGeometryBufferData* GeoData = Geo->geomData->BuffData;
					if (GeoData) {
						RenderActor(Geo, ShadowData, lightIndex);
					}
					else if (Geo->skinInstance && Geo->skinInstance->SkinPartition && Geo->skinInstance->SkinPartition->Partitions) {
						GeoData = Geo->skinInstance->SkinPartition->Partitions[0].BuffData;
						if (GeoData) RenderActor(Geo, ShadowData, lightIndex);
					}
				}
			}
		}
	}

}

void ShadowManager::RenderTerrain(NiAVObject* Object, ShadowMapTypeEnum ShadowMapType, D3DXVECTOR4* ShadowData) {

	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled)) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode) {
			NiNode* Node = (NiNode*)Object;
			if (InShadowFrustum(ShadowMapType, Node)) {
				for (int i = 0; i < Node->m_children.end; i++) {
					RenderTerrain(Node->m_children.data[i], ShadowMapType, ShadowData);
				}
			}
		}
		else if (VFT == VFTNiTriShape || VFT == VFTNiTriStrips) {
			Render((NiGeometry*)Object, ShadowData);
		}
	}

}

void ShadowManager::Render(NiGeometry* Geo, D3DXVECTOR4* ShadowData) {

	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	int StartIndex = 0;
	int PrimitiveCount = 0;
	int StartRegister = 9;
	NiGeometryData* ModelData = Geo->geomData;
	NiGeometryBufferData* GeoData = ModelData->BuffData;
	NiSkinInstance* SkinInstance = Geo->skinInstance;
	NiD3DShaderDeclaration* ShaderDeclaration = Geo->shader->ShaderDeclaration;

	if (Geo->m_pcName && !memcmp(Geo->m_pcName, "Torch", 5)) return; // No torch geo, it is too near the light and a bad square is rendered.

	ShadowData->x = 0.0f; // Type of geo (0 normal, 1 actors (skinned), 2 speedtree leaves)
	ShadowData->y = 0.0f; // Alpha control
	if (GeoData) {
		CreateD3DMatrix(&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld, &Geo->m_worldTransform);
		if (Geo->m_parent->m_pcName && !memcmp(Geo->m_parent->m_pcName, "Leaves", 6)) {
			NiVector4* RockParams = (NiVector4*)kRockParams;
			NiVector4* RustleParams = (NiVector4*)kRustleParams;
			NiVector4* WindMatrixes = (NiVector4*)kWindMatrixes;
			SpeedTreeLeafShaderProperty* STProp = (SpeedTreeLeafShaderProperty*)Geo->GetProperty(NiProperty::PropertyType::kType_Lighting);
			BSTreeNode* Node = (BSTreeNode*)Geo->m_parent->m_parent;
			NiDX9SourceTextureData* Texture = (NiDX9SourceTextureData*)Node->TreeModel->LeavesTexture->rendererData;

			ShadowData->x = 2.0f;
			Device->SetVertexShaderConstantF(63, (float*)&BillboardRight, 1);
			Device->SetVertexShaderConstantF(64, (float*)&BillboardUp, 1);
			Device->SetVertexShaderConstantF(65, (float*)RockParams, 1);
			Device->SetVertexShaderConstantF(66, (float*)RustleParams, 1);
			Device->SetVertexShaderConstantF(67, (float*)WindMatrixes, 16);
			Device->SetVertexShaderConstantF(83, STProp->leafData->leafBase, 48);
			RenderState->SetTexture(0, Texture->dTexture);
			RenderState->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP, false);
			RenderState->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP, false);
			RenderState->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT, false);
			RenderState->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT, false);
			RenderState->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT, false);
		}
		else {
			BSShaderProperty* LProp = (BSShaderProperty*)Geo->GetProperty(NiProperty::PropertyType::kType_Lighting);
			if (!LProp || !LProp->IsLightingProperty()) return;
			if (AlphaEnabled) {
				NiAlphaProperty* AProp = (NiAlphaProperty*)Geo->GetProperty(NiProperty::PropertyType::kType_Alpha);
				if (AProp->flags & NiAlphaProperty::AlphaFlags::ALPHA_BLEND_MASK || AProp->flags & NiAlphaProperty::AlphaFlags::TEST_ENABLE_MASK) {
					if (NiTexture* Texture = *((BSShaderPPLightingProperty*)LProp)->textures[0]) {
						ShadowData->y = 1.0f;
						RenderState->SetTexture(0, Texture->rendererData->dTexture);
						RenderState->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP, false);
						RenderState->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP, false);
						RenderState->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT, false);
						RenderState->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT, false);
						RenderState->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT, false);
					}
				}
			}
		}
		TheRenderManager->PackGeometryBuffer(GeoData, ModelData, SkinInstance, ShaderDeclaration);
		for (UInt32 i = 0; i < GeoData->StreamCount; i++) {
			Device->SetStreamSource(i, GeoData->VBChip[i]->VB, 0, GeoData->VertexStride[i]);
		}
		Device->SetIndices(GeoData->IB);
		if (GeoData->FVF)
			RenderState->SetFVF(GeoData->FVF, false);
		else
			RenderState->SetVertexDeclaration(GeoData->VertexDeclaration, false);
		CurrentVertex->SetCT();
		CurrentPixel->SetCT();
		for (UInt32 i = 0; i < GeoData->NumArrays; i++) {
			if (GeoData->ArrayLengths)
				PrimitiveCount = GeoData->ArrayLengths[i] - 2;
			else
				PrimitiveCount = GeoData->TriCount;
			Device->DrawIndexedPrimitive(GeoData->PrimitiveType, GeoData->BaseVertexIndex, 0, GeoData->VertCount, StartIndex, PrimitiveCount);
			StartIndex += PrimitiveCount + 2;
		}
	}
	else {
		ShadowData->x = 1.0f;
		NiSkinPartition* SkinPartition = SkinInstance->SkinPartition;
		D3DPRIMITIVETYPE PrimitiveType = (SkinPartition->Partitions[0].Strips == 0) ? D3DPT_TRIANGLELIST : D3DPT_TRIANGLESTRIP;
		TheRenderManager->CalculateBoneMatrixes(SkinInstance, &Geo->m_worldTransform);
		if (SkinInstance->SkinToWorldWorldToSkin) memcpy(&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld, SkinInstance->SkinToWorldWorldToSkin, 0x40);
		for (UInt32 p = 0; p < SkinPartition->PartitionsCount; p++) {
			StartIndex = 0;
			StartRegister = 9;
			NiSkinPartition::Partition* Partition = &SkinPartition->Partitions[p];
			for (int i = 0; i < Partition->Bones; i++) {
				UInt16 NewIndex = (Partition->pBones == NULL) ? i : Partition->pBones[i];
				Device->SetVertexShaderConstantF(StartRegister, ((float*)SkinInstance->BoneMatrixes) + (NewIndex * 3 * 4), 3);
				StartRegister += 3;
			}
			GeoData = Partition->BuffData;
			TheRenderManager->PackSkinnedGeometryBuffer(GeoData, ModelData, SkinInstance, Partition, ShaderDeclaration);

			for (UInt32 i = 0; i < GeoData->StreamCount; i++) {
				Device->SetStreamSource(i, GeoData->VBChip[i]->VB, 0, GeoData->VertexStride[i]);
			}
			Device->SetIndices(GeoData->IB);
			if (GeoData->FVF)
				RenderState->SetFVF(GeoData->FVF, false);
			else
				RenderState->SetVertexDeclaration(GeoData->VertexDeclaration, false);
			CurrentVertex->SetCT();
			CurrentPixel->SetCT();
			for (UInt32 i = 0; i < GeoData->NumArrays; i++) {
				if (GeoData->ArrayLengths)
					PrimitiveCount = GeoData->ArrayLengths[i] - 2;
				else
					PrimitiveCount = GeoData->TriCount;
				Device->DrawIndexedPrimitive(PrimitiveType, GeoData->BaseVertexIndex, 0, Partition->Vertices, StartIndex, PrimitiveCount);
				StartIndex += PrimitiveCount + 2;
			}
		}
	}

}

void ShadowManager::RenderActor(NiGeometry* Geo, D3DXVECTOR4* ShadowData, int lightIndex) {

	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	int StartIndex = 0;
	int PrimitiveCount = 0;
	int StartRegister = 9;

	D3DXMATRIX View, Proj;
	D3DXVECTOR3 Eye, At, Up;
	float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].w;
	D3DXMatrixPerspectiveFovRH(&Proj, D3DXToRadian(90.0f), 1.0f, 1.0f, FarPlane);

	NiGeometryData* ModelData = Geo->geomData;
	NiGeometryBufferData* GeoData = ModelData->BuffData;
	NiSkinInstance* SkinInstance = Geo->skinInstance;
	NiD3DShaderDeclaration* ShaderDeclaration = Geo->shader->ShaderDeclaration;

	if (Geo->m_pcName && !memcmp(Geo->m_pcName, "Torch", 5)) return; // No torch geo, it is too near the light and a bad square is rendered.

	ShadowData->x = 0.0f; // Type of geo (0 normal, 1 actors (skinned), 2 speedtree leaves)
	ShadowData->y = 0.0f; // Alpha control
	if (GeoData) {
		CreateD3DMatrix(&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld, &Geo->m_worldTransform);
		BSShaderProperty* LProp = (BSShaderProperty*)Geo->GetProperty(NiProperty::PropertyType::kType_Lighting);
		if (AlphaEnabled) {
			NiAlphaProperty* AProp = (NiAlphaProperty*)Geo->GetProperty(NiProperty::PropertyType::kType_Alpha);
			if (AProp->flags & NiAlphaProperty::AlphaFlags::ALPHA_BLEND_MASK || AProp->flags & NiAlphaProperty::AlphaFlags::TEST_ENABLE_MASK) {
				if (NiTexture* Texture = *((BSShaderPPLightingProperty*)LProp)->textures[0]) {
					ShadowData->y = 1.0f;
					RenderState->SetTexture(0, Texture->rendererData->dTexture);
					RenderState->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP, false);
					RenderState->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP, false);
					RenderState->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT, false);
					RenderState->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT, false);
					RenderState->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT, false);
				}
			}
		}
		TheRenderManager->PackGeometryBuffer(GeoData, ModelData, SkinInstance, ShaderDeclaration);
		for (UInt32 i = 0; i < GeoData->StreamCount; i++) {
			Device->SetStreamSource(i, GeoData->VBChip[i]->VB, 0, GeoData->VertexStride[i]);
		}
		Device->SetIndices(GeoData->IB);
		if (GeoData->FVF)
			RenderState->SetFVF(GeoData->FVF, false);
		else
			RenderState->SetVertexDeclaration(GeoData->VertexDeclaration, false);

		for (int Face = 0; Face < 6; Face++) {

			At.x = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].x;
			At.y = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].y;
			At.z = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].z;
			Eye = At;
			switch (Face) {
			case D3DCUBEMAP_FACE_POSITIVE_X:
				At += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_X:
				At += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_POSITIVE_Y:
				At += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_Y:
				At += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				break;
			case D3DCUBEMAP_FACE_POSITIVE_Z:
				At += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_Z:
				At += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			}
			D3DXMatrixLookAtRH(&View, &Eye, &At, &Up);
			TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj = View * Proj;
			Device->SetDepthStencilSurface(ShadowCubeMapDepthSurface[lightIndex][Face]);
			Device->SetRenderTarget(0, ShadowCubeMapSurface[lightIndex][Face]);
			CurrentVertex->SetCT();
			CurrentPixel->SetCT();

			for (UInt32 i = 0; i < GeoData->NumArrays; i++) {
				if (GeoData->ArrayLengths)
					PrimitiveCount = GeoData->ArrayLengths[i] - 2;
				else
					PrimitiveCount = GeoData->TriCount;

				Device->DrawIndexedPrimitive(GeoData->PrimitiveType, GeoData->BaseVertexIndex, 0, GeoData->VertCount, StartIndex, PrimitiveCount);
				StartIndex += PrimitiveCount + 2;
			}
			StartIndex = 0;
		}
	}
	else {
		ShadowData->x = 1.0f;
		NiSkinPartition* SkinPartition = SkinInstance->SkinPartition;
		D3DPRIMITIVETYPE PrimitiveType = (SkinPartition->Partitions[0].Strips == 0) ? D3DPT_TRIANGLELIST : D3DPT_TRIANGLESTRIP;
		TheRenderManager->CalculateBoneMatrixes(SkinInstance, &Geo->m_worldTransform);
		if (SkinInstance->SkinToWorldWorldToSkin) memcpy(&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld, SkinInstance->SkinToWorldWorldToSkin, 0x40);
		for (UInt32 p = 0; p < SkinPartition->PartitionsCount; p++) {
			StartIndex = 0;
			StartRegister = 9;
			NiSkinPartition::Partition* Partition = &SkinPartition->Partitions[p];
			for (int i = 0; i < Partition->Bones; i++) {
				UInt16 NewIndex = (Partition->pBones == NULL) ? i : Partition->pBones[i];
				Device->SetVertexShaderConstantF(StartRegister, ((float*)SkinInstance->BoneMatrixes) + (NewIndex * 3 * 4), 3);
				StartRegister += 3;
			}
			GeoData = Partition->BuffData;
			TheRenderManager->PackSkinnedGeometryBuffer(GeoData, ModelData, SkinInstance, Partition, ShaderDeclaration);

			for (UInt32 i = 0; i < GeoData->StreamCount; i++) {
				Device->SetStreamSource(i, GeoData->VBChip[i]->VB, 0, GeoData->VertexStride[i]);
			}
			Device->SetIndices(GeoData->IB);
			if (GeoData->FVF)
				RenderState->SetFVF(GeoData->FVF, false);
			else
				RenderState->SetVertexDeclaration(GeoData->VertexDeclaration, false);

			for (int Face = 0; Face < 6; Face++) {
				At.x = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].x;
				At.y = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].y;
				At.z = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[lightIndex].z;
				Eye = At;
				switch (Face) {
				case D3DCUBEMAP_FACE_POSITIVE_X:
					At += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
					Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					break;
				case D3DCUBEMAP_FACE_NEGATIVE_X:
					At += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
					Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					break;
				case D3DCUBEMAP_FACE_POSITIVE_Y:
					At += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
					break;
				case D3DCUBEMAP_FACE_NEGATIVE_Y:
					At += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
					Up = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
					break;
				case D3DCUBEMAP_FACE_POSITIVE_Z:
					At += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
					Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					break;
				case D3DCUBEMAP_FACE_NEGATIVE_Z:
					At += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
					Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					break;
				}
				D3DXMatrixLookAtRH(&View, &Eye, &At, &Up);
				TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj = View * Proj;
				Device->SetDepthStencilSurface(ShadowCubeMapDepthSurface[lightIndex][Face]);
				Device->SetRenderTarget(0, ShadowCubeMapSurface[lightIndex][Face]);
				CurrentVertex->SetCT();
				CurrentPixel->SetCT();

				for (UInt32 i = 0; i < GeoData->NumArrays; i++) {
					if (GeoData->ArrayLengths)
						PrimitiveCount = GeoData->ArrayLengths[i] - 2;
					else
						PrimitiveCount = GeoData->TriCount;

					Device->DrawIndexedPrimitive(PrimitiveType, GeoData->BaseVertexIndex, 0, Partition->Vertices, StartIndex, PrimitiveCount);
					StartIndex += PrimitiveCount + 2;
				}

				StartIndex = 0;
			}
		}
	}

}

void ShadowManager::RenderShadowMap(ShadowMapTypeEnum ShadowMapType, SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors, D3DXVECTOR3* At, D3DXVECTOR4* ShadowLightDir, D3DXVECTOR4* ShadowData) {

	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	float FarPlane = ShadowsExteriors->ShadowMapFarPlane;
	float Radius = ShadowsExteriors->ShadowMapRadius[ShadowMapType];
	D3DXVECTOR3 Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXMATRIX View, Proj;
	D3DXVECTOR3 Eye;
	GridCellArray* CellArray = Tes->gridCellArray;
	UInt32 CellArraySize = CellArray->size * CellArray->size;

	AlphaEnabled = ShadowsExteriors->AlphaEnabled[ShadowMapType];

	Eye.x = At->x - FarPlane * ShadowLightDir->x * -1;
	Eye.y = At->y - FarPlane * ShadowLightDir->y * -1;
	Eye.z = At->z - FarPlane * ShadowLightDir->z * -1;
	D3DXMatrixLookAtRH(&View, &Eye, At, &Up);
	D3DXMatrixOrthoRH(&Proj, 2.0f * Radius, (1 + ShadowLightDir->z) * Radius, 0.0f, 2.0f * FarPlane);
	TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj = View * Proj;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[ShadowMapType] = TheRenderManager->InvViewProjMatrix * TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj;
	BillboardRight = { View._11, View._21, View._31, 0.0f };
	BillboardUp = { View._12, View._22, View._32, 0.0f };
	GetShadowFrustum(ShadowMapType, &TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj);
	Device->SetRenderTarget(0, ShadowMapSurface[ShadowMapType]);
	Device->SetDepthStencilSurface(ShadowMapDepthSurface[ShadowMapType]);
	Device->SetViewport(&ShadowMapViewPort[ShadowMapType]);
	Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
	if (ShadowsExteriors->Enabled[ShadowMapType]) {
		RenderState->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE, RenderStateArgs);
		RenderState->SetRenderState(D3DRS_ZWRITEENABLE, 1, RenderStateArgs);
		RenderState->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE, RenderStateArgs);
		RenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, 0, RenderStateArgs);
		RenderState->SetVertexShader(ShadowMapVertexShader, false);
		RenderState->SetPixelShader(ShadowMapPixelShader, false);
		Device->BeginScene();
		for (UInt32 x = 0; x < *SettingGridsToLoad; x++) {
			for (UInt32 y = 0; y < *SettingGridsToLoad; y++) {
				if (TESObjectCELL* Cell = Tes->gridCellArray->GetCell(x, y)) {
					NiNode* CellNode = Cell->niNode;
					for (int i = 2; i < 6; i++) {
						NiNode* TerrainNode = (NiNode*)CellNode->m_children.data[i];
						if (TerrainNode->m_children.end) RenderTerrain(TerrainNode->m_children.data[0], ShadowMapType, ShadowData);
					}
					TList<TESObjectREFR>::Entry* Entry = &Cell->objectList.First;
					while (Entry) {
						if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowsExteriors->Forms[ShadowMapType], &ShadowsExteriors->ExcludedForms)) {
							NiNode* RefNode = Ref->GetNode();
							if (InShadowFrustum(ShadowMapType, RefNode)) {
								switch (ShadowMapType) {
								case ShadowMapTypeEnum::MapNear:
									RenderObject(RefNode, ShadowData, TheShaderManager->ShaderConst.HasWater, 9.0f);
									break;
								case ShadowMapTypeEnum::MapFar:
									RenderObject(RefNode, ShadowData, TheShaderManager->ShaderConst.HasWater, 100.0f);
									break;
								case ShadowMapTypeEnum::MapSkin:
									RenderObject(RefNode, ShadowData, TheShaderManager->ShaderConst.HasWater, 0.0f);
									break;
								case ShadowMapTypeEnum::MapOrtho:
									RenderObject(RefNode, ShadowData, TheShaderManager->ShaderConst.HasWater, 100.0f);
									break;
								}
							}
						}
						Entry = Entry->next;
					}
				}
			}
		}
		Device->EndScene();
	}

}

void ShadowManager::RenderShadowCubeMapExt(NiPointLight** Lights, int LightIndex, float radiusScan, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {

	std::map<int, std::vector<NiNode*>> refMap;
	std::map<int, std::vector<NiNode*>> actorMap;
	float radius;
	double StaticValues[12] = { 0 };
	bool forceRedrawMap[12] = { false };
	bool isActorType = false;

	for (UInt32 x = 0; x < *SettingGridsToLoad - 1; x++) {
		for (UInt32 y = 0; y < *SettingGridsToLoad; y++) {
			if (TESObjectCELL* Cell = Tes->gridCellArray->GetCell(x, y)) {
				TList<TESObjectREFR>::Entry* Entry = &Cell->objectList.First;
				while (Entry) {
					if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowSettings->Forms, &ShadowSettings->ExcludedForms)) {

						for (int L = 0; L <= LightIndex; L++) {
							NiPoint3* LightPos = &Lights[L]->m_worldTransform.pos;
							float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].w;

							UInt8 TypeID = Ref->baseForm->formType;
							isActorType = (TypeID >= TESForm::FormType::kFormType_NPC && TypeID <= TESForm::FormType::kFormType_LeveledCreature);

							if ((Lights[L]->CanCarry) || isActorType) {
								radius = FarPlane * 1.2f;
							}
							else {
								radius = FarPlane * radiusScan;
							}

							if (Ref->GetNode()->GetDistance(LightPos) - Ref->GetNode()->GetWorldBoundRadius() <= radius) {
								if (Lights[L]->CanCarry) {
									forceRedrawMap[L] = true;
								}
								if (isActorType) {
									actorMap[L].emplace_back(Ref->GetNode());
								}
								else {
									refMap[L].emplace_back(Ref->GetNode());
								}

								StaticValues[L] += (((double)Ref->niNode->GetWorldBound()->Center.x) + ((double)Ref->niNode->GetWorldBound()->Center.y) + ((double)Ref->niNode->GetWorldBound()->Center.z));
							}


						}
					}
					Entry = Entry->next;
				}
			}
		}
	}
	for (int i = 0; i <= LightIndex; i++) {
		if (StaticValues[i] == ShadowCubeMapStaticValue[i]) {
			ShadowCubeMapStaticTracker[i] = true && !forceRedrawMap[i];
		}
		else {
			ShadowCubeMapStaticTracker[i] = false;
			ShadowCubeMapStaticValue[i] = StaticValues[i];
		}
	}
	RenderShadowCubeMap(LightIndex, refMap, ShadowData, ShadowSettings->Enabled);
	RenderShadowCubeMapActor(LightIndex, actorMap, ShadowData, ShadowSettings->Enabled);
}

void ShadowManager::RenderShadowCubeMapInt(NiPointLight** Lights, int LightIndex, float radiusScan, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {

	std::map<int, std::vector<NiNode*>> refMap;
	std::map<int, std::vector<NiNode*>> actorMap;
	float radius;
	bool isActorType = false;
	TList<TESObjectREFR>::Entry* Entry = &Player->parentCell->objectList.First;
	double StaticValues[12] = { 0 };
	bool forceRedrawMap[12] = { false };

	while (Entry) {
		if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowSettings->Forms, &ShadowSettings->ExcludedForms)) {
			for (int L = 0; L <= LightIndex; L++) {
				NiPoint3* LightPos = &Lights[L]->m_worldTransform.pos;
				float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].w;


				UInt8 TypeID = Ref->baseForm->formType;
				isActorType = (TypeID >= TESForm::FormType::kFormType_NPC && TypeID <= TESForm::FormType::kFormType_LeveledCreature);

				if ((Lights[L]->CanCarry) || isActorType) {
					radius = FarPlane * 1.2f;
				}
				else {
					radius = FarPlane * radiusScan;
				}

				float distance = Ref->GetNode()->GetDistance(LightPos);
				if (distance - Ref->GetNode()->GetWorldBoundRadius() <= radius) {
					if (Lights[L]->CanCarry) {
						forceRedrawMap[L] = true;
					}
					if (isActorType) {
						actorMap[L].emplace_back(Ref->GetNode());
					}
					else {
						refMap[L].emplace_back(Ref->GetNode());
					}

					StaticValues[L] += (((double)Ref->niNode->GetWorldBound()->Center.x) + ((double)Ref->niNode->GetWorldBound()->Center.y) + ((double)Ref->niNode->GetWorldBound()->Center.z));
				}
			}
		}
		Entry = Entry->next;
	}

	for (int i = 0; i <= LightIndex; i++) {
		if (StaticValues[i] == ShadowCubeMapStaticValue[i]) {
			ShadowCubeMapStaticTracker[i] = true && !forceRedrawMap[i];
		}
		else {
			ShadowCubeMapStaticTracker[i] = false;
			ShadowCubeMapStaticValue[i] = StaticValues[i];
		}
	}
	RenderShadowCubeMap(LightIndex, refMap, ShadowData, ShadowSettings->Enabled);
	RenderShadowCubeMapActor(LightIndex, actorMap, ShadowData, ShadowSettings->Enabled);
}

void ShadowManager::RenderShadowCubeMapFakeInt(int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {
	D3DXVECTOR3 At, Eye;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;

	if (!FakeExtShadowLightDirSet) {
		if (FakeExtShadowLightDirCnt > 4) {
			FakeExtShadowLightDir = TheRenderManager->CameraPosition;
			FakeExtShadowLightDirSet = true;
		}
		FakeExtShadowLightDirCnt++;
	}

	Eye.x = ((ShadowLightDir->x * 4000) + FakeExtShadowLightDir.x) - TheRenderManager->CameraPosition.x;
	Eye.y = ((ShadowLightDir->y * 4000) + FakeExtShadowLightDir.y) - TheRenderManager->CameraPosition.y;
	Eye.z = ((ShadowLightDir->z * 4000) + FakeExtShadowLightDir.z) - TheRenderManager->CameraPosition.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[LightIndex].x = Eye.x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[LightIndex].y = Eye.y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[LightIndex].z = Eye.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[LightIndex].w = 15000;

	std::map<int, std::vector<NiNode*>> refMap;
	TList<TESObjectREFR>::Entry* Entry = &Player->parentCell->objectList.First;
	while (Entry) {
		if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowSettings->Forms, &ShadowSettings->ExcludedForms)) {
			refMap[LightIndex].emplace_back(Ref->GetNode());
		}
		Entry = Entry->next;
	}
	RenderShadowCubeMap(LightIndex, refMap, ShadowData, ShadowSettings->Enabled);
}


void ShadowManager::RenderShadowCubeMap(int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, D3DXVECTOR4* ShadowData, bool enabled) {
	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	D3DXMATRIX View, Proj;
	D3DXVECTOR3 Eye, At, Up;


	for (int L = 0; L <= LightIndex; L++) {

		SetShadowCubeMapRegisters(L);

		if (ShadowCubeMapStaticTracker[L] && EnableStaticMaps) {
			continue;
		}

		float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].w;
		D3DXMatrixPerspectiveFovRH(&Proj, D3DXToRadian(90.0f), 1.0f, 1.0f, FarPlane);
		for (int Face = 0; Face < 6; Face++) {
			Device->SetDepthStencilSurface(ShadowCubeMapDepthSurface[L][Face]);
			At.x = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].x;
			At.y = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].y;
			At.z = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[L].z;
			Eye = At;
			switch (Face) {
			case D3DCUBEMAP_FACE_POSITIVE_X:
				At += D3DXVECTOR3(1.0f, 0.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_X:
				At += D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_POSITIVE_Y:
				At += D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_Y:
				At += D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				Up = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				break;
			case D3DCUBEMAP_FACE_POSITIVE_Z:
				At += D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			case D3DCUBEMAP_FACE_NEGATIVE_Z:
				At += D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				break;
			}
			D3DXMatrixLookAtRH(&View, &Eye, &At, &Up);
			TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj = View * Proj;
			Device->SetRenderTarget(0, ShadowCubeMapSurface[L][Face]);
			Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
			if (enabled) {
				Device->BeginScene();
				RenderState->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE, RenderStateArgs);
				RenderState->SetRenderState(D3DRS_ZWRITEENABLE, 1, RenderStateArgs);
				RenderState->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE, RenderStateArgs);
				RenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, 0, RenderStateArgs);
				Device->SetViewport(&ShadowCubeMapViewPort);
				RenderState->SetVertexShader(ShadowCubeMapVertexShader, false);
				if (Player->GetWorldSpace()) {
					RenderState->SetPixelShader(ShadowCubeMapExteriorPixelShader, false);
				}
				else {
					RenderState->SetPixelShader(ShadowCubeMapPixelShader, false);
				}
				std::vector<NiNode*>::iterator RefNode;
				for (RefNode = refMap[L].begin(); RefNode != refMap[L].end(); ++RefNode) {
					RenderObjectPoint((*RefNode), ShadowData, TheShaderManager->ShaderConst.HasWater);
				}
				Device->EndScene();
			}
		}
	}
}

void ShadowManager::RenderShadowCubeMapActor(int LightIndex, std::map<int, std::vector<NiNode*>>& refMap, D3DXVECTOR4* ShadowData, bool enabled) {
	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	D3DXMATRIX View, Proj;
	D3DXVECTOR3 Eye, At, Up;


	for (int L = 0; L <= LightIndex; L++) {

		SetShadowCubeMapRegisters(L);

		if (ShadowCubeMapStaticTracker[L] && EnableStaticMaps) {
			continue;
		}

		if (enabled) {
			Device->BeginScene();
			RenderState->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE, RenderStateArgs);
			RenderState->SetRenderState(D3DRS_ZWRITEENABLE, 1, RenderStateArgs);
			RenderState->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE, RenderStateArgs);
			RenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, 0, RenderStateArgs);
			Device->SetViewport(&ShadowCubeMapViewPort);
			RenderState->SetVertexShader(ShadowCubeMapVertexShader, false);
			if (Player->GetWorldSpace()) {
				RenderState->SetPixelShader(ShadowCubeMapExteriorPixelShader, false);
			}
			else {
				RenderState->SetPixelShader(ShadowCubeMapPixelShader, false);
			}
			std::vector<NiNode*>::iterator RefNode;
			for (RefNode = refMap[L].begin(); RefNode != refMap[L].end(); ++RefNode) {
				RenderObjectPointActor((*RefNode), ShadowData, TheShaderManager->ShaderConst.HasWater, L);
			}
			Device->EndScene();
		}
	}
}

//TODO: rename
void ShadowManager::RenderExteriorShadows() {

	if (!Player->GetWorldSpace()) {
		return;
	}

	SettingsShadowStruct::ExteriorsStruct* ShadowsExteriors = &TheSettingManager->SettingsShadows.Exteriors;
	IDirect3DDevice9* Device = TheRenderManager->device;
	D3DXVECTOR4* ShadowData = &TheShaderManager->ShaderConst.Shadow.Data;
	D3DXVECTOR4* ShadowSkinData = &TheShaderManager->ShaderConst.Shadow.ShadowSkinData;
	D3DXVECTOR4* OrthoData = &TheShaderManager->ShaderConst.Shadow.OrthoData;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;
	D3DXVECTOR4 ShadowLightDirInterval;

	D3DXVECTOR4 OrthoDir = D3DXVECTOR3(0.05f, 0.05f, 1.0f);
	NiNode* PlayerNode = Player->GetNode();
	D3DXVECTOR3 At, Eye, SkinAt;

	CurrentVertex = ShadowMapVertex;
	CurrentPixel = ShadowMapPixel;

	ClearShadowCubeMaps(Device, -1, ShadowCubeMapStateEnum::Exterior);

	SkinAt.x = PlayerNode->m_worldTransform.pos.x - TheRenderManager->CameraPosition.x;
	SkinAt.y = PlayerNode->m_worldTransform.pos.y - TheRenderManager->CameraPosition.y;
	SkinAt.z = PlayerNode->m_worldTransform.pos.z - TheRenderManager->CameraPosition.z;

	At.x = LookAtPosition.x - TheRenderManager->CameraPosition.x;
	At.y = LookAtPosition.y - TheRenderManager->CameraPosition.y;
	At.z = LookAtPosition.z - TheRenderManager->CameraPosition.z;
	D3DXVECTOR3 newPos(PlayerNode->m_worldTransform.pos.x, PlayerNode->m_worldTransform.pos.y, PlayerNode->m_worldTransform.pos.z);

	if (D3DXVec3Length(&(newPos - LookAtPosition)) > ShadowsExteriors->ShadowMapRadius[MapNear] / 2.0f) {
		LookAtPosition = newPos;
	}

	if (ShadowLightDir->z < 0.0f && TheShaderManager->ShaderConst.DayPhase == Dusk) {
		ShadowLightDir = &TheShaderManager->ShaderConst.MasserDir;
	}
	else if (ShadowLightDir->z < 0.0f && TheShaderManager->ShaderConst.DayPhase == Dawn) {
		ShadowLightDir = &TheShaderManager->ShaderConst.SunDir;
	}

	//reduces shadow artifacts caused by low z value
	if (ShadowLightDir->z < 0.3f) {
		ShadowLightDir->z = 0.3f;
	}
	if (TheSettingManager->SettingsShadows.Exteriors.UseIntervalUpdate && TheShaderManager->isFullyInitialized) {
		float NewGameTime = TheShaderManager->ShaderConst.GameTime.y;

		if (NewGameTime < GameTime) {
			NewGameTime += 24.0f;
		}

		float gameTimeDiff = NewGameTime - GameTime;
		if ((gameTimeDiff) > .10f) {
			if (GameTime > 0) {
				if (!(gameTimeDiff > .15f)) {
					UpdateShadowLightDir = true;
					UpdateTargetTime = NewGameTime + 0.025f;
					ShadowLightDirNew.x = ShadowLightDir->x;
					ShadowLightDirNew.y = ShadowLightDir->y;
					ShadowLightDirNew.z = ShadowLightDir->z;
					ShadowLightDirNew.w = ShadowLightDir->w;
					GameTime = NewGameTime;
				}
				else {
					TheShaderManager->isFullyInitialized = false;
					return;
				}
			}
			else {
				ShadowLightDirInterval.x = ShadowLightDir->x;
				ShadowLightDirInterval.y = ShadowLightDir->y;
				ShadowLightDirInterval.z = ShadowLightDir->z;
				ShadowLightDirInterval.w = ShadowLightDir->w;
				ShadowLightDirOld = ShadowLightDirInterval;
				UpdateShadowLightDir = false;
				GameTime = NewGameTime;
			}


		}

		if (UpdateShadowLightDir) {
			float newTime = NewGameTime - GameTime;
			float targetTime = UpdateTargetTime - GameTime;
			float t = newTime / targetTime;

			ShadowLightDirInterval.x = std::lerp(ShadowLightDirOld.x, ShadowLightDirNew.x, t);
			ShadowLightDirInterval.y = std::lerp(ShadowLightDirOld.y, ShadowLightDirNew.y, t);
			ShadowLightDirInterval.z = std::lerp(ShadowLightDirOld.z, ShadowLightDirNew.z, t);

			if (newTime >= targetTime) {
				UpdateShadowLightDir = false;
				ShadowLightDirOld = ShadowLightDirInterval;
				if (UpdateTargetTime >= 24.0f) {
					GameTime = 0.0f;
				}
			}
		}
		else {
			ShadowLightDirInterval = ShadowLightDirOld;
		}

		RenderShadowMap(MapNear, ShadowsExteriors, &At, &ShadowLightDirInterval, ShadowData);
		RenderShadowMap(MapFar, ShadowsExteriors, &At, &ShadowLightDirInterval, ShadowData);
		RenderShadowMap(MapSkin, ShadowsExteriors, &SkinAt, &ShadowLightDirInterval, ShadowData);
	}
	else {
		RenderShadowMap(MapNear, ShadowsExteriors, &At, ShadowLightDir, ShadowData);
		RenderShadowMap(MapFar, ShadowsExteriors, &At, ShadowLightDir, ShadowData);
		RenderShadowMap(MapSkin, ShadowsExteriors, &SkinAt, ShadowLightDir, ShadowData);
	}

	RenderShadowMap(MapOrtho, ShadowsExteriors, &At, &OrthoDir, ShadowData);

	//ShadowData->x = ShadowsExteriors->Quality;
	ShadowData->y = ShadowsExteriors->Darkness;

	ShadowData->z = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapNear];
	ShadowData->w = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapFar];
	OrthoData->z = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapOrtho];
	ShadowSkinData->z = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapSkin];
}

//TODO: rename, doesn't apply solely to interiors
void ShadowManager::RenderInteriorShadows() {

	IDirect3DDevice9* Device = TheRenderManager->device;
	D3DXVECTOR4* ShadowData = &TheShaderManager->ShaderConst.ShadowCube.Data;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;
	SettingsShadowStruct::InteriorsStruct* ShadowSettings;
	
	CurrentVertex = ShadowCubeMapVertex;

	if (Player->GetWorldSpace()) {
		ShadowSettings = &TheSettingManager->SettingsShadows.ExteriorsPoint;
		CurrentPixel = ShadowCubeMapExteriorPixel;
	}
	else {
		ShadowSettings = &TheSettingManager->SettingsShadows.Interiors;
		CurrentPixel = ShadowCubeMapPixel;
	}

	if (CurrentCell != Player->parentCell) {
		ShadowCubeMapState = ShadowCubeMapStateEnum::None; CurrentCell = Player->parentCell;
		LoadShadowLightPointSettings();
		EnableStaticMapsFrameCount = 0;
		EnableStaticMaps = false;
	}

	if (!EnableStaticMaps) {
		if (EnableStaticMapsFrameCount < EnableStaticMapsFrameThreshold) {
			EnableStaticMapsFrameCount++;
		}
		else {
			EnableStaticMaps = true;
		}
	}

	AlphaEnabled = ShadowSettings->AlphaEnabled;

	std::map<int, NiPointLight*> SceneLights;
	NiPointLight* ShadowCastLights[12] = { NULL };
	NiPointLight* ShadowCullLights[24] = { NULL };
	NiPointLight* GeneralPointLights[2] = { NULL }; //certainly a more appropriate place for this, but this is convenient
	int ShadowCastLightIndex = -1;
	int ShadowCullLightIndex = -1;
	int GeneralPointLightIndex = -1;

	if (ShadowLightPointSettings->bEnabled) {
		if (!(Player->parentCell->flags0 & Player->parentCell->kFlags0_BehaveLikeExterior && ShadowLightDir->z > 0.01f)) {
			FakeExtShadowLightDirSet = false;
			FakeExtShadowLightDirCnt = 0;
			GetShadowSceneLights(SceneLights, ShadowCastLights, ShadowCullLights, GeneralPointLights, ShadowCastLightIndex, ShadowCullLightIndex, GeneralPointLightIndex, ShadowLightPointSettings);
			SetAllShadowCastLightPos(ShadowCastLights, ShadowCastLightIndex);
			SetAllShadowCullLightPos(ShadowCullLights, ShadowCullLightIndex);
			if (Player->GetWorldSpace()) {
				RenderShadowCubeMapExt(ShadowCastLights, ShadowCastLightIndex, ShadowLightPointSettings->fShadowObjectScanRadius, ShadowSettings, ShadowData);
				SetAllGeneralLightPos(GeneralPointLights, GeneralPointLightIndex);
			}
			else {
				RenderShadowCubeMapInt(ShadowCastLights, ShadowCastLightIndex, ShadowLightPointSettings->fShadowObjectScanRadius, ShadowSettings, ShadowData);
				ClearShadowCubeMaps(Device, ShadowCastLightIndex, ShadowCubeMapStateEnum::Interior);
				GeneralPointLightIndex = -1; //not needed for interiors
			}
			ShadowData->y = ShadowSettings->Darkness;
		}
		else {
			ShadowCastLightIndex = 0;
			RenderShadowCubeMapFakeInt(ShadowCastLightIndex, ShadowSettings, ShadowData);
			ClearShadowCubeMaps(Device, ShadowCastLightIndex, ShadowCubeMapStateEnum::Interior);
			ShadowData->y = 2.0f;
		}
	}
	else {
		if (Player->GetWorldSpace()) { //set these to cull the normal exterior shadows
			GetShadowSceneLights(SceneLights, ShadowCastLights, ShadowCullLights, GeneralPointLights, ShadowCastLightIndex, ShadowCullLightIndex, GeneralPointLightIndex, ShadowLightPointSettings);
			SetAllShadowCastLightPos(ShadowCastLights, ShadowCastLightIndex);
			SetAllShadowCullLightPos(ShadowCullLights, ShadowCullLightIndex);
			SetAllGeneralLightPos(GeneralPointLights, GeneralPointLightIndex);
		}
		ShadowCastLightIndex = -1; //clears shadowmaps
	}

	if (ShadowCastLightIndex < ShadowCubeLightCount) {
		ClearShadowCubeMaps(Device, ShadowCastLightIndex);
	}

	if (ShadowCullLightIndex < ShadowCubeCullLightCount) {
		ClearShadowCubeLightCullRegister(ShadowCullLightIndex);
	}

	if (GeneralPointLightIndex < GeneralPointLightCount) {
		ClearGeneralPointLightRegister(GeneralPointLightIndex);
	}

	ShadowCubeLightCount = ShadowCastLightIndex;
	ShadowCubeCullLightCount = ShadowCullLightIndex;
	GeneralPointLightCount = GeneralPointLightIndex;

	//ShadowData->x = ShadowSettings->Quality;
	ShadowData->z = 1.0f / (float)ShadowSettings->ShadowCubeMapSize;
}

void ShadowManager::RenderShadowMaps() {
	Global->RenderShadowMaps(); //Window reflections seem to be rendered here

	IDirect3DDevice9* Device = TheRenderManager->device;
	IDirect3DSurface9* DepthSurface = NULL;

#if defined(OBLIVION)
	// This part "creates" a fake canopy map only one time to avoid random canopy shadows if i forgot to replace a shader.
	// By now i cannot disable the canopy map pass in Oblivion.ini otherwise the game changes the shaders used for the rendering.
	NiRenderedTexture* CanopyMap = *(NiRenderedTexture**)0x00B4310C;
	if (!CanopyMap) {
		NiRenderedTexture* (__cdecl * CreateNiRenderedTexture)(UInt32, UInt32, NiRenderer*, NiTexture::FormatPrefs*) = (NiRenderedTexture * (__cdecl*)(UInt32, UInt32, NiRenderer*, NiTexture::FormatPrefs*))0x0072A9B0;
		void(__cdecl * SetTextureCanopyMap)(NiRenderedTexture*) = (void(__cdecl*)(NiRenderedTexture*))0x00441850;
		NiTexture::FormatPrefs FP = { NiRenderedTexture::PixelLayout::kPixelLayout_TrueColor32, NiRenderedTexture::AlphaFormat::kAlpha_Smooth, NiRenderedTexture::MipMapFlag::kMipMap_Default };
		SetTextureCanopyMap(CreateNiRenderedTexture(1, 1, TheRenderManager, &FP));
	}
#endif

	Device->GetDepthStencilSurface(&DepthSurface);
	TheRenderManager->SetupSceneCamera();
	RenderExteriorShadows();
	RenderInteriorShadows();
	Device->SetDepthStencilSurface(DepthSurface);
}

void ShadowManager::ClearShadowMap(IDirect3DDevice9* Device) {
	Device->SetRenderTarget(0, ShadowMapSurface[MapNear]);
	Device->SetDepthStencilSurface(ShadowMapDepthSurface[MapNear]);
	Device->SetViewport(&ShadowMapViewPort[MapNear]);
	Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
	Device->SetRenderTarget(0, ShadowMapSurface[MapFar]);
	Device->SetDepthStencilSurface(ShadowMapDepthSurface[MapFar]);
	Device->SetViewport(&ShadowMapViewPort[MapFar]);
	Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
	Device->SetRenderTarget(0, ShadowMapSurface[MapOrtho]);
	Device->SetDepthStencilSurface(ShadowMapDepthSurface[MapOrtho]);
	Device->SetViewport(&ShadowMapViewPort[MapOrtho]);
	Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
	Device->SetRenderTarget(0, ShadowMapSurface[MapSkin]);
	Device->SetDepthStencilSurface(ShadowMapDepthSurface[MapSkin]);
	Device->SetViewport(&ShadowMapViewPort[MapSkin]);
	Device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
}

void ShadowManager::ClearShadowCubeMaps(IDirect3DDevice9* Device, int From, ShadowCubeMapStateEnum NewState) {
	if (ShadowCubeMapState != NewState) {
		ClearShadowMap(Device);
		ClearShadowCubeMaps(Device, From);
		ShadowCubeMapState = NewState;
	}
}

void ShadowManager::ClearShadowCubeLightRegister(int From) {

	int del1 = (12 - (From + 1)) * sizeof(D3DXVECTOR4);
	int del2 = (12 - (From + 1)) * sizeof(float);
	memset(TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition + ((From + 1)), 0, del1);
	memset(ShadowCubeMapStaticValue + ((From + 1)), 0, del2);
}

void ShadowManager::ClearShadowCubeLightCullRegister(int From) {

	int del = (24 - (From + 1)) * 16;
	memset(TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition + ((From + 1)), 0, del);
}

void ShadowManager::ClearGeneralPointLightRegister(int From) {

	int del = (2 - (From + 1)) * 16;
	memset(TheShaderManager->ShaderConst.PointLights.LightPosition + ((From + 1)), 0, del);
}

void ShadowManager::ClearShadowCubeMaps(IDirect3DDevice9* Device, int From) {

	ClearShadowCubeLightRegister(From);
	for (int L = From + 1; L < 12; L++) {
		for (int Face = 0; Face < 6; Face++) {
			Device->SetRenderTarget(0, ShadowCubeMapSurface[L][Face]);
			Device->Clear(0L, NULL, D3DCLEAR_TARGET, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
		}
	}
}

int ShadowManager::GetShadowSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** ShadowCastLights, NiPointLight** ShadowCullLights, NiPointLight** GeneralPointLights, int& shadowCastLightIndex, int& shadowCullLightIndex, int& GeneralPointLightIndex, SettingsShadowPointLightsStruct* ShadowSettings) {
	SettingsMainStruct::EquipmentModeStruct* EquipmentModeSettings = &TheSettingManager->SettingsMain.EquipmentMode;
	ShadowSceneNode* SceneNode = *(ShadowSceneNode**)kShadowSceneNode; // ShadowSceneNode array, first element is for gamemode
	bool TorchOnBeltEnabled = EquipmentModeSettings->Enabled && EquipmentModeSettings->TorchKey != 255;

	int shadowCastIndex = -1;
	int shadowCullIndex = -1;
	int LightIndex = -1;

	NiTList<ShadowSceneLight>::Entry* Entry = SceneNode->lights.start;
	while (Entry) {
		NiPointLight* Light = Entry->data->sourceLight;
		int distance = (int)Light->GetDistance(&Player->pos);
		AddSceneLight(Light, distance, SceneLights);
		Entry = Entry->next;
	}

	std::map<int, NiPointLight*>::iterator v = SceneLights.begin();
	while (v != SceneLights.end()) {
		NiPointLight* Light = v->second;
		if (LightIndex < 1) {
			GeneralPointLights[++LightIndex] = Light;
		}
		bool CastShadow = true;
		if (TorchOnBeltEnabled && Light->CanCarry == 2) {
			HighProcessEx* Process = (HighProcessEx*)Player->process;
			if (Process->OnBeltState == HighProcessEx::State::In) CastShadow = false;
		}

		//Magic effects typically cause problematic shadows, just allow them to cull
		if (IsLightFromMagic(Light) && shadowCullIndex < (ShadowSettings->iShadowCullLightPoints - 1)) {
			Light->CanCarry = 1;
			ShadowCullLights[++shadowCullIndex] = Light;
		} 
		else if (Light->CastShadows && CastShadow) {

			if (Light->Spec.r >= ShadowSettings->fShadowLightRadiusMin && Light->Spec.r <= ShadowSettings->fShadowLightRadiusMax && shadowCastIndex < (ShadowSettings->iShadowLightPoints - 1) && Player->GetNode()->GetDistance(&Light->m_worldTransform.pos) < 4000.0f) {
				ShadowCastLights[++shadowCastIndex] = Light;
			}
			else if (Light->Spec.r >= ShadowSettings->fShadowCullLightRadiusMin && Light->Spec.r <= ShadowSettings->fShadowCullLightRadiusMax && shadowCullIndex < (ShadowSettings->iShadowCullLightPoints - 1)) {
				ShadowCullLights[++shadowCullIndex] = Light;
			}			
		}
		if ((shadowCastIndex + shadowCullIndex) == (ShadowSettings->iShadowLightPoints + ShadowSettings->iShadowCullLightPoints) - 1) break;
		v++;
	}

	shadowCastLightIndex = shadowCastIndex;
	shadowCullLightIndex = shadowCullIndex;
	GeneralPointLightIndex = LightIndex;

	return shadowCastIndex;
}

void ShadowManager::SetAllShadowCastLightPos(NiPointLight** Lights, int LightIndex) {
	for (int L = 0; L <= LightIndex; L++) {
		SetShadowCastLightPos(Lights, L);
	}
}

void ShadowManager::SetShadowCastLightPos(NiPointLight** Lights, int index) {
	D3DXVECTOR3 Eye;
	NiPoint3* LightPos = &Lights[index]->m_worldTransform.pos;

	float FarPlane = Lights[index]->Spec.r; // Light radius is stored in Spec.r, Spec.g and Spec.b for NiPointLight
	if (Lights[index]->CanCarry) FarPlane = 257.00f; // Set torch shadow to a fixed value to obtain a better effect
	Eye.x = LightPos->x - TheRenderManager->CameraPosition.x;
	Eye.y = LightPos->y - TheRenderManager->CameraPosition.y;
	Eye.z = LightPos->z - TheRenderManager->CameraPosition.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].x = Eye.x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].y = Eye.y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].z = Eye.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].w = FarPlane;
}

void ShadowManager::SetAllShadowCullLightPos(NiPointLight** Lights, int LightIndex) {
	for (int L = 0; L <= LightIndex; L++) {
		SetShadowCullLightPos(Lights, L);
	}
}

void ShadowManager::SetShadowCullLightPos(NiPointLight** Lights, int index) {
	D3DXVECTOR3 Eye;
	NiPoint3* LightPos = &Lights[index]->m_worldTransform.pos;
	float avgDiff = ((Lights[index]->Diff.r + Lights[index]->Diff.g + Lights[index]->Diff.b) / 3.0f);
	float FarPlane = Lights[index]->Spec.r * avgDiff;
	Eye.x = LightPos->x - TheRenderManager->CameraPosition.x;
	Eye.y = LightPos->y - TheRenderManager->CameraPosition.y;
	Eye.z = LightPos->z - TheRenderManager->CameraPosition.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[index].x = Eye.x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[index].y = Eye.y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[index].z = Eye.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[index].w = FarPlane;
}

void ShadowManager::SetAllGeneralLightPos(NiPointLight** Lights, int LightIndex) {
	for (int L = 0; L <= LightIndex; L++) {
		SetGeneralLightPos(Lights, L);
	}
}

void ShadowManager::SetGeneralLightPos(NiPointLight** Lights, int index) {
	NiPoint3* LightPos = &Lights[index]->m_worldTransform.pos;
	NiColor* LightColor = &Lights[index]->Diff;
	float FarPlane = Lights[index]->Spec.r; // Light radius is stored in Spec.r, Spec.g and Spec.b for NiPointLight
	TheShaderManager->ShaderConst.PointLights.LightPosition[index].x = LightPos->x;
	TheShaderManager->ShaderConst.PointLights.LightPosition[index].y = LightPos->y;
	TheShaderManager->ShaderConst.PointLights.LightPosition[index].z = LightPos->z;
	TheShaderManager->ShaderConst.PointLights.LightPosition[index].w = FarPlane;
	TheShaderManager->ShaderConst.PointLights.LightColor[index].x = LightColor->r;
	TheShaderManager->ShaderConst.PointLights.LightColor[index].y = LightColor->g;
	TheShaderManager->ShaderConst.PointLights.LightColor[index].z = LightColor->b;
}

void ShadowManager::SetShadowCubeMapRegisters(int index) {
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.x = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.y = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.z = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.w = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].w;
	TheShaderManager->ShaderConst.ShadowCube.Data.z = TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[index].w;
}

void ShadowManager::AddSceneLight(NiPointLight* Light, int Key, std::map<int, NiPointLight*>& SceneLights) {
	while (SceneLights[Key]) { --Key; }
	SceneLights[Key] = Light;
}

void ShadowManager::ResetIntervals() {
	GameTime = -1;
}

void ShadowManager::LoadShadowLightPointSettings() {

	if (Player->GetWorldSpace()) {
		ShadowLightPointSettings = TheSettingManager->GetSettingsShadowPointLight(Player->GetWorldSpace()->GetEditorName());
		if (!ShadowLightPointSettings) ShadowLightPointSettings = TheSettingManager->GetSettingsShadowPointLight("DefaultExterior");
	}
	else {
		ShadowLightPointSettings = TheSettingManager->GetSettingsShadowPointLight(Player->parentCell->GetEditorName());
		if (!ShadowLightPointSettings) ShadowLightPointSettings = TheSettingManager->GetSettingsShadowPointLight("DefaultInterior");
	}
}

bool ShadowManager::IsLightFromMagic(NiPointLight* light) {
	//TODO: a better way to check this
	return light->m_parent && strstr(light->m_parent->m_pcName, "agic") != NULL;
}

static __declspec(naked) void RenderShadowMapHook() {

	__asm
	{
		pushad
		mov		ecx, TheShadowManager
		call	ShadowManager::RenderShadowMaps
		popad
		jmp		kRenderShadowMapReturn
	}

}

void AddCastShadowFlag(TESObjectREFR* Ref, TESObjectLIGH* Light, NiPointLight* LightPoint) {

	SettingsShadowStruct::InteriorsStruct* ShadowSettings;
	SettingsMainStruct::EquipmentModeStruct* EquipmentModeSettings = &TheSettingManager->SettingsMain.EquipmentMode;

	if (Player->GetWorldSpace()) {
		ShadowSettings = &TheSettingManager->SettingsShadows.ExteriorsPoint;
	}
	else {
		ShadowSettings = &TheSettingManager->SettingsShadows.Interiors;
	}

	if (Light->lightFlags & TESObjectLIGH::LightFlags::kLightFlags_CanCarry) {
		LightPoint->CastShadows = ShadowSettings->TorchesCastShadows;
		LightPoint->CanCarry = 1;
		if (EquipmentModeSettings->Enabled) {
			if (Ref == Player) {
				if (Player->isThirdPerson) {
					if (Player->firstPersonSkinInfo->LightForm == Light) LightPoint->CastShadows = 0;
				}
				else {
					if (Player->ActorSkinInfo->LightForm == NULL && Player->firstPersonSkinInfo->LightForm == Light) LightPoint->CastShadows = 0;
				}
				LightPoint->CanCarry = 2;
			}
		}
	}
	else {
		LightPoint->CastShadows = !(Light->flags & TESForm::FormFlags::kFormFlags_NotCastShadows);
		LightPoint->CanCarry = 0;
	}

}

static __declspec(naked) void AddCastShadowFlagHook() {

	__asm
	{
#if defined(OBLIVION)
		mov     ecx, [esp + 0x158]
		pushad
		push	esi
		push	edi
		push	ecx
		call	AddCastShadowFlag
		pop		ecx
		pop		edi
		pop		esi
		popad
		pop		ecx
		pop		edi
		pop		esi
		pop		ebp
		pop		ebx
#elif defined(NEWVEGAS)
		mov     esi, [ebp + 0x008]
		pushad
		mov		ecx, [ebp - 0x164]
		push	eax
		push	ecx
		push	esi
		call	AddCastShadowFlag
		pop		esi
		pop		ecx
		pop		eax
		popad
		pop		ecx
		pop		esi
		mov     ecx, [ebp - 0x34]
#endif
		jmp		kAddCastShadowFlagReturn
	}

}

#if defined(NEWVEGAS)
void LeavesNodeName(BSTreeNode* TreeNode) {

	TreeNode->m_children.data[1]->SetName("Leaves");

}

static __declspec(naked) void LeavesNodeNameHook() {

	__asm
	{
		pushad
		push	eax
		call	LeavesNodeName
		pop		eax
		popad
		jmp		kLeavesNodeNameReturn
	}

}
#endif

void CreateShadowsHook() {

	WriteRelJump(kRenderShadowMapHook, (UInt32)RenderShadowMapHook);
	WriteRelJump(kAddCastShadowFlagHook, (UInt32)AddCastShadowFlagHook);

#if defined(NEWVEGAS)
	WriteRelJump(kLeavesNodeName, (UInt32)LeavesNodeNameHook);
#elif defined(OBLIVION)
	// This part "disables" the canopy map pass but values are anyway passed to the shaders (they are not used when OR shadows).
	// By now i cannot disable the canopy map pass in Oblivion.ini otherwise the game changes the shaders used for the rendering.
	WriteRelJump(0x0040D637, 0x0040D655); //Avoid tree canopy shadows rendering
	WriteRelJump(0x004425F7, 0x00442621); //Skip canopy map deinitialization (the game disposes/recreates the map every cell changed)
	WriteRelJump(0x004446FB, 0x00444723); //Skip canopy map deinitialization (the game disposes/recreates the map every cell changed)
	WriteRelJump(0x00444CCF, 0x00444CF9); //Skip canopy map deinitialization (the game disposes/recreates the map every cell changed)
	WriteRelJump(0x0055F5C9, 0x0055F5ED); //Skip canopy map deinitialization (the game disposes/recreates the map every cell changed)
#endif
}

void EditorCastShadowFlag(HWND Window, TESForm* Form) {

	if (Window && Form) {
		SetDlgItemTextA(Window, 0x697, "Does Not Cast Shadows");
		SetWindowPos(GetDlgItem(Window, 0x697), HWND_BOTTOM, 0, 0, 140, 15, SWP_NOMOVE | SWP_NOZORDER);
	}

}

static __declspec(naked) void EditorCastShadowFlagHook() {

	__asm
	{
		pushad
		push	eax
		push	edi
		call	EditorCastShadowFlag
		pop		edi
		pop		eax
		popad
		jmp		kEditorCastShadowFlagReturn
	}

}

void CreateEditorShadowsHook() {

	WriteRelJump(kEditorCastShadowFlagHook, (UInt32)EditorCastShadowFlagHook);

}
#elif defined(SKYRIM)
ShadowManager::ShadowManager() {

	Logger::Log("Starting the shadows manager...");
	TheShadowManager = this;

}
#endif