#include "ShaderIOHook.h"

#if defined(NEWVEGAS)
#define kCreateVertexShader 0x00BE0FE0
#define kCreatePixelShader 0x00BE1750
#elif defined(OBLIVION)
#define kCreateVertexShader 0x008014E0
#define kCreatePixelShader 0x00801C90
#elif defined(SKYRIM)
#define kCreateVertexShader 0x00CCBB00
#define kCreatePixelShader 0x00CCC420
static char ShaderPrefix[8] = "SHADER";
static int VertexShaderCounter = 0;
static int PixelShaderCounter = 0;
class BSIStream;
#endif

class ShaderIOHook {

public:
#if defined(NEWVEGAS)
	NiD3DVertexShader* TrackCreateVertexShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName);
	NiD3DPixelShader* TrackCreatePixelShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName);
#elif defined(OBLIVION)
	NiD3DVertexShader* TrackCreateVertexShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName, UInt32 Arg5, UInt32 Arg6);
	NiD3DPixelShader* TrackCreatePixelShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName, UInt32 Arg5, UInt32 Arg6);
#elif defined(SKYRIM)
	int TrackCreateVertexShader(BSIStream* ShaderPackage, int Arg2);
	int TrackCreatePixelShader(BSIStream* ShaderPackage, int Arg2);
	bool TrackLoadBloodSplatterShader(BSIStream* ShaderPackage);
	bool TrackLoadDistantTreeShader(BSIStream* ShaderPackage);
	bool TrackLoadGrassShader(BSIStream* ShaderPackage);
	bool TrackLoadParticleShader(BSIStream* ShaderPackage);
	bool TrackLoadSkyShader(BSIStream* ShaderPackage);
	bool TrackLoadEffectShader(BSIStream* ShaderPackage);
	bool TrackLoadLightingShader(BSIStream* ShaderPackage);
	bool TrackLoadUtilityShader(BSIStream* ShaderPackage);
	bool TrackLoadWaterShader(BSIStream* ShaderPackage);
	int TrackLoadImageSpaceShaders();
	int TrackLoadLightingShaderExtra();
#endif

};

#if defined(NEWVEGAS)
NiD3DVertexShader* (__thiscall ShaderIOHook::* CreateVertexShader)(char*, char*, char*, char*);
NiD3DVertexShader* (__thiscall ShaderIOHook::* TrackCreateVertexShader)(char*, char*, char*, char*);
NiD3DVertexShader* ShaderIOHook::TrackCreateVertexShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName) {

	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)(this->*CreateVertexShader)(FileName, Arg2, ShaderType, ShaderName);

	VertexShader->ShaderProg = NULL;
	VertexShader->ShaderHandleBackup = NULL;
	VertexShader->ShaderName = new char[40];
	strcpy(VertexShader->ShaderName, ShaderName);
	if (!memcmp(VertexShader->ShaderName, "ISNOISENORMALMAP", 16)) {
		strcpy(VertexShader->ShaderName, "WATERHEIGHTMAP.vso");
		TheShaderManager->WaterHeightMapVertexShader = VertexShader;
	}
	TheShaderManager->LoadShader(VertexShader);
	return (NiD3DVertexShader*)VertexShader;

}

