#include <fstream>
#include <ctime>
#define EFFECTQUADFORMAT D3DFVF_XYZ | D3DFVF_TEX1

#if defined(NEWVEGAS)
#define CurrentBlend 0.0f
#define DeepColorR properties.deepColorR
#define DeepColorG properties.deepColorG
#define DeepColorB properties.deepColorB
#define DeepColorA properties.deepColorA
#define ShallowColorR properties.shallowColorR
#define ShallowColorG properties.shallowColorG
#define ShallowColorB properties.shallowColorB
#define ShallowColorA properties.shallowColorA
#define TerrainShaders ""
#define BloodShaders ""
#elif defined(OBLIVION)
#define CurrentBlend *WaterBlend
#define DeepColorR deepColorR
#define DeepColorG deepColorG
#define DeepColorB deepColorB
#define DeepColorA deepColorA
#define ShallowColorR shallowColorR
#define ShallowColorG shallowColorG
#define ShallowColorB shallowColorB
#define ShallowColorA shallowColorA
#define TerrainShaders "SLS2001.vso SLS2001.pso SLS2064.vso SLS2068.pso SLS2042.vso SLS2048.pso SLS2043.vso SLS2049.pso"
#define BloodShaders "GDECALS.vso GDECAL.pso SLS2040.vso SLS2046.pso"
#elif defined(SKYRIM)
#define sunGlare general.sunGlare
#define windSpeed general.windSpeed
#define weatherType general.weatherType
#define CurrentBlend 0.0f
#define DeepColorR properties.deepColorR
#define DeepColorG properties.deepColorG
#define DeepColorB properties.deepColorB
#define DeepColorA properties.deepColorA
#define ShallowColorR properties.shallowColorR
#define ShallowColorG properties.shallowColorG
#define ShallowColorB properties.shallowColorB
#define ShallowColorA properties.shallowColorA
#define TerrainShaders ""
#define BloodShaders ""
#endif

ShaderProgram::ShaderProgram() {

	FloatShaderValues = NULL;
	TextureShaderValues = NULL;
	FloatShaderValuesCount = 0;
	TextureShaderValuesCount = 0;

}

ShaderProgram::~ShaderProgram() {

	if (FloatShaderValues) free(FloatShaderValues);
	if (TextureShaderValues) free(TextureShaderValues);

}

