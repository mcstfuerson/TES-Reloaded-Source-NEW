#if defined(NEWVEGAS)
#define kCameraWorldTranslate 0x011F474C
#elif defined(OBLIVION)
#define kCameraWorldTranslate 0x00B3F92C
#elif defined(SKYRIM)
#define kCameraWorldTranslate 0x01B913FC
#endif
#define RESZ_CODE 0x7FA05000

void RenderManager::GetSceneCameraData() {

	NiCamera* Camera = WorldSceneGraph->camera;

	if (Camera) {
		NiMatrix33* WorldRotate = &Camera->m_worldTransform.rot;
		NiPoint3* WorldTranslate = &Camera->m_worldTransform.pos;
		
		CameraForward.x = WorldRotate->data[0][0];
		CameraForward.y = WorldRotate->data[1][0];
		CameraForward.z = WorldRotate->data[2][0];
		CameraPosition.x = WorldTranslate->x;
		CameraPosition.y = WorldTranslate->y;
		CameraPosition.z = WorldTranslate->z;
	}

}

void RenderManager::SetupSceneCamera() {

	NiCamera* Camera = WorldSceneGraph->camera;

	if (Camera) {
		NiPoint3 Forward = { 0.0f, 0.0f, 0.0f };
		NiPoint3 Up = { 0.0f, 0.0f, 0.0f };
		NiPoint3 Right = { 0.0f, 0.0f, 0.0f };
		NiPoint3* CameraWorldTranslate = (NiPoint3*)kCameraWorldTranslate;
		D3DMATRIX* World = &TheRenderManager->worldMatrix;
		D3DMATRIX* View = &TheRenderManager->viewMatrix;
		D3DMATRIX* InvView = &TheRenderManager->invViewMatrix;
		D3DMATRIX* Proj = &TheRenderManager->projMatrix;
		D3DXMATRIX InvProj;
		NiMatrix33* WorldRotate = &Camera->m_worldTransform.rot;
		NiPoint3* WorldTranslate = &Camera->m_worldTransform.pos;
		NiFrustum* Frustum = &Camera->Frustum;
		float RmL = Frustum->Right - Frustum->Left;
		float RpL = Frustum->Right + Frustum->Left;
		float TmB = Frustum->Top - Frustum->Bottom;
		float TpB = Frustum->Top + Frustum->Bottom;
		float InvFmN = 1.0f / (Frustum->Far - Frustum->Near);

		memcpy(CameraWorldTranslate, WorldTranslate, 0x0C);

		Forward.x = WorldRotate->data[0][0];
		Forward.y = WorldRotate->data[1][0];
		Forward.z = WorldRotate->data[2][0];
		Up.x = WorldRotate->data[0][1];
		Up.y = WorldRotate->data[1][1];
		Up.z = WorldRotate->data[2][1];
		Right.x = WorldRotate->data[0][2];
		Right.y = WorldRotate->data[1][2];
		Right.z = WorldRotate->data[2][2];

		// We set up the world matrix always to default (image space) because we use it only in image space shaders
		World->_11 = 1.0f;
		World->_12 = 0.0f;
		World->_13 = 0.0f;
		World->_14 = 0.0f;
		World->_21 = 0.0f;
		World->_22 = 1.0f;
		World->_23 = 0.0f;
		World->_24 = 0.0f;
		World->_31 = 0.0f;
		World->_32 = 0.0f;
		World->_33 = 1.0f;
		World->_34 = 0.0f;
		World->_41 = -WorldTranslate->x;
		World->_42 = -WorldTranslate->y;
		World->_43 = -WorldTranslate->z;
		World->_44 = 1.0f;

		View->_11 = Right.x;
		View->_12 = Up.x;
		View->_13 = Forward.x;
		View->_14 = 0.0f;
		View->_21 = Right.y;
		View->_22 = Up.y;
		View->_23 = Forward.y;
		View->_24 = 0.0f;
		View->_31 = Right.z;
		View->_32 = Up.z;
		View->_33 = Forward.z;
		View->_34 = 0.0f;
		View->_41 = 0.0f;
		View->_42 = 0.0f;
		View->_43 = 0.0f;
		View->_44 = 1.0f;

		InvView->_11 = Right.x;
		InvView->_12 = Right.y;
		InvView->_13 = Right.z;
		InvView->_14 = 0.0f;
		InvView->_21 = Up.x;
		InvView->_22 = Up.y;
		InvView->_23 = Up.z;
		InvView->_24 = 0.0f;
		InvView->_31 = Forward.x;
		InvView->_32 = Forward.y;
		InvView->_33 = Forward.z;
		InvView->_34 = 0.0f;
		InvView->_41 = 0.0f;
		InvView->_42 = 0.0f;
		InvView->_43 = 0.0f;
		InvView->_44 = 1.0f;

		Proj->_11 = 2.0f / RmL;
		Proj->_12 = 0.0f;
		Proj->_13 = 0.0f;
		Proj->_14 = 0.0f;
		Proj->_21 = 0.0f;
		Proj->_22 = 2.0f / TmB;
		Proj->_23 = 0.0f;
		Proj->_24 = 0.0f;
		Proj->_31 = -RpL / RmL;
		Proj->_32 = -TpB / TmB;
		Proj->_33 = Frustum->Far * InvFmN;
		Proj->_34 = 1.0f;
		Proj->_41 = 0.0f;
		Proj->_42 = 0.0f;
		Proj->_43 = -(Frustum->Near * Frustum->Far * InvFmN);
		Proj->_44 = 0.0f;

		D3DXMatrixInverse(&InvProj, NULL, (D3DXMATRIX*)Proj);
		InvViewProjMatrix = InvProj * invViewMatrix;

		WorldViewProjMatrix = worldMatrix * viewMatrix * projMatrix;

		CameraForward.x = Forward.x;
		CameraForward.y = Forward.y;
		CameraForward.z = Forward.z;
		CameraPosition.x = WorldTranslate->x;
		CameraPosition.y = WorldTranslate->y;
		CameraPosition.z = WorldTranslate->z;
	}

}