NiD3DPixelShader* (__thiscall ShaderIOHook::* CreatePixelShader)(char*, char*, char*, char*);
NiD3DPixelShader* (__thiscall ShaderIOHook::* TrackCreatePixelShader)(char*, char*, char*, char*);
NiD3DPixelShader* ShaderIOHook::TrackCreatePixelShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName) {

	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)(this->*CreatePixelShader)(FileName, Arg2, ShaderType, ShaderName);

	PixelShader->ShaderProg = NULL;
	PixelShader->ShaderHandleBackup = NULL;
	PixelShader->ShaderName = new char[40];
	strcpy(PixelShader->ShaderName, ShaderName);
	if (!memcmp(PixelShader->ShaderName, "ISNOISENORMALMAP", 16)) {
		strcpy(PixelShader->ShaderName, "WATERHEIGHTMAP.pso");
		TheShaderManager->WaterHeightMapPixelShader = PixelShader;
	}
	TheShaderManager->LoadShader(PixelShader);
	return (NiD3DPixelShader*)PixelShader;

}
#elif defined(OBLIVION)
NiD3DVertexShader* (__thiscall ShaderIOHook::* CreateVertexShader)(char*, char*, char*, char*, UInt32, UInt32);
NiD3DVertexShader* (__thiscall ShaderIOHook::* TrackCreateVertexShader)(char*, char*, char*, char*, UInt32, UInt32);
NiD3DVertexShader* ShaderIOHook::TrackCreateVertexShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName, UInt32 Arg5, UInt32 Arg6) {

	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)(this->*CreateVertexShader)(FileName, Arg2, ShaderType, ShaderName, Arg5, Arg6);

	VertexShader->ShaderProg = NULL;
	VertexShader->ShaderHandleBackup = NULL;
	VertexShader->ShaderName = new char[24];
	strcpy(VertexShader->ShaderName, ShaderName);
	TheShaderManager->LoadShader(VertexShader);
	return (NiD3DVertexShader*)VertexShader;

}

NiD3DPixelShader* (__thiscall ShaderIOHook::* CreatePixelShader)(char*, char*, char*, char*, UInt32, UInt32);
NiD3DPixelShader* (__thiscall ShaderIOHook::* TrackCreatePixelShader)(char*, char*, char*, char*, UInt32, UInt32);
NiD3DPixelShader* ShaderIOHook::TrackCreatePixelShader(char* FileName, char* Arg2, char* ShaderType, char* ShaderName, UInt32 Arg5, UInt32 Arg6) {

	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)(this->*CreatePixelShader)(FileName, Arg2, ShaderType, ShaderName, Arg5, Arg6);

	PixelShader->ShaderProg = NULL;
	PixelShader->ShaderHandleBackup = NULL;
	PixelShader->ShaderName = new char[24];
	strcpy(PixelShader->ShaderName, ShaderName);
	TheShaderManager->LoadShader(PixelShader);
	return (NiD3DPixelShader*)PixelShader;

}
#elif defined(SKYRIM)
int (__thiscall ShaderIOHook::* CreateVertexShader)(BSIStream*, int);
int (__thiscall ShaderIOHook::* TrackCreateVertexShader)(BSIStream*, int);
int ShaderIOHook::TrackCreateVertexShader(BSIStream* ShaderPackage, int Arg2) {
	
	int r = (this->*CreateVertexShader)(ShaderPackage, Arg2);
	NiD3DVertexShaderEx* VertexShader = (NiD3DVertexShaderEx*)this;
	char Counter[8];

	VertexShader->ShaderProg = NULL;
	VertexShader->ShaderHandleBackup = NULL;
	VertexShader->ShaderName = new char[16];
	VertexShaderCounter += 1;
	_itoa(VertexShaderCounter, Counter, 10);
	strcpy(VertexShader->ShaderName, ShaderPrefix);
	strcat(VertexShader->ShaderName, Counter);
	strcat(VertexShader->ShaderName, ".vso");
	TheShaderManager->LoadShader(VertexShader);
	if (!memcmp(VertexShader->ShaderName, "WATER", 5)) TheShaderManager->WaterVertexShaders[VertexShaderCounter - 1] = VertexShader;
	return r;

}

int (__thiscall ShaderIOHook::* CreatePixelShader)(BSIStream*, int);
int (__thiscall ShaderIOHook::* TrackCreatePixelShader)(BSIStream*, int);
int ShaderIOHook::TrackCreatePixelShader(BSIStream* ShaderPackage, int Arg2) {

	int r = (this->*CreatePixelShader)(ShaderPackage, Arg2);
	NiD3DPixelShaderEx* PixelShader = (NiD3DPixelShaderEx*)this;
	char Counter[8];
	
	PixelShader->ShaderProg = NULL;
	PixelShader->ShaderHandleBackup = NULL;
	PixelShader->ShaderName = new char[16];
	PixelShaderCounter += 1;
	_itoa(PixelShaderCounter, Counter, 10);
	strcpy(PixelShader->ShaderName, ShaderPrefix);
	strcat(PixelShader->ShaderName, Counter);
	strcat(PixelShader->ShaderName, ".pso");
	TheShaderManager->LoadShader(PixelShader);
	if (!memcmp(PixelShader->ShaderName, "WATER", 5)) TheShaderManager->WaterPixelShaders[PixelShaderCounter - 1] = PixelShader;
	return r;

}