void ShaderProgram::SetConstantTableValue(LPCSTR Name, UInt32 Index) {

	if (!strcmp(Name, "TESR_Tick"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Tick;
	else if (!strcmp(Name, "TESR_ToneMapping"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.HDR.ToneMapping;
	else if (!strcmp(Name, "TESR_ParallaxData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.POM.ParallaxData;
	else if (!strcmp(Name, "TESR_GrassScale"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Grass.Scale;
	else if (!strcmp(Name, "TESR_TerrainData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Terrain.Data;
	else if (!strcmp(Name, "TESR_SkinData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Skin.SkinData;
	else if (!strcmp(Name, "TESR_SkinColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Skin.SkinColor;
	else if (!strcmp(Name, "TESR_ShadowData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Shadow.Data;
	else if (!strcmp(Name, "TESR_OrthoData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Shadow.OrthoData;
	else if (!strcmp(Name, "TESR_RainData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Precipitations.RainData;
	else if (!strcmp(Name, "TESR_SnowData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Precipitations.SnowData;
	else if (!strcmp(Name, "TESR_WorldTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->worldMatrix;
	else if (!strcmp(Name, "TESR_ViewTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->viewMatrix;
	else if (!strcmp(Name, "TESR_ProjectionTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->projMatrix;
	else if (!strcmp(Name, "TESR_WorldViewProjectionTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->WorldViewProjMatrix;
	else if (!strcmp(Name, "TESR_InvViewProjectionTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheRenderManager->InvViewProjMatrix;
	else if (!strcmp(Name, "TESR_ShadowWorldTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowWorld;
	else if (!strcmp(Name, "TESR_ShadowViewProjTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowViewProj;
	else if (!strcmp(Name, "TESR_ShadowCameraToLightTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight;
	else if (!strcmp(Name, "TESR_ShadowCameraToLightTransformNear"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[0];
	else if (!strcmp(Name, "TESR_ShadowCameraToLightTransformFar"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[1];
	else if (!strcmp(Name, "TESR_ShadowCameraToLightTransformOrtho"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[2];
	else if (!strcmp(Name, "TESR_ShadowCubeMapLightPosition"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition;
	else if (!strcmp(Name, "TESR_ShadowLightPosition"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition;
	else if (!strcmp(Name, "TESR_ShadowLightPosition0"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[0];
	else if (!strcmp(Name, "TESR_ShadowLightPosition1"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[1];
	else if (!strcmp(Name, "TESR_ShadowLightPosition2"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[2];
	else if (!strcmp(Name, "TESR_ShadowLightPosition3"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowLightPosition[3];
	else if (!strcmp(Name, "TESR_ShadowCubeMapFarPlanes"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapFarPlanes;
	else if (!strcmp(Name, "TESR_ShadowCubeMapBlend"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapBlend;
	else if (!strcmp(Name, "TESR_ReciprocalResolution"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ReciprocalResolution;
	else if (!strcmp(Name, "TESR_ReciprocalResolutionWater"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ReciprocalResolutionWater;
	else if (!strcmp(Name, "TESR_CameraForward"))
		FloatShaderValues[Index].Value = &TheRenderManager->CameraForward;
	else if (!strcmp(Name, "TESR_CameraPosition"))
		FloatShaderValues[Index].Value = &TheRenderManager->CameraPosition;
	else if (!strcmp(Name, "TESR_SunDirection"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunDir;
	else if (!strcmp(Name, "TESR_SunTiming"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunTiming;
	else if (!strcmp(Name, "TESR_SunAmount"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunAmount;
	else if (!strcmp(Name, "TESR_GameTime"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GameTime;
	else if (!strcmp(Name, "TESR_TextureData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.TextureData;
	else if (!strcmp(Name, "TESR_WaterCoefficients"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.waterCoefficients;
	else if (!strcmp(Name, "TESR_WaveParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.waveParams;
	else if (!strcmp(Name, "TESR_WaterVolume"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.waterVolume;
	else if (!strcmp(Name, "TESR_WaterSettings"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.waterSettings;
	else if (!strcmp(Name, "TESR_WaterDeepColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.deepColor;
	else if (!strcmp(Name, "TESR_WaterShallowColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.shallowColor;
	else if (!strcmp(Name, "TESR_WaterShorelineParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Water.shorelineParams;
	else if (!strcmp(Name, "TESR_FogColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.fogColor;
	else if (!strcmp(Name, "TESR_SunColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.sunColor;
	else if (!strcmp(Name, "TESR_FogData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.fogData;
	else if (!strcmp(Name, "TESR_AmbientOcclusionAOData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.AmbientOcclusion.AOData;
	else if (!strcmp(Name, "TESR_AmbientOcclusionData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.AmbientOcclusion.Data;
	else if (!strcmp(Name, "TESR_BloodLensParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.BloodLens.Params;
	else if (!strcmp(Name, "TESR_BloodLensColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.BloodLens.BloodColor;
	else if (!strcmp(Name, "TESR_BloomData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Bloom.BloomData;
	else if (!strcmp(Name, "TESR_BloomValues"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Bloom.BloomValues;
	else if (!strcmp(Name, "TESR_CinemaData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Cinema.Data;
	else if (!strcmp(Name, "TESR_ColoringColorCurve"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring.ColorCurve;
	else if (!strcmp(Name, "TESR_ColoringEffectGamma"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring.EffectGamma;
	else if (!strcmp(Name, "TESR_ColoringData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring.Data;
	else if (!strcmp(Name, "TESR_ColoringValues"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Coloring.Values;
	else if (!strcmp(Name, "TESR_DepthOfFieldBlur"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.DepthOfField.Blur;
	else if (!strcmp(Name, "TESR_DepthOfFieldData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.DepthOfField.Data;
	else if (!strcmp(Name, "TESR_GodRaysRay"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays.Ray;
	else if (!strcmp(Name, "TESR_GodRaysRayColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays.RayColor;
	else if (!strcmp(Name, "TESR_GodRaysData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GodRays.Data;
	else if (!strcmp(Name, "TESR_LowHFData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.LowHF.Data;
	else if (!strcmp(Name, "TESR_MotionBlurParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MotionBlur.BlurParams;
	else if (!strcmp(Name, "TESR_MotionBlurData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MotionBlur.Data;
	else if (!strcmp(Name, "TESR_SharpeningData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Sharpening.Data;
	else if (!strcmp(Name, "TESR_SnowAccumulationParams"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SnowAccumulation.Params;
	else if (!strcmp(Name, "TESR_VolumetricFogData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricFog.Data;
	else if (!strcmp(Name, "TESR_WaterLensData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WaterLens.Time;
	else if (!strcmp(Name, "TESR_WetWorldCoeffs"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WetWorld.Coeffs;
	else if (!strcmp(Name, "TESR_WetWorldData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.WetWorld.Data;
	else {
		Logger::Log("Custom constant found: %s", Name);
		D3DXVECTOR4 v; v.x = v.y = v.z = v.w = 0.0f;
		TheShaderManager->CustomConst[Name] = v;
		FloatShaderValues[Index].Value = &TheShaderManager->CustomConst[Name];
	}
}

ShaderRecord::ShaderRecord() {
	
	Enabled		= false;
	HasCT		= false;
	HasRB		= false;
	HasDB		= false;
	Function	= NULL;
	Source		= NULL;
	Shader		= NULL;
	Table		= NULL;
	Errors		= NULL;

}

ShaderRecord::~ShaderRecord() {

	if (Shader) Shader->Release();
	if (Table) Table->Release();
	if (Errors) Errors->Release();
	if (Source) delete Source;

}

bool ShaderRecord::LoadShader(const char* Name) {
  
	char FileName[MAX_PATH];
	char FileNameBinary[MAX_PATH];
	
	strcpy(FileName, ShadersPath);
	if (!memcmp(Name, "WATER", 5)) {
		if (!TheSettingManager->SettingsMain.Shaders.Water) return false;
		strcat(FileName, "Water\\");
	}
	else if (!memcmp(Name, "GRASS", 5)) {
		if (!TheSettingManager->SettingsMain.Shaders.Grass) return false;
		strcat(FileName, "Grass\\");
	}
	else if (!memcmp(Name, "HDR", 3)) {
		if (!TheSettingManager->SettingsMain.Shaders.HDR) return false;
		strcat(FileName, "HDR\\");
	}
	else if (!memcmp(Name, "PAR", 3)) {
		if (!TheSettingManager->SettingsMain.Shaders.POM) return false;
		strcat(FileName, "POM\\");
	}
	else if (!memcmp(Name, "SKIN", 4)) {
		if (!TheSettingManager->SettingsMain.Shaders.Skin) return false;
		strcat(FileName, "Skin\\");
	}
	else if (strstr(TerrainShaders, Name)) {
		if (!TheSettingManager->SettingsMain.Shaders.Terrain) return false;
		strcat(FileName, "Terrain\\");
	}
	else if (strstr(BloodShaders, Name)) {
		if (!TheSettingManager->SettingsMain.Shaders.Blood) return false;
		strcat(FileName, "Blood\\");
	}
	else if (!memcmp(Name, "NIGHTEYE", 8)) {
		if (!TheSettingManager->SettingsMain.Shaders.NightEye) return false;
		strcat(FileName, "NightEye\\");
	}
	else if (!memcmp(Name, "Shadow", 6)) {
		strcat(FileName, "Shadows\\");
	}	
	else {
		strcat(FileName, "ExtraShaders\\");
	}
	strcat(FileName, Name);
	strcpy(FileNameBinary, FileName);
	strcat(FileName, ".hlsl");
	std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (FileSource.is_open()) {
		std::streampos size = FileSource.tellg();
		Source = new char[size];
		FileSource.seekg(0, std::ios::beg);
		FileSource.read(Source, size);
		FileSource.close();
		if (strstr(Name, ".vso"))
			Type = ShaderType_Vertex;
		else if (strstr(Name, ".pso"))
			Type = ShaderType_Pixel;
		if (TheSettingManager->SettingsMain.Develop.CompileShaders) {
			D3DXCompileShaderFromFileA(FileName, NULL, NULL, "main", (Type == ShaderType_Vertex ? "vs_3_0" : "ps_3_0"), NULL, &Shader, &Errors, &Table);
			if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
			if (Shader) {
				std::ofstream FileBinary(FileNameBinary, std::ios::out|std::ios::binary);
				FileBinary.write((char*)Shader->GetBufferPointer(), Shader->GetBufferSize());
				FileBinary.flush();
				FileBinary.close();
				Logger::Log("Shader compiled: %s", FileName);
			}
		}
		else {
			std::ifstream FileBinary(FileNameBinary, std::ios::in | std::ios::binary | std::ios::ate);
			if (FileBinary.is_open()) {
				size = FileBinary.tellg();
				D3DXCreateBuffer(size, &Shader);
				FileBinary.seekg(0, std::ios::beg);
				void* pShaderBuffer = Shader->GetBufferPointer();
				FileBinary.read((char*)pShaderBuffer, size);
				FileBinary.close();
				D3DXGetShaderConstantTable((const DWORD*)pShaderBuffer, &Table);
			}
			else {
				Logger::Log("ERROR: Shader %s not found. Try to enable the CompileShader option to recompile the shaders.", FileNameBinary);
			}
		}
		if (Shader) {
			Function = Shader->GetBufferPointer();
			CreateCT();
			Logger::Log("Shader loaded: %s", FileNameBinary);
			return true;
		}
	}
	return false;
}

void ShaderRecord::CreateCT() {

	D3DXCONSTANTTABLE_DESC ConstantTableDesc;
	D3DXCONSTANT_DESC ConstantDesc;
	D3DXHANDLE Handle;
	UINT ConstantCount = 1;
	UInt32 FloatIndex = 0;
	UInt32 TextureIndex = 0;
	
	Table->GetDesc(&ConstantTableDesc);
    for (UINT c = 0; c < ConstantTableDesc.Constants; c++) {
		Handle = Table->GetConstant(NULL, c);
		Table->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
		if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4 && !memcmp(ConstantDesc.Name, "TESR_", 5)) FloatShaderValuesCount += 1;
		if (ConstantDesc.RegisterSet == D3DXRS_SAMPLER && !memcmp(ConstantDesc.Name, "TESR_", 5)) TextureShaderValuesCount += 1;
    }
	HasCT = FloatShaderValuesCount + TextureShaderValuesCount;
    if (HasCT) {
		FloatShaderValues = (ShaderValue*)malloc(FloatShaderValuesCount * sizeof(ShaderValue));
		TextureShaderValues = (ShaderValue*)malloc(TextureShaderValuesCount * sizeof(ShaderValue));
		for (UINT c = 0; c < ConstantTableDesc.Constants; c++) {
			Handle = Table->GetConstant(NULL, c);
			Table->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
			if (!memcmp(ConstantDesc.Name, "TESR_", 5)) {
				switch (ConstantDesc.RegisterSet) {
					case D3DXRS_FLOAT4:
						SetConstantTableValue(ConstantDesc.Name, FloatIndex);
						FloatShaderValues[FloatIndex].RegisterIndex = ConstantDesc.RegisterIndex;
						FloatShaderValues[FloatIndex].RegisterCount = ConstantDesc.RegisterCount;
						FloatIndex++;
 						break;
					case D3DXRS_SAMPLER:
						if (!strcmp(ConstantDesc.Name, WordRenderedBuffer)) HasRB = true;
						if (!strcmp(ConstantDesc.Name, WordDepthBuffer)) HasDB = true;
						TextureShaderValues[TextureIndex].Texture = TheTextureManager->LoadTexture(Source, ConstantDesc.RegisterIndex);
						TextureShaderValues[TextureIndex].RegisterIndex = ConstantDesc.RegisterIndex;
						TextureShaderValues[TextureIndex].RegisterCount = 1;
						TextureIndex++;
						break;
				}
			}
		}
	}

}

void ShaderRecord::SetCT() {
	
	ShaderValue* Value;

	if (HasCT) {
		if (HasRB && !TheShaderManager->RenderedBufferFilled) {
			TheRenderManager->device->StretchRect(TheRenderManager->currentRTGroup->RenderTargets[0]->data->Surface, NULL, TheShaderManager->RenderedSurface, NULL, D3DTEXF_NONE);
			TheShaderManager->RenderedBufferFilled = true;
		}
		if (HasDB && !TheShaderManager->DepthBufferFilled) {
			TheRenderManager->ResolveDepthBuffer();
			TheShaderManager->DepthBufferFilled = true;
		}
		for (UInt32 c = 0; c < TextureShaderValuesCount; c++) {
			Value = &TextureShaderValues[c];
			if (Value->Texture->Texture) TheRenderManager->device->SetTexture(Value->RegisterIndex, Value->Texture->Texture);
			if (Value->Texture->SamplerStates[0]) {
				for (int t = 1; t < SamplerStatesMax; t++) {
					if (Value->Texture->SamplerStates[t]) TheRenderManager->SetSamplerState(Value->RegisterIndex, (D3DSAMPLERSTATETYPE)t, Value->Texture->SamplerStates[t]);
				}
			}
		}
		for (UInt32 c = 0; c < FloatShaderValuesCount; c++) {
			Value = &FloatShaderValues[c];
			if (Type == ShaderType_Vertex)
				TheRenderManager->device->SetVertexShaderConstantF(Value->RegisterIndex, (const float *)Value->Value, Value->RegisterCount);
			else
				TheRenderManager->device->SetPixelShaderConstantF(Value->RegisterIndex, (const float *)Value->Value, Value->RegisterCount);
		}
	}

}

EffectRecord::EffectRecord() {

	Enabled = false;
	Source = NULL;
	Effect = NULL;
	Errors = NULL;

}

EffectRecord::~EffectRecord() {

	if (Effect) Effect->Release();
	if (Errors) Errors->Release();
	if (Source) delete Source;

}

bool EffectRecord::LoadEffect(const char* Name) {

	char FileName[MAX_PATH];
	bool Compiled = true;

	strcpy(FileName, Name);
	strcat(FileName, ".hlsl");
	std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (FileSource.is_open()) {
		std::streampos size = FileSource.tellg();
		Source = new char[size];
		FileSource.seekg(0, std::ios::beg);
		FileSource.read(Source, size);
		FileSource.close();
		if (TheSettingManager->SettingsMain.Develop.CompileEffects) {
			Compiled = false;
			ID3DXEffectCompiler* Compiler = NULL;
			ID3DXBuffer* EffectBuffer = NULL;
			D3DXCreateEffectCompilerFromFileA(FileName, NULL, NULL, NULL, &Compiler, &Errors);
			if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
			if (Compiler) {
				Compiler->CompileEffect(NULL, &EffectBuffer, &Errors);
				if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
			}
			if (EffectBuffer) {
				std::ofstream FileBinary(Name, std::ios::out | std::ios::binary);
				FileBinary.write((char*)EffectBuffer->GetBufferPointer(), EffectBuffer->GetBufferSize());
				FileBinary.flush();
				FileBinary.close();
				Compiled = true;
				Logger::Log("Effect compiled: %s", FileName);
			}
			if (EffectBuffer) EffectBuffer->Release();
			if (Compiler) Compiler->Release();
		}
		if (Compiled) {
			D3DXCreateEffectFromFileA(TheRenderManager->device, Name, NULL, NULL, NULL, NULL, &Effect, &Errors);
			if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
			if (Effect) {
				CreateCT();
				Logger::Log("Effect loaded: %s", Name);
				return true;
			}
		}
	}
	return false;

}

void EffectRecord::CreateCT() {

	D3DXEFFECT_DESC ConstantTableDesc;
	D3DXPARAMETER_DESC ConstantDesc;
	D3DXHANDLE Handle;
	UINT ConstantCount = 1;
	UInt32 FloatIndex = 0;
	UInt32 TextureIndex = 0;

	Effect->GetDesc(&ConstantTableDesc);
	for (UINT c = 0; c < ConstantTableDesc.Parameters; c++) {
		Handle = Effect->GetParameter(NULL, c);
		Effect->GetParameterDesc(Handle, &ConstantDesc);
		if ((ConstantDesc.Class == D3DXPC_VECTOR || ConstantDesc.Class == D3DXPC_MATRIX_ROWS) && !memcmp(ConstantDesc.Name, "TESR_", 5)) FloatShaderValuesCount += 1;
		if (ConstantDesc.Class == D3DXPC_OBJECT && ConstantDesc.Type >= D3DXPT_SAMPLER && ConstantDesc.Type <= D3DXPT_SAMPLERCUBE && !memcmp(ConstantDesc.Name, "TESR_", 5)) TextureShaderValuesCount += 1;
	}
	FloatShaderValues = (ShaderValue*)malloc(FloatShaderValuesCount * sizeof(ShaderValue));
	TextureShaderValues = (ShaderValue*)malloc(TextureShaderValuesCount * sizeof(ShaderValue));
	for (UINT c = 0; c < ConstantTableDesc.Parameters; c++) {
		Handle = Effect->GetParameter(NULL, c);
		Effect->GetParameterDesc(Handle, &ConstantDesc);
		if (!memcmp(ConstantDesc.Name, "TESR_", 5)) {
			switch (ConstantDesc.Class) {
				case D3DXPC_VECTOR:
				case D3DXPC_MATRIX_ROWS:
					SetConstantTableValue(ConstantDesc.Name, FloatIndex);
					FloatShaderValues[FloatIndex].RegisterIndex = (UInt32)Handle;
					FloatShaderValues[FloatIndex].RegisterCount = ConstantDesc.Rows;
					FloatIndex++;
					break;
				case D3DXPC_OBJECT:
					if (ConstantDesc.Class == D3DXPC_OBJECT && ConstantDesc.Type >= D3DXPT_SAMPLER && ConstantDesc.Type <= D3DXPT_SAMPLERCUBE) {
						TextureShaderValues[TextureIndex].Texture = TheTextureManager->LoadTexture(Source, TextureIndex);
						TextureShaderValues[TextureIndex].RegisterIndex = TextureIndex;
						TextureShaderValues[TextureIndex].RegisterCount = 1;
						TextureIndex++;
					}
					break;
			}
		}
	}

}

void EffectRecord::SetCT() {

	ShaderValue* Value;

	for (UInt32 c = 0; c < TextureShaderValuesCount; c++) {
		Value = &TextureShaderValues[c];
		if (Value->Texture->Texture) TheRenderManager->device->SetTexture(Value->RegisterIndex, Value->Texture->Texture);
	}
	for (UInt32 c = 0; c < FloatShaderValuesCount; c++) {
		Value = &FloatShaderValues[c];
		if (Value->RegisterCount == 1)
			Effect->SetVector((D3DXHANDLE)Value->RegisterIndex, Value->Value);
		else
			Effect->SetMatrix((D3DXHANDLE)Value->RegisterIndex, (D3DXMATRIX*)Value->Value);
	}

}

void EffectRecord::Render(IDirect3DDevice9* Device, IDirect3DSurface9* RenderTarget, IDirect3DSurface9* RenderedSurface, bool ClearRenderTarget) {

	UINT Passes;

	Effect->Begin(&Passes, NULL);
	for (UINT p = 0; p < Passes; p++) {
		if (ClearRenderTarget) Device->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0L);
		Effect->BeginPass(p);
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Effect->EndPass();
		Device->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	}
	Effect->End();

}

ShaderManager::ShaderManager() {

	Logger::Log("Starting the shaders manager...");
	TheShaderManager = this;

	LARGE_INTEGER Frequency;
	float UAdj, VAdj;
	void* VertexPointer;

	SourceTexture = NULL;
	SourceSurface = NULL;
	RenderedTexture = NULL;
	RenderedSurface = NULL;
	RenderTextureSMAA = NULL;
	RenderSurfaceSMAA = NULL;
	RenderedBufferFilled = false;
	DepthBufferFilled = false;
	EffectVertex = NULL;
	UnderwaterEffect = NULL;
	WaterLensEffect = NULL;
	GodRaysEffect = NULL;
	DepthOfFieldEffect = NULL;
	AmbientOcclusionEffect = NULL;
	ColoringEffect = NULL;
	CinemaEffect = NULL;
	BloomEffect = NULL;
	SnowAccumulationEffect = NULL;
	BloodLensEffect = NULL;
	SMAAEffect = NULL;
	MotionBlurEffect = NULL;
	LowHFEffect = NULL;
	WetWorldEffect = NULL;
	SharpeningEffect = NULL;
	VolumetricFogEffect = NULL;
	RainEffect = NULL;
	SnowEffect = NULL;
	ShadowsExteriorsEffect = NULL;
	ShadowsInteriorsEffect = NULL;
	WaterHeightMapVertexShader = NULL;
	WaterHeightMapPixelShader = NULL;
	memset(WaterVertexShaders, NULL, sizeof(WaterVertexShaders));
	memset(WaterPixelShaders, NULL, sizeof(WaterPixelShaders));
	InitializeConstants();
	QueryPerformanceFrequency(&Frequency);
	PerformanceFrequency = Frequency.QuadPart;
	ShaderConst.ReciprocalResolution.x = 1.0f / TheRenderManager->width;
	ShaderConst.ReciprocalResolution.y = 1.0f / TheRenderManager->height;
	ShaderConst.ReciprocalResolution.z = TheRenderManager->width / TheRenderManager->height;
	ShaderConst.ReciprocalResolution.w = 0.0f; // Reserved to store the FoV
	ShaderConst.ReciprocalResolutionWater.x = 1.0f / WaterHeightMapSize;
	ShaderConst.ReciprocalResolutionWater.y = 1.0f / WaterHeightMapSize;
	ShaderConst.ReciprocalResolutionWater.z = 1.0f / WaterDisplacementMapSize;
	ShaderConst.ReciprocalResolutionWater.w = 1.0f / WaterDisplacementMapSize;
	UAdj = (1.0f / (float)TheRenderManager->width) * 0.5f;
	VAdj = (1.0f / (float)TheRenderManager->height) * 0.5f;
	EffectQuad ShaderVertices[] = {
		{ -1.0f,  1.0f, 1.0f, 0.0f + UAdj, 0.0f + VAdj },
		{ -1.0f, -1.0f, 1.0f, 0.0f + UAdj, 1.0f + VAdj },
		{  1.0f,  1.0f, 1.0f, 1.0f + UAdj, 0.0f + VAdj },
		{  1.0f, -1.0f, 1.0f, 1.0f + UAdj, 1.0f + VAdj }
	};
	TheRenderManager->device->CreateVertexBuffer(4 * sizeof(EffectQuad), D3DUSAGE_WRITEONLY, EFFECTQUADFORMAT, D3DPOOL_DEFAULT, &EffectVertex, 0);
	EffectVertex->Lock(0, 0, &VertexPointer, 0);
	CopyMemory(VertexPointer, ShaderVertices, sizeof(ShaderVertices));
	EffectVertex->Unlock();
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &SourceTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderedTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &RenderTextureSMAA, NULL);
	SourceTexture->GetSurfaceLevel(0, &SourceSurface);
	RenderedTexture->GetSurfaceLevel(0, &RenderedSurface);
	RenderTextureSMAA->GetSurfaceLevel(0, &RenderSurfaceSMAA);

}

void ShaderManager::CreateEffects() {
	
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;

	if (Effects->AmbientOcclusion) CreateEffect(EffectRecordType_AmbientOcclusion);
	if (Effects->BloodLens) CreateEffect(EffectRecordType_BloodLens);
	if (Effects->Bloom) CreateEffect(EffectRecordType_Bloom);
	if (Effects->Cinema) CreateEffect(EffectRecordType_Cinema);
	if (Effects->Coloring) CreateEffect(EffectRecordType_Coloring);
	if (Effects->DepthOfField) CreateEffect(EffectRecordType_DepthOfField);
	if (Effects->GodRays) CreateEffect(EffectRecordType_GodRays);
	if (Effects->LowHF) CreateEffect(EffectRecordType_LowHF);
	if (Effects->MotionBlur) CreateEffect(EffectRecordType_MotionBlur);
	if (Effects->Sharpening) CreateEffect(EffectRecordType_Sharpening);
	if (Effects->SMAA) CreateEffect(EffectRecordType_SMAA);
	if (Effects->SnowAccumulation) CreateEffect(EffectRecordType_SnowAccumulation);
	if (Effects->Underwater) CreateEffect(EffectRecordType_Underwater);
	if (Effects->VolumetricFog) CreateEffect(EffectRecordType_VolumetricFog);
	if (Effects->WaterLens) CreateEffect(EffectRecordType_WaterLens);
	if (Effects->WetWorld) CreateEffect(EffectRecordType_WetWorld);
	if (Effects->Precipitations) CreateEffect(EffectRecordType_Precipitations);
	if (Effects->Extra) CreateEffect(EffectRecordType_Extra);
	if (TheSettingManager->SettingsShadows.Exteriors.Quality == -1) CreateEffect(EffectRecordType_ShadowsExteriors);
	if (TheSettingManager->SettingsShadows.Interiors.Quality == -1) CreateEffect(EffectRecordType_ShadowsInteriors);

}

void ShaderManager::InitializeConstants() {

	ShaderConst.pWeather = NULL;
	ShaderConst.WaterLens.Percent = 0.0f;
	ShaderConst.BloodLens.Percent = 0.0f;
	ShaderConst.SnowAccumulation.Params.w = 0.0f;
	ShaderConst.WetWorld.Data.x = 0.0f;
}

void ShaderManager::UpdateConstants() {

	LARGE_INTEGER PerformanceCount;
	double Tick;
	bool IsThirdPersonView;
	Sky* WorldSky = Tes->sky;
	NiNode* SunRoot = WorldSky->sun->RootNode;
	TESClimate* currentClimate = WorldSky->firstClimate;
	TESWeather* currentWeather = WorldSky->firstWeather;
	TESWeather* previousWeather = WorldSky->secondWeather;
	TESObjectCELL* currentCell = Player->parentCell;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();
	TESRegion* currentRegion = Player->GetRegion();
	float weatherPercent = WorldSky->weatherPercent;

	QueryPerformanceCounter(&PerformanceCount);
	Tick = (double)PerformanceCount.QuadPart / (double)PerformanceFrequency;
	ShaderConst.Tick.x = Tick;
	ShaderConst.Tick.y = (double)PerformanceCount.QuadPart * 1000.0 / (double)PerformanceFrequency;
	TheFrameRateManager->SetFrameTime(Tick);
	IsThirdPersonView = Player->IsThirdPersonView(TheSettingManager->SettingsMain.CameraMode.Enabled, TheRenderManager->FirstPersonView);
	TheRenderManager->GetSceneCameraData();
	
	if (currentCell) {
		ShaderConst.SunTiming.x = currentClimate->sunriseBegin / 6.0f - 1.0f;
		ShaderConst.SunTiming.y = currentClimate->sunriseEnd / 6.0f;
		ShaderConst.SunTiming.z = currentClimate->sunsetBegin / 6.0f;
		ShaderConst.SunTiming.w = currentClimate->sunsetEnd / 6.0f + 1.0f;
		
		float deltay = ShaderConst.GameTime.y;
		ShaderConst.GameTime.x = TimeGlobals::GetGameTime();
		ShaderConst.GameTime.y = ShaderConst.GameTime.x / 3600.0f;
		ShaderConst.GameTime.z = ((int)ShaderConst.GameTime.x / 60) % 60;
		ShaderConst.GameTime.w = ((int)ShaderConst.GameTime.x) % 60;

		if (deltay != ShaderConst.GameTime.y) {
			float deltaz = ShaderConst.SunDir.z;
			ShaderConst.SunDir.x = SunRoot->m_localTransform.pos.x;
			ShaderConst.SunDir.y = SunRoot->m_localTransform.pos.y;
			ShaderConst.SunDir.z = SunRoot->m_localTransform.pos.z;
			((NiVector4*)&ShaderConst.SunDir)->Normalize();
			if (ShaderConst.GameTime.y > ShaderConst.SunTiming.w || ShaderConst.GameTime.y < ShaderConst.SunTiming.x)
				ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
			else if (ShaderConst.GameTime.y > ShaderConst.SunTiming.z && fabs(deltaz) - ShaderConst.SunDir.z <= 0.0f)
				ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
			else if (ShaderConst.GameTime.y < ShaderConst.SunTiming.y && fabs(deltaz) - ShaderConst.SunDir.z >= 0.0f)
				ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
		}

		if (currentWorldSpace) {
			if (currentWeather) {
				ShaderConst.SunDir.w = 1.0f;
				if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.y && ShaderConst.GameTime.y <= ShaderConst.SunTiming.z) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 1.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 0.0f;
				}
				else if ((ShaderConst.GameTime.y >= ShaderConst.SunTiming.w && ShaderConst.GameTime.y <= 23.59) || (ShaderConst.GameTime.y >= 0 && ShaderConst.GameTime.y <= ShaderConst.SunTiming.x)) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 0.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 1.0f;
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.x && ShaderConst.GameTime.y <= ShaderConst.SunTiming.y) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 <= ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) { 
						ShaderConst.SunAmount.x = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
					}
					else {
						ShaderConst.SunAmount.x = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) - 1.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 0.0f;
					}
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.z && ShaderConst.GameTime.y <= ShaderConst.SunTiming.w) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 <= ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.z = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = 0.0f;
					}
					else {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) - 1.0f;
					}
				}

				if (ShaderConst.pWeather == NULL) ShaderConst.pWeather = currentWeather;

				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					RGBA color = currentWeather->colors[TESWeather::eColor_Fog].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.fogColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.fogColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.fogColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.fogColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.fogColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.fogColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}
				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					RGBA color = ShaderConst.pWeather->colors[TESWeather::eColor_Fog].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.oldfogColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldfogColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldfogColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.oldfogColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldfogColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldfogColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}

				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					RGBA color = currentWeather->colors[TESWeather::eColor_Sun].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.sunColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.sunColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.sunColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.sunColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.sunColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.sunColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}
				for (int t = TESWeather::eTime_Sunrise; t <= TESWeather::eTime_Night ; t++) {
					RGBA color = ShaderConst.pWeather->colors[TESWeather::eColor_Sun].colors[t];
					switch (t)
					{
						case TESWeather::eTime_Sunrise:
							ShaderConst.oldsunColor.x = color.r / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldsunColor.y = color.g / 255.0f * ShaderConst.SunAmount.x;
							ShaderConst.oldsunColor.z = color.b / 255.0f * ShaderConst.SunAmount.x;
							break;
						case TESWeather::eTime_Day:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.y;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.y;
							break;
						case TESWeather::eTime_Sunset:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.z;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.z;
							break;
						case TESWeather::eTime_Night:
							ShaderConst.oldsunColor.x += color.r / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldsunColor.y += color.g / 255.0f * ShaderConst.SunAmount.w;
							ShaderConst.oldsunColor.z += color.b / 255.0f * ShaderConst.SunAmount.w;
							break;
					}
				}

				if (ShaderConst.SunAmount.w == 1.0f) {
					ShaderConst.currentfogStart = currentWeather->GetFogNightNear();
					ShaderConst.currentfogEnd = currentWeather->GetFogNightFar();
					ShaderConst.oldfogStart = ShaderConst.pWeather->GetFogNightNear();
					ShaderConst.oldfogEnd = ShaderConst.pWeather->GetFogNightFar();
				}
				else {
					ShaderConst.currentfogStart = currentWeather->GetFogDayNear();
					ShaderConst.currentfogEnd = currentWeather->GetFogDayFar();
					ShaderConst.oldfogStart = ShaderConst.pWeather->GetFogDayNear();
					ShaderConst.oldfogEnd = ShaderConst.pWeather->GetFogDayFar();
				}

				ShaderConst.oldsunGlare = ShaderConst.pWeather->sunGlare;
				ShaderConst.oldwindSpeed = ShaderConst.pWeather->windSpeed;
				ShaderConst.currentsunGlare = (ShaderConst.oldsunGlare - ((ShaderConst.oldsunGlare - currentWeather->sunGlare) * weatherPercent)) / 255.0f;
				ShaderConst.currentwindSpeed = (ShaderConst.oldwindSpeed - ((ShaderConst.oldwindSpeed - currentWeather->windSpeed) * weatherPercent)) / 255.0f;

				ShaderConst.fogColor.x = ShaderConst.oldfogColor.x - ((ShaderConst.oldfogColor.x - ShaderConst.fogColor.x) * weatherPercent);
				ShaderConst.fogColor.y = ShaderConst.oldfogColor.y - ((ShaderConst.oldfogColor.y - ShaderConst.fogColor.y) * weatherPercent);
				ShaderConst.fogColor.z = ShaderConst.oldfogColor.z - ((ShaderConst.oldfogColor.z - ShaderConst.fogColor.z) * weatherPercent);
				ShaderConst.fogColor.w = 1.0f;

				ShaderConst.sunColor.x = ShaderConst.oldsunColor.x - ((ShaderConst.oldsunColor.x - ShaderConst.sunColor.x) * weatherPercent);
				ShaderConst.sunColor.y = ShaderConst.oldsunColor.y - ((ShaderConst.oldsunColor.y - ShaderConst.sunColor.y) * weatherPercent);
				ShaderConst.sunColor.z = ShaderConst.oldsunColor.z - ((ShaderConst.oldsunColor.z - ShaderConst.sunColor.z) * weatherPercent);
				ShaderConst.sunColor.w = ShaderConst.SunAmount.w;

				ShaderConst.fogData.x = ShaderConst.oldfogStart - ((ShaderConst.oldfogStart - ShaderConst.currentfogStart) * weatherPercent);
				ShaderConst.fogData.y = ShaderConst.oldfogEnd - ((ShaderConst.oldfogEnd - ShaderConst.currentfogEnd) * weatherPercent);
				ShaderConst.fogData.z = ShaderConst.currentsunGlare;

				if (weatherPercent == 1.0f) ShaderConst.pWeather = currentWeather;
			}
		}
		else {
			ShaderConst.SunDir.w = 0.0f;
			ShaderConst.SunAmount.x = 0.0f;
			ShaderConst.SunAmount.y = 1.0f;
			ShaderConst.SunAmount.z = 0.0f;
			ShaderConst.SunAmount.w = 0.0f;
			ShaderConst.currentsunGlare = 0.5f;
			TESObjectCELL::LightingData* LightData = currentCell->lighting;
			ShaderConst.fogColor.x = LightData->fog.r / 255.0f;
			ShaderConst.fogColor.y = LightData->fog.g / 255.0f;
			ShaderConst.fogColor.z = LightData->fog.b / 255.0f;
			ShaderConst.fogColor.w = 1.0f;

			ShaderConst.sunColor.x = LightData->ambient.r / 255.0f;
			ShaderConst.sunColor.y = LightData->ambient.g / 255.0f;
			ShaderConst.sunColor.z = LightData->ambient.b / 255.0f;
			ShaderConst.sunColor.w = ShaderConst.SunAmount.w;

			ShaderConst.fogData.x = LightData->fogNear;
			ShaderConst.fogData.y = LightData->fogFar;
			ShaderConst.fogData.z = ShaderConst.currentsunGlare;
		}

		if (TheSettingManager->SettingsMain.Shaders.Water || TheSettingManager->SettingsMain.Effects.Underwater) {
			SettingsWaterStruct* sws = NULL;
			TESWaterForm* currentWater = currentCell->GetWaterForm();
			
			if (CurrentBlend == 0.25f)
				sws = TheSettingManager->GetSettingsWater("Blood");
			else if (CurrentBlend == 0.50f)
				sws = TheSettingManager->GetSettingsWater("Lava");
			else
				if (!(sws = TheSettingManager->GetSettingsWater(currentCell->GetEditorName())))
					if (currentWorldSpace) sws = TheSettingManager->GetSettingsWater(currentWorldSpace->GetEditorName());

			if (!sws) sws = TheSettingManager->GetSettingsWater("Default");

			if (currentWater) {
				ShaderConst.Water.deepColor.x = currentWater->DeepColorR / 255.0f;
				ShaderConst.Water.deepColor.y = currentWater->DeepColorG / 255.0f;
				ShaderConst.Water.deepColor.z = currentWater->DeepColorB / 255.0f;
				ShaderConst.Water.deepColor.w = currentWater->DeepColorA / 255.0f;
			
				ShaderConst.Water.shallowColor.x = currentWater->ShallowColorR / 255.0f;
				ShaderConst.Water.shallowColor.y = currentWater->ShallowColorG / 255.0f;
				ShaderConst.Water.shallowColor.z = currentWater->ShallowColorB / 255.0f;
				ShaderConst.Water.shallowColor.w = currentWater->ShallowColorA / 255.0f;
			}

			ShaderConst.Water.waterCoefficients.x = sws->inExtCoeff_R;
			ShaderConst.Water.waterCoefficients.y = sws->inExtCoeff_G;
			ShaderConst.Water.waterCoefficients.z = sws->inExtCoeff_B;
			ShaderConst.Water.waterCoefficients.w = sws->inScattCoeff;

			ShaderConst.Water.waveParams.x = sws->choppiness;
			ShaderConst.Water.waveParams.y = sws->waveWidth;
			ShaderConst.Water.waveParams.z = sws->waveSpeed;
			ShaderConst.Water.waveParams.w = sws->reflectivity;

			ShaderConst.Water.waterSettings.x = Tes->GetWaterHeight(Player);
			ShaderConst.Water.waterSettings.y = sws->depthDarkness;
			ShaderConst.Water.waterSettings.z = sws->LODdistance;
			ShaderConst.Water.waterSettings.w = sws->MinLOD;

			ShaderConst.Water.waterVolume.x = sws->causticsStrength * ShaderConst.currentsunGlare;
			ShaderConst.Water.waterVolume.y = sws->shoreFactor;
			ShaderConst.Water.waterVolume.z = sws->turbidity;
			ShaderConst.Water.waterVolume.w = sws->causticsStrengthS;
			
			ShaderConst.Water.shorelineParams.x = sws->shoreMovement;

			ShaderConst.HasWater = currentCell->flags0 & TESObjectCELL::kFlags0_HasWater;

			if (TheSettingManager->SettingsMain.Effects.Underwater && TheSettingManager->SettingsMain.Effects.WaterLens) {
				ShaderConst.WaterLens.Time.x = sws->LensTimeMultA;
				ShaderConst.WaterLens.Time.y = sws->LensTimeMultB;
				ShaderConst.WaterLens.Time.z = sws->LensViscosity;
				if (ShaderConst.WaterLens.Percent == -1.0f) {
					ShaderConst.WaterLens.TimeAmount = 0.0f;
					ShaderConst.WaterLens.Time.w = sws->LensAmount;
				}
				else if (ShaderConst.WaterLens.Percent > 0.0f) {
					ShaderConst.WaterLens.TimeAmount += 1.0f;
					ShaderConst.WaterLens.Percent = 1.0f - ShaderConst.WaterLens.TimeAmount / sws->LensTime;
					if (ShaderConst.WaterLens.Percent < 0.0f) ShaderConst.WaterLens.Percent = 0.0f;
					ShaderConst.WaterLens.Time.w = sws->LensAmount * ShaderConst.WaterLens.Percent;
				}
			}

		}		

		if (TheSettingManager->SettingsMain.Effects.SnowAccumulation) {
			if (currentWeather->weatherType == TESWeather::WeatherType::kType_Snow) {
				if (ShaderConst.SnowAccumulation.Params.w < TheSettingManager->SettingsPrecipitations.SnowAccumulation.Amount) ShaderConst.SnowAccumulation.Params.w = ShaderConst.SnowAccumulation.Params.w + TheSettingManager->SettingsPrecipitations.SnowAccumulation.Increase;
			}
			else if (!previousWeather || (previousWeather && previousWeather->weatherType == TESWeather::WeatherType::kType_Snow)) {
				if (ShaderConst.SnowAccumulation.Params.w > 0.0f)
					ShaderConst.SnowAccumulation.Params.w = ShaderConst.SnowAccumulation.Params.w - TheSettingManager->SettingsPrecipitations.SnowAccumulation.Decrease;
				else if (ShaderConst.SnowAccumulation.Params.w < 0.0f)
					ShaderConst.SnowAccumulation.Params.w = 0.0f;
			}
			ShaderConst.SnowAccumulation.Params.x = TheSettingManager->SettingsPrecipitations.SnowAccumulation.BlurNormDropThreshhold;
			ShaderConst.SnowAccumulation.Params.y = TheSettingManager->SettingsPrecipitations.SnowAccumulation.BlurRadiusMultiplier;
			ShaderConst.SnowAccumulation.Params.z = TheSettingManager->SettingsPrecipitations.SnowAccumulation.SunPower;
		}
			
		if (TheSettingManager->SettingsMain.Effects.WetWorld) {
			if (currentWeather->weatherType == TESWeather::WeatherType::kType_Rainy) {
				ShaderConst.WetWorld.Data.y = 1.0f;
				if (ShaderConst.WetWorld.Data.x < TheSettingManager->SettingsPrecipitations.WetWorld.Amount) ShaderConst.WetWorld.Data.x = ShaderConst.WetWorld.Data.x + TheSettingManager->SettingsPrecipitations.WetWorld.Increase;
			}
			else if (!previousWeather || (previousWeather && previousWeather->weatherType == TESWeather::WeatherType::kType_Rainy)) {
				ShaderConst.WetWorld.Data.y = 0.3f - weatherPercent;
				if (ShaderConst.WetWorld.Data.y <= 0.0f) ShaderConst.WetWorld.Data.y = 0.05f;
				if (ShaderConst.WetWorld.Data.x > 0.0f)
					ShaderConst.WetWorld.Data.x = ShaderConst.WetWorld.Data.x - TheSettingManager->SettingsPrecipitations.WetWorld.Decrease;
				else if (ShaderConst.WetWorld.Data.x < 0.0f)
					ShaderConst.WetWorld.Data.x = 0.0f;
			}
			ShaderConst.WetWorld.Coeffs.x = TheSettingManager->SettingsPrecipitations.WetWorld.PuddleCoeff_R;
			ShaderConst.WetWorld.Coeffs.y = TheSettingManager->SettingsPrecipitations.WetWorld.PuddleCoeff_G;
			ShaderConst.WetWorld.Coeffs.z = TheSettingManager->SettingsPrecipitations.WetWorld.PuddleCoeff_B;
			ShaderConst.WetWorld.Coeffs.w = TheSettingManager->SettingsPrecipitations.WetWorld.PuddleSpecularMultiplier;
		}
		
		if (TheSettingManager->SettingsMain.Effects.Precipitations) {
			if (currentWeather->weatherType == TESWeather::WeatherType::kType_Rainy)
				ShaderConst.Precipitations.RainData.x = weatherPercent;
			else if (!previousWeather || (previousWeather && previousWeather->weatherType == TESWeather::WeatherType::kType_Rainy))
				ShaderConst.Precipitations.RainData.x = 1.0f - weatherPercent;
			if (currentWeather->weatherType == TESWeather::WeatherType::kType_Snow)
				ShaderConst.Precipitations.SnowData.x = weatherPercent;
			else if (!previousWeather || (previousWeather && previousWeather->weatherType == TESWeather::WeatherType::kType_Snow))
				ShaderConst.Precipitations.SnowData.x = 1.0f - weatherPercent;
			ShaderConst.Precipitations.SnowData.y = TheSettingManager->SettingsPrecipitations.Snow.DepthStep;
			ShaderConst.Precipitations.SnowData.z = TheSettingManager->SettingsPrecipitations.Snow.Flakes;
			ShaderConst.Precipitations.SnowData.w = TheSettingManager->SettingsPrecipitations.Snow.Speed;
		}
		
		if (TheSettingManager->SettingsMain.Shaders.Grass) {
			ShaderConst.Grass.Scale.x = TheSettingManager->SettingsGrass.ScaleX;
			ShaderConst.Grass.Scale.y = TheSettingManager->SettingsGrass.ScaleY;
			ShaderConst.Grass.Scale.z = TheSettingManager->SettingsGrass.ScaleZ;
			ShaderConst.Grass.Scale.w = TheSettingManager->SettingsGrass.MinHeight;
			switch (TheSettingManager->SettingsGrass.GrassDensity)
			{
				case 1:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 2:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 3:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 4:
					*SettingMinGrassSize = 240;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 5:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 6:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 7:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 8:
					*SettingMinGrassSize = 120;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 9:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 10:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 11:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 12:
					*SettingMinGrassSize = 80;
					*SettingTexturePctThreshold = 0.0f;
					break;
				case 13:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.3f;
					break;
				case 14:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.2f;
					break;
				case 15:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.1f;
					break;
				case 16:
					*SettingMinGrassSize = 20;
					*SettingTexturePctThreshold = 0.0f;
					break;
				default:
					break;
			}
			*SettingGrassStartFadeDistance = TheSettingManager->SettingsGrass.MinDistance;
			*SettingGrassEndDistance = TheSettingManager->SettingsGrass.MaxDistance;
			if (TheSettingManager->SettingsGrass.WindEnabled) {
				*SettingGrassWindMagnitudeMax = *LocalGrassWindMagnitudeMax = TheSettingManager->SettingsGrass.WindCoefficient * ShaderConst.currentwindSpeed;
				*SettingGrassWindMagnitudeMin = *LocalGrassWindMagnitudeMin = *SettingGrassWindMagnitudeMax * 0.5f;
			}
		}

		if (TheSettingManager->SettingsMain.Shaders.HDR) {
			ShaderConst.HDR.ToneMapping.x = TheSettingManager->SettingsHDR.ToneMapping;
			ShaderConst.HDR.ToneMapping.y = TheSettingManager->SettingsHDR.ToneMappingBlur;
			ShaderConst.HDR.ToneMapping.z = TheSettingManager->SettingsHDR.ToneMappingColor;
			ShaderConst.HDR.ToneMapping.w = TheSettingManager->SettingsHDR.Linearization;
		}

		if (TheSettingManager->SettingsMain.Shaders.POM) {
			ShaderConst.POM.ParallaxData.x = TheSettingManager->SettingsPOM.HeightMapScale;
			ShaderConst.POM.ParallaxData.y = TheSettingManager->SettingsPOM.ShadowSoftening;
			ShaderConst.POM.ParallaxData.z = TheSettingManager->SettingsPOM.MinSamples;
			ShaderConst.POM.ParallaxData.w = TheSettingManager->SettingsPOM.MaxSamples;
		}
	
		if (TheSettingManager->SettingsMain.Shaders.Terrain) {
			ShaderConst.Terrain.Data.x = TheSettingManager->SettingsTerrain.DistantSpecular;
			ShaderConst.Terrain.Data.y = TheSettingManager->SettingsTerrain.DistantNoise;
			ShaderConst.Terrain.Data.z = TheSettingManager->SettingsTerrain.NearSpecular;
			ShaderConst.Terrain.Data.w = TheSettingManager->SettingsTerrain.MiddleSpecular;
		}
	
		if (TheSettingManager->SettingsMain.Shaders.Skin) {
			ShaderConst.Skin.SkinData.x = TheSettingManager->SettingsSkin.Attenuation;
			ShaderConst.Skin.SkinData.y = TheSettingManager->SettingsSkin.SpecularPower;
			ShaderConst.Skin.SkinData.z = TheSettingManager->SettingsSkin.MaterialThickness;
			ShaderConst.Skin.SkinData.w = TheSettingManager->SettingsSkin.RimScalar;
			ShaderConst.Skin.SkinColor.x = TheSettingManager->SettingsSkin.CoeffRed;
			ShaderConst.Skin.SkinColor.y = TheSettingManager->SettingsSkin.CoeffGreen;
			ShaderConst.Skin.SkinColor.z = TheSettingManager->SettingsSkin.CoeffBlue;
		}

		if (TheSettingManager->SettingsMain.Effects.GodRays) {
			ShaderConst.GodRays.Ray.x = TheSettingManager->SettingsGodRays.RayIntensity;
			ShaderConst.GodRays.Ray.y = TheSettingManager->SettingsGodRays.RayLength;
			if (TheSettingManager->SettingsGodRays.SunGlareEnabled) {
				ShaderConst.GodRays.Ray.z = TheSettingManager->SettingsGodRays.RayDensity * ShaderConst.currentsunGlare;
				ShaderConst.GodRays.Ray.w = TheSettingManager->SettingsGodRays.RayVisibility * ShaderConst.currentsunGlare;
			}
			else {
				ShaderConst.GodRays.Ray.z = TheSettingManager->SettingsGodRays.RayDensity;
				ShaderConst.GodRays.Ray.w = TheSettingManager->SettingsGodRays.RayVisibility;
			}
			ShaderConst.GodRays.RayColor.x = TheSettingManager->SettingsGodRays.RayR;
			ShaderConst.GodRays.RayColor.y = TheSettingManager->SettingsGodRays.RayG;
			ShaderConst.GodRays.RayColor.z = TheSettingManager->SettingsGodRays.RayB;
			ShaderConst.GodRays.RayColor.w = TheSettingManager->SettingsGodRays.Saturate;
			ShaderConst.GodRays.Data.x = TheSettingManager->SettingsGodRays.LightShaftPasses;
			ShaderConst.GodRays.Data.y = TheSettingManager->SettingsGodRays.Luminance;
			ShaderConst.GodRays.Data.z = TheSettingManager->SettingsGodRays.GlobalMultiplier;
			ShaderConst.GodRays.Data.w = TheSettingManager->SettingsGodRays.TimeEnabled;
		}

		if (TheSettingManager->SettingsMain.Effects.AmbientOcclusion) {
			SettingsAmbientOcclusionStruct* sas = NULL;

			if (currentWorldSpace)
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Exterior");
			else
				sas = TheSettingManager->GetSettingsAmbientOcclusion("Interior");

			ShaderConst.AmbientOcclusion.Enabled = sas->Enabled;
			if (ShaderConst.AmbientOcclusion.Enabled) {
				ShaderConst.AmbientOcclusion.AOData.x = sas->RadiusMultiplier;
				ShaderConst.AmbientOcclusion.AOData.y = sas->StrengthMultiplier;
				ShaderConst.AmbientOcclusion.AOData.z = sas->ClampStrength;
				ShaderConst.AmbientOcclusion.AOData.w = sas->Range;
				ShaderConst.AmbientOcclusion.Data.x = sas->AngleBias;
				ShaderConst.AmbientOcclusion.Data.y = sas->LumThreshold * ShaderConst.SunAmount.y;
				ShaderConst.AmbientOcclusion.Data.z = sas->BlurDropThreshold;
				ShaderConst.AmbientOcclusion.Data.w = sas->BlurRadiusMultiplier;
			}
		}

		if (TheSettingManager->SettingsMain.Effects.Bloom) {
			SettingsBloomStruct* sbs = NULL;
			if (!(sbs = TheSettingManager->GetSettingsBloom(currentCell->GetEditorName())))
				if (currentWorldSpace)
					sbs = TheSettingManager->GetSettingsBloom(currentWorldSpace->GetEditorName());

			if (!sbs) sbs = TheSettingManager->GetSettingsBloom("Default");

			ShaderConst.Bloom.BloomData.x = sbs->Luminance;
			ShaderConst.Bloom.BloomData.y = sbs->MiddleGray;
			ShaderConst.Bloom.BloomData.z = sbs->WhiteCutOff;
			ShaderConst.Bloom.BloomValues.x = sbs->BloomIntensity;
			ShaderConst.Bloom.BloomValues.y = sbs->OriginalIntensity;
			ShaderConst.Bloom.BloomValues.z = sbs->BloomSaturation;
			ShaderConst.Bloom.BloomValues.w = sbs->OriginalSaturation;
		}

		if (TheSettingManager->SettingsMain.Effects.Coloring) {
			SettingsColoringStruct* scs = NULL;
			if (!(scs = TheSettingManager->GetSettingsColoring(currentCell->GetEditorName())))
				if (currentWorldSpace)
					scs = TheSettingManager->GetSettingsColoring(currentWorldSpace->GetEditorName());

			if (!scs) scs = TheSettingManager->GetSettingsColoring("Default");

			ShaderConst.Coloring.Data.x = scs->Strength;
			ShaderConst.Coloring.Data.y = scs->BaseGamma;
			ShaderConst.Coloring.Data.z = scs->Fade;
			ShaderConst.Coloring.Data.w = scs->Contrast;
			ShaderConst.Coloring.Values.x = scs->Saturation;
			ShaderConst.Coloring.Values.y = scs->Bleach;
			ShaderConst.Coloring.Values.z = scs->BleachLuma;
			ShaderConst.Coloring.Values.w = scs->Linearization;
			ShaderConst.Coloring.ColorCurve.x = scs->ColorCurve;
			ShaderConst.Coloring.ColorCurve.y = scs->ColorCurveR;
			ShaderConst.Coloring.ColorCurve.z = scs->ColorCurveG;
			ShaderConst.Coloring.ColorCurve.w = scs->ColorCurveB;
			ShaderConst.Coloring.EffectGamma.x = scs->EffectGamma;
			ShaderConst.Coloring.EffectGamma.y = scs->EffectGammaR;
			ShaderConst.Coloring.EffectGamma.z = scs->EffectGammaG;
			ShaderConst.Coloring.EffectGamma.w = scs->EffectGammaB;
		}

		if (TheSettingManager->SettingsMain.Effects.BloodLens) {
			if (ShaderConst.BloodLens.Percent > 0.0f) {
				ShaderConst.BloodLens.Time.z = TheSettingManager->SettingsBlood.LensTime;
				if (ShaderConst.BloodLens.Percent == 1.0f) {
					ShaderConst.BloodLens.Time.w = 0.0f;
					srand(time(NULL));
					ShaderConst.BloodLens.Params.x = (double)rand() / (RAND_MAX + 1) * (0.75f - 0.25f) + 0.25f; //from 0.25 to 0.75
					ShaderConst.BloodLens.Params.y = (double)rand() / (RAND_MAX + 1) * (0.5f + 0.1f) - 0.1f; //from -0.1 to 0.5
					ShaderConst.BloodLens.Params.z = (double)rand() / (RAND_MAX + 1) * (2.0f + 2.0f) - 2.0f; //from -2 to 2
					ShaderConst.BloodLens.Params.w = TheSettingManager->SettingsBlood.LensIntensity;
				}
				ShaderConst.BloodLens.Time.w += 1.0f;
				ShaderConst.BloodLens.Percent = 1.0f - ShaderConst.BloodLens.Time.w / ShaderConst.BloodLens.Time.z;
				if (ShaderConst.BloodLens.Percent < 0.0f)
					ShaderConst.BloodLens.Percent = 0.0f;
				ShaderConst.BloodLens.Params.w = TheSettingManager->SettingsBlood.LensIntensity * ShaderConst.BloodLens.Percent;
				ShaderConst.BloodLens.BloodColor.x = TheSettingManager->SettingsBlood.LensColorR;
				ShaderConst.BloodLens.BloodColor.y = TheSettingManager->SettingsBlood.LensColorG;
				ShaderConst.BloodLens.BloodColor.z = TheSettingManager->SettingsBlood.LensColorB;
			}
		}

		if (TheSettingManager->SettingsMain.Effects.LowHF) {
			float PlayerHealthPercent = (float)Player->GetActorValue(Actor::ActorVal::kActorVal_Health) / (float)Player->GetBaseActorValue(Actor::ActorVal::kActorVal_Health);
			float PlayerFatiguePercent = (float)Player->GetActorValue(Actor::ActorVal::kActorVal_Stamina) / (float)Player->GetBaseActorValue(Actor::ActorVal::kActorVal_Stamina);

			ShaderConst.LowHF.Data.x = 0.0f;
			ShaderConst.LowHF.Data.y = 0.0f;
			ShaderConst.LowHF.Data.z = 0.0f;
			ShaderConst.LowHF.Data.w = 0.0f;
			if (Player->GetLifeState(1)) {
				ShaderConst.LowHF.HealthCoeff = 1.0f - PlayerHealthPercent / TheSettingManager->SettingsLowHF.HealthLimit;
				ShaderConst.LowHF.FatigueCoeff = 1.0f - PlayerFatiguePercent / TheSettingManager->SettingsLowHF.FatigueLimit;
				if (PlayerHealthPercent < TheSettingManager->SettingsLowHF.HealthLimit) {
					ShaderConst.LowHF.Data.x = ShaderConst.LowHF.HealthCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
					ShaderConst.LowHF.Data.y = ShaderConst.LowHF.HealthCoeff * 0.01f * TheSettingManager->SettingsLowHF.BlurMultiplier;
					ShaderConst.LowHF.Data.z = ShaderConst.LowHF.HealthCoeff * 20.0f * TheSettingManager->SettingsLowHF.VignetteMultiplier;
					ShaderConst.LowHF.Data.w = (1.0f - ShaderConst.LowHF.HealthCoeff) * TheSettingManager->SettingsLowHF.DarknessMultiplier;
				}
				if (!ShaderConst.LowHF.Data.x && PlayerFatiguePercent < TheSettingManager->SettingsLowHF.FatigueLimit) ShaderConst.LowHF.Data.x = ShaderConst.LowHF.FatigueCoeff * TheSettingManager->SettingsLowHF.LumaMultiplier;
			}
		}

		if (TheSettingManager->SettingsMain.Effects.DepthOfField) {
			SettingsDepthOfFieldStruct* sds = NULL;

			if (Player->IsVanityView())
				sds = TheSettingManager->GetSettingsDepthOfField("VanityView");
			else if (IsThirdPersonView)
				sds = TheSettingManager->GetSettingsDepthOfField("ThirdPersonView");
			else
				sds = TheSettingManager->GetSettingsDepthOfField("FirstPersonView");

			if (sds->Mode == 1) {
				if (MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) || MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) sds->Enabled = false;
			}
			else if (sds->Mode == 2) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog)) sds->Enabled = false;
			}
			else if (sds->Mode == 3) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) sds->Enabled = false;
			}
			else if (sds->Mode == 4) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) && !MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) sds->Enabled = false;
			}
			if (ShaderConst.DepthOfField.Enabled = sds->Enabled) {
				ShaderConst.DepthOfField.Blur.x = sds->DistantBlur;
				ShaderConst.DepthOfField.Blur.y = sds->DistantBlurStartRange;
				ShaderConst.DepthOfField.Blur.z = sds->DistantBlurEndRange;
				ShaderConst.DepthOfField.Blur.w = sds->BaseBlurRadius;
				ShaderConst.DepthOfField.Data.x = sds->BlurFallOff;
				ShaderConst.DepthOfField.Data.y = sds->Radius;
				ShaderConst.DepthOfField.Data.z = sds->DiameterRange;
				ShaderConst.DepthOfField.Data.w = sds->NearBlurCutOff;
			}
		}

		if (TheSettingManager->SettingsMain.Effects.Cinema) {
			UInt8 Mode = TheSettingManager->SettingsCinema.Mode;

			ShaderConst.Cinema.Data.x = TheSettingManager->SettingsCinema.AspectRatio;
			ShaderConst.Cinema.Data.y = TheSettingManager->SettingsCinema.VignetteRadius;
			if (Mode == 1) {
				if (MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) || MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) Mode = -1;
			}
			else if (Mode == 2) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog)) Mode = -1;
			}
			else if (Mode == 3) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) Mode = -1;
			}
			else if (Mode == 4) {
				if (!MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) && !MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) Mode = -1;
			}
			if (Mode == -1) {
				ShaderConst.Cinema.Data.x = 0.0f;
				ShaderConst.Cinema.Data.y = 0.0f;
			}
			ShaderConst.Cinema.Data.z = TheSettingManager->SettingsCinema.VignetteDarkness;
		}

		if (TheSettingManager->SettingsMain.Effects.MotionBlur) {
			SettingsMotionBlurStruct* sms = NULL;

			if (IsThirdPersonView)
				sms = TheSettingManager->GetSettingsMotionBlur("ThirdPersonView");
			else
				sms = TheSettingManager->GetSettingsMotionBlur("FirstPersonView");

			float AngleZ = D3DXToDegree(Player->rot.z);
			float AngleX = D3DXToDegree(Player->rot.x);
			float fMotionBlurAmtX = ShaderConst.MotionBlur.oldAngleZ - AngleZ;
			float fMotionBlurAmtY = ShaderConst.MotionBlur.oldAngleX - AngleX;
			float fBlurDistScratchpad = fMotionBlurAmtX + 360.0f;
			float fBlurDistScratchpad2 = (AngleZ - ShaderConst.MotionBlur.oldAngleZ + 360.0f) * -1.0f;

			if (abs(fMotionBlurAmtX) > abs(fBlurDistScratchpad))
				fMotionBlurAmtX = fBlurDistScratchpad;
			else if (abs(fMotionBlurAmtX) > abs(fBlurDistScratchpad2))
				fMotionBlurAmtX = fBlurDistScratchpad2;

			if (pow(fMotionBlurAmtX, 2) + pow(fMotionBlurAmtY, 2) < sms->BlurCutOff) {
				fMotionBlurAmtX = 0.0f;
				fMotionBlurAmtY = 0.0f;
			}

			if (sms->Enabled) {
				ShaderConst.MotionBlur.Data.x = (ShaderConst.MotionBlur.oldoldAmountX + ShaderConst.MotionBlur.oldAmountX + fMotionBlurAmtX) / 3.0f;
				ShaderConst.MotionBlur.Data.y = (ShaderConst.MotionBlur.oldoldAmountY + ShaderConst.MotionBlur.oldAmountY + fMotionBlurAmtY) / 3.0f;
			}
			else {
				ShaderConst.MotionBlur.Data.x = 0.0f;
				ShaderConst.MotionBlur.Data.y = 0.0f;
			}
			ShaderConst.MotionBlur.oldAngleZ = AngleZ;
			ShaderConst.MotionBlur.oldAngleX = AngleX;
			ShaderConst.MotionBlur.oldoldAmountX = ShaderConst.MotionBlur.oldAmountX;
			ShaderConst.MotionBlur.oldoldAmountY = ShaderConst.MotionBlur.oldAmountY;
			ShaderConst.MotionBlur.oldAmountX = fMotionBlurAmtX;
			ShaderConst.MotionBlur.oldAmountY = fMotionBlurAmtY;
			ShaderConst.MotionBlur.BlurParams.x = sms->GaussianWeight;
			ShaderConst.MotionBlur.BlurParams.y = sms->BlurScale;
			ShaderConst.MotionBlur.BlurParams.z = sms->BlurOffsetMax;
		}

		if (TheSettingManager->SettingsMain.Effects.Sharpening) {
			ShaderConst.Sharpening.Data.x = TheSettingManager->SettingsSharpening.Strength;
			ShaderConst.Sharpening.Data.y = TheSettingManager->SettingsSharpening.Clamp;
			ShaderConst.Sharpening.Data.z = TheSettingManager->SettingsSharpening.Offset;
		}

		if (TheSettingManager->SettingsMain.Effects.VolumetricFog) {
			ShaderConst.VolumetricFog.Data.x = TheSettingManager->SettingsVolumetricFog.Exponent;
			ShaderConst.VolumetricFog.Data.y = TheSettingManager->SettingsVolumetricFog.ColorCoeff;
			ShaderConst.VolumetricFog.Data.z = TheSettingManager->SettingsVolumetricFog.Amount;
			ShaderConst.VolumetricFog.Data.w = 1.0f;
			if (weatherPercent == 1.0f && ShaderConst.fogData.y > TheSettingManager->SettingsVolumetricFog.MaxDistance) ShaderConst.VolumetricFog.Data.w = 0.0f;
		}

		if (TheSettingManager->SettingsMain.FrameRate.Enabled) TheFrameRateManager->Set();
	}
}

void ShaderManager::BeginScene() {

	RenderedBufferFilled = false;
	DepthBufferFilled = false;

}

void ShaderManager::CreateShader(const char* Name) {

#if defined(NEWVEGAS)
	BSShader* (__cdecl * GetShaderDefinition)(UInt32) = (BSShader* (__cdecl *)(UInt32))0x00B55560;
	if (!strcmp(Name, "Water")) {
		WaterShader* WS = (WaterShader*)GetShaderDefinition(17);
		for each (NiD3DVertexShader* VS in WS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in WS->Pixel) LoadShader(PS);
		LoadShader(WaterHeightMapVertexShader);
		LoadShader(WaterHeightMapPixelShader);
	}
#elif defined(OBLIVION)
	NiD3DVertexShaderEx** PrecipitationVertexShaders = (NiD3DVertexShaderEx**)0x00B466E0;
	NiD3DPixelShaderEx** PrecipitationPixelShaders = (NiD3DPixelShaderEx**)0x00B46708;
	NiD3DVertexShaderEx** ShadowLightVertexShaders = (NiD3DVertexShaderEx**)0x00B45364;
	NiD3DPixelShaderEx** ShadowLightPixelShaders = (NiD3DPixelShaderEx**)0x00B45144;

	if (!strcmp(Name, "Water")) {
		WaterShader* WS = (WaterShader*)GetShaderDefinition(17)->Shader;
		for each (NiD3DVertexShader* VS in WS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in WS->Pixel) LoadShader(PS);
		WaterShaderHeightMap* WSHM = (WaterShaderHeightMap*)GetShaderDefinition(19)->Shader;
		LoadShader(WSHM->Vertex);
		for each (NiD3DPixelShader* PS in WSHM->Pixel) LoadShader(PS);
		WaterShaderDisplacement* WSD = (WaterShaderDisplacement*)GetShaderDefinition(20)->Shader;
		for each (NiD3DVertexShader* VS in WSD->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in WSD->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "Grass")) {
		TallGrassShader* TGS = (TallGrassShader*)GetShaderDefinition(2)->Shader;
		for each (NiD3DVertexShader* VS in TGS->Vertex2) LoadShader(VS);
		for each (NiD3DPixelShader* PS in TGS->Pixel2) LoadShader(PS);
	}
	else if (!strcmp(Name, "Precipitations")) {
		for (int i = 0; i < 4; i++) LoadShader(PrecipitationVertexShaders[i]);
		for (int i = 0; i < 2; i++) LoadShader(PrecipitationPixelShaders[i]);
	}
	else if (!strcmp(Name, "HDR")) {
		HDRShader* HS = (HDRShader*)GetShaderDefinition(8)->Shader;
		for each (NiD3DVertexShader* VS in HS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in HS->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "POM")) {
		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DVertexShader* VS in PRS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in PRS->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "Skin")) {
		SkinShader* SS = (SkinShader*)GetShaderDefinition(14)->Shader;
		for each (NiD3DVertexShader* VS in SS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in SS->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "Terrain")) {
		for (int i = 0; i < 76; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && strstr(TerrainShaders, VS->ShaderName)) LoadShader(VS);
		}
		for (int i = 0; i < 82; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(TerrainShaders, PS->ShaderName)) LoadShader(PS);
		}
	}
	else if (!strcmp(Name, "Blood")) {
		GeometryDecalShader* GDS = (GeometryDecalShader*)GetShaderDefinition(16)->Shader;
		for each (NiD3DVertexShader* VS in GDS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in GDS->Pixel) LoadShader(PS);
	}
#elif defined(SKYRIM)
	if (!strcmp(Name, "Water")) {
		for each (NiD3DVertexShader* VS in WaterVertexShaders) LoadShader(VS);
		for each (NiD3DPixelShader* PS in WaterPixelShaders) LoadShader(PS);
	}
#endif

}

void ShaderManager::LoadShader(NiD3DVertexShader* Shader) {
	
	ShaderRecord* ShaderProg = new ShaderRecord();
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)Shader;

	if (ShaderProg->LoadShader(VertexShader->ShaderName)) {
		VertexShader->ShaderProg = ShaderProg;
		VertexShader->ShaderHandleBackup = VertexShader->ShaderHandle;
		TheRenderManager->device->CreateVertexShader((const DWORD*)ShaderProg->Function, &VertexShader->ShaderHandle);
	}
	else {
		delete ShaderProg;
	}

}

void ShaderManager::LoadShader(NiD3DPixelShader* Shader) {

	ShaderRecord* ShaderProg = new ShaderRecord();
	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)Shader;

	if (ShaderProg->LoadShader(PixelShader->ShaderName)) {
		PixelShader->ShaderProg = ShaderProg;
		PixelShader->ShaderHandleBackup = PixelShader->ShaderHandle;
		TheRenderManager->device->CreatePixelShader((const DWORD*)ShaderProg->Function, &PixelShader->ShaderHandle);
	}
	else {
		delete ShaderProg;
	}

}

void ShaderManager::DisposeShader(const char* Name) {

#if defined(NEWVEGAS)
	BSShader* (__cdecl * GetShader)(UInt32) = (BSShader* (__cdecl *)(UInt32))0x00B55560;
	if (!strcmp(Name, "Water")) {
		WaterShader* WS = (WaterShader*)GetShader(17);
		for each (NiD3DVertexShaderEx* VS in WS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in WS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
		NiD3DVertexShaderEx* VS = (NiD3DVertexShaderEx*)WaterHeightMapVertexShader;
		if (VS->ShaderProg) {
			VS->ShaderHandle = VS->ShaderHandleBackup;
			delete VS->ShaderProg; VS->ShaderProg = NULL;
		}
		NiD3DPixelShaderEx* PS = (NiD3DPixelShaderEx*)WaterHeightMapPixelShader;
		if (PS->ShaderProg) {
			PS->ShaderHandle = PS->ShaderHandleBackup;
			delete PS->ShaderProg; PS->ShaderProg = NULL;
		}
	}
#elif defined(OBLIVION)
	ShaderDefinition* (__cdecl * GetShaderDefinition)(UInt32) = (ShaderDefinition* (__cdecl *)(UInt32))0x007B4290;
	NiD3DVertexShaderEx** ShadowLightVertexShaders = (NiD3DVertexShaderEx**)0x00B45364;
	NiD3DPixelShaderEx** ShadowLightPixelShaders = (NiD3DPixelShaderEx**)0x00B45144;

	if (!strcmp(Name, "Water")) {
		WaterShader* WS = (WaterShader*)GetShaderDefinition(17)->Shader;
		for each (NiD3DVertexShaderEx* VS in WS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in WS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
		WaterShaderHeightMap* WSHM = (WaterShaderHeightMap*)GetShaderDefinition(19)->Shader;
		NiD3DVertexShaderEx* VS = (NiD3DVertexShaderEx*)WSHM->Vertex;
		if (VS->ShaderProg) {
			VS->ShaderHandle = VS->ShaderHandleBackup;
			delete VS->ShaderProg; VS->ShaderProg = NULL;
		}
		for each (NiD3DPixelShaderEx* PS in WSHM->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
		WaterShaderDisplacement* WSD = (WaterShaderDisplacement*)GetShaderDefinition(20)->Shader;
		for each (NiD3DVertexShaderEx* VS in WSD->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in WSD->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "Grass")) {
		TallGrassShader* TGS = (TallGrassShader*)GetShaderDefinition(2)->Shader;
		for each (NiD3DVertexShaderEx* VS in TGS->Vertex2) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in TGS->Pixel2) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "HDR")) {
		HDRShader* HS = (HDRShader*)GetShaderDefinition(8)->Shader;
		for each (NiD3DVertexShaderEx* VS in HS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in HS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "POM")) {
		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DVertexShaderEx* VS in PRS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in PRS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "Skin")) {
		SkinShader* SS = (SkinShader*)GetShaderDefinition(14)->Shader;
		for each (NiD3DVertexShaderEx* VS in SS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in SS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "Terrain")) {
		for (int i = 0; i < 76; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && VS->ShaderProg && strstr(TerrainShaders, VS->ShaderName)) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for (int i = 0; i < 82; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && PS->ShaderProg && strstr(TerrainShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "Blood")) {
		GeometryDecalShader* GDS = (GeometryDecalShader*)GetShaderDefinition(16)->Shader;
		for each (NiD3DVertexShaderEx* VS in GDS->Vertex) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in GDS->Pixel) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
#elif defined(SKYRIM)
	if (!strcmp(Name, "Water")) {
		for each (NiD3DVertexShaderEx* VS in WaterVertexShaders) {
			if (VS->ShaderProg) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for each (NiD3DPixelShaderEx* PS in WaterPixelShaders) {
			if (PS->ShaderProg) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
#endif

}

void ShaderManager::CreateEffect(EffectRecordType EffectType) {

	char Filename[MAX_PATH];

	strcpy(Filename, EffectsPath);
	switch (EffectType) {
		case EffectRecordType_Underwater:
			strcat(Filename, "Water\\Underwater.fx");
			UnderwaterEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Underwater = LoadEffect(UnderwaterEffect, Filename, NULL);
			break;
		case EffectRecordType_WaterLens:
			strcat(Filename, "Water\\WaterLens.fx");
			WaterLensEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.WaterLens = LoadEffect(WaterLensEffect, Filename, NULL);
			break;
		case EffectRecordType_GodRays:
			strcat(Filename, "GodRays\\GodRays.fx");
			GodRaysEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.GodRays = LoadEffect(GodRaysEffect, Filename, NULL);
			break;
		case EffectRecordType_DepthOfField:
			strcat(Filename, "DepthOfField\\DepthOfField.fx");
			DepthOfFieldEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.DepthOfField = LoadEffect(DepthOfFieldEffect, Filename, NULL);
			break;
		case EffectRecordType_AmbientOcclusion:
			strcat(Filename, "AmbientOcclusion\\AmbientOcclusion.fx");
			AmbientOcclusionEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.AmbientOcclusion = LoadEffect(AmbientOcclusionEffect, Filename, NULL);
			break;
		case EffectRecordType_Coloring:
			strcat(Filename, "Coloring\\Coloring.fx");
			ColoringEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Coloring = LoadEffect(ColoringEffect, Filename, NULL);
			break;
		case EffectRecordType_Cinema:
			strcat(Filename, "Cinema\\Cinema.fx");
			CinemaEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Cinema = LoadEffect(CinemaEffect, Filename, NULL);
			break;
		case EffectRecordType_Bloom:
			strcat(Filename, "Bloom\\Bloom.fx");
			BloomEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Bloom = LoadEffect(BloomEffect, Filename, NULL);
			break;
		case EffectRecordType_SnowAccumulation:
			strcat(Filename, "Precipitations\\SnowAccumulation.fx");
			SnowAccumulationEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.SnowAccumulation = LoadEffect(SnowAccumulationEffect, Filename, NULL);
			break;
		case EffectRecordType_BloodLens:
			strcat(Filename, "Blood\\BloodLens.fx");
			BloodLensEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.BloodLens = LoadEffect(BloodLensEffect, Filename, NULL);
			break;
		case EffectRecordType_SMAA:
			strcat(Filename, "SMAA\\SMAA.fx");
			SMAAEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.SMAA = LoadEffect(SMAAEffect, Filename, NULL);
			break;
		case EffectRecordType_MotionBlur:
			strcat(Filename, "MotionBlur\\MotionBlur.fx");
			MotionBlurEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.MotionBlur = LoadEffect(MotionBlurEffect, Filename, NULL);
			break;
		case EffectRecordType_LowHF:
			strcat(Filename, "LowHF\\LowHF.fx");
			LowHFEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.LowHF = LoadEffect(LowHFEffect, Filename, NULL);
			break;
		case EffectRecordType_WetWorld:
			strcat(Filename, "Precipitations\\WetWorld.fx");
			WetWorldEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.WetWorld = LoadEffect(WetWorldEffect, Filename, NULL);
			break;
		case EffectRecordType_Sharpening:
			strcat(Filename, "Sharpening\\Sharpening.fx");
			SharpeningEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Sharpening = LoadEffect(SharpeningEffect, Filename, NULL);
			break;
		case EffectRecordType_VolumetricFog:
			strcat(Filename, "Fog\\VolumetricFog.fx");
			VolumetricFogEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.VolumetricFog = LoadEffect(VolumetricFogEffect, Filename, NULL);
			break;
		case EffectRecordType_Precipitations:
			strcat(Filename, "Precipitations\\Rain.fx");
			RainEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.Precipitations = LoadEffect(RainEffect, Filename, NULL);
			if (TheSettingManager->SettingsMain.Effects.Precipitations) {
				strcpy(Filename, EffectsPath);
				strcat(Filename, "Precipitations\\Snow.fx");
				SnowEffect = new EffectRecord();
				TheSettingManager->SettingsMain.Effects.Precipitations = LoadEffect(SnowEffect, Filename, NULL);
			}
			break;
		case EffectRecordType_ShadowsExteriors:
			strcat(Filename, "Shadows\\ShadowsExteriors.fx");
			ShadowsExteriorsEffect = new EffectRecord();
			TheSettingManager->SettingsShadows.Exteriors.Quality = ((int)LoadEffect(ShadowsExteriorsEffect, Filename, NULL)) * -1;
			break;
		case EffectRecordType_ShadowsInteriors:
			strcat(Filename, "Shadows\\ShadowsInteriors.fx");
			ShadowsInteriorsEffect = new EffectRecord();
			TheSettingManager->SettingsShadows.Interiors.Quality = ((int)LoadEffect(ShadowsInteriorsEffect, Filename, NULL)) * -1;
			break;
		case EffectRecordType_Extra:
			WIN32_FIND_DATAA File;
			HANDLE H;
			char* cFileName = NULL;
			EffectRecord* ExtraEffect = NULL;

			if (TheSettingManager->SettingsMain.Develop.CompileEffects)
				strcat(Filename, "ExtraEffects\\*.hlsl");
			else
				strcat(Filename, "ExtraEffects\\*.fx");
			H = FindFirstFileA((LPCSTR)Filename, &File);
			if (H != INVALID_HANDLE_VALUE) {
				cFileName = (char*)File.cFileName;
				if (TheSettingManager->SettingsMain.Develop.CompileEffects) File.cFileName[strlen(cFileName) - 5] = NULL;
				strcpy(Filename, EffectsPath);
				strcat(Filename, "ExtraEffects\\");
				strcat(Filename, cFileName);
				ExtraEffect = new EffectRecord();
				LoadEffect(ExtraEffect, Filename, cFileName);
				while (FindNextFileA(H, &File)) {
					cFileName = (char*)File.cFileName;
					if (TheSettingManager->SettingsMain.Develop.CompileEffects) File.cFileName[strlen(cFileName) - 5] = NULL;
					strcpy(Filename, EffectsPath);
					strcat(Filename, "ExtraEffects\\");
					strcat(Filename, cFileName);
					ExtraEffect = new EffectRecord();
					LoadEffect(ExtraEffect, Filename, cFileName);
				}
				FindClose(H);
			}
			break;
	}

}

bool ShaderManager::LoadEffect(EffectRecord* TheEffect, char* Filename, char* CustomEffectName) {

	bool IsLoaded = TheEffect->LoadEffect(Filename);

	if (IsLoaded) {
		if (CustomEffectName) ExtraEffects[std::string(CustomEffectName).substr(0, strlen(CustomEffectName) - 3)] = TheEffect;
	}
	else
		DisposeEffect(TheEffect);
	return IsLoaded;

}

void ShaderManager::DisposeEffect(EffectRecord* TheEffect) {

	if (TheEffect == AmbientOcclusionEffect) AmbientOcclusionEffect = NULL;
	else if (TheEffect == BloodLensEffect) BloodLensEffect = NULL;
	else if (TheEffect == BloomEffect) BloomEffect = NULL;
	else if (TheEffect == CinemaEffect) CinemaEffect = NULL;
	else if (TheEffect == ColoringEffect) ColoringEffect = NULL;
	else if (TheEffect == DepthOfFieldEffect) DepthOfFieldEffect = NULL;
	else if (TheEffect == GodRaysEffect) GodRaysEffect = NULL;
	else if (TheEffect == LowHFEffect) LowHFEffect = NULL;
	else if (TheEffect == MotionBlurEffect) MotionBlurEffect = NULL;
	else if (TheEffect == SMAAEffect) SMAAEffect = NULL;
	else if (TheEffect == SnowAccumulationEffect) SnowAccumulationEffect = NULL;
	else if (TheEffect == UnderwaterEffect) UnderwaterEffect = NULL;
	else if (TheEffect == WaterLensEffect) WaterLensEffect = NULL;
	else if (TheEffect == WetWorldEffect) WetWorldEffect = NULL;
	else if (TheEffect == SharpeningEffect) SharpeningEffect = NULL;
	else if (TheEffect == VolumetricFogEffect) VolumetricFogEffect = NULL;
	else if (TheEffect == RainEffect) RainEffect = NULL;
	else if (TheEffect == SnowEffect) SnowEffect = NULL;
	else if (TheEffect == ShadowsExteriorsEffect) ShadowsExteriorsEffect = NULL;
	else if (TheEffect == ShadowsInteriorsEffect) ShadowsInteriorsEffect = NULL;

	if (TheEffect) delete TheEffect;

}

void ShaderManager::RenderEffects(IDirect3DSurface9* RenderTarget) {
	
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;
	IDirect3DDevice9* Device = TheRenderManager->device;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();

	TheRenderManager->SetupSceneCamera();
	Device->SetStreamSource(0, EffectVertex, 0, sizeof(EffectQuad));
	Device->SetFVF(EFFECTQUADFORMAT);
	Device->StretchRect(RenderTarget, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	if (Effects->WetWorld && currentWorldSpace && ShaderConst.WetWorld.Data.x > 0.0f) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		WetWorldEffect->SetCT();
		WetWorldEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	else if (Effects->SnowAccumulation && currentWorldSpace && ShaderConst.SnowAccumulation.Params.w > 0.0f) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		SnowAccumulationEffect->SetCT();
		SnowAccumulationEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (TheSettingManager->SettingsShadows.Exteriors.Quality == -1 && currentWorldSpace) {
		ShadowsExteriorsEffect->SetCT();
		ShadowsExteriorsEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (TheSettingManager->SettingsShadows.Interiors.Quality == -1 && !currentWorldSpace) {
		ShadowsInteriorsEffect->SetCT();
		ShadowsInteriorsEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Coloring) {
		ColoringEffect->SetCT();
		ColoringEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Bloom) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		BloomEffect->SetCT();
		BloomEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Underwater && ShaderConst.HasWater && TheRenderManager->CameraPosition.z < ShaderConst.Water.waterSettings.x + 20.0f) {
		if (TheRenderManager->CameraPosition.z < ShaderConst.Water.waterSettings.x) {
			ShaderConst.BloodLens.Percent = 0.0f;
			if (ShaderConst.WaterLens.Percent > -2.0f) ShaderConst.WaterLens.Percent = ShaderConst.WaterLens.Percent - 1.0f;
		}
		UnderwaterEffect->SetCT();
		UnderwaterEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	else {
		if (ShaderConst.WaterLens.Percent <= -2.0f)
			ShaderConst.WaterLens.Percent = 1.0f;
		else if (ShaderConst.WaterLens.Percent <= -1.0f)
			ShaderConst.WaterLens.Percent = 0.0f;

		if (Effects->Precipitations && currentWorldSpace) {
			if (ShaderConst.Precipitations.RainData.x > 0.0f) {
				RainEffect->SetCT();
				RainEffect->Render(Device, RenderTarget, RenderedSurface, false);
			}
			if (ShaderConst.Precipitations.SnowData.x > 0.0f) {
				SnowEffect->SetCT();
				SnowEffect->Render(Device, RenderTarget, RenderedSurface, false);
			}
		}
		if (Effects->AmbientOcclusion && ShaderConst.AmbientOcclusion.Enabled) {
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			AmbientOcclusionEffect->SetCT();
			AmbientOcclusionEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
		if (Effects->GodRays && currentWorldSpace) {
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			GodRaysEffect->SetCT();
			GodRaysEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
		if (Effects->VolumetricFog && currentWorldSpace && ShaderConst.VolumetricFog.Data.w) {
			VolumetricFogEffect->SetCT();
			VolumetricFogEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
	}
	if (Effects->DepthOfField && ShaderConst.DepthOfField.Enabled) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		DepthOfFieldEffect->SetCT();
		DepthOfFieldEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->BloodLens && ShaderConst.BloodLens.Percent > 0.0f) {
		BloodLensEffect->SetCT();
		BloodLensEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->WaterLens && ShaderConst.WaterLens.Percent > 0.0f) {
		WaterLensEffect->SetCT();
		WaterLensEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->MotionBlur && (ShaderConst.MotionBlur.Data.x || ShaderConst.MotionBlur.Data.y)) {
		MotionBlurEffect->SetCT();
		MotionBlurEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->LowHF && ShaderConst.LowHF.Data.x) {
		LowHFEffect->SetCT();
		LowHFEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Sharpening) {
		SharpeningEffect->SetCT();
		SharpeningEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Extra) {
		for (ExtraEffectsList::iterator iter = ExtraEffects.begin(); iter != ExtraEffects.end(); ++iter) {
			if (iter->second->Enabled) {
				Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
				iter->second->SetCT();
				iter->second->Render(Device, RenderTarget, RenderedSurface, false);
			}
		}
	}
	if (Effects->Cinema && (ShaderConst.Cinema.Data.x != 0.0f || ShaderConst.Cinema.Data.y != 0.0f)) {
		CinemaEffect->SetCT();
		CinemaEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->SMAA) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		Device->SetRenderTarget(0, RenderSurfaceSMAA);
		SMAAEffect->SetCT();
		SMAAEffect->Render(Device, RenderSurfaceSMAA, RenderedSurface, true);
		Device->StretchRect(RenderSurfaceSMAA, NULL, RenderTarget, NULL, D3DTEXF_NONE);
		Device->SetRenderTarget(0, RenderTarget);
	}
	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Main.ScreenshotKey)) {
		char Filename[MAX_PATH];
		char Name[80];
		time_t CurrentTime = time(NULL);

		strcpy(Filename, TheSettingManager->SettingsMain.Main.ScreenshotPath);
		strcat(Filename, ScreenshotFilenamePrefix);
		strftime(Name, 80, "%Y%m%d %H.%M.%S", localtime(&CurrentTime));
		strcat(Filename, Name);
		if (TheSettingManager->SettingsMain.Main.ScreenshotType == 0)
			strcat(Filename, ".bmp");
		else
			strcat(Filename, ".jpg");
		if (GetFileAttributesA(TheSettingManager->SettingsMain.Main.ScreenshotPath) == INVALID_FILE_ATTRIBUTES) CreateDirectoryA(TheSettingManager->SettingsMain.Main.ScreenshotPath, NULL);
		D3DXSaveSurfaceToFileA(Filename, (D3DXIMAGE_FILEFORMAT)TheSettingManager->SettingsMain.Main.ScreenshotType, RenderTarget, NULL, NULL);
		MenuManager->ShowMessage("Screenshot taken!");
	}

}

void ShaderManager::SwitchShaderStatus(const char* Name) {
	
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;
	SettingsMainStruct::ShadersStruct* Shaders = &TheSettingManager->SettingsMain.Shaders;

	bool Value = false;

	if (!strcmp(Name, "AmbientOcclusion")) {
		Value = !Effects->AmbientOcclusion;
		Effects->AmbientOcclusion = Value;
		DisposeEffect(AmbientOcclusionEffect);
		if (Value) CreateEffect(EffectRecordType_AmbientOcclusion);
	}
	else if (!strcmp(Name, "Blood")) {
		Value = !Shaders->Blood;
		Shaders->Blood = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "BloodLens")) {
		Value = !Effects->BloodLens;
		Effects->BloodLens = Value;
		DisposeEffect(BloodLensEffect);
		if (Value) CreateEffect(EffectRecordType_BloodLens);
	}
	else if (!strcmp(Name, "Bloom")) {
		Value = !Effects->Bloom;
		Effects->Bloom = Value;
		DisposeEffect(BloomEffect);
		if (Value) CreateEffect(EffectRecordType_Bloom);
	}
	else if (!strcmp(Name, "Cinema")) {
		Value = !Effects->Cinema;
		Effects->Cinema = Value;
		DisposeEffect(CinemaEffect);
		if (Value) CreateEffect(EffectRecordType_Cinema);
	}
	else if (!strcmp(Name, "Coloring")) {
		Value = !Effects->Coloring;
		Effects->Coloring = Value;
		DisposeEffect(ColoringEffect);
		if (Value) CreateEffect(EffectRecordType_Coloring);
	}
	else if (!strcmp(Name, "DepthOfField")) {
		Value = !Effects->DepthOfField;
		Effects->DepthOfField = Value;
		DisposeEffect(DepthOfFieldEffect);
		if (Value) CreateEffect(EffectRecordType_DepthOfField);
	}
	else if (!strcmp(Name, "Grass")) {
		Value = !Shaders->Grass;
		Shaders->Grass = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "GodRays")) {
		Value = !Effects->GodRays;
		Effects->GodRays = Value;
		DisposeEffect(GodRaysEffect);
		if (Value) CreateEffect(EffectRecordType_GodRays);
	}
	else if (!strcmp(Name, "HDR")) {
		Value = !Shaders->HDR;
		Shaders->HDR = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "LowHF")) {
		Value = !Effects->LowHF;
		Effects->LowHF = Value;
		DisposeEffect(LowHFEffect);
		if (Value) CreateEffect(EffectRecordType_LowHF);
	}
	else if (!strcmp(Name, "MotionBlur")) {
		Value = !Effects->MotionBlur;
		Effects->MotionBlur = Value;
		DisposeEffect(MotionBlurEffect);
		if (Value) CreateEffect(EffectRecordType_MotionBlur);
	}
	else if (!strcmp(Name, "POM")) {
		Value = !Shaders->POM;
		Shaders->POM = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "Precipitations")) {
		Value = !Effects->Precipitations;
		Effects->Precipitations = Value;
		DisposeEffect(RainEffect);
		DisposeEffect(SnowEffect);
		if (Value) CreateEffect(EffectRecordType_Precipitations);
	}
	else if (!strcmp(Name, "Skin")) {
		Value = !Shaders->Skin;
		Shaders->Skin = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "SMAA")) {
		Value = !Effects->SMAA;
		Effects->SMAA = Value;
		DisposeEffect(SMAAEffect);
		if (Value) CreateEffect(EffectRecordType_SMAA);
	}
	else if (!strcmp(Name, "SnowAccumulation")) {
		Value = !Effects->SnowAccumulation;
		Effects->SnowAccumulation = Value;
		DisposeEffect(SnowAccumulationEffect);
		if (Value) CreateEffect(EffectRecordType_SnowAccumulation);
	}
	else if (!strcmp(Name, "Terrain")) {
		Value = !Shaders->Terrain;
		Shaders->Terrain = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "Underwater")) {
		Value = !Effects->Underwater;
		Effects->Underwater = Value;
		DisposeEffect(UnderwaterEffect);
		if (Value) CreateEffect(EffectRecordType_Underwater);
	}
	else if (!strcmp(Name, "Water")) {
		Value = !Shaders->Water;
		Shaders->Water = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "WaterLens")) {
		Value = !Effects->WaterLens;
		Effects->WaterLens = Value;
		DisposeEffect(WaterLensEffect);
		if (Value) CreateEffect(EffectRecordType_WaterLens);
	}
	else if (!strcmp(Name, "WetWorld")) {
		Value = !Effects->WetWorld;
		Effects->WetWorld = Value;
		DisposeEffect(WetWorldEffect);
		if (Value) CreateEffect(EffectRecordType_WetWorld);
	}
	else if (!strcmp(Name, "Sharpening")) {
		Value = !Effects->Sharpening;
		Effects->Sharpening = Value;
		DisposeEffect(SharpeningEffect);
		if (Value) CreateEffect(EffectRecordType_Sharpening);
	}
	else if (!strcmp(Name, "VolumetricFog")) {
		Value = !Effects->VolumetricFog;
		Effects->VolumetricFog = Value;
		DisposeEffect(VolumetricFogEffect);
		if (Value) CreateEffect(EffectRecordType_VolumetricFog);
	}
	else if (!strcmp(Name, "ShadowsExteriors")) {
		DisposeEffect(ShadowsExteriorsEffect);
		if (TheSettingManager->SettingsShadows.Exteriors.Quality == -1) CreateEffect(EffectRecordType_ShadowsExteriors);
	}
	else if (!strcmp(Name, "ShadowsInteriors")) {
		DisposeEffect(ShadowsInteriorsEffect);
		if (TheSettingManager->SettingsShadows.Interiors.Quality == -1) CreateEffect(EffectRecordType_ShadowsInteriors);
	}

}

void ShaderManager::SetCustomConstant(const char* Name, D3DXVECTOR4 Value) {
	
	CustomConstants::iterator v = CustomConst.find(std::string(Name));
	if (v != CustomConst.end()) v->second = Value;

}

void ShaderManager::SetExtraEffectEnabled(const char* Name, bool Value) {

	ExtraEffectsList::iterator v = ExtraEffects.find(std::string(Name));
	if (v != ExtraEffects.end()) v->second->Enabled = Value;

}