void RenderManager::SetSceneGraph() {

	float FoV = TheSettingManager->SettingsMain.Main.FoV;

	FirstPersonView = !Player->IsThirdPersonView(TheSettingManager->SettingsMain.CameraMode.Enabled, FirstPersonView);
	Player->SetFoV(WorldSceneGraph, SettingWorldFoV, Setting1stPersonFoV, &FoV, MenuManager->IsActive(Menu::MenuType::kMenuType_None));
	TheShaderManager->ShaderConst.ReciprocalResolution.w = FoV;
	if (TheSettingManager->SettingsMain.CameraMode.Enabled) {
		float FarPlaneDistance = WorldSceneGraph->GetFarPlaneDistance();
		if (FirstPersonView && *SettingNearDistance != TheSettingManager->SettingsMain.CameraMode.NearDistanceFirst) {
			*SettingNearDistance = TheSettingManager->SettingsMain.CameraMode.NearDistanceFirst;
			WorldSceneGraph->camera->Frustum.Near = *SettingNearDistance;
			WorldSceneGraph->camera->MaxFarNearRatio = FarPlaneDistance / *SettingNearDistance;
		}
		else if (!FirstPersonView && *SettingNearDistance != TheSettingManager->SettingsMain.CameraMode.NearDistanceThird) {
			*SettingNearDistance = TheSettingManager->SettingsMain.CameraMode.NearDistanceThird;
			WorldSceneGraph->camera->Frustum.Near = *SettingNearDistance;
			WorldSceneGraph->camera->MaxFarNearRatio = FarPlaneDistance / *SettingNearDistance;
		}
	}

}

void RenderManager::Initialize() {

	IDirect3D9* D3D;
	D3DDISPLAYMODE currentDisplayMode;

	Logger::Log("Extending the render manager...");
	CameraForward.x = CameraForward.y = CameraForward.z = CameraForward.w = 0.0f;
	CameraPosition.x = CameraPosition.y = CameraPosition.z = CameraPosition.w = 0.0f;
	BackBuffer = NULL;
	DepthSurface = NULL;
	DepthTexture = NULL;
	DepthTextureINTZ = NULL;
	SaveGameScreenShotRECT = { 0, 0, 256, 144 };
	IsSaveGameScreenShot = false;
	FirstPersonView = false;
	device->CreateTexture(width, height, 1, D3DUSAGE_DEPTHSTENCIL, (D3DFORMAT)MAKEFOURCC('I','N','T','Z'), D3DPOOL_DEFAULT, &DepthTexture, NULL);
	device->GetDirect3D(&D3D);
	D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentDisplayMode);
	RESZ = D3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, currentDisplayMode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, (D3DFORMAT)MAKEFOURCC('R','E','S','Z')) == D3D_OK;
	if (RESZ)
		Logger::Log("AMD/Intel detected: RESZ supported.");
	else if (NvAPI_Initialize() == NVAPI_OK)
		Logger::Log("NVIDIA detected: NVAPI supported.");
	else
		Logger::Log("ERROR: Cannot initialize the render manager. Graphics device not supported.");
	if (TheSettingManager->SettingsMain.Main.AnisotropicFilter >= 2) device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, TheSettingManager->SettingsMain.Main.AnisotropicFilter);
