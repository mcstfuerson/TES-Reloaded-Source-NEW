#define HookD3DDevice 0
#define WaitForDebugger 0

#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "CameraMode.h"
#include "SleepingMode.h"
#include "WeatherMode.h"
#include "FlyCam.h"
#include "PluginVersion.h"
#include "MemoryManagement.h"

#ifdef _DEBUG
#if HookD3DDevice
#include "D3D9Hook.h"
#endif
#endif

extern "C" {

	bool NVSEPlugin_Query(const PluginInterface* Interface, PluginInfo* Info) {
		
		Info->InfoVersion = PluginInfo::kInfoVersion;
		Info->Name = "NewVegasReloaded";
		Info->Version = 3;
		return true;

	}

	bool NVSEPlugin_Load(const PluginInterface* Interface) {

#ifdef _DEBUG
#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
#endif
#if HookD3DDevice
		if (!Interface->IsEditor) CreateD3D9Hook();
#endif
#endif
		Logger::CreateLog("NewVegasReloaded.log");
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
			CreateShadowsHook();
			if (TheSettingManager->SettingsMain.Main.MemoryManagement) CreateMemoryManagementHook();
			if (TheSettingManager->SettingsMain.CameraMode.Enabled) CreateCameraModeHook();
			if (TheSettingManager->SettingsMain.SleepingMode.Enabled) CreateSleepingModeHook();
			if (TheSettingManager->SettingsMain.FlyCam.Enabled) CreateFlyCamHook();
			if (TheSettingManager->SettingsMain.WeatherMode.Enabled) CreateWeatherModeHook();

			SafeWrite16(0x0086A170, 0x9090);		// Avoids to pause the game when ALT-TAB
		}
		return true;

	}

};