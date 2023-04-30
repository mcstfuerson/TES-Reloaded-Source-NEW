#define WaitForDebugger 0

#include "RenderHook.h"
#include "ShaderIOHook.h"
#include "FormHook.h"
#include "WeatherMode.h"
#include "PluginVersion.h"
#include "MemoryManagement.h"
#include "D3D9Hook.h"

extern "C" {

	bool OBSEPlugin_Query(const PluginInterface* Interface, PluginInfo* Info) {
		
		Info->InfoVersion = PluginInfo::kInfoVersion;
		Info->Name = "OblivionReloaded E3";
		Info->Version = 301;
		return true;

	}

	bool OBSEPlugin_Load(const PluginInterface* Interface) {

#if WaitForDebugger
		while (!IsDebuggerPresent()) Sleep(10);
#endif
		Logger::CreateLog("OblivionReloaded.log");
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
			CreateScriptHook();
			CreateShadowsHook();
			CreateWeatherModeHook();
			if (TheSettingManager->SettingsMain.Main.MemoryManagement) CreateMemoryManagementHook();

			//TODO: Add additonal setting for Moon phase luminance to toggle this
			//if (TheSettingManager->SettingsMain.WeatherMode.Enabled) CreateWeatherModeHook();
			if (TheSettingManager->SettingsMain.Develop.LogShaders) CreateD3D9Hook();

			WriteRelJump(0x0049849A, 0x004984A0); // Skips antialiasing deactivation if HDR is enabled on the D3DDevice
			WriteRelJump(0x004984BD, 0x004984CD); // Skips antialiasing deactivation if AllowScreenshot is enabled
			WriteRelJump(0x005DEE60, 0x005DEE68); // Skips antialiasing deactivation if HDR is enabled on loading the video menu
			WriteRelJump(0x005DF69E, 0x005DF755); // Skips HDR deactivation changing antialising (video menu)
			WriteRelJump(0x00497D5A, 0x00497D63); // Unlocks antialising bar if HDR is enabled (video menu)
			WriteRelJump(0x005DF8E9, 0x005DF983); // Skips antialising deactivation changing HDR (video menu)
			WriteRelJump(0x006738B1, 0x00673935); // Cancels the fPlayerDeathReloadTime
		}
		else {
			CreateEditorShadowsHook();
		}
		return true;

	}

};