#include "FrameRateManager.h"

static const UInt32 kUpdateTimeInfoHook = 0x0040D8AB;
static const UInt32 kUpdateTimeInfoReturn = 0x0040D8B0;
static float* MPF = (float*)0x00B33E94;

FrameRateManager::FrameRateManager() {

	Logger::Log("Starting the framerate manager...");
	TheFrameRateManager = this;

	LARGE_INTEGER Frequency;
	LARGE_INTEGER PerformanceCounter;

	Time = 0.0;
	LastTime = 0.0;
	ElapsedTime = 0.0;
	LastPerformance = 0;
	SmartControlMPF = 1000.0 / TheSettingManager->SettingsMain.FrameRate.SmartControlFPS;
	QueryPerformanceFrequency(&Frequency); PerformanceFrequency = Frequency.QuadPart;
	QueryPerformanceCounter(&PerformanceCounter); PerformanceCounterStart = PerformanceCounter.QuadPart;

}

void FrameRateManager::UpdatePerformance() {

	LARGE_INTEGER PerformanceCounterEnd;

	QueryPerformanceCounter(&PerformanceCounterEnd);
	Time = (double)(PerformanceCounterEnd.QuadPart - PerformanceCounterStart) / (double)PerformanceFrequency;
	ElapsedTime = Time - LastTime;
	LastTime = Time;

}

double FrameRateManager::GetPerformance() {

	LARGE_INTEGER PerformanceCounterEnd;

	QueryPerformanceCounter(&PerformanceCounterEnd);
	return (double)(PerformanceCounterEnd.QuadPart - TheFrameRateManager->PerformanceCounterStart) * 1000.0 / (double)TheFrameRateManager->PerformanceFrequency;

}

void FrameRateManager::PerformSync() {

	double CurrentPerformance = GetPerformance();
	double FrameTime = CurrentPerformance - LastPerformance;

	if (FrameTime < SmartControlMPF) Sleep(SmartControlMPF - FrameTime);
	LastPerformance = GetPerformance();
	*MPF = (float)(FrameTime + TheSettingManager->SettingsMain.FrameRate.FlowControl + (LastPerformance - CurrentPerformance));

}

static __declspec(naked) void UpdateTimeInfoHook() {

	__asm {
		pushad
		mov		ecx, TheFrameRateManager
		call	FrameRateManager::PerformSync
		popad
		mov		eax, 0x0047D170
		call	eax
		jmp		kUpdateTimeInfoReturn
	}

}

void EndProcess() {

	void* SettingCollection = (void*)0x00B07BF0;
	char* SettingName = (char*)0x00B07BF4;

	ThisCall(0x0040C180, SettingCollection, SettingName);
	TerminateProcess(GetCurrentProcess(), 0);

}

void CreateFrameRateHook() {

	SettingsMainStruct::FrameRateStruct* FrameRate = &TheSettingManager->SettingsMain.FrameRate;

	if (FrameRate->SmartControl) WriteRelJump(kUpdateTimeInfoHook, (UInt32)UpdateTimeInfoHook);
	if (FrameRate->SmartBackgroundProcess) {
		WriteRelJump(0x00701739, 0x00701748); // Skips init	 SourceDataCriticalSection (NiRenderer::New)
		WriteRelJump(0x00763565, 0x0076357F); // Skips entering SourceDataCriticalSection (NiRenderer::CreateSourceTextureRendererData)
		WriteRelJump(0x00763596, 0x007635AA); // Skips leaving	 SourceDataCriticalSection (NiRenderer::CreateSourceTextureRendererData)
		WriteRelJump(0x007635C4, 0x007635DE); // Skips entering SourceDataCriticalSection (NiRenderer::CreateSourceCubeMapRendererData)
		WriteRelJump(0x007635FB, 0x0076360F); // Skips leaving	 SourceDataCriticalSection (NiRenderer::CreateSourceCubeMapRendererData)
		SafeWrite8(0x007635F9, 0x8B); // Patches the NiRenderer::CreateSourceCubeMapRendererData
		SafeWrite8(0x007635FA, 0xF8); // Patches the NiRenderer::CreateSourceCubeMapRendererData
		WriteRelJump(0x0077ACBF, 0x0077ACD9); // Skips entering SourceDataCriticalSection (NiDX9TextureManager::PrepareTextureForRendering)
		WriteRelJump(0x0077AD08, 0x0077AD1C); // Skips leaving	 SourceDataCriticalSection (NiDX9TextureManager::PrepareTextureForRendering)
		WriteRelJump(0x0077AD52, 0x0077AD66); // Skips leaving	 SourceDataCriticalSection (NiDX9TextureManager::PrepareTextureForRendering)
		WriteRelJump(0x0077AE2A, 0x0077AE44); // Skips entering SourceDataCriticalSection (NiDX9TextureManager::PrecacheTexture)
		WriteRelJump(0x0077AE6C, 0x0077AE80); // Skips leaving	 SourceDataCriticalSection (NiDX9TextureManager::PrecacheTexture)
		WriteRelJump(0x0077AEAF, 0x0077AEC3); // Skips leaving	 SourceDataCriticalSection (NiDX9TextureManager::PrecacheTexture)
		WriteRelJump(0x007D6AC5, 0x007D6ACA); // Skips entering RendererLockCriticalSection (NiRenderer::BeginRenderTargetGroup)
		WriteRelJump(0x007D6AEF, 0x007D6AF6); // Skips leaving  RendererLockCriticalSection (NiRenderer::BeginRenderTargetGroup)
		WriteRelJump(0x007D6B30, 0x007D6B62); // Skips leaving  RendererLockCriticalSection (NiRenderer::EndRenderTargetGroup)
		SafeWrite32(0x004335ED, 1000000); // Allows to manage less time between threads
		SafeWrite8(0x004344AB, 0x78); // Sets threads in the BSTaskManager
		SafeWrite8(0x004344AF, 0x0A); // Sets threads in the BSTaskManager
	}
	WriteRelJump(0x0040F488, (UInt32)EndProcess);

}