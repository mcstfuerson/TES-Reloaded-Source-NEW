#include <fstream>
#include <ctime>
#include <math.h>
#include <cmath>
#include <WeatherMode.h>
#include <algorithm>
#include <iostream>
#include <filesystem>
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
#define ExteriorPom "PAR2022.pso"
#define ExteriorExtraShaders "SM3LL003.pso SM3002.vso"
#define InteriorShadowShaders "SLS2022.pso SLS2021.pso SLS2016.vso SLS2015.vso SLS2015.pso SLS2012.vso SLS2011.vso SLS2010.pso SLS2008.vso SLS2007.vso SLS2002.vso SLS2002.pso SLS2000.vso SLS2000.pso SLS1006.vso SLS1005.vso SLS1004.pso SLS1S006.vso SLS1S005.vso SLS1003.pso SLS2009.pso SLS2035.vso SLS2036.vso SLS2041.pso SM3002.vso SM3001.vso SM3001.pso SM3000.vso SM3LL003.pso SM3LL001.pso SM3LL000.pso PAR2022.pso"
#define InteriorSpecularShadowShaders "SLS2021.pso SLS2035.vso SLS2036.vso SLS2041.pso PAR2025.pso PAR2026.pso PAR2034.vso"
#define ExteriorDialogShaders "SLS2003.pso SLS2018.pso SLS2039.pso SKIN2001.pso SKIN2003.pso SKIN2007.pso"
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