#if defined(OBLIVION)
	if (*SettingHDR == 1 && *SettingMultiSample >= 2) {
		Ni2DBuffer* Buffer = (Ni2DBuffer*)MemoryAlloc(0x14); *(void**)Buffer = (void*)0x00A8098C; Buffer->m_uiRefCount = 1;
		NiDX92DBufferData* BufferData = (NiDX92DBufferData*)MemoryAlloc(0x10); *(void**)BufferData = (void*)0x00A89818; BufferData->m_uiRefCount = 1; BufferData->ParentData = Buffer;
		Buffer->width = width;
		Buffer->height = height;
		Buffer->data = BufferData;
		device->CreateRenderTarget(width, height, D3DFMT_A16B16G16R16F, (D3DMULTISAMPLE_TYPE)(*SettingMultiSample), 0, false, &BufferData->Surface, NULL);
		defaultRTGroup->RenderTargets[1] = Buffer;
		BackBuffer = defaultRTGroup->RenderTargets[0]->data->Surface;
	}
#endif
}

void RenderManager::ResolveDepthBuffer() {

	if (RESZ) {
		IDirect3DBaseTexture9 *pCurrTX = NULL;
		IDirect3DVertexShader9 *pCurrVS = NULL;
		IDirect3DPixelShader9 *pCurrPS = NULL;
		IDirect3DVertexBuffer9 *pCurrVX = NULL;
		UINT32 dCurrVO;
		UINT32 dCurrVS;
		DWORD dCurrZE;
		DWORD dCurrZW;
		DWORD dCurrCW;
		D3DXVECTOR3 vDummyPoint(0.0f, 0.0f, 0.0f);

		device->GetTexture(0, &pCurrTX);
		device->GetVertexShader(&pCurrVS);
		device->GetPixelShader(&pCurrPS);
		device->GetStreamSource(0, &pCurrVX, &dCurrVO, &dCurrVS);

		device->GetRenderState(D3DRS_ZENABLE, &dCurrZE);
		device->GetRenderState(D3DRS_ZWRITEENABLE, &dCurrZW);
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &dCurrCW);
		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(D3DFVF_XYZ);
		device->SetTexture(0, DepthTexture);
		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, NULL);
		device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vDummyPoint, sizeof(D3DXVECTOR3));
		device->SetRenderState(D3DRS_ZENABLE, dCurrZE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, dCurrZW);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, dCurrCW);
		device->SetRenderState(D3DRS_POINTSIZE, RESZ_CODE);
		device->SetRenderState(D3DRS_POINTSIZE, NULL);

		device->SetTexture(0, pCurrTX);
		device->SetVertexShader(pCurrVS);
		device->SetPixelShader(pCurrPS);
		device->SetStreamSource(0, pCurrVX, dCurrVO, dCurrVS);
		
		if (pCurrTX) pCurrTX->Release();
		if (pCurrVX) pCurrVX->Release();
	}
	else {
		if (!DepthSurface) {
			D3DSURFACE_DESC desc;
			device->GetDepthStencilSurface(&DepthSurface);
			DepthSurface->GetDesc(&desc);
			if (desc.Format == (D3DFORMAT)MAKEFOURCC('I','N','T','Z')) {
				void *Container = NULL;
				DepthSurface->GetContainer(IID_IDirect3DTexture9, &Container);
				DepthTextureINTZ = (IDirect3DTexture9*)Container;
				NvAPI_D3D9_RegisterResource(DepthTextureINTZ);
			}
			else
				NvAPI_D3D9_RegisterResource(DepthSurface);
			NvAPI_D3D9_RegisterResource(DepthTexture);
		}
		if (!DepthTextureINTZ)
			NvAPI_D3D9_StretchRectEx(device, DepthSurface, NULL, DepthTexture, NULL, D3DTEXF_NONE);
		else
			NvAPI_D3D9_StretchRectEx(device, DepthTextureINTZ, NULL, DepthTexture, NULL, D3DTEXF_NONE);
	}

}