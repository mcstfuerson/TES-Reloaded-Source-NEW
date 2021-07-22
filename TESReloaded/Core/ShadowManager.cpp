#include <algorithm>
#include <cmath>
#include <list>
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
	UINT ShadowMapSize = 0;

	//TODO: should this setting be on it's own? choose smaller of two for now
	UINT ShadowCubeMapSize = min(ShadowsInteriors->ShadowCubeMapSize, ShadowsExteriorsPoint->ShadowCubeMapSize);

	CurrentCell = NULL;
	ShadowCubeMapState = ShadowCubeMapStateEnum::None;
	int ShadowCubeLightCount = 0;

	ShadowMapVertex = new ShaderRecord();
	if (ShadowMapVertex->LoadShader("ShadowMap.vso")) Device->CreateVertexShader((const DWORD*)ShadowMapVertex->Function, &ShadowMapVertexShader);
	ShadowMapPixel = new ShaderRecord();
	if (ShadowMapPixel->LoadShader("ShadowMap.pso")) Device->CreatePixelShader((const DWORD*)ShadowMapPixel->Function, &ShadowMapPixelShader);

	ShadowCubeMapVertex = new ShaderRecord();
	if (ShadowCubeMapVertex->LoadShader("ShadowCubeMap.vso")) Device->CreateVertexShader((const DWORD*)ShadowCubeMapVertex->Function, &ShadowCubeMapVertexShader);
	ShadowCubeMapPixel = new ShaderRecord();
	if (ShadowCubeMapPixel->LoadShader("ShadowCubeMap.pso")) Device->CreatePixelShader((const DWORD*)ShadowCubeMapPixel->Function, &ShadowCubeMapPixelShader);

	for (int i = 0; i < 3; i++) {
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
		}
	}
	Device->CreateDepthStencilSurface(ShadowCubeMapSize, ShadowCubeMapSize, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &ShadowCubeMapDepthSurface, NULL);
	ShadowCubeMapViewPort = { 0, 0, ShadowCubeMapSize, ShadowCubeMapSize, 0.0f, 1.0f };
	
	ShadowCubeMapLights[12] = { NULL };
	D3DXVECTOR4* ShadowCubeMapBlend = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend;
	D3DXVECTOR4* ShadowCubeMapBlend2 = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend2;
	D3DXVECTOR4* ShadowCubeMapBlend3 = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend3;
	ShadowCubeMapBlend->x =  1.0f; ShadowCubeMapBlend->y = 1.0f; ShadowCubeMapBlend->z = 1.0f; ShadowCubeMapBlend->w = 1.0f;
	ShadowCubeMapBlend2->x = 1.0f; ShadowCubeMapBlend2->y = 1.0f; ShadowCubeMapBlend2->z = 1.0f; ShadowCubeMapBlend2->w = 1.0f;
	ShadowCubeMapBlend3->x = 1.0f; ShadowCubeMapBlend3->y = 1.0f; ShadowCubeMapBlend3->z = 1.0f; ShadowCubeMapBlend3->w = 1.0f;

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

void ShadowManager::GetFrustum(ShadowMapTypeEnum ShadowMapType, D3DMATRIX* Matrix) {

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

bool ShadowManager::InFrustum(ShadowMapTypeEnum ShadowMapType, NiAVObject* Object) {
	
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
void ShadowManager::RenderObject(NiAVObject* Object, D3DXVECTOR4* ShadowData, bool HasWater) {

	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled)) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode || VFT == VFTBSFadeNode || VFT == VFTBSFaceGenNiNode || VFT == VFTBSTreeNode) {
			NiNode* Node = (NiNode*)Object;
			for (int i = 0; i < Node->m_children.end; i++) {
				RenderObject(Node->m_children.data[i], ShadowData, HasWater);
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

void ShadowManager::RenderTerrain(NiAVObject* Object, ShadowMapTypeEnum ShadowMapType, D3DXVECTOR4* ShadowData ) {

	if (Object && !(Object->m_flags & NiAVObject::kFlag_AppCulled)) {
		void* VFT = *(void**)Object;
		if (VFT == VFTNiNode) {
			NiNode* Node = (NiNode*)Object;
			if (InFrustum(ShadowMapType, Node)) {
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
			Device->SetFVF(GeoData->FVF);
		else
			Device->SetVertexDeclaration(GeoData->VertexDeclaration);
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
				Device->SetFVF(GeoData->FVF);
			else
				Device->SetVertexDeclaration(GeoData->VertexDeclaration);
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
	GetFrustum(ShadowMapType, &TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj);
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
							if (InFrustum(ShadowMapType, RefNode)) RenderObject(RefNode, ShadowData, TheShaderManager->ShaderConst.HasWater);
						}
						Entry = Entry->next;
					}
				}
			}
		}
		Device->EndScene();
	}

}