bool ShaderProgram::SetConstantTableValue1(LPCSTR Name, UInt32 Index) {

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
	else if (!strcmp(Name, "TESR_ShadowSkinData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Shadow.ShadowSkinData;
	else if (!strcmp(Name, "TESR_ShadowCubeData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowCube.Data;
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
	else if (!strcmp(Name, "TESR_ShadowCameraToLightTransformSkin"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCameraToLight[3];
	else if (!strcmp(Name, "TESR_PointLightPosition"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.PointLights.LightPosition;
	else if (!strcmp(Name, "TESR_PointLightColor"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.PointLights.LightColor;
	else if (!strcmp(Name, "TESR_ShadowCubeMapLightPosition"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapLightPosition;
	else if (!strcmp(Name, "TESR_ShadowLightPosition"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition;
	else if (!strcmp(Name, "TESR_ShadowLightPosition0"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[0];
	else if (!strcmp(Name, "TESR_ShadowLightPosition1"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[1];
	else if (!strcmp(Name, "TESR_ShadowLightPosition2"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[2];
	else if (!strcmp(Name, "TESR_ShadowLightPosition3"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[3];
	else if (!strcmp(Name, "TESR_ShadowLightPosition4"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[4];
	else if (!strcmp(Name, "TESR_ShadowLightPosition5"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[5];
	else if (!strcmp(Name, "TESR_ShadowLightPosition6"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[6];
	else if (!strcmp(Name, "TESR_ShadowLightPosition7"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[7];
	else if (!strcmp(Name, "TESR_ShadowLightPosition8"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[8];
	else if (!strcmp(Name, "TESR_ShadowLightPosition9"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[9];
	else if (!strcmp(Name, "TESR_ShadowLightPosition10"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[10];
	else if (!strcmp(Name, "TESR_ShadowLightPosition11"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCastLightPosition[11];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition;
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition0"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[0];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition1"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[1];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition2"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[2];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition3"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[3];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition4"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[4];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition5"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[5];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition6"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[6];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition7"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[7];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition8"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[8];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition9"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[9];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition10"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[10];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition11"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[11];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition12"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[12];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition13"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[13];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition14"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[14];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition15"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[15];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition16"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[16];
	else if (!strcmp(Name, "TESR_ShadowCullLightPosition17"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->ShaderConst.ShadowMap.ShadowCullLightPosition[17];
	else if (!strcmp(Name, "TESR_ShadowCubeMapFarPlanes"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowCubeMapFarPlanes;
	else if (!strcmp(Name, "TESR_ShadowBiasDeferred"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred;
	else if (!strcmp(Name, "TESR_ShadowBiasForward"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward;
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
	else if (!strcmp(Name, "TESR_ReflectionLightDir"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ReflectionLightDir;
	else if (!strcmp(Name, "TESR_ShadowLightDir"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.ShadowMap.ShadowLightDir;
	else if (!strcmp(Name, "TESR_SunTiming"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunTiming;
	else if (!strcmp(Name, "TESR_SunAmount"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SunAmount;
	else if (!strcmp(Name, "TESR_MasserDirection"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MasserDir;
	else if (!strcmp(Name, "TESR_MasserAmount"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.MasserAmount;
	else if (!strcmp(Name, "TESR_SecundaDirection"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SecundaDir;
	else if (!strcmp(Name, "TESR_SecundaAmount"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.SecundaAmount;
	else if (!strcmp(Name, "TESR_RaysPhaseCoeff"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.RaysPhaseCoeff;
	else if (!strcmp(Name, "TESR_GameTime"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.GameTime;
	else if (!strcmp(Name, "TESR_InteriorDimmer"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.InteriorDimmer;
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
	else if (!strcmp(Name, "TESR_MasserRaysRay"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysMasser.Ray;
	else if (!strcmp(Name, "TESR_MasserRaysRayColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysMasser.RayColor;
	else if (!strcmp(Name, "TESR_MasserRaysData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysMasser.Data;
	else if (!strcmp(Name, "TESR_SecundaRaysRay"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysSecunda.Ray;
	else if (!strcmp(Name, "TESR_SecundaRaysRayColor"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysSecunda.RayColor;
	else if (!strcmp(Name, "TESR_SecundaRaysData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.KhajiitRaysSecunda.Data;
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
		return false;
	}
	return true;
}

bool ShaderProgram::SetConstantTableValue2(LPCSTR Name, UInt32 Index) {

	if (!strcmp(Name, "TESR_VolumetricLightData1"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data1;
	else if (!strcmp(Name, "TESR_VolumetricLightData2"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data2;
	else if (!strcmp(Name, "TESR_VolumetricLightData3"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data3;
	else if (!strcmp(Name, "TESR_VolumetricLightData4"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data4;
	else if (!strcmp(Name, "TESR_VolumetricLightData5"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data5;
	else if (!strcmp(Name, "TESR_VolumetricLightData6"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.VolumetricLight.data6;
	else if (!strcmp(Name, "TESR_SpecularData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Specular.SpecularData;
	else if (!strcmp(Name, "TESR_TAAData"))
		FloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.TAA.Data;
	else if (!strcmp(Name, "TESR_PrevWorldViewProjectionTransform"))
		FloatShaderValues[Index].Value = (D3DXVECTOR4*)&TheShaderManager->PrevWorldViewProjMatrix;
	else {
		return false;
	}
	return true;
}

bool ShaderProgram::SetPerGeomConstantTableValue(LPCSTR Name, UInt32 Index) {

	if (!strcmp(Name, "TESR_GEOM_EyePosition"))
		PerGeomFloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Specular.EyePosition;
	else if (!strcmp(Name, "TESR_GEOM_Toggles"))
		PerGeomFloatShaderValues[Index].Value = &TheShaderManager->ShaderConst.Geometry.Toggles;
	else {
		return false;
	}
	return true;
}

void ShaderProgram::SetConstantTableCustom(LPCSTR Name, UInt32 Index) {
	Logger::Log("Custom constant found: %s", Name);
	D3DXVECTOR4 v; v.x = v.y = v.z = v.w = 0.0f;
	TheShaderManager->CustomConst[Name] = v;
	FloatShaderValues[Index].Value = &TheShaderManager->CustomConst[Name];
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

bool ShaderRecord::LoadShader(const char* Name, const char* DirPostFix) {
  
	char FileName[MAX_PATH];
	char FileNameBinary[MAX_PATH];
	
	strcpy(FileName, ShadersPath);
	if (!memcmp(Name, "WATER", 5)) {
		if (!TheSettingManager->SettingsMain.Shaders.Water) return false;
		strcat(FileName, "Water");
	}
	else if (!memcmp(Name, "GRASS", 5)) {
		if (!TheSettingManager->SettingsMain.Shaders.Grass) return false;
		strcat(FileName, "Grass");
	}
	else if (!memcmp(Name, "HDR", 3)) {
		if (!TheSettingManager->SettingsMain.Shaders.HDR) return false;
		strcat(FileName, "HDR");
	}
	else if (!memcmp(Name, "PART", 4)) {
		strcat(FileName, "ExtraShaders");
	}
	else if (!memcmp(Name, "PAR", 3)) {
		if (!TheSettingManager->SettingsMain.Shaders.POM) return false;
		strcat(FileName, "POM");
	}
	else if (!memcmp(Name, "SKIN", 4)) {
		if (!TheSettingManager->SettingsMain.Shaders.Skin) return false;

		if (TheSettingManager->SettingsSkin.UseVanillaShaders) {
			strcat(FileName, "SkinVanilla");
		}
		else {
			strcat(FileName, "Skin");
		}
	}
	else if (strstr(TerrainShaders, Name)) {
		if (!TheSettingManager->SettingsMain.Shaders.Terrain) return false;
		strcat(FileName, "Terrain");
	}
	else if (strstr(BloodShaders, Name)) {
		if (!TheSettingManager->SettingsMain.Shaders.Blood) return false;
		strcat(FileName, "Blood");
	}
	else if (!memcmp(Name, "NIGHTEYE", 8)) {
		if (!TheSettingManager->SettingsMain.Shaders.NightEye) return false;
		strcat(FileName, "NightEye");
	}
	else if (!memcmp(Name, "Shadow", 6)) {
		strcat(FileName, "Shadows");
	}	
	else if (!memcmp(Name, "ISHIT", 5)) {
		if (!TheSettingManager->SettingsMain.Main.RenderEffectsBeforeHdr)
		{
			strcat(FileName, "ExtraShaders");
		}
		else {
			strcat(FileName, "PreHdrEffectPatches");
		}
	}
	else {
		strcat(FileName, "ExtraShaders");
	}

	strcat(FileName, DirPostFix);
	strcat(FileName, "\\");
	strcat(FileName, Name);
	strcpy(FileNameBinary, FileName);
	strcat(FileName, ".hlsl");
	std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (FileSource.is_open()) {	
		size_t size = FileSource.tellg();
		Source = new char[size + 1];
		FileSource.seekg(0, std::ios::beg);
		FileSource.read(Source, size);
		Source[size] = 0;
		FileSource.close();
		if (strstr(Name, ".vso"))
			Type = ShaderType_Vertex;
		else if (strstr(Name, ".pso"))
			Type = ShaderType_Pixel;

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
	UInt32 PerGeomFloatIndex = 0;
	UInt32 TextureIndex = 0;
	
	Table->GetDesc(&ConstantTableDesc);
    for (UINT c = 0; c < ConstantTableDesc.Constants; c++) {
		Handle = Table->GetConstant(NULL, c);
		Table->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
		//if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4 && !memcmp(ConstantDesc.Name, "TESR_", 5)) FloatShaderValuesCount += 1;
		if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4 && !memcmp(ConstantDesc.Name, "TESR_GEOM_", 10)) { PerGeomFloatShaderValuesCount += 1; }
		else if(ConstantDesc.RegisterSet == D3DXRS_FLOAT4 && !memcmp(ConstantDesc.Name, "TESR_", 5)) { FloatShaderValuesCount += 1; }
		if (ConstantDesc.RegisterSet == D3DXRS_SAMPLER && !memcmp(ConstantDesc.Name, "TESR_", 5)) TextureShaderValuesCount += 1;
    }
	HasCT = FloatShaderValuesCount + TextureShaderValuesCount + PerGeomFloatShaderValuesCount;
    if (HasCT) {
		FloatShaderValues = (ShaderValue*)malloc(FloatShaderValuesCount * sizeof(ShaderValue));
		PerGeomFloatShaderValues = (ShaderValue*)malloc(PerGeomFloatShaderValuesCount * sizeof(ShaderValue));
		TextureShaderValues = (ShaderValue*)malloc(TextureShaderValuesCount * sizeof(ShaderValue));
		for (UINT c = 0; c < ConstantTableDesc.Constants; c++) {
			Handle = Table->GetConstant(NULL, c);
			Table->GetConstantDesc(Handle, &ConstantDesc, &ConstantCount);
			if (!memcmp(ConstantDesc.Name, "TESR_", 5)) {
				Logger::Log("%s", ConstantDesc.Name);
				switch (ConstantDesc.RegisterSet) {
					case D3DXRS_FLOAT4:
						if (SetPerGeomConstantTableValue(ConstantDesc.Name, PerGeomFloatIndex)) {
							PerGeomFloatShaderValues[PerGeomFloatIndex].RegisterIndex = ConstantDesc.RegisterIndex;
							PerGeomFloatShaderValues[PerGeomFloatIndex].RegisterCount = ConstantDesc.RegisterCount;
							PerGeomFloatIndex++;
						}
						else {
							if (!(SetConstantTableValue1(ConstantDesc.Name, FloatIndex) || SetConstantTableValue2(ConstantDesc.Name, FloatIndex))) {
								SetConstantTableCustom(ConstantDesc.Name, FloatIndex);
							}
							FloatShaderValues[FloatIndex].RegisterIndex = ConstantDesc.RegisterIndex;
							FloatShaderValues[FloatIndex].RegisterCount = ConstantDesc.RegisterCount;
							FloatIndex++;
						}
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

void ShaderRecord::SetPerGeomCT() {

	ShaderValue* Value;

	if (HasCT) {
		for (UInt32 c = 0; c < PerGeomFloatShaderValuesCount; c++) {
			Value = &PerGeomFloatShaderValues[c];
			if (Type == ShaderType_Vertex)
				TheRenderManager->device->SetVertexShaderConstantF(Value->RegisterIndex, (const float*)Value->Value, Value->RegisterCount);
			else
				TheRenderManager->device->SetPixelShaderConstantF(Value->RegisterIndex, (const float*)Value->Value, Value->RegisterCount);
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
	strcpy(FileName, Name);
	strcat(FileName, ".hlsl");
	std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (FileSource.is_open()) {
		size_t size = FileSource.tellg();
		Source = new char[size + 1];
		FileSource.seekg(0, std::ios::beg);
		FileSource.read(Source, size);
		Source[size] = 0;
		FileSource.close();

		D3DXCreateEffectFromFileA(TheRenderManager->device, Name, NULL, NULL, NULL, NULL, &Effect, &Errors);
		if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
		if (Effect) {
			CreateCT();
			Logger::Log("Effect loaded: %s", Name);
			return true;
		}
	}
	return false;
}

void ShaderManager::CompileShader(char* FileName, char* FileNameBinary, char* Source, ShaderType Type, ID3DXBuffer* Errors, ID3DXBuffer* Shader, ID3DXConstantTable* Table) {

	bool useFlowControl = false;
	useFlowControl = strstr(Source, "Includes/ShadowCube") != nullptr;
	if (useFlowControl) {
		Logger::Log("%s will be optimized for flow control", FileName);
	}
	D3DXCompileShaderFromFileA(FileName, NULL, NULL, "main", (Type == ShaderType_Vertex ? "vs_3_0" : "ps_3_0"), (useFlowControl ? D3DXSHADER_PREFER_FLOW_CONTROL : NULL), &Shader, &Errors, &Table);
	if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
	if (Shader) {
		std::ofstream FileBinary(FileNameBinary, std::ios::out | std::ios::binary);
		FileBinary.write((char*)Shader->GetBufferPointer(), Shader->GetBufferSize());
		FileBinary.flush();
		FileBinary.close();
		Logger::Log("Shader compiled: %s", FileNameBinary);
	}
}

void ShaderManager::CompileEffect(char* FileName, char* FileNameBinary, char* Source, ID3DXBuffer* Errors) {

	bool useFlowControl = false;
	std::string s = Source;
	useFlowControl = strstr(Source, "/Gfp") != nullptr;
	if (useFlowControl) {
		Logger::Log("%s will be optimized for flow control", FileName);
	}
	ID3DXEffectCompiler* Compiler = NULL;
	ID3DXBuffer* EffectBuffer = NULL;
	D3DXCreateEffectCompilerFromFileA(FileName, NULL, NULL, NULL, &Compiler, &Errors);
	if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
	if (Compiler) {
		Compiler->CompileEffect((useFlowControl ? D3DXSHADER_PREFER_FLOW_CONTROL : NULL), &EffectBuffer, &Errors);
		if (Errors) Logger::Log((char*)Errors->GetBufferPointer());
	}
	if (EffectBuffer) {
		std::ofstream FileBinary(FileNameBinary, std::ios::out | std::ios::binary);
		FileBinary.write((char*)EffectBuffer->GetBufferPointer(), EffectBuffer->GetBufferSize());
		FileBinary.flush();
		FileBinary.close();
		Logger::Log("Effect compiled: %s", FileNameBinary);
	}
	if (EffectBuffer) EffectBuffer->Release();
	if (Compiler) Compiler->Release();
}

void ShaderManager::CompileShaders(const std::filesystem::path& path){
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			Logger::Log("Compiling Directory: %s", entry.path().string());
			CompileShaders(entry.path()); 
		}
		else if (entry.is_regular_file()) {

			std::string FileNameStr = entry.path().string();
			
			ShaderType Type;
			bool validFile = false;
			bool isEffect = false;
			if (FileNameStr.ends_with(".vso.hlsl")) {
				Type = ShaderType_Vertex;
				validFile = true;
			}
			else if (FileNameStr.ends_with(".pso.hlsl")) {
				Type = ShaderType_Pixel;
				validFile = true;
			}
			else if (FileNameStr.ends_with(".fx.hlsl")) {
				validFile = true;
				isEffect = true;
			}

			if (validFile) {
				char* FileName = FileNameStr.data();
				Logger::Log("Compiling File: %s", entry.path().string());
				std::ifstream FileSource(FileName, std::ios::in | std::ios::binary | std::ios::ate);
				if (FileSource.is_open()) {
					size_t size = FileSource.tellg();
					char* Source = new char[size+1];
					FileSource.seekg(0, std::ios::beg);
					FileSource.read(Source, size);
					Source[size] = 0;
					FileSource.close();
					std::string FileNameBinaryStr = FileNameStr.substr(0, FileNameStr.length() - 5);
					char* FileNameBinary = FileNameBinaryStr.data();
					ID3DXBuffer* Errors = NULL;
					ID3DXBuffer* Shader = NULL;
					ID3DXConstantTable* Table = NULL;
					if (!isEffect) {
						CompileShader(FileName, FileNameBinary, Source, Type, Errors, Shader, Table);
					}
					else {
						CompileEffect(FileName, FileNameBinary, Source, Errors);
					}
				}			
			}			
		}
	}
}

void EffectRecord::CreateCT() {

	D3DXEFFECT_DESC ConstantTableDesc;
	D3DXPARAMETER_DESC ConstantDesc;
	D3DXHANDLE Handle;
	UINT ConstantCount = 1;
	UInt32 FloatIndex = 0;
	UInt32 PerGeomFloatIndex = 0;
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
					if (!(SetConstantTableValue1(ConstantDesc.Name, FloatIndex) || SetConstantTableValue2(ConstantDesc.Name, FloatIndex))) {
							SetConstantTableCustom(ConstantDesc.Name, FloatIndex);
					}
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

	float UAdj, VAdj;
	void* VertexPointer;

	SourceTexture = NULL;
	SourceSurface = NULL;
	RenderedTexture = NULL;
	RenderedSurface = NULL;
	RenderTextureSMAA = NULL;
	RenderSurfaceSMAA = NULL;
	EffectTexture = NULL;
	EffectSurface = NULL;
	RenderedBufferFilled = false;
	DepthBufferFilled = false;
	EffectVertex = NULL;
	UnderwaterEffect = NULL;
	WaterLensEffect = NULL;
	GodRaysEffect = NULL;
	MasserRaysEffect = NULL;
	SecundaRaysEffect = NULL;
	DepthOfFieldEffect = NULL;
	AmbientOcclusionEffect = NULL;
	ColoringEffect = NULL;
	CinemaEffect = NULL;
	BloomEffect = NULL;
	SnowAccumulationEffect = NULL;
	SMAAEffect = NULL;
	TAAEffect = NULL;
	MotionBlurEffect = NULL;
	WetWorldEffect = NULL;
	SharpeningEffect = NULL;
	VolumetricFogEffect = NULL;
	VolumetricLightEffect = NULL;
	RainEffect = NULL;
	SnowEffect = NULL;
	ShadowsExteriorsEffect = NULL;
	ShadowsExteriorsPointEffect = NULL;
	ShadowsExteriorsPointDialogEffect = NULL;
	ShadowsInteriorsEffect = NULL;
	WaterHeightMapVertexShader = NULL;
	WaterHeightMapPixelShader = NULL;
	memset(WaterVertexShaders, NULL, sizeof(WaterVertexShaders));
	memset(WaterPixelShaders, NULL, sizeof(WaterPixelShaders));
	InitializeConstants();
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
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &SourceTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &RenderedTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &RenderTextureSMAA, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &EffectTexture, NULL);
	TheRenderManager->device->CreateTexture(TheRenderManager->width, TheRenderManager->height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &TAATexture, NULL);
	SourceTexture->GetSurfaceLevel(0, &SourceSurface);
	RenderedTexture->GetSurfaceLevel(0, &RenderedSurface);
	RenderTextureSMAA->GetSurfaceLevel(0, &RenderSurfaceSMAA);
	EffectTexture->GetSurfaceLevel(0, &EffectSurface);
	TAATexture->GetSurfaceLevel(0, &TAASurface);
	UseIntervalUpdate = TheSettingManager->SettingsShadows.Exteriors.UseIntervalUpdate;
	if (TheSettingManager->SettingsMain.Develop.CompileShaders) {
		CompileShaders(ShadersPath);
	}
}

void ShaderManager::CreateEffects() {
	
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;

	if (Effects->AmbientOcclusion) CreateEffect(EffectRecordType_AmbientOcclusion);
	if (Effects->Bloom) CreateEffect(EffectRecordType_Bloom);
	if (Effects->Cinema) CreateEffect(EffectRecordType_Cinema);
	if (Effects->Coloring) CreateEffect(EffectRecordType_Coloring);
	if (Effects->DepthOfField) CreateEffect(EffectRecordType_DepthOfField);
	if (Effects->GodRays) CreateEffect(EffectRecordType_GodRays);
	if (Effects->KhajiitRays) CreateEffect(EffectRecordType_KhajiitRays);
	if (Effects->MotionBlur) CreateEffect(EffectRecordType_MotionBlur);
	if (Effects->Sharpening) CreateEffect(EffectRecordType_Sharpening);
	if (Effects->SMAA) CreateEffect(EffectRecordType_SMAA);
	if (Effects->TAA) CreateEffect(EffectRecordType_TAA);
	if (Effects->SnowAccumulation) CreateEffect(EffectRecordType_SnowAccumulation);
	if (Effects->Underwater) CreateEffect(EffectRecordType_Underwater);
	if (Effects->VolumetricFog) CreateEffect(EffectRecordType_VolumetricFog);
	if (Effects->VolumetricLight) CreateEffect(EffectRecordType_VolumetricLight);
	if (Effects->WaterLens) CreateEffect(EffectRecordType_WaterLens);
	if (Effects->WetWorld) CreateEffect(EffectRecordType_WetWorld);
	if (Effects->Precipitations) CreateEffect(EffectRecordType_Precipitations);
	if (Effects->Extra) CreateEffect(EffectRecordType_Extra);
	if (TheSettingManager->SettingsShadows.Exteriors.UsePostProcessing) CreateEffect(EffectRecordType_ShadowsExteriors);
	if (TheSettingManager->SettingsShadows.ExteriorsPoint.UsePostProcessing) { CreateEffect(EffectRecordType_ShadowsExteriorsPoint); CreateEffect(EffectRecordType_ShadowsExteriorsPointDialog); }
	if (TheSettingManager->SettingsShadows.Interiors.UsePostProcessing) CreateEffect(EffectRecordType_ShadowsInteriors);

}

void ShaderManager::InitializeConstants() {

	ShaderConst.pWeather = NULL;
	ShaderConst.WaterLens.Percent = 0.0f;
	ShaderConst.SnowAccumulation.Params.w = 0.0f;
	ShaderConst.WetWorld.Data.x = 0.0f;
	GameDay = 0;
	ShaderConst.EveningTransLightDirSet = false;
	isFullyInitialized = false;
	InitFrameCount = 0;
	InitFrameTarget = 10;
}

void ShaderManager::UpdateShaderStates() {

	if (Player->GetWorldSpace()) {
		if (LocationState != CellLocation::Exterior) {
			LocationState = CellLocation::Exterior;
			DisposeShader("InteriorShadows");
			CreateShader("ExteriorPom"); //no disposal needed since they are disposed as part of "InteriorShadows"
			CreateShader("ExteriorExtraShaders");//no disposal needed since they are disposed as part of "InteriorShadows"
		}

		if (MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) || MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion)) {
			if (!DialogState) {
				DialogState = true;
				DisposeShader("ExteriorDialog");
				CreateShader("ExteriorDialogActive");
			}
		}
		else if (DialogState) {
			DialogState = false;
			DisposeShader("ExteriorDialog");
			CreateShader("ExteriorDialogInactive");
		}

	}
	else {
		if (LocationState != CellLocation::Interior) {
			LocationState = CellLocation::Interior;
			DisposeShader("InteriorShadows");
			CreateShader("InteriorShadows");

			//If interior shadow spec is on
			if (TheSettingManager->SettingsShadows.Interiors.EnableSpecularShadow) {
				DisposeShader("InteriorSpecularShadow");
				CreateShader("InteriorSpecularShadowActive");
			} 
			else {
				DisposeShader("InteriorSpecularShadow");
				CreateShader("InteriorSpecularShadowInactive");
			}
		}

	}

}

void ShaderManager::UpdateConstants() {

	bool IsThirdPersonView;
	Sky* WorldSky = Tes->sky;
	NiNode* SunRoot = WorldSky->sun->RootNode;
	Sun* Sun = WorldSky->sun;
	Moon* Masser = WorldSky->masserMoon;
	Moon* Secunda = WorldSky->secundaMoon;
	TESClimate* currentClimate = WorldSky->firstClimate;
	TESWeather* currentWeather = WorldSky->firstWeather;
	TESWeather* previousWeather = WorldSky->secondWeather;
	TESObjectCELL* currentCell = Player->parentCell;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();
	TESRegion* currentRegion = Player->GetRegion();
	float weatherPercent = WorldSky->weatherPercent;
	float dayPercent = 0;
	float lastGameTime = ShaderConst.GameTime.y;
	TheSettingManager->SettingsShadows.Exteriors.UseIntervalUpdate = UseIntervalUpdate;

	if (currentCell != previousCell) {
		LoadEffectSettings();
		previousCell = currentCell;
	}

	if (CurrentBlend != previousBlend) {
		LoadEffectSettings();
		previousBlend = CurrentBlend;
	}

	IsThirdPersonView = Player->IsThirdPersonView(TheSettingManager->SettingsMain.CameraMode.Enabled, TheRenderManager->FirstPersonView);
	TheRenderManager->GetSceneCameraData();

	//Is fully init'd after two frame passes due to time calculations with sundir
	if (!isFullyInitialized && currentWorldSpace) {
		if (InitFrameCount < InitFrameTarget) {
			InitFrameCount++;
		}
		else {	
			TheShadowManager->ResetIntervals();
			isFullyInitialized = true;
		}
	}

	ShaderConst.GameTime.x = TimeGlobals::GetGameTime();
	ShaderConst.GameTime.y = ShaderConst.GameTime.x / 3600.0f;
	ShaderConst.GameTime.z = ShaderConst.Tick.x = TheFrameRateManager->Time;
	ShaderConst.Tick.y = TheFrameRateManager->GetPerformance();
	
	if (currentCell) {
		ShaderConst.SunTiming.x = currentClimate->sunriseBegin / 6.0f - 1.0f;
		ShaderConst.SunTiming.y = currentClimate->sunriseEnd / 6.0f;
		ShaderConst.SunTiming.z = currentClimate->sunsetBegin / 6.0f;
		ShaderConst.SunTiming.w = currentClimate->sunsetEnd / 6.0f + 1.0f;

		if (lastGameTime != ShaderConst.GameTime.y) {
			float deltaz = ShaderConst.SunDir.z;
			ShaderConst.SunDir.x = SunRoot->m_localTransform.pos.x;
			ShaderConst.SunDir.y = SunRoot->m_localTransform.pos.y;
			ShaderConst.SunDir.z = SunRoot->m_localTransform.pos.z;
			((NiVector4*)&ShaderConst.SunDir)->Normalize();
			if (ShaderConst.GameTime.y > ShaderConst.SunTiming.w || ShaderConst.GameTime.y < ShaderConst.SunTiming.x)
			{
				ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
			}
			else if (ShaderConst.GameTime.y > ShaderConst.SunTiming.z && fabs(deltaz) - ShaderConst.SunDir.z < 0.0f)
			{
				ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
			}
			else if (ShaderConst.GameTime.y < ShaderConst.SunTiming.y && fabs(deltaz) - ShaderConst.SunDir.z > 0.0f) {
				//Will leave this for now, but truly dont know what it does besides making the sun direction incorrect.
				//ShaderConst.SunDir.z = -ShaderConst.SunDir.z;
			}

			//TODO: use kClimate_Masser and kClimate_Secunda but not sure what to compare against?
			if (Masser && Secunda)  {
				//TODO: set x properly
				ShaderConst.MasserDir.x = 0.7f;
				ShaderConst.MasserDir.y = cos(Masser->degree * M_PI / 180.0);
				ShaderConst.MasserDir.z = sin(Masser->degree * M_PI / 180.0);

				ShaderConst.SecundaDir.x = 1.2f;
				ShaderConst.SecundaDir.y = cos(Secunda->degree * M_PI / 180.0);
				ShaderConst.SecundaDir.z = sin(Secunda->degree * M_PI / 180.0);

				((NiVector4*)&ShaderConst.MasserDir)->Normalize();
				((NiVector4*)&ShaderConst.SecundaDir)->Normalize();

				//TODO: make configurable
				Masser->AngleFadeStart = 0.0f;
				Masser->AngleFadeEnd = 0.0f;
				Secunda->AngleFadeStart = 0.0f;
				Secunda->AngleFadeEnd = 0.0f;

				ShaderConst.MasserFade = Masser->fadeValue();
				ShaderConst.SecundaFade = Secunda->fadeValue();
				ShaderConst.MoonsExist = true;
			}
			else {
				ShaderConst.MasserDir.x = 0.0f;
				ShaderConst.MasserDir.y = 0.0f;
				ShaderConst.MasserDir.z = 0.0f;

				ShaderConst.SecundaDir.x = 0.0f;
				ShaderConst.SecundaDir.y = 0.0f;
				ShaderConst.SecundaDir.z = 0.0f;

				ShaderConst.MasserFade = 0.0f;
				ShaderConst.SecundaFade = 0.0f;
				ShaderConst.MoonsExist = false;
			}

		}

		//set moon phase Coeff during day to avoid jump in luminance when phases shift
		if (GameDay == 0 || (GameDay != TimeGlobals::GetGameDaysPassed() && ShaderConst.GameTime.y > 12.00)) {
			int phaseLength = (currentClimate->phaseLength & currentClimate->kClimate_PhaseLengthMask);
			int phase = TimeGlobals::GetGameDaysPassed() % (phaseLength * 8);
			SetPhaseLumCoeff(phaseLength, phase);
			GameDay = TimeGlobals::GetGameDaysPassed();
		}

		if (currentWorldSpace) {
			if (currentWeather) {
				ShaderConst.SunDir.w = 1.0f;
				ShaderConst.MasserDir.w = 1.0f;
				ShaderConst.SecundaDir.w = 1.0f;
				ShaderConst.ShadowMap.ShadowLightDir.w = 1.0f;
				if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.y && ShaderConst.GameTime.y <= ShaderConst.SunTiming.z) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 1.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 0.0f;
					ShaderConst.MasserAmount.x = (ShaderConst.MasserFade - ShaderConst.SunAmount.y);
					ShaderConst.SecundaAmount.x = (ShaderConst.SecundaFade - ShaderConst.SunAmount.y);
					ShaderConst.OverrideVanillaDirectionalLight = false;
					ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
					ShaderConst.DayPhase = Day;
					dayPercent = 1.0f;
					ShaderConst.EveningTransLightDirSet = false;
				}
				else if ((ShaderConst.GameTime.y >= ShaderConst.SunTiming.w && ShaderConst.GameTime.y <= 23.99) || (ShaderConst.GameTime.y >= 0 && ShaderConst.GameTime.y <= ShaderConst.SunTiming.x)) {
					ShaderConst.SunAmount.x = 0.0f;
					ShaderConst.SunAmount.y = 0.0f;
					ShaderConst.SunAmount.z = 0.0f;
					ShaderConst.SunAmount.w = 1.0f;
					ShaderConst.MasserAmount.x = ShaderConst.MasserFade;
					ShaderConst.SecundaAmount.x = ShaderConst.SecundaFade;
					ShaderConst.DayPhase = Night;
					dayPercent = 0.0f;
					ShaderConst.EveningTransLightDirSet = false;
					if (TheSettingManager->SettingsMain.Main.DirectionalLightOverride) {
						ShaderConst.DirectionalLight.x = TheShaderManager->ShaderConst.MasserDir.x * -1;
						ShaderConst.DirectionalLight.y = TheShaderManager->ShaderConst.MasserDir.y * -1;
						ShaderConst.DirectionalLight.z = TheShaderManager->ShaderConst.MasserDir.z * -1;
						ShaderConst.OverrideVanillaDirectionalLight = true;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.MasserDir;
					}
					else {
						ShaderConst.OverrideVanillaDirectionalLight = false;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
					}
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.x && ShaderConst.GameTime.y <= ShaderConst.SunTiming.y) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 <= ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) { 
						ShaderConst.SunAmount.x = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x ) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.MasserAmount.x = (ShaderConst.MasserFade - (ShaderConst.SunAmount.x / 0.4f));
						ShaderConst.SecundaAmount.x = (ShaderConst.SecundaFade - (ShaderConst.SunAmount.x / 0.4f));
						ShaderConst.OverrideVanillaDirectionalLight = false;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;		

						float start = 0.2f;
						float end = 0.6f;
						float diff = end - start;
						float scale = (ShaderConst.SunAmount.x - start) / diff;

						ShaderConst.ShadowMap.ShadowLightDir.w = std::clamp(scale, ShaderConst.Shadow.Data.y, 1.0f);
						ShaderConst.DayPhase = Dawn;
						dayPercent = ShaderConst.ShadowMap.ShadowLightDir.w;
						ShaderConst.EveningTransLightDirSet = false;

						if (ShaderConst.MasserAmount.x > 0.0f) {
							ShaderConst.OverrideVanillaDirectionalLight = true;
							if (!ShaderConst.MorningTransLightDirSet) {
								ShaderConst.DirectionalLight.x = std::lerp(ShaderConst.SunDir.x * -1, TheShaderManager->ShaderConst.MasserDir.x * -1, ShaderConst.MasserAmount.x);
								ShaderConst.DirectionalLight.y = std::lerp(ShaderConst.SunDir.y * -1, TheShaderManager->ShaderConst.MasserDir.y * -1, ShaderConst.MasserAmount.x);
								ShaderConst.DirectionalLight.z = std::lerp(ShaderConst.SunDir.z * -1, TheShaderManager->ShaderConst.MasserDir.z * -1, ShaderConst.MasserAmount.x);
							}
							else {
								ShaderConst.DirectionalLight.x = std::lerp(ShaderConst.MorningTransLightDir.x, TheShaderManager->ShaderConst.MasserDir.x * -1, ShaderConst.MasserAmount.x);
								ShaderConst.DirectionalLight.y = std::lerp(ShaderConst.MorningTransLightDir.y, TheShaderManager->ShaderConst.MasserDir.y * -1, ShaderConst.MasserAmount.x);
								ShaderConst.DirectionalLight.z = std::lerp(ShaderConst.MorningTransLightDir.z, TheShaderManager->ShaderConst.MasserDir.z * -1, ShaderConst.MasserAmount.x);
							}
							ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.MasserDir;
							ShaderConst.ShadowMap.ShadowLightDir.w = std::clamp(ShaderConst.MasserAmount.x, ShaderConst.Shadow.Data.y, 1.0f);
						}
						else {
							//Override the interval update here only to update the shadow map immediately, sometimes the refresh period transitions too late, showing the shadow map rotation
							TheSettingManager->SettingsShadows.Exteriors.UseIntervalUpdate = false;

							if (!ShaderConst.MorningTransLightDirSet && ShaderConst.MasserAmount.x > -0.1f) {
								ShaderConst.MorningTransLightDir = D3DXVECTOR4(Tes->niDirectionalLight->m_direction.x, Tes->niDirectionalLight->m_direction.y, Tes->niDirectionalLight->m_direction.z, 1);							
								if (fabs(ShaderConst.MorningTransLightDir.x) > 1.0f) {
									ShaderConst.MorningTransLightDirSet = true;
									((NiVector4*)&ShaderConst.MorningTransLightDir)->Normalize();
								}
							}
						}
					}
					else {
						ShaderConst.SunAmount.x = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x);
						ShaderConst.SunAmount.y = (ShaderConst.GameTime.y - ShaderConst.SunTiming.x) * 2 / (ShaderConst.SunTiming.y - ShaderConst.SunTiming.x) - 1.0f;
						ShaderConst.SunAmount.z = 0.0f;
						ShaderConst.SunAmount.w = 0.0f;
						ShaderConst.MasserAmount.x = (ShaderConst.MasserFade - (ShaderConst.SunAmount.x + ShaderConst.SunAmount.y));
						ShaderConst.SecundaAmount.x = (ShaderConst.SecundaFade - (ShaderConst.SunAmount.x + ShaderConst.SunAmount.y));
						ShaderConst.OverrideVanillaDirectionalLight = false;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
						ShaderConst.DayPhase = Sunrise;
						dayPercent = 1.0f;
						ShaderConst.EveningTransLightDirSet = false;
					}
				}
				else if (ShaderConst.GameTime.y >= ShaderConst.SunTiming.z && ShaderConst.GameTime.y <= ShaderConst.SunTiming.w) {
					if ((ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 <= ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 1.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.z = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = 0.0f;
						ShaderConst.MasserAmount.x = (ShaderConst.MasserFade - (ShaderConst.SunAmount.y + ShaderConst.SunAmount.z));
						ShaderConst.SecundaAmount.x = (ShaderConst.SecundaFade - (ShaderConst.SunAmount.y + ShaderConst.SunAmount.z));
						ShaderConst.OverrideVanillaDirectionalLight = false;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
						ShaderConst.DayPhase = Sunset;
						dayPercent = 1.0f;
						ShaderConst.EveningTransLightDirSet = false;
					}
					else {
						ShaderConst.SunAmount.x = 0.0f;
						ShaderConst.SunAmount.y = 0.0f;
						ShaderConst.SunAmount.z = 2.0f - (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z);
						ShaderConst.SunAmount.w = (ShaderConst.GameTime.y - ShaderConst.SunTiming.z) * 2 / (ShaderConst.SunTiming.w - ShaderConst.SunTiming.z) - 1.0f;
						ShaderConst.MasserAmount.x = (ShaderConst.MasserFade - (ShaderConst.SunAmount.z / 0.3f));
						ShaderConst.SecundaAmount.x = (ShaderConst.SecundaFade - (ShaderConst.SunAmount.z / 0.3f));
						ShaderConst.OverrideVanillaDirectionalLight = false;
						ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
						ShaderConst.DayPhase = Dusk;
						dayPercent = 1.0f;

						if (ShaderConst.SunAmount.z < .5) {
							float start = 0.5f;
							float end = 0.3f;
							float diff = start - end;
							float scale = (ShaderConst.SunAmount.z - end) / diff;
							ShaderConst.ShadowMap.ShadowLightDir.w = std::clamp(scale, ShaderConst.Shadow.Data.y, 1.0f);
							dayPercent = ShaderConst.ShadowMap.ShadowLightDir.w;
						}
						if (ShaderConst.MasserAmount.x > 0.0f) {						
							ShaderConst.OverrideVanillaDirectionalLight = true;

							if (!ShaderConst.EveningTransLightDirSet) {
								ShaderConst.EveningTransLightDir = D3DXVECTOR4(Tes->niDirectionalLight->m_direction.x, Tes->niDirectionalLight->m_direction.y, Tes->niDirectionalLight->m_direction.z, 1);
								((NiVector4*)&ShaderConst.EveningTransLightDir)->Normalize();
								ShaderConst.EveningTransLightDirSet = true;
							}
							ShaderConst.DirectionalLight.x = std::lerp(ShaderConst.EveningTransLightDir.x, TheShaderManager->ShaderConst.MasserDir.x * -1, ShaderConst.MasserAmount.x);
							ShaderConst.DirectionalLight.y = std::lerp(ShaderConst.EveningTransLightDir.y, TheShaderManager->ShaderConst.MasserDir.y * -1, ShaderConst.MasserAmount.x);
							ShaderConst.DirectionalLight.z = std::lerp(ShaderConst.EveningTransLightDir.z, TheShaderManager->ShaderConst.MasserDir.z * -1, ShaderConst.MasserAmount.x);
							ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.MasserDir;
							//Override the interval update here only to update the shadow map immediately, sometimes the refresh period transitions too late, showing the shadow map rotation
							TheSettingManager->SettingsShadows.Exteriors.UseIntervalUpdate = false;
							ShaderConst.ShadowMap.ShadowLightDir.w = std::clamp(ShaderConst.MasserAmount.x, ShaderConst.Shadow.Data.y, 1.0f);
							dayPercent = 0.0f;
						}
					}
				}

				TESWeatherEx* currentWeatherEx = ((TESWeatherEx*)currentWeather);
				currentWeather->colors[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].r = currentWeatherEx->colorsb[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].r * ShaderConst.MoonPhaseCoeff;
				currentWeather->colors[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].g = currentWeatherEx->colorsb[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].g * ShaderConst.MoonPhaseCoeff;
				currentWeather->colors[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].b = currentWeatherEx->colorsb[TESWeather::eColor_Sunlight].colors[TESWeather::eTime_Night].b * ShaderConst.MoonPhaseCoeff;
				currentWeather->colors[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].r = currentWeatherEx->colorsb[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].r * ShaderConst.MoonPhaseCoeff;
				currentWeather->colors[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].g = currentWeatherEx->colorsb[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].g * ShaderConst.MoonPhaseCoeff;
				currentWeather->colors[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].b = currentWeatherEx->colorsb[TESWeather::eColor_Ambient].colors[TESWeather::eTime_Night].b * ShaderConst.MoonPhaseCoeff;

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
			ShaderConst.MasserDir.w = 0.0f;
			ShaderConst.SecundaDir.w = 0.0f;
			ShaderConst.SunAmount.x = 0.0f;
			ShaderConst.SunAmount.y = 1.0f;
			ShaderConst.SunAmount.z = 0.0f;
			ShaderConst.SunAmount.w = 1.0f;
			ShaderConst.currentsunGlare = 0.5f;
			ShaderConst.ShadowMap.ShadowLightDir = ShaderConst.SunDir;
			ShaderConst.OverrideVanillaDirectionalLight = false;
			ShaderConst.EveningTransLightDirSet = false;
			isFullyInitialized = false;
			InitFrameCount = 0;
			TESObjectCELL::LightingData* LightData = currentCell->lighting;

			if (!(currentCell->flags0 & currentCell->kFlags0_BehaveLikeExterior)) {
				ShaderConst.fogColor.x = LightData->fog.r / 255.0f;
				ShaderConst.fogColor.y = LightData->fog.g / 255.0f;
				ShaderConst.fogColor.z = LightData->fog.b / 255.0f;
			}
			else {
				//TODO: fog color causes issues in SKYT shader for these cells
				ShaderConst.fogColor.x = 0.0f;
				ShaderConst.fogColor.y = 0.0f;
				ShaderConst.fogColor.z = 0.0f;
			}
			ShaderConst.fogColor.w = 1.0f;

			ShaderConst.sunColor.x = LightData->ambient.r / 255.0f;
			ShaderConst.sunColor.y = LightData->ambient.g / 255.0f;
			ShaderConst.sunColor.z = LightData->ambient.b / 255.0f;
			ShaderConst.sunColor.w = 0.0f;

			ShaderConst.fogData.x = LightData->fogNear;
			ShaderConst.fogData.y = LightData->fogFar;
			ShaderConst.fogData.z = ShaderConst.currentsunGlare;

			//TODO do these 
			ShaderConst.InteriorDimmerStart = 6.0f;
			ShaderConst.InteriorDimmerEnd = 9.0f;
			float dimmer;
			if (ShaderConst.GameTime.y > 12) {
				dimmer = ShaderConst.GameTime.y - (12 + ShaderConst.InteriorDimmerStart);
				dimmer = 1 - (dimmer / (ShaderConst.InteriorDimmerEnd - ShaderConst.InteriorDimmerStart));
				dimmer = std::clamp(dimmer, 0.0f, 1.0f);
			}
			else {
				dimmer = ShaderConst.GameTime.y - ShaderConst.InteriorDimmerStart;
				dimmer = (dimmer / (ShaderConst.InteriorDimmerEnd - ShaderConst.InteriorDimmerStart));
				dimmer = std::clamp(dimmer, 0.0f, 1.0f);
			}

			ShaderConst.InteriorDimmer.x = dimmer;

			if (TheSettingManager->SettingsMain.Main.InteriorDimmerCoeff < 1.0f) {
				float dimmerAdj = std::clamp(dimmer, TheSettingManager->SettingsMain.Main.InteriorDimmerCoeff, 1.0f);

				LightData->ambient.r = InteriorLighting.r * dimmerAdj;
				LightData->ambient.g = InteriorLighting.g * dimmerAdj;
				LightData->ambient.b = InteriorLighting.b * dimmerAdj;
			}
		}

		if (TheSettingManager->SettingsMain.Shaders.Water || TheSettingManager->SettingsMain.Effects.Underwater) {
			TESWaterForm* currentWater = currentCell->GetWaterForm();

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
			if (currentWeather->weatherType == TESWeather::WeatherType::kType_Rainy) {
				if (weatherPercent > 0.8f) {
					ShaderConst.Precipitations.RainData.x = (weatherPercent - 0.8f) / (1.0f - 0.8f);
				}
				else {
					ShaderConst.Precipitations.RainData.x = 0.0f;
				}
			}
			else if (!previousWeather || (previousWeather && previousWeather->weatherType == TESWeather::WeatherType::kType_Rainy)) {
				if ((1.0f - weatherPercent) > 0.8f) {
					ShaderConst.Precipitations.RainData.x = ((1.0f - weatherPercent) - 0.8f) / (1.0f - 0.8f);
				}
				else {
					ShaderConst.Precipitations.RainData.x = 0.0f;
				}
			}
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

		if (TheSettingManager->SettingsMain.Effects.KhajiitRays) {
			ShaderConst.KhajiitRaysMasser.Ray.x = TheSettingManager->SettingsKhajiitRays.mRayIntensity;
			ShaderConst.KhajiitRaysMasser.Ray.y = TheSettingManager->SettingsKhajiitRays.mRayLength;
			ShaderConst.KhajiitRaysMasser.Ray.z = TheSettingManager->SettingsKhajiitRays.mRayDensity;
			ShaderConst.KhajiitRaysMasser.Ray.w = TheSettingManager->SettingsKhajiitRays.mRayVisibility;
			ShaderConst.KhajiitRaysMasser.RayColor.x = TheSettingManager->SettingsKhajiitRays.mRayR;
			ShaderConst.KhajiitRaysMasser.RayColor.y = TheSettingManager->SettingsKhajiitRays.mRayG;
			ShaderConst.KhajiitRaysMasser.RayColor.z = TheSettingManager->SettingsKhajiitRays.mRayB;
			ShaderConst.KhajiitRaysMasser.RayColor.w = TheSettingManager->SettingsKhajiitRays.mSaturate;
			ShaderConst.KhajiitRaysMasser.Data.x = TheSettingManager->SettingsKhajiitRays.mLightShaftPasses;
			ShaderConst.KhajiitRaysMasser.Data.y = TheSettingManager->SettingsKhajiitRays.mLuminance;
			ShaderConst.KhajiitRaysMasser.Data.z = TheSettingManager->SettingsKhajiitRays.mGlobalMultiplier;

			ShaderConst.KhajiitRaysSecunda.Ray.x = TheSettingManager->SettingsKhajiitRays.sRayIntensity;
			ShaderConst.KhajiitRaysSecunda.Ray.y = TheSettingManager->SettingsKhajiitRays.sRayLength;
			ShaderConst.KhajiitRaysSecunda.Ray.z = TheSettingManager->SettingsKhajiitRays.sRayDensity;
			ShaderConst.KhajiitRaysSecunda.Ray.w = TheSettingManager->SettingsKhajiitRays.sRayVisibility;
			ShaderConst.KhajiitRaysSecunda.RayColor.x = TheSettingManager->SettingsKhajiitRays.sRayR;
			ShaderConst.KhajiitRaysSecunda.RayColor.y = TheSettingManager->SettingsKhajiitRays.sRayG;
			ShaderConst.KhajiitRaysSecunda.RayColor.z = TheSettingManager->SettingsKhajiitRays.sRayB;
			ShaderConst.KhajiitRaysSecunda.RayColor.w = TheSettingManager->SettingsKhajiitRays.sSaturate;
			ShaderConst.KhajiitRaysSecunda.Data.x = TheSettingManager->SettingsKhajiitRays.sLightShaftPasses;
			ShaderConst.KhajiitRaysSecunda.Data.y = TheSettingManager->SettingsKhajiitRays.sLuminance;
			ShaderConst.KhajiitRaysSecunda.Data.z = TheSettingManager->SettingsKhajiitRays.sGlobalMultiplier;
		}

		if (TheSettingManager->SettingsMain.Effects.AmbientOcclusion) {
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
			ShaderConst.Bloom.BloomData.x = sbs->Luminance;
			ShaderConst.Bloom.BloomData.y = sbs->MiddleGray;
			ShaderConst.Bloom.BloomData.z = sbs->WhiteCutOff;
			ShaderConst.Bloom.BloomValues.x = sbs->BloomIntensity;
			ShaderConst.Bloom.BloomValues.y = sbs->OriginalIntensity;
			ShaderConst.Bloom.BloomValues.z = sbs->BloomSaturation;
			ShaderConst.Bloom.BloomValues.w = sbs->OriginalSaturation;
		}

		if (TheSettingManager->SettingsMain.Effects.Coloring) {
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
			ShaderConst.Cinema.Data.w = TheSettingManager->SettingsCinema.ChromaticAberrationPower;
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

		if (TheSettingManager->SettingsMain.Effects.TAA) {
			ShaderConst.TAA.Data.x = TheSettingManager->SettingsTAA.BlendWeight;
			ShaderConst.TAA.Data.y = TheSettingManager->SettingsTAA.ClampRadius;
			ShaderConst.TAA.Data.z = TheSettingManager->SettingsTAA.Sharpening;
		}

		if (TheSettingManager->SettingsMain.Effects.VolumetricLight) {

			SettingsVolumetricLightStruct* currentSettings = TheSettingManager->GetSettingsVolumetricLight(((TESWeatherEx*)currentWeather)->EditorName);
			SettingsVolumetricLightStruct* previousSettings;

			ShaderConstants::VolumetricLightStruct currentValues;
			ShaderConstants::VolumetricLightStruct previousValues;

			if (currentSettings == NULL) {
				currentSettings = TheSettingManager->GetSettingsVolumetricLight("Default");
			}

			if (!modifiersInitialzed) {
				SetVolumetricLightModifiers(currentSettings);
				previousModifier = currentModifier;
				previousFogHeight = currentFogHeight;
				previousAccumDistance = currentAccumDistance;
				modifiersInitialzed = true;
			}

			if (previousWeather == NULL) {
				previousSettings = TheSettingManager->GetSettingsVolumetricLight(((TESWeatherEx*)currentWeather)->EditorName);
				modifiersSet = false;
			}
			else {
				//we suspect the weather has changed, here we can determine the random modifier of the next (current) weather
				//set the random Modifer before first transition
				//if modifiers not set
				if (!modifiersSet) {
					SetVolumetricLightModifiers(currentSettings);
				}
				previousSettings = TheSettingManager->GetSettingsVolumetricLight(((TESWeatherEx*)previousWeather)->EditorName);
			}

			if (previousSettings == NULL) {
				previousSettings = TheSettingManager->GetSettingsVolumetricLight("Default");
			}

			if (ShaderConst.DayPhase < 2) {
				//sunrise -> midday
				currentValues.data1.x = std::lerp(currentSettings->accumSunriseR, currentSettings->accumMiddayR, ShaderConst.SunAmount.y);
				currentValues.data1.y = std::lerp(currentSettings->accumSunriseG, currentSettings->accumMiddayG, ShaderConst.SunAmount.y);
				currentValues.data1.z = std::lerp(currentSettings->accumSunriseB, currentSettings->accumMiddayB, ShaderConst.SunAmount.y);
				currentValues.data2.x = std::lerp(currentSettings->baseSunriseR, currentSettings->baseMiddayR, ShaderConst.SunAmount.y);
				currentValues.data2.y = std::lerp(currentSettings->baseSunriseG, currentSettings->baseMiddayG, ShaderConst.SunAmount.y);
				currentValues.data2.z = std::lerp(currentSettings->baseSunriseB, currentSettings->baseMiddayB, ShaderConst.SunAmount.y);
				currentValues.data6.x = currentSettings->sunScatterR;
				currentValues.data6.y = currentSettings->sunScatterG;
				currentValues.data6.z = currentSettings->sunScatterB;

				previousValues.data1.x = std::lerp(previousSettings->accumSunriseR, previousSettings->accumMiddayR, ShaderConst.SunAmount.y);
				previousValues.data1.y = std::lerp(previousSettings->accumSunriseG, previousSettings->accumMiddayG, ShaderConst.SunAmount.y);
				previousValues.data1.z = std::lerp(previousSettings->accumSunriseB, previousSettings->accumMiddayB, ShaderConst.SunAmount.y);
				previousValues.data2.x = std::lerp(previousSettings->baseSunriseR, previousSettings->baseMiddayR, ShaderConst.SunAmount.y);
				previousValues.data2.y = std::lerp(previousSettings->baseSunriseG, previousSettings->baseMiddayG, ShaderConst.SunAmount.y);
				previousValues.data2.z = std::lerp(previousSettings->baseSunriseB, previousSettings->baseMiddayB, ShaderConst.SunAmount.y);
				previousValues.data6.x = previousSettings->sunScatterR;
				previousValues.data6.y = previousSettings->sunScatterG;
				previousValues.data6.z = previousSettings->sunScatterB;
			}
			else {
				//midday -> sunset
				currentValues.data1.x = std::lerp(currentSettings->accumMiddayR, currentSettings->accumSunsetR, ShaderConst.SunAmount.z);
				currentValues.data1.y = std::lerp(currentSettings->accumMiddayG, currentSettings->accumSunsetG, ShaderConst.SunAmount.z);
				currentValues.data1.z = std::lerp(currentSettings->accumMiddayB, currentSettings->accumSunsetB, ShaderConst.SunAmount.z);
				currentValues.data2.x = std::lerp(currentSettings->baseMiddayR, currentSettings->baseSunsetR, ShaderConst.SunAmount.z);
				currentValues.data2.y = std::lerp(currentSettings->baseMiddayG, currentSettings->baseSunsetG, ShaderConst.SunAmount.z);
				currentValues.data2.z = std::lerp(currentSettings->baseMiddayB, currentSettings->baseSunsetB, ShaderConst.SunAmount.z);
				currentValues.data6.x = currentSettings->sunScatterR;
				currentValues.data6.y = currentSettings->sunScatterG;
				currentValues.data6.z = currentSettings->sunScatterB;

				previousValues.data1.x = std::lerp(previousSettings->accumMiddayR, previousSettings->accumSunsetR, ShaderConst.SunAmount.z);
				previousValues.data1.y = std::lerp(previousSettings->accumMiddayG, previousSettings->accumSunsetG, ShaderConst.SunAmount.z);
				previousValues.data1.z = std::lerp(previousSettings->accumMiddayB, previousSettings->accumSunsetB, ShaderConst.SunAmount.z);
				previousValues.data2.x = std::lerp(previousSettings->baseMiddayR, previousSettings->baseSunsetR, ShaderConst.SunAmount.z);
				previousValues.data2.y = std::lerp(previousSettings->baseMiddayG, previousSettings->baseSunsetG, ShaderConst.SunAmount.z);
				previousValues.data2.z = std::lerp(previousSettings->baseMiddayB, previousSettings->baseSunsetB, ShaderConst.SunAmount.z);
				previousValues.data6.x = previousSettings->sunScatterR;
				previousValues.data6.y = previousSettings->sunScatterG;
				previousValues.data6.z = previousSettings->sunScatterB;
			}

			if (dayPercent < 1.0f) {
				float phaseModifier = 1;
				if (ShaderConst.MoonPhaseCoeff == 0.0f) {
					phaseModifier = 0;
				}
				currentValues.data1.x = std::lerp(currentSettings->accumNightR * phaseModifier, currentValues.data1.x, dayPercent);
				currentValues.data1.y = std::lerp(currentSettings->accumNightG * phaseModifier, currentValues.data1.y, dayPercent);
				currentValues.data1.z = std::lerp(currentSettings->accumNightB * phaseModifier, currentValues.data1.z, dayPercent);
				currentValues.data2.x = std::lerp(currentSettings->baseNightR, currentValues.data2.x, dayPercent);
				currentValues.data2.y = std::lerp(currentSettings->baseNightG, currentValues.data2.y, dayPercent);
				currentValues.data2.z = std::lerp(currentSettings->baseNightB, currentValues.data2.z, dayPercent);
				currentValues.data6.x = std::lerp(0.3f * phaseModifier, currentSettings->sunScatterR, dayPercent);
				currentValues.data6.y = std::lerp(0.3f * phaseModifier, currentSettings->sunScatterG, dayPercent);
				currentValues.data6.z = std::lerp(0.3f * phaseModifier, currentSettings->sunScatterB, dayPercent);

				previousValues.data1.x = std::lerp(previousSettings->accumNightR * phaseModifier, previousValues.data1.x, dayPercent);
				previousValues.data1.y = std::lerp(previousSettings->accumNightG * phaseModifier, previousValues.data1.y, dayPercent);
				previousValues.data1.z = std::lerp(previousSettings->accumNightB * phaseModifier, previousValues.data1.z, dayPercent);
				previousValues.data2.x = std::lerp(previousSettings->baseNightR, previousValues.data2.x, dayPercent);
				previousValues.data2.y = std::lerp(previousSettings->baseNightG, previousValues.data2.y, dayPercent);
				previousValues.data2.z = std::lerp(previousSettings->baseNightB, previousValues.data2.z, dayPercent);
				previousValues.data6.x = std::lerp(0.3f * phaseModifier, previousSettings->sunScatterR, dayPercent);
				previousValues.data6.y = std::lerp(0.3f * phaseModifier, previousSettings->sunScatterG, dayPercent);
				previousValues.data6.z = std::lerp(0.3f * phaseModifier, previousSettings->sunScatterB, dayPercent);
			}

			ShaderConst.VolumetricLight.data1.x = std::lerp(previousValues.data1.x * previousModifier, currentValues.data1.x * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data1.y = std::lerp(previousValues.data1.y * previousModifier, currentValues.data1.y * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data1.z = std::lerp(previousValues.data1.z * previousModifier, currentValues.data1.z * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data1.w = std::lerp(previousAccumDistance, currentAccumDistance, weatherPercent);

			ShaderConst.VolumetricLight.data2.x = std::lerp(previousValues.data2.x * previousModifier, currentValues.data2.x * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data2.y = std::lerp(previousValues.data2.y * previousModifier, currentValues.data2.y * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data2.z = std::lerp(previousValues.data2.z * previousModifier, currentValues.data2.z * currentModifier, weatherPercent);
			ShaderConst.VolumetricLight.data2.w = std::lerp(previousSettings->baseDistance, currentSettings->baseDistance, weatherPercent);

			ShaderConst.VolumetricLight.data3.y = std::lerp(previousSettings->accumCutOff, currentSettings->accumCutOff, weatherPercent);
			ShaderConst.VolumetricLight.data3.z = std::lerp(previousSettings->blurDistance * (1.0f/previousModifier), currentSettings->blurDistance * (1.0f/currentModifier), weatherPercent);
			ShaderConst.VolumetricLight.data3.w = std::lerp(previousFogHeight, currentFogHeight, weatherPercent);

			ShaderConst.VolumetricLight.data4.y = std::lerp(previousSettings->animatedFogToggle, currentSettings->animatedFogToggle, weatherPercent);
			ShaderConst.VolumetricLight.data4.z = TheRenderManager->width;
			ShaderConst.VolumetricLight.data4.w = TheRenderManager->height;

			ShaderConst.VolumetricLight.data5.w = std::lerp(previousSettings->fogPower, currentSettings->fogPower, weatherPercent);

			ShaderConst.VolumetricLight.data6.x = std::lerp(previousValues.data6.x, currentValues.data6.x, weatherPercent);
			ShaderConst.VolumetricLight.data6.y = std::lerp(previousValues.data6.y, currentValues.data6.y, weatherPercent);
			ShaderConst.VolumetricLight.data6.z = std::lerp(previousValues.data6.z, currentValues.data6.z, weatherPercent);
			ShaderConst.VolumetricLight.data6.w = dayPercent;

			if (weatherPercent > 0.5f) {
				ShaderConst.VolumetricLight.data4.y = std::lerp(0.0f, currentSettings->animatedFogToggle * 2, weatherPercent - .5);
				ShaderConst.VolumetricLight.data5.x = currentWind.x;
				ShaderConst.VolumetricLight.data5.y = currentWind.y;
				ShaderConst.VolumetricLight.data5.z = currentWind.z;
			} else {
				ShaderConst.VolumetricLight.data4.y = std::lerp(previousSettings->animatedFogToggle, 0.0f, weatherPercent * 2);
			}
		}

		//Specular data
		ShaderConst.Specular.SpecularData.x = TheSettingManager->SettingsSpecular.SpecularPower;
		ShaderConst.Specular.SpecularData.y = TheSettingManager->SettingsSpecular.FresnelPowerActors;
		ShaderConst.Specular.SpecularData.z = TheSettingManager->SettingsSpecular.FresnelPowerObjects;
	}
}

void ShaderManager::SetVolumetricLightModifiers(SettingsVolumetricLightStruct* currentSettings) {
	srand(TimeGlobals::GetGameTime());
	previousModifier = currentModifier;
	previousWind.x = currentWind.x;
	previousWind.y = currentWind.y;
	previousWind.z = currentWind.z;

	currentModifier = (rand() % (int)currentSettings->randomizer + 1) / currentSettings->randomizer;
	currentWind.x = ((rand() % 10 + 1) / 10.0f) * ((rand() % 2) + 1) - 1;
	currentWind.y = ((rand() % 2) + 1) == 1 ? -1 : 1;
	currentWind.z = ((rand() % 10 + 1) / 10.0f);

	previousFogHeight = currentFogHeight;
	int minHeight = (int)currentSettings->accumHeightMin;
	currentFogHeight = (rand() + rand() + rand()) % ((int)currentSettings->accumHeightMax - minHeight) + minHeight;

	previousAccumDistance = currentAccumDistance;
	int minDistance = (int)currentSettings->accumDistanceMin;
	currentAccumDistance = (rand() * ((rand() % 10 + 1))) % ((int)currentSettings->accumDistanceMax - minDistance) + minDistance;

	modifiersSet = true;
}

void ShaderManager::BeginScene() {

	if (ShaderConst.OverrideVanillaDirectionalLight) {
		Tes->niDirectionalLight->m_direction.x = TheShaderManager->ShaderConst.DirectionalLight.x;
		Tes->niDirectionalLight->m_direction.y = TheShaderManager->ShaderConst.DirectionalLight.y;
		Tes->niDirectionalLight->m_direction.z = TheShaderManager->ShaderConst.DirectionalLight.z;
	}

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
	NiD3DVertexShaderEx** ShadowLightVertexShaders = (NiD3DVertexShaderEx**)0x00B4528C;
	NiD3DPixelShaderEx** ShadowLightPixelShaders = (NiD3DPixelShaderEx**)0x00B45088;
	NiD3DPixelShaderEx** SM3PixelShaders = (NiD3DPixelShaderEx**)0x00B46ED8;
	NiD3DPixelShaderEx** SM3LLPixelShaders = (NiD3DPixelShaderEx**)0x00B46C20;
	NiD3DVertexShaderEx** SM3VertexShaders = (NiD3DVertexShaderEx**)0x00B47288;

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
		for each (NiD3DVertexShader * VS in SS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader * PS in SS->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "Terrain")) {
		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && strstr(TerrainShaders, VS->ShaderName)) {
				LoadShader(VS);
			}
		}
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(TerrainShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}
	}
	else if (!strcmp(Name, "Blood")) {
		GeometryDecalShader* GDS = (GeometryDecalShader*)GetShaderDefinition(16)->Shader;
		for each (NiD3DVertexShader* VS in GDS->Vertex) LoadShader(VS);
		for each (NiD3DPixelShader* PS in GDS->Pixel) LoadShader(PS);
	}
	else if (!strcmp(Name, "InteriorShadows")) {
		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && strstr(InteriorShadowShaders, VS->ShaderName)) {
				LoadShader(VS);
			}
		}
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(InteriorShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}

		for (int i = 0; i < 39; i++) {
			NiD3DPixelShaderEx* PS = SM3PixelShaders[i];
			if (PS && strstr(InteriorShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}

		for (int i = 0; i < 20; i++) {
			NiD3DPixelShaderEx* PS = SM3LLPixelShaders[i];
			if (PS && strstr(InteriorShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}

		for (int i = 0; i < 32; i++) {
			NiD3DVertexShaderEx* VS = SM3VertexShaders[i];
			if (VS && strstr(InteriorShadowShaders, VS->ShaderName)) {
				LoadShader(VS);
			}
		}

		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && strstr(InteriorShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}
	}
	else if (!strcmp(Name, "ExteriorExtraShaders")) {
		for (int i = 0; i < 20; i++) {
			NiD3DPixelShaderEx* PS = SM3LLPixelShaders[i];
			if (PS && strstr(InteriorShadowShaders, PS->ShaderName)) {
				LoadShader(PS, "Exterior");
			}
		}
		for (int i = 0; i < 32; i++) {
			NiD3DVertexShaderEx* VS = SM3VertexShaders[i];
			if (VS && strstr(InteriorShadowShaders, VS->ShaderName)) {
				LoadShader(VS, "Exterior");
			}
		}
	}
	else if (!strcmp(Name, "ExteriorPom")) {
		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && strstr(ExteriorPom, PS->ShaderName)) {
				LoadShader(PS, "Exterior");
			}
		}
	}
	else if (!strcmp(Name, "ExteriorDialogActive")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				LoadShader(PS,"Dialog");
			}
		}

		SkinShader* SS = (SkinShader*)GetShaderDefinition(14)->Shader;
		for each (NiD3DPixelShaderEx * PS in SS->Pixel) {
			if (PS && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				LoadShader(PS, "Dialog");
			}
		}
	}
	else if (!strcmp(Name, "ExteriorDialogInactive")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}

		SkinShader* SS = (SkinShader*)GetShaderDefinition(14)->Shader;
		for each (NiD3DPixelShaderEx * PS in SS->Pixel) {
			if (PS && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}
	}
	else if (!strcmp(Name, "InteriorSpecularShadowActive")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
				LoadShader(PS, "InteriorSpecular");
			}
		}

		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && strstr(InteriorSpecularShadowShaders, VS->ShaderName)) {
				LoadShader(VS, "InteriorSpecular");
			}
		}

		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DVertexShaderEx * VS in PRS->Vertex) {
			if (VS && strstr(InteriorSpecularShadowShaders, VS->ShaderName)) {
				LoadShader(VS, "InteriorSpecular");
			}
		}
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
				LoadShader(PS, "InteriorSpecular");
			}
		}
	}
	else if (!strcmp(Name, "InteriorSpecularShadowInactive")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}

		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && strstr(InteriorSpecularShadowShaders, VS->ShaderName)) {
				LoadShader(VS);
			}
		}

		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DVertexShaderEx * VS in PRS->Vertex) {
			if (VS && strstr(InteriorSpecularShadowShaders, VS->ShaderName)) {
				LoadShader(VS);
			}
		}
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
				LoadShader(PS);
			}
		}
	}
#elif defined(SKYRIM)
	if (!strcmp(Name, "Water")) {
		for each (NiD3DVertexShader* VS in WaterVertexShaders) LoadShader(VS);
		for each (NiD3DPixelShader* PS in WaterPixelShaders) LoadShader(PS);
	}
#endif

}

void ShaderManager::LoadShader(NiD3DVertexShader* Shader, const char* DirPostFix) {
	
	ShaderRecord* ShaderProg = new ShaderRecord();
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)Shader;

	if (ShaderProg->LoadShader(VertexShader->ShaderName, DirPostFix)) {
		VertexShader->ShaderProg = ShaderProg;
		VertexShader->ShaderHandleBackup = VertexShader->ShaderHandle;
		TheRenderManager->device->CreateVertexShader((const DWORD*)ShaderProg->Function, &VertexShader->ShaderHandle);
	}
	else {
		delete ShaderProg;
	}

}

void ShaderManager::LoadShader(NiD3DPixelShader* Shader, const char* DirPostFix) {

	ShaderRecord* ShaderProg = new ShaderRecord();
	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)Shader;

	if (ShaderProg->LoadShader(PixelShader->ShaderName, DirPostFix)) {
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
	NiD3DVertexShaderEx** ShadowLightVertexShaders = (NiD3DVertexShaderEx**)0x00B4528C;
	NiD3DPixelShaderEx** ShadowLightPixelShaders = (NiD3DPixelShaderEx**)0x00B45088;
	NiD3DPixelShaderEx** SM3PixelShaders = (NiD3DPixelShaderEx**)0x00B46ED8;//SM3 psos + 39
	NiD3DPixelShaderEx** SM3LLPixelShaders = (NiD3DPixelShaderEx**)0x00B46C20; //SM3LL psos + 20
	NiD3DVertexShaderEx** SM3VertexShaders = (NiD3DVertexShaderEx**)0x00B47288;//SM3* vsos + 32

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
		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && VS->ShaderProg && strstr(TerrainShaders, VS->ShaderName)) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for (int i = 0; i < 130; i++) {
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
	else if (!strcmp(Name, "InteriorShadows")) {
		for (int i = 0; i < 130; i++) {
			NiD3DVertexShaderEx* VS = ShadowLightVertexShaders[i];
			if (VS && VS->ShaderProg && strstr(InteriorShadowShaders, VS->ShaderName)) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && PS->ShaderProg && strstr(InteriorShadowShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}

		for (int i = 0; i < 39; i++) {
			NiD3DPixelShaderEx* PS = SM3PixelShaders[i];
			if (PS && PS->ShaderProg && strstr(InteriorShadowShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}

		for (int i = 0; i < 20; i++) {
			NiD3DPixelShaderEx* PS = SM3LLPixelShaders[i];
			if (PS && PS->ShaderProg && strstr(InteriorShadowShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
		for (int i = 0; i < 32; i++) {
			NiD3DVertexShaderEx* VS = SM3VertexShaders[i];
			if (VS && VS->ShaderProg && strstr(InteriorShadowShaders, VS->ShaderName)) {
				VS->ShaderHandle = VS->ShaderHandleBackup;
				delete VS->ShaderProg; VS->ShaderProg = NULL;
			}
		}

		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && PS->ShaderProg && strstr(InteriorShadowShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "ExteriorDialog")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && PS->ShaderProg && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}

		SkinShader* SS = (SkinShader*)GetShaderDefinition(14)->Shader;
		for each (NiD3DPixelShaderEx * PS in SS->Pixel) {
			if (PS && PS->ShaderProg && strstr(ExteriorDialogShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}
	}
	else if (!strcmp(Name, "InteriorSpecularShadow")) {
		for (int i = 0; i < 130; i++) {
			NiD3DPixelShaderEx* PS = ShadowLightPixelShaders[i];
			if (PS && PS->ShaderProg && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
				PS->ShaderHandle = PS->ShaderHandleBackup;
				delete PS->ShaderProg; PS->ShaderProg = NULL;
			}
		}

		ParallaxShader* PRS = (ParallaxShader*)GetShaderDefinition(15)->Shader;
		for each (NiD3DPixelShaderEx * PS in PRS->Pixel) {
			if (PS && PS->ShaderProg && strstr(InteriorSpecularShadowShaders, PS->ShaderName)) {
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
		case EffectRecordType_KhajiitRays:
			char mFilename[MAX_PATH];
			strcpy(mFilename, EffectsPath);
			strcat(mFilename, "KhajiitRays\\MasserRays.fx");
			char sFilename[MAX_PATH];
			strcpy(sFilename, EffectsPath);
			strcat(sFilename, "KhajiitRays\\SecundaRays.fx");
			MasserRaysEffect = new EffectRecord();
			SecundaRaysEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.KhajiitRays = LoadEffect(MasserRaysEffect, mFilename, NULL) && LoadEffect(SecundaRaysEffect, sFilename, NULL);
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
		case EffectRecordType_SMAA:
			strcat(Filename, "SMAA\\SMAA.fx");
			SMAAEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.SMAA = LoadEffect(SMAAEffect, Filename, NULL);
			break;
		case EffectRecordType_TAA:
			strcat(Filename, "TAA\\TAA.fx");
			TAAEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.TAA = LoadEffect(TAAEffect, Filename, NULL);
			break;
		case EffectRecordType_MotionBlur:
			strcat(Filename, "MotionBlur\\MotionBlur.fx");
			MotionBlurEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.MotionBlur = LoadEffect(MotionBlurEffect, Filename, NULL);
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
		case EffectRecordType_VolumetricLight:
			strcat(Filename, "VolumetricLight\\VolumetricLight.fx");
			VolumetricLightEffect = new EffectRecord();
			TheSettingManager->SettingsMain.Effects.VolumetricLight = LoadEffect(VolumetricLightEffect, Filename, NULL);
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
			TheSettingManager->SettingsShadows.Exteriors.UsePostProcessing = LoadEffect(ShadowsExteriorsEffect, Filename, NULL);
			break;
		case EffectRecordType_ShadowsExteriorsPoint:
			strcat(Filename, "Shadows\\ShadowsExteriorsPoint.fx");
			ShadowsExteriorsPointEffect = new EffectRecord();
			TheSettingManager->SettingsShadows.ExteriorsPoint.UsePostProcessing = LoadEffect(ShadowsExteriorsPointEffect, Filename, NULL);
			break;
		case EffectRecordType_ShadowsExteriorsPointDialog:
			strcat(Filename, "Shadows\\ShadowsExteriorsPointDialog.fx");
			ShadowsExteriorsPointDialogEffect = new EffectRecord();
			LoadEffect(ShadowsExteriorsPointDialogEffect, Filename, NULL);
			break;
		case EffectRecordType_ShadowsInteriors:
			strcat(Filename, "Shadows\\ShadowsInteriors.fx");
			ShadowsInteriorsEffect = new EffectRecord();
			TheSettingManager->SettingsShadows.Interiors.UsePostProcessing = LoadEffect(ShadowsInteriorsEffect, Filename, NULL);
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
	else if (TheEffect == BloomEffect) BloomEffect = NULL;
	else if (TheEffect == CinemaEffect) CinemaEffect = NULL;
	else if (TheEffect == ColoringEffect) ColoringEffect = NULL;
	else if (TheEffect == DepthOfFieldEffect) DepthOfFieldEffect = NULL;
	else if (TheEffect == GodRaysEffect) GodRaysEffect = NULL;
	else if (TheEffect == MasserRaysEffect) MasserRaysEffect = NULL;
	else if (TheEffect == SecundaRaysEffect) SecundaRaysEffect = NULL;
	else if (TheEffect == MotionBlurEffect) MotionBlurEffect = NULL;
	else if (TheEffect == SMAAEffect) SMAAEffect = NULL;
	else if (TheEffect == TAAEffect) TAAEffect = NULL;
	else if (TheEffect == SnowAccumulationEffect) SnowAccumulationEffect = NULL;
	else if (TheEffect == UnderwaterEffect) UnderwaterEffect = NULL;
	else if (TheEffect == WaterLensEffect) WaterLensEffect = NULL;
	else if (TheEffect == WetWorldEffect) WetWorldEffect = NULL;
	else if (TheEffect == SharpeningEffect) SharpeningEffect = NULL;
	else if (TheEffect == VolumetricFogEffect) VolumetricFogEffect = NULL;
	else if (TheEffect == VolumetricLightEffect) VolumetricLightEffect = NULL;
	else if (TheEffect == RainEffect) RainEffect = NULL;
	else if (TheEffect == SnowEffect) SnowEffect = NULL;
	else if (TheEffect == ShadowsExteriorsEffect) ShadowsExteriorsEffect = NULL;
	else if (TheEffect == ShadowsExteriorsPointEffect) ShadowsExteriorsPointEffect = NULL;
	else if (TheEffect == ShadowsExteriorsPointDialogEffect) ShadowsExteriorsPointDialogEffect = NULL;	
	else if (TheEffect == ShadowsInteriorsEffect) ShadowsInteriorsEffect = NULL;

	if (TheEffect) delete TheEffect;

}

void ShaderManager::RenderEffects(IDirect3DSurface9* RenderTarget) {
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;
	IDirect3DDevice9* Device = TheRenderManager->device;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();
	D3DXVECTOR4* SunDir = &TheShaderManager->ShaderConst.SunDir;

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
	if (TheSettingManager->SettingsShadows.Exteriors.Enabled && TheSettingManager->SettingsShadows.Exteriors.UsePostProcessing && currentWorldSpace) {
		ShadowsExteriorsEffect->SetCT();
		ShadowsExteriorsEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (TheSettingManager->SettingsShadows.ExteriorsPoint.Enabled && TheSettingManager->SettingsShadows.ExteriorsPoint.UsePostProcessing && currentWorldSpace) {

		if (!(MenuManager->IsActive(Menu::MenuType::kMenuType_Dialog) || MenuManager->IsActive(Menu::MenuType::kMenuType_Persuasion))) {
			ShadowsExteriorsPointEffect->SetCT();
			ShadowsExteriorsPointEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
		else {
			ShadowsExteriorsPointDialogEffect->SetCT();
			ShadowsExteriorsPointDialogEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
	}
	if (TheSettingManager->SettingsShadows.Interiors.UsePostProcessing && !currentWorldSpace) {
		ShadowsInteriorsEffect->SetCT();
		ShadowsInteriorsEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Bloom) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		BloomEffect->SetCT();
		BloomEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Underwater && ShaderConst.HasWater && TheRenderManager->CameraPosition.z < ShaderConst.Water.waterSettings.x + 20.0f) {
		if (TheRenderManager->CameraPosition.z < ShaderConst.Water.waterSettings.x) {
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
		if (Effects->GodRays && currentWorldSpace && (ShaderConst.SunAmount.x >= 0.4 || ShaderConst.SunAmount.y > 0 || ShaderConst.SunAmount.z >= 0.3)) {
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			GodRaysEffect->SetCT();
			GodRaysEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
		else if (ShaderConst.MoonsExist && Effects->KhajiitRays && currentWorldSpace && (ShaderConst.SunAmount.x < 0.4 || ShaderConst.SunAmount.z < 0.3)) {
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			SecundaRaysEffect->SetCT();
			SecundaRaysEffect->Render(Device, RenderTarget, RenderedSurface, false);
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			MasserRaysEffect->SetCT();
			MasserRaysEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
		if (Effects->VolumetricFog && currentWorldSpace && ShaderConst.VolumetricFog.Data.w) {
			Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
			VolumetricFogEffect->SetCT();
			VolumetricFogEffect->Render(Device, RenderTarget, RenderedSurface, false);
		}
	}
	if (Effects->VolumetricLight && currentWorldSpace) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		VolumetricLightEffect->SetCT();
		VolumetricLightEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->SMAA) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		Device->SetRenderTarget(0, RenderSurfaceSMAA);
		SMAAEffect->SetCT();
		SMAAEffect->Render(Device, RenderSurfaceSMAA, RenderedSurface, true);
		Device->StretchRect(RenderSurfaceSMAA, NULL, RenderTarget, NULL, D3DTEXF_NONE);
		Device->SetRenderTarget(0, RenderTarget);
	}
	if (Effects->TAA) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		TAAEffect->SetCT();
		TAAEffect->Render(Device, RenderTarget, RenderedSurface, false);
		Device->StretchRect(RenderedSurface, NULL, TAASurface, NULL, D3DTEXF_NONE);
	}
	if (Effects->DepthOfField && ShaderConst.DepthOfField.Enabled) {
		Device->StretchRect(RenderTarget, NULL, SourceSurface, NULL, D3DTEXF_NONE);
		DepthOfFieldEffect->SetCT();
		DepthOfFieldEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->WaterLens && ShaderConst.WaterLens.Percent > 0.0f) {
		WaterLensEffect->SetCT();
		WaterLensEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->MotionBlur && (ShaderConst.MotionBlur.Data.x || ShaderConst.MotionBlur.Data.y)) {
		MotionBlurEffect->SetCT();
		MotionBlurEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Sharpening) {
		SharpeningEffect->SetCT();
		SharpeningEffect->Render(Device, RenderTarget, RenderedSurface, false);
	}
	if (Effects->Coloring) {
		ColoringEffect->SetCT();
		ColoringEffect->Render(Device, RenderTarget, RenderedSurface, false);
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

void ShaderManager::RenderEffectsPreHdr(IDirect3DSurface9* RenderTargetParam) {
	IDirect3DDevice9* Device = TheRenderManager->device;
	NiDX9RenderState* RenderState = TheRenderManager->renderState;
	int rs1 = RenderState->GetRenderState(D3DRS_ZENABLE); //1
	int rs2 = RenderState->GetRenderState(D3DRS_ALPHAREF); //1
	int rs3 = RenderState->GetRenderState(D3DRS_ALPHABLENDENABLE); //1
	int rs4 = RenderState->GetRenderState(D3DRS_COLORWRITEENABLE); //1
	RenderState->SetRenderState(D3DRS_ZENABLE, FALSE, 0); //1
	RenderState->SetRenderState(D3DRS_ALPHAREF, FALSE, 0); //10
	RenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE, 0);//1
	RenderState->SetRenderState(D3DRS_COLORWRITEENABLE, 15, 0); //0
	TheRenderManager->SetupSceneCamera();
	Device->SetStreamSource(0, EffectVertex, 0, sizeof(EffectQuad));
	Device->SetFVF(EFFECTQUADFORMAT);
    Device->StretchRect(RenderTargetParam, NULL, EffectSurface, NULL, D3DTEXF_NONE); 
	Device->StretchRect(RenderTargetParam, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	Device->SetRenderTarget(0,EffectSurface);
	IDirect3DSurface9* RenderTarget = EffectSurface;
	RenderEffects(RenderTarget);
	Device->SetRenderTarget(0, RenderTargetParam);
	RenderState->SetRenderState(D3DRS_ZENABLE, rs1, 0); //1
	RenderState->SetRenderState(D3DRS_ALPHAREF, rs2, 0); //10
	RenderState->SetRenderState(D3DRS_ALPHABLENDENABLE, rs3, 0);//1
	RenderState->SetRenderState(D3DRS_COLORWRITEENABLE, rs4, 0); //0
}

void ShaderManager::RenderEffectsPostHdr(IDirect3DSurface9* RenderTargetParam) {
	IDirect3DDevice9* Device = TheRenderManager->device;
	TheRenderManager->SetupSceneCamera();
	Device->SetStreamSource(0, EffectVertex, 0, sizeof(EffectQuad));
	Device->SetFVF(EFFECTQUADFORMAT);
	Device->StretchRect(RenderTargetParam, NULL, RenderedSurface, NULL, D3DTEXF_NONE);
	RenderEffects(RenderTargetParam);
	TheShaderManager->PrevWorldViewProjMatrix = TheRenderManager->WorldViewProjMatrix;
}

void ShaderManager::LoadEffectSettings() {
	TESObjectCELL* currentCell = Player->parentCell;
	TESWorldSpace* currentWorldSpace = Player->GetWorldSpace();
		//Color
		if (!(scs = TheSettingManager->GetSettingsColoring(currentCell->GetEditorName())))
			if (currentWorldSpace)
				scs = TheSettingManager->GetSettingsColoring(currentWorldSpace->GetEditorName());

		if (!scs) scs = TheSettingManager->GetSettingsColoring("Default");

		//Water
		if (CurrentBlend == 0.25f)
			sws = TheSettingManager->GetSettingsWater("Blood");
		else if (CurrentBlend == 0.50f)
			sws = TheSettingManager->GetSettingsWater("Lava");
		else
			if (!(sws = TheSettingManager->GetSettingsWater(currentCell->GetEditorName())))
				if (currentWorldSpace) sws = TheSettingManager->GetSettingsWater(currentWorldSpace->GetEditorName());

		if (!sws) sws = TheSettingManager->GetSettingsWater("Default");


		//Bloom
		if (!(sbs = TheSettingManager->GetSettingsBloom(currentCell->GetEditorName())))
			if (currentWorldSpace)
				sbs = TheSettingManager->GetSettingsBloom(currentWorldSpace->GetEditorName());

		if (!sbs) sbs = TheSettingManager->GetSettingsBloom("Default");


		//Ambient Occlusion
		if (currentWorldSpace)
			sas = TheSettingManager->GetSettingsAmbientOcclusion("Exterior");
		else
			sas = TheSettingManager->GetSettingsAmbientOcclusion("Interior");


		//Interior Lighting
		if (currentWorldSpace) {
			//do nothing
		}
		else {
			if (ShaderConst.InteriorLighting.find(currentCell->GetEditorName()) == ShaderConst.InteriorLighting.end()) {
				TESObjectCELL::LightingData* LightData = currentCell->lighting;
				ShaderConstants::SimpleLightingStruct sls;
				sls.r = LightData->ambient.r;
				sls.g = LightData->ambient.g;
				sls.b = LightData->ambient.b;
				sls.a = LightData->ambient.a;
				ShaderConst.InteriorLighting.emplace(currentCell->GetEditorName(), sls);
			}

			InteriorLighting.r = ShaderConst.InteriorLighting[currentCell->GetEditorName()].r;
			InteriorLighting.g = ShaderConst.InteriorLighting[currentCell->GetEditorName()].g;
			InteriorLighting.b = ShaderConst.InteriorLighting[currentCell->GetEditorName()].b;
		}

}
void ShaderManager::SwitchShaderStatus(const char* Name) {
	
	SettingsMainStruct::EffectsStruct* Effects = &TheSettingManager->SettingsMain.Effects;
	SettingsMainStruct::ShadersStruct* Shaders = &TheSettingManager->SettingsMain.Shaders;

	bool Value = false;

	LoadEffectSettings();

	if (!strcmp(Name, "AmbientOcclusion")) {
		Value = !Effects->AmbientOcclusion;
		Effects->AmbientOcclusion = Value;
		DisposeEffect(AmbientOcclusionEffect);
		if (Value) {
			CreateEffect(EffectRecordType_AmbientOcclusion);
		}
	}
	else if (!strcmp(Name, "Blood")) {
		Value = !Shaders->Blood;
		Shaders->Blood = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
	}
	else if (!strcmp(Name, "Bloom")) {
		Value = !Effects->Bloom;
		Effects->Bloom = Value;
		DisposeEffect(BloomEffect);
		if (Value) {
			CreateEffect(EffectRecordType_Bloom);
		}
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
		if (Value) {
			CreateEffect(EffectRecordType_Coloring);
		}
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
		if (Value) {
			CreateEffect(EffectRecordType_GodRays);
		}
	}
	else if (!strcmp(Name, "KhajiitRays")) {
		Value = Effects->KhajiitRays = !Effects->KhajiitRays;
		DisposeEffect(MasserRaysEffect);
		DisposeEffect(SecundaRaysEffect);
		if (Value) {
			CreateEffect(EffectRecordType_KhajiitRays);
		}
	}
	else if (!strcmp(Name, "HDR")) {
		Value = !Shaders->HDR;
		Shaders->HDR = Value;
		DisposeShader(Name);
		if (Value) CreateShader(Name);
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
		if (Value) {
			CreateShader(Name);
			//Have to reload this since it includes PAR shaders
			SwitchShaderStatus("InteriorSpecularShadow");
		}
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
	else if (!strcmp(Name, "SkinVanilla")) {
		DisposeShader("Skin");
		CreateShader("Skin");
	}
	else if (!strcmp(Name, "SMAA")) {
		Value = !Effects->SMAA;
		Effects->SMAA = Value;
		DisposeEffect(SMAAEffect);
		if (Value) CreateEffect(EffectRecordType_SMAA);
	}
	else if (!strcmp(Name, "TAA")) {
		Value = !Effects->TAA;
		Effects->TAA = Value;
		DisposeEffect(TAAEffect);
		if (Value) CreateEffect(EffectRecordType_TAA);
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
	else if (!strcmp(Name, "VolumetricLight")) {
		Value = !Effects->VolumetricLight;
		Effects->VolumetricLight = Value;
		DisposeEffect(VolumetricLightEffect);
		if (Value) CreateEffect(EffectRecordType_VolumetricLight);
	}
	else if (!strcmp(Name, "ShadowsExteriors")) {
		DisposeEffect(ShadowsExteriorsEffect);
		if (TheSettingManager->SettingsShadows.Exteriors.UsePostProcessing) CreateEffect(EffectRecordType_ShadowsExteriors);
	}
	else if (!strcmp(Name, "ShadowsExteriorsPoint")) {
		DisposeEffect(ShadowsExteriorsPointEffect);
		DisposeEffect(ShadowsExteriorsPointDialogEffect);
		if (TheSettingManager->SettingsShadows.ExteriorsPoint.UsePostProcessing) { 
			CreateEffect(EffectRecordType_ShadowsExteriorsPoint);
			CreateEffect(EffectRecordType_ShadowsExteriorsPointDialog);
		}
	}
	else if (!strcmp(Name, "ShadowsInteriors")) {
		DisposeEffect(ShadowsInteriorsEffect);
		if (TheSettingManager->SettingsShadows.Interiors.UsePostProcessing) CreateEffect(EffectRecordType_ShadowsInteriors);
	}
	else if (!strcmp(Name, "InteriorSpecularShadow")) {
		//edge case is when PAR is turned off, this setting will still enable it's associated PAR shaders
		//will have to rethink this approach because additional subsets of shaders will become too difficult to maintain
		if (LocationState == CellLocation::Interior) {
			DisposeShader(Name);
			if (TheSettingManager->SettingsShadows.Interiors.EnableSpecularShadow) {
				CreateShader("InteriorSpecularShadowActive");
			}
			else {
				CreateShader("InteriorSpecularShadowInactive");
			}
		}
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

void ShaderManager::SetPhaseLumCoeff(int phaseLength, int phaseDay) {
	if (phaseDay < phaseLength * 1) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumFull;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumFull;
		return;
	}
	if (phaseDay < phaseLength * 2) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumTQtr;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumTQtr;
		return;
	}
	if (phaseDay < phaseLength * 3) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumHalf;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumHalf;
		return;
	}
	if (phaseDay < phaseLength * 4) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumQtr;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumQtr;
		return;
	}
	if (phaseDay < phaseLength * 5) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumNew;
		ShaderConst.RaysPhaseCoeff.x = 0.0f;
		return;
	}
	if (phaseDay < phaseLength * 6) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumQtr;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumQtr;
		return;
	}
	if (phaseDay < phaseLength * 7) {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumHalf;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumHalf;
		return;
	}
	else {
		ShaderConst.MoonPhaseCoeff = TheSettingManager->SettingsMain.Main.MoonPhaseLumTQtr;
		ShaderConst.RaysPhaseCoeff.x = TheSettingManager->SettingsKhajiitRays.phaseLumTQtr;
		return;
	}
}