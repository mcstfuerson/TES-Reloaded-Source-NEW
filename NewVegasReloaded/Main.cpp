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
#include "D3D9Hook.h"

extern "C" {

	bool NVSEPlugin_Query(const PluginInterface* Interface, PluginInfo* Info) {
		
		Info->InfoVersion = PluginInfo::kInfoVersion;
		Info->Name = "NewVegasReloaded";
		Info->Version = 3;
		return true;

	}

	bool NVSEPlugin_Load(const PluginInterface* Interface) {

#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
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
			if (TheSettingManager->SettingsMain.Develop.LogShaders) CreateD3D9Hook();

			SafeWrite16(0x0086A170, 0x9090);		// Avoids to pause the game when ALT-TAB
		}
		return true;

	}

};