void ShadowManager::RenderShadowCubeMapExt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {

	std::map<int, std::vector<NiNode*>> refMap;

	for (UInt32 x = 0; x < *SettingGridsToLoad - 1; x++) {
		for (UInt32 y = 0; y < *SettingGridsToLoad; y++) {
			if (TESObjectCELL* Cell = Tes->gridCellArray->GetCell(x, y)) {
				TList<TESObjectREFR>::Entry* Entry = &Cell->objectList.First;
				while (Entry) {
					if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowSettings->Forms, &ShadowSettings->ExcludedForms)) {
						for (int L = 0; L <= LightIndex; L++) {
							NiPoint3* LightPos = &Lights[L]->m_worldTransform.pos;
							float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].w;
							if (Ref->GetNode()->GetDistance(LightPos) - Ref->GetNode()->GetWorldBoundRadius() <= FarPlane * 1.2f) {
								refMap[L].emplace_back(Ref->GetNode());
							}
						}
					}
					Entry = Entry->next;
				}
			}
		}
	}
	RenderShadowCubeMap(LightIndex, refMap, ShadowData, ShadowSettings->Enabled);
}

void ShadowManager::RenderShadowCubeMapInt(NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {

	std::map<int, std::vector<NiNode*>> refMap;

	TList<TESObjectREFR>::Entry* Entry = &Player->parentCell->objectList.First;
	while (Entry) {
		if (TESObjectREFR* Ref = GetRef(Entry->item, &ShadowSettings->Forms, &ShadowSettings->ExcludedForms)) {
			for (int L = 0; L <= LightIndex; L++) {
				NiPoint3* LightPos = &Lights[L]->m_worldTransform.pos;
				float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].w;
				if (Ref->GetNode()->GetDistance(LightPos) - Ref->GetNode()->GetWorldBoundRadius() <= FarPlane * 1.2f) {
					refMap[L].emplace_back(Ref->GetNode());
				}
			}
		}
		Entry = Entry->next;
	}
	RenderShadowCubeMap(LightIndex, refMap, ShadowData, ShadowSettings->Enabled);
}

void ShadowManager::RenderShadowCubeMapFakeInt(int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings, D3DXVECTOR4* ShadowData) {
	D3DXVECTOR3 At, Eye;
	D3DXVECTOR4* ShadowCubeMapBlend = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;

	if (!FakeExtShadowLightDirSet) {
		FakeExtShadowLightDirSet = true;
		fakeExtShadowLightDir = TheRenderManager->CameraPosition;
	}

	Eye.x = ((ShadowLightDir->x * 4000) + fakeExtShadowLightDir.x) - TheRenderManager->CameraPosition.x;
	Eye.y = ((ShadowLightDir->y * 4000) + fakeExtShadowLightDir.y) - TheRenderManager->CameraPosition.y;
	Eye.z = ((ShadowLightDir->z * 4000) + fakeExtShadowLightDir.z) - TheRenderManager->CameraPosition.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[LightIndex].x = Eye.x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[LightIndex].y = Eye.y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[LightIndex].z = Eye.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[LightIndex].w = 15000;
	ShadowCubeMapBlend->x = 0.6f;

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

	Device->SetDepthStencilSurface(ShadowCubeMapDepthSurface);
	for (int L = 0; L <= LightIndex; L++) {
		SetShadowCubeMapRegisters(L);
		float FarPlane = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].w;
		D3DXMatrixPerspectiveFovRH(&Proj, D3DXToRadian(90.0f), 1.0f, 1.0f, FarPlane);
		for (int Face = 0; Face < 6; Face++) {
			At.x = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].x;
			At.y = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].y;
			At.z = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].z;
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
				RenderState->SetPixelShader(ShadowCubeMapPixelShader, false);
				std::vector<NiNode*>::iterator RefNode;
				for (RefNode = refMap[L].begin(); RefNode != refMap[L].end(); ++RefNode) {
					RenderObject((*RefNode), ShadowData, TheShaderManager->ShaderConst.HasWater);
				}
				Device->EndScene();
			}
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
	D3DXVECTOR4* OrthoData = &TheShaderManager->ShaderConst.Shadow.OrthoData;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;

	D3DXVECTOR4 OrthoDir = D3DXVECTOR3(0.05f, 0.05f, 1.0f);
	NiNode* PlayerNode = Player->GetNode();
	D3DXVECTOR3 At, Eye;
	std::map<int, NiPointLight*> SceneLights;
	NiPointLight* Lights[12] = { NULL };
	int LightIndex = -1;

	CurrentVertex = ShadowMapVertex;
	CurrentPixel = ShadowMapPixel;

	ClearShadowCubeMaps(Device, -1, ShadowCubeMapStateEnum::Exterior);

	At.x = PlayerNode->m_worldTransform.pos.x - TheRenderManager->CameraPosition.x;
	At.y = PlayerNode->m_worldTransform.pos.y - TheRenderManager->CameraPosition.y;
	At.z = PlayerNode->m_worldTransform.pos.z - TheRenderManager->CameraPosition.z;

	RenderShadowMap(MapNear, ShadowsExteriors, &At, ShadowLightDir, ShadowData);
	RenderShadowMap(MapFar, ShadowsExteriors, &At, ShadowLightDir, ShadowData);
	RenderShadowMap(MapOrtho, ShadowsExteriors, &At, &OrthoDir, ShadowData);

	ShadowData->x = ShadowsExteriors->Quality;
	ShadowData->y = ShadowsExteriors->Darkness;

	ShadowData->z = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapNear];
	ShadowData->w = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapFar];
	OrthoData->z = 1.0f / (float)ShadowsExteriors->ShadowMapSize[MapOrtho];
}

