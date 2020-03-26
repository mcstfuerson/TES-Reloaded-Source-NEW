#define HookD3DDevice 0
#define WaitForDebugger 0

#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#include "PluginVersion.h"

#ifdef _DEBUG
#if HookD3DDevice
#include "D3D9Hook.h"
#endif
#endif

extern "C" {

	bool SKSEPlugin_Query(const PluginInterface* Interface, PluginInfo* Info) {
		
		Info->InfoVersion = PluginInfo::kInfoVersion;
		Info->Name = "SkyrimReloaded";
		Info->Version = 4;
		return true;

	}

	bool SKSEPlugin_Load(const PluginInterface* Interface) {

#ifdef _DEBUG
#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
#endif
#if HookD3DDevice
		if (!Interface->IsEditor) CreateD3D9Hook();
#endif
#endif
		Logger::CreateLog("SkyrimReloaded.log");
		new CommandManager();
		TheCommandManager->AddCommands(Interface);

		if (!Interface->IsEditor) {
			PluginVersion::CreateVersionString();
			new SettingManager();

			TheSettingManager->LoadSettings();
			
			PerformGameInitialization();
			CreateShaderIOHook();
			CreateRenderHook();
			CreateFormLoadHook();
			CreateSettingsHook();
			CreateGameEventHook();
			if (TheSettingManager->SettingsMain.CameraMode.Enabled) CreateCameraModeHook();
			if (TheSettingManager->SettingsMain.SleepingMode.Enabled) CreateSleepingModeHook();
		}
		return true;

	}

};