bool (__thiscall ShaderIOHook::* LoadBloodSplatterShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadBloodSplatterShader)(BSIStream*);
bool ShaderIOHook::TrackLoadBloodSplatterShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "BLOOD");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadBloodSplatterShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadDistantTreeShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadDistantTreeShader)(BSIStream*);
bool ShaderIOHook::TrackLoadDistantTreeShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "DTREE");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadDistantTreeShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadGrassShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadGrassShader)(BSIStream*);
bool ShaderIOHook::TrackLoadGrassShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "GRASS");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadGrassShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadParticleShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadParticleShader)(BSIStream*);
bool ShaderIOHook::TrackLoadParticleShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "PARTIC");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadParticleShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadSkyShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadSkyShader)(BSIStream*);
bool ShaderIOHook::TrackLoadSkyShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "SKY");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadSkyShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadEffectShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadEffectShader)(BSIStream*);
bool ShaderIOHook::TrackLoadEffectShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "EFFECT");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadEffectShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadLightingShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadLightingShader)(BSIStream*);
bool ShaderIOHook::TrackLoadLightingShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "LIGHT");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadLightingShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadUtilityShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadUtilityShader)(BSIStream*);
bool ShaderIOHook::TrackLoadUtilityShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "UTIL");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadUtilityShader)(ShaderPackage);

}

bool (__thiscall ShaderIOHook::* LoadWaterShader)(BSIStream*);
bool (__thiscall ShaderIOHook::* TrackLoadWaterShader)(BSIStream*);
bool ShaderIOHook::TrackLoadWaterShader(BSIStream* ShaderPackage) {

	strcpy(ShaderPrefix, "WATER");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadWaterShader)(ShaderPackage);

}

int (__thiscall ShaderIOHook::* LoadImageSpaceShaders)();
int (__thiscall ShaderIOHook::* TrackLoadImageSpaceShaders)();
int ShaderIOHook::TrackLoadImageSpaceShaders() {

	strcpy(ShaderPrefix, "IMAGE");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadImageSpaceShaders)();

}

int (__thiscall ShaderIOHook::* LoadLightingShaderExtra)();
int (__thiscall ShaderIOHook::* TrackLoadLightingShaderExtra)();
int ShaderIOHook::TrackLoadLightingShaderExtra() {

	strcpy(ShaderPrefix, "LIGHTE");
	VertexShaderCounter = 0;
	PixelShaderCounter = 0;
	return (this->*LoadLightingShaderExtra)();

}
#endif