//TODO: rename, doesn't apply solely to interiors
void ShadowManager::RenderInteriorShadows() {
	IDirect3DDevice9* Device = TheRenderManager->device;
	D3DXVECTOR4* ShadowData = &TheShaderManager->ShaderConst.ShadowCube.Data;
	D3DXVECTOR4* ShadowLightDir = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;
	SettingsShadowStruct::InteriorsStruct* ShadowSettings;

	if (Player->GetWorldSpace()) {
		ShadowSettings = &TheSettingManager->SettingsShadows.ExteriorsPoint;
	}
	else {
		ShadowSettings = &TheSettingManager->SettingsShadows.Interiors;
	}

	if (CurrentCell != Player->parentCell) { ShadowCubeMapState = ShadowCubeMapStateEnum::None; CurrentCell = Player->parentCell; }

	CurrentVertex = ShadowCubeMapVertex;
	CurrentPixel = ShadowCubeMapPixel;
	AlphaEnabled = ShadowSettings->AlphaEnabled;

	std::map<int, NiPointLight*> SceneLights;
	NiPointLight* Lights[12] = { NULL };
	int LightIndex = -1;

	if (!(Player->parentCell->flags0 & Player->parentCell->kFlags0_BehaveLikeExterior && ShadowLightDir->z > 0.01f)) {
		FakeExtShadowLightDirSet = false;
		if (ShadowSettings->Enabled) {
			LightIndex = GetShadowSceneLights(SceneLights, Lights, LightIndex, ShadowSettings);
			SetAllShadowMapLightPos(Lights, LightIndex);

			if (Player->GetWorldSpace()) {
				RenderShadowCubeMapExt(Lights, LightIndex, ShadowSettings, ShadowData);
			}
			else {
				RenderShadowCubeMapInt(Lights, LightIndex, ShadowSettings, ShadowData);
				ClearShadowCubeMaps(Device, LightIndex, ShadowCubeMapStateEnum::Interior);
			}

			CalculateBlend(Lights, LightIndex);
		}
		else {
			LightIndex = GetExtSceneLights(SceneLights, Lights, LightIndex);
			SetAllShadowMapLightPos(Lights, LightIndex);
			LightIndex = -1; //clears shadowmaps
		}	
	}
	else {
		LightIndex = 0;
		RenderShadowCubeMapFakeInt(LightIndex, ShadowSettings, ShadowData);
	}

	if (LightIndex < ShadowCubeLightCount) { ClearShadowCubeMaps(Device, LightIndex); }
	ShadowCubeLightCount = LightIndex;

	ShadowData->x = ShadowSettings->Quality;
	ShadowData->y = ShadowSettings->Darkness;
	ShadowData->z = 1.0f / (float)ShadowSettings->ShadowCubeMapSize;
}

void ShadowManager::RenderShadowMaps() {
	Global->RenderShadowMaps();
	//return;

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
}

