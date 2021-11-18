#pragma once
#define SamplerStatesMax 12
#define WordSourceBuffer "TESR_SourceBuffer"
#define WordRenderedBuffer "TESR_RenderedBuffer"
#define WordDepthBuffer "TESR_DepthBuffer"
#define WordShadowMapBufferNear "TESR_ShadowMapBufferNear"
#define WordShadowMapBufferFar "TESR_ShadowMapBufferFar"
#define WordShadowMapBufferSkin "TESR_ShadowMapBufferSkin"
#define WordOrthoMapBuffer "TESR_OrthoMapBuffer"
#define WordShadowCubeMapBuffer0 "TESR_ShadowCubeMapBuffer0"
#define WordShadowCubeMapBuffer1 "TESR_ShadowCubeMapBuffer1"
#define WordShadowCubeMapBuffer2 "TESR_ShadowCubeMapBuffer2"
#define WordShadowCubeMapBuffer3 "TESR_ShadowCubeMapBuffer3"
#define WordShadowCubeMapBuffer4 "TESR_ShadowCubeMapBuffer4"
#define WordShadowCubeMapBuffer5 "TESR_ShadowCubeMapBuffer5"
#define WordShadowCubeMapBuffer6 "TESR_ShadowCubeMapBuffer6"
#define WordShadowCubeMapBuffer7 "TESR_ShadowCubeMapBuffer7"
#define WordShadowCubeMapBuffer8 "TESR_ShadowCubeMapBuffer8"
#define WordShadowCubeMapBuffer9 "TESR_ShadowCubeMapBuffer9"
#define WordShadowCubeMapBuffer10 "TESR_ShadowCubeMapBuffer10"
#define WordShadowCubeMapBuffer11 "TESR_ShadowCubeMapBuffer11"

enum TextureRecordType {
	TextureRecordType_None,
	TextureRecordType_PlanarBuffer,
	TextureRecordType_VolumeBuffer,
	TextureRecordType_CubeBuffer,
	TextureRecordType_SourceBuffer,
	TextureRecordType_RenderedBuffer,
	TextureRecordType_DepthBuffer,
	TextureRecordType_ShadowMapBufferNear,
	TextureRecordType_ShadowMapBufferFar,
	TextureRecordType_ShadowMapBufferSkin,
	TextureRecordType_OrthoMapBuffer,
	TextureRecordType_ShadowCubeMapBuffer0,
	TextureRecordType_ShadowCubeMapBuffer1,
	TextureRecordType_ShadowCubeMapBuffer2,
	TextureRecordType_ShadowCubeMapBuffer3,
	TextureRecordType_ShadowCubeMapBuffer4,
	TextureRecordType_ShadowCubeMapBuffer5,
	TextureRecordType_ShadowCubeMapBuffer6,
	TextureRecordType_ShadowCubeMapBuffer7,
	TextureRecordType_ShadowCubeMapBuffer8,
	TextureRecordType_ShadowCubeMapBuffer9,
	TextureRecordType_ShadowCubeMapBuffer10,
	TextureRecordType_ShadowCubeMapBuffer11,
	TextureRecordType_Max,
};

class TextureRecord
{
public:
	TextureRecord();

	bool					LoadTexture(TextureRecordType Type, const char* Filename);
	void					SetSamplerState(D3DSAMPLERSTATETYPE SamplerType, DWORD Value);

	IDirect3DBaseTexture9*	Texture;
	DWORD					SamplerStates[SamplerStatesMax];

};

typedef std::map<std::string, TextureRecord*> TextureList;

class TextureManager // Never disposed
{
public:
	TextureManager();

	TextureRecord*			LoadTexture(const char* ShaderSource, UInt32 RegisterIndex);
	
	TextureList				Textures;
};