void CreateShaderIOHook() {

	*((int *)&CreateVertexShader)		= kCreateVertexShader;
	TrackCreateVertexShader				= &ShaderIOHook::TrackCreateVertexShader;
	*((int *)&CreatePixelShader)		= kCreatePixelShader;
	TrackCreatePixelShader				= &ShaderIOHook::TrackCreatePixelShader;
#if defined(SKYRIM)
	*((int *)&LoadBloodSplatterShader)	= 0x00C9D0D0;
	TrackLoadBloodSplatterShader		= &ShaderIOHook::TrackLoadBloodSplatterShader;
	*((int *)&LoadDistantTreeShader)	= 0x00CAE6C0;
	TrackLoadDistantTreeShader			= &ShaderIOHook::TrackLoadDistantTreeShader;
	*((int *)&LoadGrassShader)			= 0x00C95FC0;
	TrackLoadGrassShader				= &ShaderIOHook::TrackLoadGrassShader;
	*((int *)&LoadParticleShader)		= 0x00CC20C0;
	TrackLoadParticleShader				= &ShaderIOHook::TrackLoadParticleShader;
	*((int *)&LoadSkyShader)			= 0x00CA9AE0;
	TrackLoadSkyShader					= &ShaderIOHook::TrackLoadSkyShader;
	*((int *)&LoadEffectShader)			= 0x00CAD980;
	TrackLoadEffectShader				= &ShaderIOHook::TrackLoadEffectShader;
	*((int *)&LoadLightingShader)		= 0x00CA2090;
	TrackLoadLightingShader				= &ShaderIOHook::TrackLoadLightingShader;
	*((int *)&LoadUtilityShader)		= 0x00CB7E60;
	TrackLoadUtilityShader				= &ShaderIOHook::TrackLoadUtilityShader;
	*((int *)&LoadWaterShader)			= 0x00CB38D0;
	TrackLoadWaterShader				= &ShaderIOHook::TrackLoadWaterShader;
	*((int *)&LoadImageSpaceShaders)	= 0x00C7CC10;
	TrackLoadImageSpaceShaders			= &ShaderIOHook::TrackLoadImageSpaceShaders;
	*((int *)&LoadLightingShaderExtra)	= 0x00CA38D0;
	TrackLoadLightingShaderExtra		= &ShaderIOHook::TrackLoadLightingShaderExtra;
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)CreateVertexShader,		*((PVOID *)&TrackCreateVertexShader));
	DetourAttach(&(PVOID&)CreatePixelShader,		*((PVOID *)&TrackCreatePixelShader));
#if defined (SKYRIM)
	DetourAttach(&(PVOID&)LoadBloodSplatterShader,	*((PVOID *)&TrackLoadBloodSplatterShader));
	DetourAttach(&(PVOID&)LoadDistantTreeShader,	*((PVOID *)&TrackLoadDistantTreeShader));
	DetourAttach(&(PVOID&)LoadGrassShader,			*((PVOID *)&TrackLoadGrassShader));
	DetourAttach(&(PVOID&)LoadParticleShader,		*((PVOID *)&TrackLoadParticleShader));
	DetourAttach(&(PVOID&)LoadSkyShader,			*((PVOID *)&TrackLoadSkyShader));
	DetourAttach(&(PVOID&)LoadEffectShader,			*((PVOID *)&TrackLoadEffectShader));
	DetourAttach(&(PVOID&)LoadLightingShader,		*((PVOID *)&TrackLoadLightingShader));
	DetourAttach(&(PVOID&)LoadUtilityShader,		*((PVOID *)&TrackLoadUtilityShader));
	DetourAttach(&(PVOID&)LoadWaterShader,			*((PVOID *)&TrackLoadWaterShader));
	DetourAttach(&(PVOID&)LoadImageSpaceShaders,	*((PVOID *)&TrackLoadImageSpaceShaders));
	DetourAttach(&(PVOID&)LoadLightingShaderExtra,	*((PVOID *)&TrackLoadLightingShaderExtra));
#endif
	DetourTransactionCommit();

#if defined(NEWVEGAS)
	SafeWrite8(0x00BE1690, 0x48);	// Extends the NiD3DVertexShader allocation from size 0x3C to 0x48 to store additional data
	SafeWrite8(0x00BE1DFB, 0x3C);	// Extends the NiD3DPixelShader allocation from size 0x30 to 0x3C to store additional data
#elif defined(OBLIVION)
	SafeWrite8(0x00801BCB, 0x44);	// Extends the NiD3DVertexShader allocation from size 0x38 to 0x44 to store additional data
	SafeWrite8(0x008023A1, 0x38);	// Extends the NiD3DPixelShader allocation from size 0x2C to 0x38 to store additional data
#elif defined(SKYRIM)
	SafeWrite8(0x00CCBD66, 0x40);	// Extends the NiD3DVertexShader allocation from size 0x34 to 0x40 to store additional data
	SafeWrite8(0x00CCC676, 0x70);	// Extends the NiD3DPixelShader allocation from size 0x64 to 0x70 to store additional data
#endif

}