void ShadowManager::ClearShadowCubeMaps(IDirect3DDevice9* Device, int From, ShadowCubeMapStateEnum NewState) {
	if (ShadowCubeMapState != NewState) {
		ClearShadowMap(Device);
		ClearShadowCubeMaps(Device, From);
		ShadowCubeMapState = NewState;
	}
}

void ShadowManager::ClearShadowCubeMaps(IDirect3DDevice9* Device, int From) {
	for (int L = From + 1; L < 12; L++) {
		TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[L].w = 0.0f;
		for (int Face = 0; Face < 6; Face++) {
			Device->SetRenderTarget(0, ShadowCubeMapSurface[L][Face]);
			Device->Clear(0L, NULL, D3DCLEAR_TARGET, D3DXCOLOR(1.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0L);
		}
	}
}

//TODO: "Blend" code has been removed, keeping code structure until it's removed from shaders
void ShadowManager::CalculateBlend(NiPointLight** Lights, int LightIndex) {
	if (memcmp(Lights, ShadowCubeMapLights, 48)) {
		memcpy(ShadowCubeMapLights, Lights, 48);
	}
}

int ShadowManager::GetExtSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** Lights, int LightIndex) {
	int lightPoints = 4; //TODO: move to config/setting?
	ShadowSceneNode* SceneNode = *(ShadowSceneNode**)kShadowSceneNode; // ShadowSceneNode array, first element is for gamemode

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
		LightIndex += 1;
		Lights[LightIndex] = Light;
		if (LightIndex == lightPoints - 1 || LightIndex == 3) break;
		v++;
	}

	return LightIndex;
}

int ShadowManager::GetShadowSceneLights(std::map<int, NiPointLight*>& SceneLights, NiPointLight** Lights, int LightIndex, SettingsShadowStruct::InteriorsStruct* ShadowSettings) {
	SettingsMainStruct::EquipmentModeStruct* EquipmentModeSettings = &TheSettingManager->SettingsMain.EquipmentMode;
	ShadowSceneNode* SceneNode = *(ShadowSceneNode**)kShadowSceneNode; // ShadowSceneNode array, first element is for gamemode
	bool TorchOnBeltEnabled = EquipmentModeSettings->Enabled && EquipmentModeSettings->TorchKey != 255;

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
		bool CastShadow = true;
		if (TorchOnBeltEnabled && Light->CanCarry == 2) {
			HighProcessEx* Process = (HighProcessEx*)Player->process;
			if (Process->OnBeltState == HighProcessEx::State::In) CastShadow = false;
		}
		if (Light->CastShadows && CastShadow) {
			LightIndex += 1;
			Lights[LightIndex] = Light;
		}
		if (LightIndex == ShadowSettings->LightPoints - 1 || LightIndex == 12) break;
		v++;
	}

	return LightIndex;
}

void ShadowManager::SetAllShadowMapLightPos(NiPointLight** Lights, int LightIndex) {
	for (int L = 0; L <= LightIndex; L++) {
		SetShadowMapLightPos(Lights, L);
	}
}

void ShadowManager::SetShadowMapLightPos(NiPointLight** Lights, int index) {
	D3DXVECTOR3 At, Eye;
	NiPoint3* LightPos = &Lights[index]->m_worldTransform.pos;
	float FarPlane = Lights[index]->Spec.r; // Light radius is stored in Spec.r, Spec.g and Spec.b for NiPointLight
	if (Lights[index]->CanCarry) FarPlane = 256.0f; // Set torch shadow to a fixed value to obtain a better effect
	Eye.x = LightPos->x - TheRenderManager->CameraPosition.x;
	Eye.y = LightPos->y - TheRenderManager->CameraPosition.y;
	Eye.z = LightPos->z - TheRenderManager->CameraPosition.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].x = Eye.x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].y = Eye.y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].z = Eye.z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].w = FarPlane;
}

void ShadowManager::SetShadowCubeMapRegisters(int index) {
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.x = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].x;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.y = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].y;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.z = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].z;
	TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition.w = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].w;
	TheShaderManager->ShaderConst.ShadowCube.Data.z = TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[index].w;
}

void ShadowManager::AddSceneLight(NiPointLight* Light, int Key, std::map<int, NiPointLight*>& SceneLights) {
	while (SceneLights[Key]) { --Key; }
	SceneLights[Key] = Light;
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
	
	WriteRelJump(kRenderShadowMapHook,		(UInt32)RenderShadowMapHook);
	WriteRelJump(kAddCastShadowFlagHook,	(UInt32)AddCastShadowFlagHook);

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