#pragma once

#include <sstream>
#include <fstream>
#include <algorithm>
#include "WindowedMode.h"
#include "WeatherMode.h"

#if defined(NEWVEGAS)
#define kSaveSettings 0x0044F950
#define kReadSetting 0x00C44280
#define kLoadGame 0x00847DF0
#define WeatherFogFarDay fogDay.farFog
#define WeatherFogFarNight fogNight.farFog
#define WeatherFogNearDay fogDay.nearFog
#define WeatherFogNearNight fogNight.nearFog
#elif defined(OBLIVION)
#define kSaveSettings 0x004A8560
#define kReadSetting 0x004A8800
#define kLoadGame 0x00465860
#define WeatherFogFarDay fogDay.farFog
#define WeatherFogFarNight fogNight.farFog
#define WeatherFogNearDay fogDay.nearFog
#define WeatherFogNearNight fogNight.nearFog
#elif defined(SKYRIM)
#define kSaveSettings 0x0042CF80
#define kReadSetting 0x00AFED60
#define kLoadGame 0x0067B720
#define cloudSpeedLower general.cloudSpeedLower
#define cloudSpeedUpper general.cloudSpeedUpper
#define sunDamage general.sunDamage
#define sunGlare general.sunGlare
#define transDelta general.transDelta
#define windSpeed general.windSpeed
#define WeatherFogFarDay fogDistance.farDay
#define WeatherFogFarNight fogDistance.farNight
#define WeatherFogNearDay fogDistance.nearDay
#define WeatherFogNearNight fogDistance.nearNight
#endif

template <typename T> std::string ToString(const T& Value) {

	std::ostringstream os;

	os << (float)Value;
	return os.str();

}

void SplitString(const std::string& S, char D, std::vector<std::string>& Values) {

	std::size_t current, previous = 0;
	
	Values.clear();
	current = S.find(D);
	while (current != std::string::npos) {
		Values.push_back(S.substr(previous, current - previous));
		previous = current + 1;
		current = S.find(D, previous);
	}
	Values.push_back(S.substr(previous, current - previous));

}

SettingManager::SettingManager() {
	
	char* Filename = SettingsMain.Main.MainFile;
	char* FilenameWeather = SettingsMain.Main.WeatherFile;
	char value[SettingStringBuffer];
	char* pNextValue = NULL;

	Logger::Log("Starting the settings manager...");
	TheSettingManager = this;

	GetCurrentDirectoryA(MAX_PATH, CurrentPath);
	strcpy(Filename, CurrentPath);
	strcat(Filename, MainSettingsFile);
	strcpy(FilenameWeather, CurrentPath);
	strcat(FilenameWeather, WeatherSettingsFile);

	GetPrivateProfileStringA("Main", "FoV", "90.0", value, SettingStringBuffer, Filename);
	SettingsMain.Main.FoV = atof(value);
	SettingsMain.Main.WaterReflectionMapSize = GetPrivateProfileIntA("Main", "WaterReflectionMapSize", 512, Filename);
	SettingsMain.Main.RemoveUnderwater = GetPrivateProfileIntA("Main", "RemoveUnderwater", 1, Filename);
	SettingsMain.Main.RemovePrecipitations = GetPrivateProfileIntA("Main", "RemovePrecipitations", 0, Filename);
	SettingsMain.Main.MemoryManagement = GetPrivateProfileIntA("Main", "MemoryManagement", 0, Filename);
	SettingsMain.Main.AnisotropicFilter = GetPrivateProfileIntA("Main", "AnisotropicFilter", 0, Filename);
	GetPrivateProfileStringA("Main", "FarPlaneDistance", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.Main.FarPlaneDistance = atof(value);
	GetPrivateProfileStringA("Main", "ScreenshotPath", CurrentPath, value, SettingStringBuffer, Filename);
	if (value[0] == '\\') {
		strcpy(SettingsMain.Main.ScreenshotPath, CurrentPath);
		strcat(SettingsMain.Main.ScreenshotPath, value);
	}
	else {
		strcpy(SettingsMain.Main.ScreenshotPath, value);
	}
	if (SettingsMain.Main.ScreenshotPath[strlen(SettingsMain.Main.ScreenshotPath) - 1] != '\\') strcat(SettingsMain.Main.ScreenshotPath, "\\");
	SettingsMain.Main.ScreenshotType = GetPrivateProfileIntA("Main", "ScreenshotType", 1, Filename);
	SettingsMain.Main.ScreenshotKey = GetPrivateProfileIntA("Main", "ScreenshotKey", 87, Filename);
	SettingsMain.Main.FPSOverlay = GetPrivateProfileIntA("Main", "FPSOverlay", 0, Filename);
	SettingsMain.Main.DirectionalLightOverride = GetPrivateProfileIntA("Main", "DirectionalLightOverride", 0, Filename);
	SettingsMain.Main.RenderEffectsBeforeHdr = GetPrivateProfileIntA("Main", "RenderEffectsBeforeHdr", 0, Filename);
	GetPrivateProfileStringA("Main", "MoonPhaseLumNew", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.Main.MoonPhaseLumNew = atof(value);
	GetPrivateProfileStringA("Main", "MoonPhaseLumQtr", "0.25", value, SettingStringBuffer, Filename);
	SettingsMain.Main.MoonPhaseLumQtr = atof(value);
	GetPrivateProfileStringA("Main", "MoonPhaseLumHalf", "0.50", value, SettingStringBuffer, Filename);
	SettingsMain.Main.MoonPhaseLumHalf = atof(value);
	GetPrivateProfileStringA("Main", "MoonPhaseLumTQtr", "0.75", value, SettingStringBuffer, Filename);
	SettingsMain.Main.MoonPhaseLumTQtr = atof(value);
	GetPrivateProfileStringA("Main", "MoonPhaseLumFull", "1.00", value, SettingStringBuffer, Filename);
	SettingsMain.Main.MoonPhaseLumFull = atof(value);
	GetPrivateProfileStringA("Main", "InteriorDimmerCoeff", "1.00", value, SettingStringBuffer, Filename);
	SettingsMain.Main.InteriorDimmerCoeff = atof(value);
	SettingsMain.Main.SaveSettings = GetPrivateProfileIntA("Main", "SaveSettings", 1, Filename);
	SettingsMain.Main.ReplaceIntro = GetPrivateProfileIntA("Main", "ReplaceIntro", 0, Filename);	

	SettingsMain.EquipmentMode.Enabled = GetPrivateProfileIntA("EquipmentMode", "Enabled", 0, Filename);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "ShieldOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.ShieldOnBackRot.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "WeaponOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.WeaponOnBackRot.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TwoHandWeaponOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TwoHandWeaponOnBackRot.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "BowOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.BowOnBackRot.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "StaffOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.StaffOnBackRot.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltPos.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltPos.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltPos.z = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltRot.x = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltRot.y = atof(value);
	GetPrivateProfileStringA("EquipmentMode", "TorchOnBeltRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.TorchOnBeltRot.z = atof(value);
	SettingsMain.EquipmentMode.TorchKey = GetPrivateProfileIntA("EquipmentMode", "TorchKey", 21, Filename);
	SettingsMain.EquipmentMode.SleepingEquipment = GetPrivateProfileIntA("EquipmentMode", "SleepingEquipment", 1, Filename);
	SettingsMain.EquipmentMode.SwimmingEquipment = GetPrivateProfileIntA("EquipmentMode", "SwimmingEquipment", 1, Filename);
	SettingsMain.EquipmentMode.CombatEquipmentKey = GetPrivateProfileIntA("EquipmentMode", "CombatEquipmentKey", 259, Filename);
	GetPrivateProfileStringA("EquipmentMode", "DualBlockDelay", "0.15", value, SettingStringBuffer, Filename);
	SettingsMain.EquipmentMode.DualBlockDelay = atof(value);
	
	SettingsMain.GrassMode.Enabled = GetPrivateProfileIntA("GrassMode", "Enabled", 0, Filename);

	SettingsMain.CameraMode.Enabled = GetPrivateProfileIntA("CameraMode", "Enabled", 0, Filename);
	SettingsMain.CameraMode.Crosshair = GetPrivateProfileIntA("CameraMode", "Crosshair", 2, Filename);
	SettingsMain.CameraMode.ChasingFirst = GetPrivateProfileIntA("CameraMode", "ChasingFirst", 0, Filename);
	SettingsMain.CameraMode.ChasingThird = GetPrivateProfileIntA("CameraMode", "ChasingThird", 0, Filename);
	GetPrivateProfileStringA("CameraMode", "OffsetX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.Offset.x = atof(value);
	GetPrivateProfileStringA("CameraMode", "OffsetY", "16.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.Offset.y = atof(value);
	GetPrivateProfileStringA("CameraMode", "OffsetZ", "3.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.Offset.z = atof(value);
	GetPrivateProfileStringA("CameraMode", "DialogOffsetX", "50.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.DialogOffset.x = atof(value);
	GetPrivateProfileStringA("CameraMode", "DialogOffsetY", "-10.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.DialogOffset.y = atof(value);
	GetPrivateProfileStringA("CameraMode", "DialogOffsetZ", "10.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.DialogOffset.z = atof(value);
	GetPrivateProfileStringA("CameraMode", "NearDistanceFirst", "2.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.NearDistanceFirst = atof(value);
	GetPrivateProfileStringA("CameraMode", "NearDistanceThird", "8.0", value, SettingStringBuffer, Filename);
	SettingsMain.CameraMode.NearDistanceThird = atof(value);

	SettingsMain.MountedCombat.Enabled = GetPrivateProfileIntA("MountedCombat", "Enabled", 0, Filename);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackPos.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackPos.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackPos.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackRot.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackRot.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "WeaponOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.WeaponOnBackRot.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "TwoHandWeaponOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackPos.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackPos.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackPos.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackRot.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackRot.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "BowOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.BowOnBackRot.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackPosX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackPos.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackPosY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackPos.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackPosZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackPos.z = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackRotX", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackRot.x = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackRotY", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackRot.y = atof(value);
	GetPrivateProfileStringA("MountedCombat", "StaffOnBackRotZ", "0.0", value, SettingStringBuffer, Filename);
	SettingsMain.MountedCombat.StaffOnBackRot.z = atof(value);

	SettingsMain.SleepingMode.Enabled = GetPrivateProfileIntA("SleepingMode", "Enabled", 0, Filename);
	SettingsMain.SleepingMode.Mode = GetPrivateProfileIntA("SleepingMode", "Mode", 3, Filename);

	SettingsMain.WeatherMode.Enabled = GetPrivateProfileIntA("WeatherMode", "Enabled", 0, Filename);
	GetPrivateProfileStringA("WeatherMode", "CoeffNightR", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffNight.x = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffNightG", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffNight.y = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffNightB", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffNight.z = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffFogR", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffFog.x = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffFogG", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffFog.y = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffFogB", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffFog.z = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffSunR", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffSun.x = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffSunG", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffSun.y = atof(value);
	GetPrivateProfileStringA("WeatherMode", "CoeffSunB", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.WeatherMode.CoeffSun.z = atof(value);

	SettingsMain.ShadowMode.MultiPointLighting = GetPrivateProfileIntA("ShadowMode", "MultiPointLighting", 0, Filename);
	GetPrivateProfileStringA("ShadowMode", "NearQuality", "3.0", value, SettingStringBuffer, Filename);
	SettingsMain.ShadowMode.NearQuality = atof(value);

	SettingsMain.Shaders.Water = GetPrivateProfileIntA("Shaders", "Water", 0, Filename);
	SettingsMain.Shaders.Grass = GetPrivateProfileIntA("Shaders", "Grass", 0, Filename);
	SettingsMain.Shaders.HDR = GetPrivateProfileIntA("Shaders", "HDR", 0, Filename);
	SettingsMain.Shaders.POM = GetPrivateProfileIntA("Shaders", "POM", 0, Filename);
	SettingsMain.Shaders.Skin = GetPrivateProfileIntA("Shaders", "Skin", 0, Filename);
	SettingsMain.Shaders.Terrain = GetPrivateProfileIntA("Shaders", "Terrain", 0, Filename);
	SettingsMain.Shaders.Blood = GetPrivateProfileIntA("Shaders", "Blood", 0, Filename);
	SettingsMain.Shaders.NightEye = GetPrivateProfileIntA("Shaders", "NightEye", 0, Filename);
	
	SettingsMain.Effects.Underwater = GetPrivateProfileIntA("Effects", "Underwater", 0, Filename);
	SettingsMain.Effects.WaterLens = GetPrivateProfileIntA("Effects", "WaterLens", 0, Filename);
	SettingsMain.Effects.GodRays = GetPrivateProfileIntA("Effects", "GodRays", 0, Filename);
	SettingsMain.Effects.KhajiitRays = GetPrivateProfileIntA("Effects", "KhajiitRays", 0, Filename);
	SettingsMain.Effects.DepthOfField = GetPrivateProfileIntA("Effects", "DepthOfField", 0, Filename);
	SettingsMain.Effects.AmbientOcclusion = GetPrivateProfileIntA("Effects", "AmbientOcclusion", 0, Filename);
	SettingsMain.Effects.Coloring = GetPrivateProfileIntA("Effects", "Coloring", 0, Filename);
	SettingsMain.Effects.Cinema = GetPrivateProfileIntA("Effects", "Cinema", 0, Filename);
	SettingsMain.Effects.Bloom = GetPrivateProfileIntA("Effects", "Bloom", 0, Filename);
	SettingsMain.Effects.SnowAccumulation = GetPrivateProfileIntA("Effects", "SnowAccumulation", 0, Filename);
	SettingsMain.Effects.MotionBlur = GetPrivateProfileIntA("Effects", "MotionBlur", 0, Filename);
	SettingsMain.Effects.WetWorld = GetPrivateProfileIntA("Effects", "WetWorld", 0, Filename);
	SettingsMain.Effects.Sharpening = GetPrivateProfileIntA("Effects", "Sharpening", 0, Filename);
	SettingsMain.Effects.SMAA = GetPrivateProfileIntA("Effects", "SMAA", 0, Filename);
	SettingsMain.Effects.TAA = GetPrivateProfileIntA("Effects", "TAA", 0, Filename);
	SettingsMain.Effects.VolumetricFog = GetPrivateProfileIntA("Effects", "VolumetricFog", 0, Filename);
	SettingsMain.Effects.VolumetricLight = GetPrivateProfileIntA("Effects", "VolumetricLight", 0, Filename);
	SettingsMain.Effects.Precipitations = GetPrivateProfileIntA("Effects", "Precipitations", 0, Filename);
	SettingsMain.Effects.Extra = GetPrivateProfileIntA("Effects", "Extra", 0, Filename);

	SettingsMain.Menu.InfoEnabled = GetPrivateProfileIntA("Menu", "InfoEnabled", 1, Filename);
	GetPrivateProfileStringA("Menu", "TextFont", "Calibri", SettingsMain.Menu.TextFont, 40, Filename);
	SettingsMain.Menu.TextSize = GetPrivateProfileIntA("Menu", "TextSize", 22, Filename);
	GetPrivateProfileStringA("Menu", "TextFontStatus", "Courier New", SettingsMain.Menu.TextFontStatus, 40, Filename);
	SettingsMain.Menu.TextSizeStatus = GetPrivateProfileIntA("Menu", "TextSizeStatus", 11, Filename);
	GetPrivateProfileStringA("Menu", "TextColorNormal", "180,180,180", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextColorNormal[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	GetPrivateProfileStringA("Menu", "TextShadowColorNormal", "50,50,50", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextShadowColorNormal[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	GetPrivateProfileStringA("Menu", "TextColorSelected", "255,255,255", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextColorSelected[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	GetPrivateProfileStringA("Menu", "TextShadowColorSelected", "50,50,50", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextShadowColorSelected[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	GetPrivateProfileStringA("Menu", "TextColorEditing", "255,100,50", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextColorEditing[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	GetPrivateProfileStringA("Menu", "TextShadowColorEditing", "50,50,50", value, SettingStringBuffer, Filename);
	pNextValue = strtok(value, ",");
	for (int i = 0; i < 3; i++) {
		SettingsMain.Menu.TextShadowColorEditing[i] = atoi(pNextValue);
		pNextValue = strtok(NULL, ",");
	}
	SettingsMain.Menu.PositionX = GetPrivateProfileIntA("Menu", "PositionX", 60, Filename);
	SettingsMain.Menu.PositionY = GetPrivateProfileIntA("Menu", "PositionY", 120, Filename);
	SettingsMain.Menu.InfoPositionX = GetPrivateProfileIntA("Menu", "InfoPositionX", 480, Filename);
	SettingsMain.Menu.InfoPositionY = GetPrivateProfileIntA("Menu", "InfoPositionY", 800, Filename);
	SettingsMain.Menu.TitleColumnSize = GetPrivateProfileIntA("Menu", "TitleColumnSize", 800, Filename);
	SettingsMain.Menu.MainItemColumnSize = GetPrivateProfileIntA("Menu", "MainItemColumnSize", 100, Filename);
	SettingsMain.Menu.ItemColumnSize = GetPrivateProfileIntA("Menu", "ItemColumnSize", 250, Filename);
	SettingsMain.Menu.InfoColumnSize = GetPrivateProfileIntA("Menu", "InfoColumnSize", 960, Filename);
	SettingsMain.Menu.RowSpace = GetPrivateProfileIntA("Menu", "RowSpace", 4, Filename);
	SettingsMain.Menu.RowsPerPage = GetPrivateProfileIntA("Menu", "RowsPerPage", 30, Filename);
	SettingsMain.Menu.KeyEnable = GetPrivateProfileIntA("Menu", "KeyEnable", 24, Filename);
	SettingsMain.Menu.KeyUp = GetPrivateProfileIntA("Menu", "KeyUp", 200, Filename);
	SettingsMain.Menu.KeyDown = GetPrivateProfileIntA("Menu", "KeyDown", 208, Filename);
	SettingsMain.Menu.KeyLeft = GetPrivateProfileIntA("Menu", "KeyLeft", 203, Filename);
	SettingsMain.Menu.KeyRight = GetPrivateProfileIntA("Menu", "KeyRight", 205, Filename);
	SettingsMain.Menu.KeyPageUp = GetPrivateProfileIntA("Menu", "KeyPageUp", 201, Filename);
	SettingsMain.Menu.KeyPageDown = GetPrivateProfileIntA("Menu", "KeyPageDown", 209, Filename);
	SettingsMain.Menu.KeyAdd = GetPrivateProfileIntA("Menu", "KeyAdd", 78, Filename);
	SettingsMain.Menu.KeySubtract = GetPrivateProfileIntA("Menu", "KeySubtract", 74, Filename);
	SettingsMain.Menu.KeySave = GetPrivateProfileIntA("Menu", "KeySave", 28, Filename);
	SettingsMain.Menu.KeyEditing = GetPrivateProfileIntA("Menu", "KeyEditing", 156, Filename);
	GetPrivateProfileStringA("Menu", "StepValue", "0.1", value, SettingStringBuffer, Filename);
	SettingsMain.Menu.StepValue = atof(value);
	GetPrivateProfileStringA("Menu", "Decimals", "3", value, SettingStringBuffer, Filename);
	strcpy(SettingsMain.Menu.ValueFormat, "%.");
	strcat(SettingsMain.Menu.ValueFormat, value);
	strcat(SettingsMain.Menu.ValueFormat, "f");

	SettingsMain.Purger.Enabled = GetPrivateProfileIntA("Purger", "Enabled", 0, Filename);
	SettingsMain.Purger.Time = GetPrivateProfileIntA("Purger", "Time", 300, Filename);
	SettingsMain.Purger.PurgeTextures = GetPrivateProfileIntA("Purger", "PurgeTextures", 1, Filename);
	SettingsMain.Purger.PurgeCells = GetPrivateProfileIntA("Purger", "PurgeCells", 1, Filename);
	SettingsMain.Purger.Key = GetPrivateProfileIntA("Purger", "Key", 88, Filename);

	SettingsMain.Gravity.Enabled = GetPrivateProfileIntA("Gravity", "Enabled", 1, Filename);
	GetPrivateProfileStringA("Gravity", "Value", "9.8", value, SettingStringBuffer, Filename);
	SettingsMain.Gravity.Value = atof(value);

	SettingsMain.Dodge.Enabled = GetPrivateProfileIntA("Dodge", "Enabled", 0, Filename);
	SettingsMain.Dodge.AcrobaticsLevel = GetPrivateProfileIntA("Dodge", "AcrobaticsLevel", 0, Filename);
	SettingsMain.Dodge.DoubleTap = GetPrivateProfileIntA("Dodge", "DoubleTap", 1, Filename);
	GetPrivateProfileStringA("Dodge", "DoubleTapTime", "0.5", value, SettingStringBuffer, Filename);
	SettingsMain.Dodge.DoubleTapTime = atof(value);

	SettingsMain.FlyCam.Enabled = GetPrivateProfileIntA("FlyCam", "Enabled", 0, Filename);
	GetPrivateProfileStringA("FlyCam", "ScrollMultiplier", "5.0", value, SettingStringBuffer, Filename);
	SettingsMain.FlyCam.ScrollMultiplier = atof(value);
	SettingsMain.FlyCam.KeyAdd = GetPrivateProfileIntA("FlyCam", "KeyAdd", 78, Filename);
	SettingsMain.FlyCam.KeySubtract = GetPrivateProfileIntA("FlyCam", "KeySubtract", 74, Filename);
	GetPrivateProfileStringA("FlyCam", "StepValue", "1.0", value, SettingStringBuffer, Filename);
	SettingsMain.FlyCam.StepValue = atof(value);

	SettingsMain.Develop.CompileShaders = GetPrivateProfileIntA("Develop", "CompileShaders", 0, Filename);
	SettingsMain.Develop.CompileEffects = GetPrivateProfileIntA("Develop", "CompileEffects", 0, Filename);
	SettingsMain.Develop.TraceShaders = GetPrivateProfileIntA("Develop", "TraceShaders", 0, Filename);
	SettingsMain.Develop.LogShaders = GetPrivateProfileIntA("Develop", "LogShaders", 0, Filename);

	GameLoading = false;

}

void SettingManager::LoadSettings() {

	char Filename[MAX_PATH];
	char value[SettingStringBuffer];
	char Sections[65535];
	char* pNextSection = NULL;

	SettingsWaterStruct SW;
	SettingsDepthOfFieldStruct SD;
	SettingsAmbientOcclusionStruct SA;
	SettingsColoringStruct SC;
	SettingsBloomStruct SB;
	SettingsMotionBlurStruct SM;
	SettingsWeatherStruct SE;
	SettingsShadowPointLightsStruct SSPL;
	SettingsVolumetricLightStruct SVL;
	
	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Water\\Water.ini");
	GetPrivateProfileStringA("Default", "choppiness", "0.8", Water_choppiness, SettingStringBuffer, Filename);
	SW.choppiness = atof(Water_choppiness);
	GetPrivateProfileStringA("Default", "waveWidth", "1.8", Water_waveWidth, SettingStringBuffer, Filename);
	SW.waveWidth = atof(Water_waveWidth);
	GetPrivateProfileStringA("Default", "waveSpeed", "0.5", Water_waveSpeed, SettingStringBuffer, Filename);
	SW.waveSpeed = atof(Water_waveSpeed);
	GetPrivateProfileStringA("Default", "reflectivity", "0.8", Water_reflectivity, SettingStringBuffer, Filename);
	SW.reflectivity = atof(Water_reflectivity);
	GetPrivateProfileStringA("Default", "causticsStrength", "0.5", Water_causticsStrength, SettingStringBuffer, Filename);
	SW.causticsStrength = atof(Water_causticsStrength);
	GetPrivateProfileStringA("Default", "causticsStrengthS", "1.0", Water_causticsStrengthS, SettingStringBuffer, Filename);
	SW.causticsStrengthS = atof(Water_causticsStrengthS);
	GetPrivateProfileStringA("Default", "shoreFactor", "1.2", Water_shoreFactor, SettingStringBuffer, Filename);
	SW.shoreFactor = atof(Water_shoreFactor);
	GetPrivateProfileStringA("Default", "shoreMovement", "35", Water_shoreMovement, SettingStringBuffer, Filename);
	SW.shoreMovement = atof(Water_shoreMovement);
	GetPrivateProfileStringA("Default", "turbidity", "0.72", Water_turbidity, SettingStringBuffer, Filename);
	SW.turbidity = atof(Water_turbidity);
	GetPrivateProfileStringA("Default", "inScattCoeff", "0.12", Water_inScattCoeff, SettingStringBuffer, Filename);
	SW.inScattCoeff = atof(Water_inScattCoeff);
	GetPrivateProfileStringA("Default", "inExtCoeff_R", "0.40", Water_inExtCoeff_R, SettingStringBuffer, Filename);
	SW.inExtCoeff_R = atof(Water_inExtCoeff_R);
	GetPrivateProfileStringA("Default", "inExtCoeff_G", "0.26", Water_inExtCoeff_G, SettingStringBuffer, Filename);
	SW.inExtCoeff_G = atof(Water_inExtCoeff_G);
	GetPrivateProfileStringA("Default", "inExtCoeff_B", "0.30", Water_inExtCoeff_B, SettingStringBuffer, Filename);
	SW.inExtCoeff_B = atof(Water_inExtCoeff_B);
	GetPrivateProfileStringA("Default", "depthDarkness", "0.1", Water_depthDarkness, SettingStringBuffer, Filename);
	SW.depthDarkness = atof(Water_depthDarkness);
	GetPrivateProfileStringA("Default", "LODdistance", "1.0", Water_LODdistance, SettingStringBuffer, Filename);
	SW.LODdistance = atof(Water_LODdistance);
	GetPrivateProfileStringA("Default", "MinLOD", "0.0", Water_MinLOD, SettingStringBuffer, Filename);
	SW.MinLOD = atof(Water_MinLOD);
	GetPrivateProfileStringA("Default", "LensTimeMultA", "0.1", Water_LensTimeMultA, SettingStringBuffer, Filename);
	SW.LensTimeMultA = atof(Water_LensTimeMultA);
	GetPrivateProfileStringA("Default", "LensTimeMultB", "0.2", Water_LensTimeMultB, SettingStringBuffer, Filename);
	SW.LensTimeMultB = atof(Water_LensTimeMultB);
	GetPrivateProfileStringA("Default", "LensTime", "80.0", Water_LensTime, SettingStringBuffer, Filename);
	SW.LensTime = atof(Water_LensTime);
	GetPrivateProfileStringA("Default", "LensAmount", "0.1", Water_LensAmount, SettingStringBuffer, Filename);
	SW.LensAmount = atof(Water_LensAmount);
	GetPrivateProfileStringA("Default", "LensViscosity", "0.05", Water_LensViscosity, SettingStringBuffer, Filename);
	SW.LensViscosity = atof(Water_LensViscosity);
	SettingsWater["Default"] = SW;

	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		if (strcmp(pNextSection, "Default") != 0) {
			GetPrivateProfileStringA(pNextSection, "choppiness", Water_choppiness, value, SettingStringBuffer, Filename);
			SW.choppiness = atof(value);
			GetPrivateProfileStringA(pNextSection, "waveWidth", Water_waveWidth, value, SettingStringBuffer, Filename);
			SW.waveWidth = atof(value);
			GetPrivateProfileStringA(pNextSection, "waveSpeed", Water_waveSpeed, value, SettingStringBuffer, Filename);
			SW.waveSpeed = atof(value);
			GetPrivateProfileStringA(pNextSection, "reflectivity", Water_reflectivity, value, SettingStringBuffer, Filename);
			SW.reflectivity = atof(value);
			GetPrivateProfileStringA(pNextSection, "causticsStrength", Water_causticsStrength, value, SettingStringBuffer, Filename);
			SW.causticsStrength = atof(value);
			GetPrivateProfileStringA(pNextSection, "causticsStrengthS", Water_causticsStrengthS, value, SettingStringBuffer, Filename);
			SW.causticsStrengthS = atof(value);
			GetPrivateProfileStringA(pNextSection, "shoreFactor", Water_shoreFactor, value, SettingStringBuffer, Filename);
			SW.shoreFactor = atof(value);
			GetPrivateProfileStringA(pNextSection, "shoreMovement", Water_shoreMovement, value, SettingStringBuffer, Filename);
			SW.shoreMovement = atof(value);
			GetPrivateProfileStringA(pNextSection, "turbidity", Water_turbidity, value, SettingStringBuffer, Filename);
			SW.turbidity = atof(value);
			GetPrivateProfileStringA(pNextSection, "inScattCoeff", Water_inScattCoeff, value, SettingStringBuffer, Filename);
			SW.inScattCoeff = atof(value);
			GetPrivateProfileStringA(pNextSection, "inExtCoeff_R", Water_inExtCoeff_R, value, SettingStringBuffer, Filename);
			SW.inExtCoeff_R = atof(value);
			GetPrivateProfileStringA(pNextSection, "inExtCoeff_G", Water_inExtCoeff_G, value, SettingStringBuffer, Filename);
			SW.inExtCoeff_G = atof(value);
			GetPrivateProfileStringA(pNextSection, "inExtCoeff_B", Water_inExtCoeff_B, value, SettingStringBuffer, Filename);
			SW.inExtCoeff_B = atof(value);
			GetPrivateProfileStringA(pNextSection, "depthDarkness", Water_depthDarkness, value, SettingStringBuffer, Filename);
			SW.depthDarkness = atof(value);
			GetPrivateProfileStringA(pNextSection, "LODdistance", Water_LODdistance, value, SettingStringBuffer, Filename);
			SW.LODdistance = atof(value);
			GetPrivateProfileStringA(pNextSection, "MinLOD", Water_MinLOD, value, SettingStringBuffer, Filename);
			SW.MinLOD = atof(value);
			GetPrivateProfileStringA(pNextSection, "LensTimeMultA", Water_LensTimeMultA, value, SettingStringBuffer, Filename);
			SW.LensTimeMultA = atof(value);
			GetPrivateProfileStringA(pNextSection, "LensTimeMultB", Water_LensTimeMultB, value, SettingStringBuffer, Filename);
			SW.LensTimeMultB = atof(value);
			GetPrivateProfileStringA(pNextSection, "LensTime", Water_LensTime, value, SettingStringBuffer, Filename);
			SW.LensTime = atof(value);
			GetPrivateProfileStringA(pNextSection, "LensAmount", Water_LensAmount, value, SettingStringBuffer, Filename);
			SW.LensAmount = atof(value);
			GetPrivateProfileStringA(pNextSection, "LensViscosity", Water_LensViscosity, value, SettingStringBuffer, Filename);
			SW.LensViscosity = atof(value);
			SettingsWater[std::string(pNextSection)] = SW;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Grass\\Grass.ini");
	SettingsGrass.WindEnabled = GetPrivateProfileIntA("Default", "WindEnabled", 1, Filename);
	SettingsGrass.GrassDensity = GetPrivateProfileIntA("Default", "GrassDensity", 6, Filename);
	GetPrivateProfileStringA("Default", "WindCoefficient", "100", value, SettingStringBuffer, Filename);
	SettingsGrass.WindCoefficient = atof(value);
	GetPrivateProfileStringA("Default", "ScaleX", "2.0", value, SettingStringBuffer, Filename);
	SettingsGrass.ScaleX = atof(value);
	GetPrivateProfileStringA("Default", "ScaleY", "2.0", value, SettingStringBuffer, Filename);
	SettingsGrass.ScaleY = atof(value);
	GetPrivateProfileStringA("Default", "ScaleZ", "1.0", value, SettingStringBuffer, Filename);
	SettingsGrass.ScaleZ = atof(value);
	GetPrivateProfileStringA("Default", "MinHeight", "0.8", value, SettingStringBuffer, Filename);
	SettingsGrass.MinHeight = atof(value);
	GetPrivateProfileStringA("Default", "MinDistance", "11000.0", value, SettingStringBuffer, Filename);
	SettingsGrass.MinDistance = atof(value);
	GetPrivateProfileStringA("Default", "MaxDistance", "12000.0", value, SettingStringBuffer, Filename);
	SettingsGrass.MaxDistance = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "HDR\\HDR.ini");
	GetPrivateProfileStringA("Default", "ToneMapping", "1.0", value, SettingStringBuffer, Filename);
	SettingsHDR.ToneMapping = atof(value);
	GetPrivateProfileStringA("Default", "ToneMappingBlur", "0.2", value, SettingStringBuffer, Filename);
	SettingsHDR.ToneMappingBlur = atof(value);
	GetPrivateProfileStringA("Default", "ToneMappingColor", "1.0", value, SettingStringBuffer, Filename);
	SettingsHDR.ToneMappingColor = atof(value);
	GetPrivateProfileStringA("Default", "Linearization", "2.2", value, SettingStringBuffer, Filename);
	SettingsHDR.Linearization = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "POM\\POM.ini");
	GetPrivateProfileStringA("Default", "HeightMapScale", "0.05", value, SettingStringBuffer, Filename);
	SettingsPOM.HeightMapScale = atof(value);
	GetPrivateProfileStringA("Default", "ShadowSoftening", "0.58", value, SettingStringBuffer, Filename);
	SettingsPOM.ShadowSoftening = atof(value);
	GetPrivateProfileStringA("Default", "MinSamples", "16", value, SettingStringBuffer, Filename);
	SettingsPOM.MinSamples = atof(value);
	GetPrivateProfileStringA("Default", "MaxSamples", "160", value, SettingStringBuffer, Filename);
	SettingsPOM.MaxSamples = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Terrain\\Terrain.ini");
	GetPrivateProfileStringA("Default", "DistantSpecular", "0.0", value, SettingStringBuffer, Filename);
	SettingsTerrain.DistantSpecular = atof(value);
	GetPrivateProfileStringA("Default", "DistantNoise", "0.4", value, SettingStringBuffer, Filename);
	SettingsTerrain.DistantNoise = atof(value);
	GetPrivateProfileStringA("Default", "NearSpecular", "0.0", value, SettingStringBuffer, Filename);
	SettingsTerrain.NearSpecular = atof(value);
	GetPrivateProfileStringA("Default", "MiddleSpecular", "0.0", value, SettingStringBuffer, Filename);
	SettingsTerrain.MiddleSpecular = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Skin\\Skin.ini");
	SettingsSkin.UseVanillaShaders = GetPrivateProfileIntA("Default", "UseVanillaShaders", 0, Filename);
	GetPrivateProfileStringA("Default", "Attenuation", "1.0", value, SettingStringBuffer, Filename);
	SettingsSkin.Attenuation = atof(value);
	GetPrivateProfileStringA("Default", "SpecularPower", "1.0", value, SettingStringBuffer, Filename);
	SettingsSkin.SpecularPower = atof(value);
	GetPrivateProfileStringA("Default", "MaterialThickness", "0.25", value, SettingStringBuffer, Filename);
	SettingsSkin.MaterialThickness = atof(value);
	GetPrivateProfileStringA("Default", "RimScalar", "1.0", value, SettingStringBuffer, Filename);
	SettingsSkin.RimScalar = atof(value);
	GetPrivateProfileStringA("Default", "CoeffRed", "0.70", value, SettingStringBuffer, Filename);
	SettingsSkin.CoeffRed = atof(value);
	GetPrivateProfileStringA("Default", "CoeffGreen", "0.62", value, SettingStringBuffer, Filename);
	SettingsSkin.CoeffGreen = atof(value);
	GetPrivateProfileStringA("Default", "CoeffBlue", "0.50", value, SettingStringBuffer, Filename);
	SettingsSkin.CoeffBlue = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "GodRays\\GodRays.ini");
	SettingsGodRays.TimeEnabled = GetPrivateProfileIntA("Default", "TimeEnabled", 1, Filename);
	SettingsGodRays.SunGlareEnabled = GetPrivateProfileIntA("Default", "SunGlareEnabled", 1, Filename);
	SettingsGodRays.LightShaftPasses = GetPrivateProfileIntA("Default", "LightShaftPasses", 30, Filename);
	GetPrivateProfileStringA("Default", "RayIntensity", "3.2", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayIntensity = atof(value);
	GetPrivateProfileStringA("Default", "RayLength", "1.0", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayLength = atof(value);
	GetPrivateProfileStringA("Default", "RayDensity", "0.8", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayDensity = atof(value);
	GetPrivateProfileStringA("Default", "RayVisibility", "0.4", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayVisibility = atof(value);
	GetPrivateProfileStringA("Default", "Luminance", "0.4", value, SettingStringBuffer, Filename);
	SettingsGodRays.Luminance = atof(value);
	GetPrivateProfileStringA("Default", "GlobalMultiplier", "1.5", value, SettingStringBuffer, Filename);
	SettingsGodRays.GlobalMultiplier = atof(value);
	GetPrivateProfileStringA("Default", "RayR", "0.5", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayR = atof(value);
	GetPrivateProfileStringA("Default", "RayG", "0.9", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayG = atof(value);
	GetPrivateProfileStringA("Default", "RayB", "0.8", value, SettingStringBuffer, Filename);
	SettingsGodRays.RayB = atof(value);
	GetPrivateProfileStringA("Default", "Saturate", "0.55", value, SettingStringBuffer, Filename);
	SettingsGodRays.Saturate = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "KhajiitRays\\KhajiitRays.ini");
	//Masser
	SettingsKhajiitRays.mLightShaftPasses = GetPrivateProfileIntA("Default", "MasserLightShaftPasses", 30, Filename);
	GetPrivateProfileStringA("Default", "MasserRayIntensity", "3.2", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayIntensity = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayLength", "1.0", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayLength = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayDensity", "0.8", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayDensity = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayVisibility", "0.4", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayVisibility = atof(value);
	GetPrivateProfileStringA("Default", "MasserLuminance", "0.4", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mLuminance = atof(value);
	GetPrivateProfileStringA("Default", "MasserGlobalMultiplier", "1.5", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mGlobalMultiplier = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayR", "0.5", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayR = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayG", "0.9", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayG = atof(value);
	GetPrivateProfileStringA("Default", "MasserRayB", "0.8", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mRayB = atof(value);
	GetPrivateProfileStringA("Default", "MasserSaturate", "0.55", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.mSaturate = atof(value);
	//Secunda
	SettingsKhajiitRays.sLightShaftPasses = GetPrivateProfileIntA("Default", "SecundaLightShaftPasses", 30, Filename);
	GetPrivateProfileStringA("Default", "SecundaRayIntensity", "3.2", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayIntensity = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayLength", "1.0", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayLength = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayDensity", "0.8", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayDensity = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayVisibility", "0.4", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayVisibility = atof(value);
	GetPrivateProfileStringA("Default", "SecundaLuminance", "0.4", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sLuminance = atof(value);
	GetPrivateProfileStringA("Default", "SecundaGlobalMultiplier", "1.5", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sGlobalMultiplier = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayR", "0.5", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayR = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayG", "0.9", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayG = atof(value);
	GetPrivateProfileStringA("Default", "SecundaRayB", "0.8", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sRayB = atof(value);
	GetPrivateProfileStringA("Default", "SecundaSaturate", "0.55", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.sSaturate = atof(value);
	//Other
	GetPrivateProfileStringA("Default", "PhaseLumQtr", "0.25", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.phaseLumQtr = atof(value);
	GetPrivateProfileStringA("Default", "PhaseLumHalf", "0.50", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.phaseLumHalf = atof(value);
	GetPrivateProfileStringA("Default", "PhaseLumTQtr", "0.75", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.phaseLumTQtr = atof(value);
	GetPrivateProfileStringA("Default", "PhaseLumFull", "1,0", value, SettingStringBuffer, Filename);
	SettingsKhajiitRays.phaseLumFull = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "DepthOfField\\DepthOfField.ini");
	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		SD.Enabled = GetPrivateProfileIntA(pNextSection, "Enabled", 1, Filename);
		SD.Mode = GetPrivateProfileIntA(pNextSection, "Mode", 0, Filename);
		SD.DistantBlur = GetPrivateProfileIntA(pNextSection, "DistantBlur", 0, Filename);
		GetPrivateProfileStringA(pNextSection, "DistantBlurStartRange", "800", value, SettingStringBuffer, Filename);
		SD.DistantBlurStartRange = atof(value);
		GetPrivateProfileStringA(pNextSection, "DistantBlurEndRange", "1000", value, SettingStringBuffer, Filename);
		SD.DistantBlurEndRange = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseBlurRadius", "1.0", value, SettingStringBuffer, Filename);
		SD.BaseBlurRadius = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurFallOff", "2.0", value, SettingStringBuffer, Filename);
		SD.BlurFallOff = atof(value);
		GetPrivateProfileStringA(pNextSection, "Radius", "8.0", value, SettingStringBuffer, Filename);
		SD.Radius = atof(value);
		GetPrivateProfileStringA(pNextSection, "DiameterRange", "0.009", value, SettingStringBuffer, Filename);
		SD.DiameterRange = atof(value);
		GetPrivateProfileStringA(pNextSection, "NearBlurCutOff", "0.8", value, SettingStringBuffer, Filename);
		SD.NearBlurCutOff = atof(value);
		SettingsDepthOfField[std::string(pNextSection)] = SD;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "AmbientOcclusion\\AmbientOcclusion.ini");
	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		SA.Enabled = GetPrivateProfileIntA(pNextSection, "Enabled", 1, Filename);
		GetPrivateProfileStringA(pNextSection, "RadiusMultiplier", "8", value, SettingStringBuffer, Filename);
		SA.RadiusMultiplier = atof(value);
		GetPrivateProfileStringA(pNextSection, "StrengthMultiplier", "4", value, SettingStringBuffer, Filename);
		SA.StrengthMultiplier = atof(value);
		GetPrivateProfileStringA(pNextSection, "ClampStrength", "0.1", value, SettingStringBuffer, Filename);
		SA.ClampStrength = atof(value);
		GetPrivateProfileStringA(pNextSection, "AngleBias", "15", value, SettingStringBuffer, Filename);
		SA.AngleBias = atof(value);
		GetPrivateProfileStringA(pNextSection, "Range", "1500", value, SettingStringBuffer, Filename);
		SA.Range = atof(value);
		GetPrivateProfileStringA(pNextSection, "LumThreshold", "0.2", value, SettingStringBuffer, Filename);
		SA.LumThreshold = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurDropThreshold", "100", value, SettingStringBuffer, Filename);
		SA.BlurDropThreshold = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurRadiusMultiplier", "1.0", value, SettingStringBuffer, Filename);
		SA.BlurRadiusMultiplier = atof(value);
		SettingsAmbientOcclusion[std::string(pNextSection)] = SA;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Coloring\\Coloring.ini");
	GetPrivateProfileStringA("Default", "Strength", "0.5", Coloring_Strength, SettingStringBuffer, Filename);
	SC.Strength = atof(Coloring_Strength);
	GetPrivateProfileStringA("Default", "BaseGamma", "1.6", Coloring_BaseGamma, SettingStringBuffer, Filename);
	SC.BaseGamma = atof(Coloring_BaseGamma);
	GetPrivateProfileStringA("Default", "Fade", "0.0", Coloring_Fade, SettingStringBuffer, Filename);
	SC.Fade = atof(Coloring_Fade);
	GetPrivateProfileStringA("Default", "Contrast", "0.8", Coloring_Contrast, SettingStringBuffer, Filename);
	SC.Contrast = atof(Coloring_Contrast);
	GetPrivateProfileStringA("Default", "Saturation", "-0.1", Coloring_Saturation, SettingStringBuffer, Filename);
	SC.Saturation = atof(Coloring_Saturation);
	GetPrivateProfileStringA("Default", "Bleach", "0.1", Coloring_Bleach, SettingStringBuffer, Filename);
	SC.Bleach = atof(Coloring_Bleach);
	GetPrivateProfileStringA("Default", "BleachLuma", "0.2", Coloring_BleachLuma, SettingStringBuffer, Filename);
	SC.BleachLuma = atof(Coloring_BleachLuma);
	GetPrivateProfileStringA("Default", "ColorCurve", "1.2", Coloring_ColorCurve, SettingStringBuffer, Filename);
	SC.ColorCurve = atof(Coloring_ColorCurve);
	GetPrivateProfileStringA("Default", "ColorCurveR", "0.9", Coloring_ColorCurveR, SettingStringBuffer, Filename);
	SC.ColorCurveR = atof(Coloring_ColorCurveR);
	GetPrivateProfileStringA("Default", "ColorCurveG", "1.0", Coloring_ColorCurveG, SettingStringBuffer, Filename);
	SC.ColorCurveG = atof(Coloring_ColorCurveG);
	GetPrivateProfileStringA("Default", "ColorCurveB", "1.0", Coloring_ColorCurveB, SettingStringBuffer, Filename);
	SC.ColorCurveB = atof(Coloring_ColorCurveB);
	GetPrivateProfileStringA("Default", "EffectGamma", "0.9", Coloring_EffectGamma, SettingStringBuffer, Filename);
	SC.EffectGamma = atof(Coloring_EffectGamma);
	GetPrivateProfileStringA("Default", "EffectGammaR", "1.0", Coloring_EffectGammaR, SettingStringBuffer, Filename);
	SC.EffectGammaR = atof(Coloring_EffectGammaR);
	GetPrivateProfileStringA("Default", "EffectGammaG", "1.0", Coloring_EffectGammaG, SettingStringBuffer, Filename);
	SC.EffectGammaG = atof(Coloring_EffectGammaG);
	GetPrivateProfileStringA("Default", "EffectGammaB", "1.0", Coloring_EffectGammaB, SettingStringBuffer, Filename);
	SC.EffectGammaB = atof(Coloring_EffectGammaB);
	GetPrivateProfileStringA("Default", "Linearization", "1.1", Coloring_Linearization, SettingStringBuffer, Filename);
	SC.Linearization = atof(Coloring_Linearization);
	SettingsColoring["Default"] = SC;

	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		if (strcmp(pNextSection, "Default") != 0) {
			GetPrivateProfileStringA(pNextSection, "Strength", Coloring_Strength, value, SettingStringBuffer, Filename);
			SC.Strength = atof(value);
			GetPrivateProfileStringA(pNextSection, "BaseGamma", Coloring_BaseGamma, value, SettingStringBuffer, Filename);
			SC.BaseGamma = atof(value);
			GetPrivateProfileStringA(pNextSection, "Fade", Coloring_Fade, value, SettingStringBuffer, Filename);
			SC.Fade = atof(value);
			GetPrivateProfileStringA(pNextSection, "Contrast", Coloring_Contrast, value, SettingStringBuffer, Filename);
			SC.Contrast = atof(value);
			GetPrivateProfileStringA(pNextSection, "Saturation", Coloring_Saturation, value, SettingStringBuffer, Filename);
			SC.Saturation = atof(value);
			GetPrivateProfileStringA(pNextSection, "Bleach", Coloring_Bleach, value, SettingStringBuffer, Filename);
			SC.Bleach = atof(value);
			GetPrivateProfileStringA(pNextSection, "BleachLuma", Coloring_BleachLuma, value, SettingStringBuffer, Filename);
			SC.BleachLuma = atof(value);
			GetPrivateProfileStringA(pNextSection, "ColorCurve", Coloring_ColorCurve, value, SettingStringBuffer, Filename);
			SC.ColorCurve = atof(value);
			GetPrivateProfileStringA(pNextSection, "ColorCurveR", Coloring_ColorCurveR, value, SettingStringBuffer, Filename);
			SC.ColorCurveR = atof(value);
			GetPrivateProfileStringA(pNextSection, "ColorCurveG", Coloring_ColorCurveG, value, SettingStringBuffer, Filename);
			SC.ColorCurveG = atof(value);
			GetPrivateProfileStringA(pNextSection, "ColorCurveB", Coloring_ColorCurveB, value, SettingStringBuffer, Filename);
			SC.ColorCurveB = atof(value);
			GetPrivateProfileStringA(pNextSection, "EffectGamma", Coloring_EffectGamma, value, SettingStringBuffer, Filename);
			SC.EffectGamma = atof(value);
			GetPrivateProfileStringA(pNextSection, "EffectGammaR", Coloring_EffectGammaR, value, SettingStringBuffer, Filename);
			SC.EffectGammaR = atof(value);
			GetPrivateProfileStringA(pNextSection, "EffectGammaG", Coloring_EffectGammaG, value, SettingStringBuffer, Filename);
			SC.EffectGammaG = atof(value);
			GetPrivateProfileStringA(pNextSection, "EffectGammaB", Coloring_EffectGammaB, value, SettingStringBuffer, Filename);
			SC.EffectGammaB = atof(value);
			GetPrivateProfileStringA(pNextSection, "Linearization", Coloring_Linearization, value, SettingStringBuffer, Filename);
			SC.Linearization = atof(value);
			SettingsColoring[std::string(pNextSection)] = SC;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Cinema\\Cinema.ini");
	SettingsCinema.Mode = GetPrivateProfileIntA("Default", "Mode", 0, Filename);
	GetPrivateProfileStringA("Default", "AspectRatio", "1.20", value, SettingStringBuffer, Filename);
	SettingsCinema.AspectRatio = atof(value);
	GetPrivateProfileStringA("Default", "VignetteRadius", "6.0", value, SettingStringBuffer, Filename);
	SettingsCinema.VignetteRadius = atof(value);
	GetPrivateProfileStringA("Default", "VignetteDarkness", "0.55", value, SettingStringBuffer, Filename);
	SettingsCinema.VignetteDarkness = atof(value);
	GetPrivateProfileStringA("Default", "ChromaticAberrationPower", "0.0", value, SettingStringBuffer, Filename);
	SettingsCinema.ChromaticAberrationPower = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Specular\\Specular.ini");
	GetPrivateProfileStringA("Default", "SpecularPower", "0.0", value, SettingStringBuffer, Filename);
	SettingsSpecular.SpecularPower = atof(value);
	GetPrivateProfileStringA("Default", "FresnelPowerActors", "0.0", value, SettingStringBuffer, Filename);
	SettingsSpecular.FresnelPowerActors = atof(value);
	GetPrivateProfileStringA("Default", "FresnelPowerObjects", "0.0", value, SettingStringBuffer, Filename);
	SettingsSpecular.FresnelPowerObjects = atof(value);
	GetPrivateProfileStringA("Default", "SunPower", "0.0", value, SettingStringBuffer, Filename);
	SettingsSpecular.SunPower = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Shadows\\ShadowPointLights.ini");

	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		SSPL.bEnabled = GetPrivateProfileIntA(pNextSection, "bEnabled", 1, Filename);
		SSPL.iShadowLightPoints = GetPrivateProfileIntA(pNextSection, "iShadowLightPoints", 12, Filename);
		SSPL.iShadowCullLightPoints = GetPrivateProfileIntA(pNextSection, "iShadowCullLightPoints", 18, Filename);
		GetPrivateProfileStringA(pNextSection, "fShadowObjectScanRadius", "1.2", value, SettingStringBuffer, Filename);
		SSPL.fShadowObjectScanRadius = atof(value);
		GetPrivateProfileStringA(pNextSection, "fShadowLightRadiusMin", "1.0", value, SettingStringBuffer, Filename);
		SSPL.fShadowLightRadiusMin = atof(value);
		GetPrivateProfileStringA(pNextSection, "fShadowLightRadiusMax", "0.06", value, SettingStringBuffer, Filename);
		SSPL.fShadowLightRadiusMax = atof(value);
		GetPrivateProfileStringA(pNextSection, "fShadowCullLightRadiusMin", "0.18", value, SettingStringBuffer, Filename);
		SSPL.fShadowCullLightRadiusMin = atof(value);
		GetPrivateProfileStringA(pNextSection, "fShadowCullLightRadiusMax", "0.8", value, SettingStringBuffer, Filename);
		SSPL.fShadowCullLightRadiusMax = atof(value);
		SettingsShadowPointLight[pNextSection] = SSPL;

		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	SSPL.bEnabled = GetPrivateProfileIntA(CreateProfileString, "bEnabled", 1, Filename);
	SSPL.iShadowLightPoints = GetPrivateProfileIntA(CreateProfileString, "iShadowLightPoints", 0, Filename);
	SSPL.iShadowCullLightPoints = GetPrivateProfileIntA(CreateProfileString, "iShadowCullLightPoints", 0, Filename);
	GetPrivateProfileStringA(CreateProfileString, "fShadowObjectScanRadius", "0.0", value, SettingStringBuffer, Filename);
	SSPL.fShadowObjectScanRadius = atof(value);
	GetPrivateProfileStringA(CreateProfileString, "fShadowLightRadiusMin", "0.0", value, SettingStringBuffer, Filename);
	SSPL.fShadowLightRadiusMin = atof(value);
	GetPrivateProfileStringA(CreateProfileString, "fShadowLightRadiusMax", "0.0", value, SettingStringBuffer, Filename);
	SSPL.fShadowLightRadiusMax = atof(value);
	GetPrivateProfileStringA(CreateProfileString, "fShadowCullLightRadiusMin", "0.0", value, SettingStringBuffer, Filename);
	SSPL.fShadowCullLightRadiusMin = atof(value);
	GetPrivateProfileStringA(CreateProfileString, "fShadowCullLightRadiusMax", "0.0", value, SettingStringBuffer, Filename);
	SSPL.fShadowCullLightRadiusMax = atof(value);
	SettingsShadowPointLight[CreateProfileString] = SSPL;

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Bloom\\Bloom.ini");
	GetPrivateProfileStringA("Default", "BloomIntensity", "1.4", Bloom_BloomIntensity, SettingStringBuffer, Filename);
	SB.BloomIntensity = atof(Bloom_BloomIntensity);
	GetPrivateProfileStringA("Default", "OriginalIntensity", "1.0", Bloom_OriginalIntensity, SettingStringBuffer, Filename);
	SB.OriginalIntensity = atof(Bloom_OriginalIntensity);
	GetPrivateProfileStringA("Default", "BloomSaturation", "1.2", Bloom_BloomSaturation, SettingStringBuffer, Filename);
	SB.BloomSaturation = atof(Bloom_BloomSaturation);
	GetPrivateProfileStringA("Default", "OriginalSaturation", "1.0", Bloom_OriginalSaturation, SettingStringBuffer, Filename);
	SB.OriginalSaturation = atof(Bloom_OriginalSaturation);
	GetPrivateProfileStringA("Default", "Luminance", "0.06", Bloom_Luminance, SettingStringBuffer, Filename);
	SB.Luminance = atof(Bloom_Luminance);
	GetPrivateProfileStringA("Default", "MiddleGray", "0.18", Bloom_MiddleGray, SettingStringBuffer, Filename);
	SB.MiddleGray = atof(Bloom_MiddleGray);
	GetPrivateProfileStringA("Default", "WhiteCutOff", "0.8", Bloom_WhiteCutOff, SettingStringBuffer, Filename);
	SB.WhiteCutOff = atof(Bloom_WhiteCutOff);
	SettingsBloom["Default"] = SB;

	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		if (strcmp(pNextSection, "Default") != 0) {
			GetPrivateProfileStringA(pNextSection, "BloomIntensity", Bloom_BloomIntensity, value, SettingStringBuffer, Filename);
			SB.BloomIntensity = atof(value);
			GetPrivateProfileStringA(pNextSection, "OriginalIntensity", Bloom_OriginalIntensity, value, SettingStringBuffer, Filename);
			SB.OriginalIntensity = atof(value);
			GetPrivateProfileStringA(pNextSection, "BloomSaturation", Bloom_BloomSaturation, value, SettingStringBuffer, Filename);
			SB.BloomSaturation = atof(value);
			GetPrivateProfileStringA(pNextSection, "OriginalSaturation", Bloom_OriginalSaturation, value, SettingStringBuffer, Filename);
			SB.OriginalSaturation = atof(value);
			GetPrivateProfileStringA(pNextSection, "Luminance", Bloom_Luminance, value, SettingStringBuffer, Filename);
			SB.Luminance = atof(value);
			GetPrivateProfileStringA(pNextSection, "MiddleGray", Bloom_MiddleGray, value, SettingStringBuffer, Filename);
			SB.MiddleGray = atof(value);
			GetPrivateProfileStringA(pNextSection, "WhiteCutOff", Bloom_WhiteCutOff, value, SettingStringBuffer, Filename);
			SB.WhiteCutOff = atof(value);
			SettingsBloom[std::string(pNextSection)] = SB;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Precipitations\\Precipitations.ini");
	GetPrivateProfileStringA("Snow", "DepthStep", "2.5", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.Snow.DepthStep = atof(value);
	GetPrivateProfileStringA("Snow", "Flakes", "0.5", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.Snow.Flakes = atof(value);
	GetPrivateProfileStringA("Snow", "Speed", "1.8", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.Snow.Speed = atof(value);
	GetPrivateProfileStringA("WetWorld", "Amount", "1.0", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.Amount = atof(value);
	GetPrivateProfileStringA("WetWorld", "Increase", "0.001", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.Increase = atof(value);
	GetPrivateProfileStringA("WetWorld", "Decrease", "0.0001", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.Decrease = atof(value);
	GetPrivateProfileStringA("WetWorld", "PuddleCoeff_R", "0.62", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.PuddleCoeff_R = atof(value);
	GetPrivateProfileStringA("WetWorld", "PuddleCoeff_G", "0.6", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.PuddleCoeff_G = atof(value);
	GetPrivateProfileStringA("WetWorld", "PuddleCoeff_B", "0.55", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.PuddleCoeff_B = atof(value);
	GetPrivateProfileStringA("WetWorld", "PuddleSpecularMultiplier", "0.2", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.WetWorld.PuddleSpecularMultiplier = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "Amount", "2.0", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.Amount = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "Increase", "0.0005", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.Increase = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "Decrease", "0.0001", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.Decrease = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "SunPower", "0.3", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.SunPower = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "BlurNormDropThreshhold", "-0.5", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.BlurNormDropThreshhold = atof(value);
	GetPrivateProfileStringA("SnowAccumulation", "BlurRadiusMultiplier", "1", value, SettingStringBuffer, Filename);
	SettingsPrecipitations.SnowAccumulation.BlurRadiusMultiplier = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "MotionBlur\\MotionBlur.ini");
	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		SM.Enabled = GetPrivateProfileIntA(pNextSection, "Enabled", 1, Filename);
		GetPrivateProfileStringA(pNextSection, "GaussianWeight", "6.0", value, SettingStringBuffer, Filename);
		SM.GaussianWeight = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurScale", "4.0", value, SettingStringBuffer, Filename);
		SM.BlurScale = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurOffsetMax", "24.0", value, SettingStringBuffer, Filename);
		SM.BlurOffsetMax = atof(value);
		GetPrivateProfileStringA(pNextSection, "BlurCutOff", "15", value, SettingStringBuffer, Filename);
		SM.BlurCutOff = atof(value);
		SettingsMotionBlur[std::string(pNextSection)] = SM;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Sharpening\\Sharpening.ini");
	GetPrivateProfileStringA("Default", "Strength", "0.75", value, SettingStringBuffer, Filename);
	SettingsSharpening.Strength = atof(value);
	GetPrivateProfileStringA("Default", "Clamp", "0.1", value, SettingStringBuffer, Filename);
	SettingsSharpening.Clamp = atof(value);
	GetPrivateProfileStringA("Default", "Offset", "2.0", value, SettingStringBuffer, Filename);
	SettingsSharpening.Offset = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "TAA\\TAA.ini");
	GetPrivateProfileStringA("Default", "BlendWeight", "0.80", value, SettingStringBuffer, Filename);
	SettingsTAA.BlendWeight = atof(value);
	GetPrivateProfileStringA("Default", "ClampRadius", "0.5", value, SettingStringBuffer, Filename);
	SettingsTAA.ClampRadius = atof(value);
	GetPrivateProfileStringA("Default", "Sharpening", "0.15", value, SettingStringBuffer, Filename);
	SettingsTAA.Sharpening = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Fog\\Fog.ini");
	GetPrivateProfileStringA("Default", "Exponent", "1.0", value, SettingStringBuffer, Filename);
	SettingsVolumetricFog.Exponent = atof(value);
	GetPrivateProfileStringA("Default", "ColorCoeff", "1.2", value, SettingStringBuffer, Filename);
	SettingsVolumetricFog.ColorCoeff = atof(value);
	GetPrivateProfileStringA("Default", "Amount", "0.8", value, SettingStringBuffer, Filename);
	SettingsVolumetricFog.Amount = atof(value);
	GetPrivateProfileStringA("Default", "MaxDistance", "100000.0", value, SettingStringBuffer, Filename);
	SettingsVolumetricFog.MaxDistance = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "VolumetricLight\\VolumetricLight.ini");
	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		GetPrivateProfileStringA(pNextSection, "BaseSunriseR", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunriseR = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseSunriseG", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunriseG = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseSunriseB", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunriseB = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseMiddayR", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseMiddayR = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseMiddayG", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseMiddayG = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseMiddayB", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseMiddayB = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseSunsetR", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunsetR = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseSunsetG", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunsetG = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseSunsetB", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseSunsetB = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseNightR", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseNightR = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseNightG", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseNightG = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseNightB", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseNightB = atof(value);
		GetPrivateProfileStringA(pNextSection, "BaseDistance", "1.0", value, SettingStringBuffer, Filename);
		SVL.baseDistance = atof(value);

		GetPrivateProfileStringA(pNextSection, "AccumSunriseR", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunriseR = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumSunriseG", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunriseG = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumSunriseB", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunriseB = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumMiddayR", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumMiddayR = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumMiddayG", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumMiddayG = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumMiddayB", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumMiddayB = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumSunsetR", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunsetR = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumSunsetG", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunsetG = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumSunsetB", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumSunsetB = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumNightR", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumNightR = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumNightG", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumNightG = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumNightB", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumNightB = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumHeightMax", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumHeightMax = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumHeightMin", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumHeightMin = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumDistanceMax", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumDistanceMax = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumDistanceMin", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumDistanceMin = atof(value);
		GetPrivateProfileStringA(pNextSection, "AccumCutOff", "1.0", value, SettingStringBuffer, Filename);
		SVL.accumCutOff = atof(value);

		GetPrivateProfileStringA(pNextSection, "SunScatterR", "1.0", value, SettingStringBuffer, Filename);
		SVL.sunScatterR = atof(value);
		GetPrivateProfileStringA(pNextSection, "SunScatterG", "1.0", value, SettingStringBuffer, Filename);
		SVL.sunScatterG = atof(value);
		GetPrivateProfileStringA(pNextSection, "SunScatterB", "1.0", value, SettingStringBuffer, Filename);
		SVL.sunScatterB = atof(value);
		GetPrivateProfileStringA(pNextSection, "FogPower", "1.0", value, SettingStringBuffer, Filename);
		SVL.fogPower = atof(value);

		GetPrivateProfileStringA(pNextSection, "BlurDistance", "1.0", value, SettingStringBuffer, Filename);
		SVL.blurDistance = atof(value);
		GetPrivateProfileStringA(pNextSection, "Randomizer", "1.0", value, SettingStringBuffer, Filename);
		SVL.randomizer = atof(value);
		GetPrivateProfileStringA(pNextSection, "AnimatedFog", "1.0", value, SettingStringBuffer, Filename);
		SVL.animatedFogToggle = atof(value);
		SettingsVolumetricLight[std::string(pNextSection)] = SVL;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	typedef std::vector<std::string> ValueList;
	strcpy(Filename, SettingsMain.Main.WeatherFile);
	GetPrivateProfileSectionNamesA(Sections, 32767, Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL) {
		char Section[12];
		for (int x = 0; x < TESWeather::kNumColorTypes; x++) {
			strcpy(Section, "Color");
			strcat(Section, ToString(x).c_str());
			GetPrivateProfileStringA(pNextSection, Section, "", value, SettingStringBuffer, Filename);
			ValueList WeatherTimes;
			ValueList WeatherValues;
			SplitString(value, ' ', WeatherTimes);
			ValueList::iterator iy = WeatherTimes.begin();
			for (int y = 0; y < TESWeather::kNumTimeOfDay; y++) {
				SplitString(*iy, ',', WeatherValues);
				ValueList::iterator iz = WeatherValues.begin();
				for (int z = 0; z < 3; z++) {
					switch (z) {
						case 0:
							SE.Colors[x].colors[y].r = atoi(iz->c_str());
							break;
						case 1:
							SE.Colors[x].colors[y].g = atoi(iz->c_str());
							break;
						case 2:
							SE.Colors[x].colors[y].b = atoi(iz->c_str());
							break;
					}
					iz++;
				}
				iy++;
			}
		}
		GetPrivateProfileStringA(pNextSection, "HDR", "", value, SettingStringBuffer, Filename);
		ValueList HDR;
		SplitString(value, ' ', HDR);
		ValueList::iterator ix = HDR.begin();
		for (int x = 0; x < 14; x++) {
			SE.HDR[x] = atof(ix->c_str());
			ix++;
		}
		GetPrivateProfileStringA(pNextSection, "FogNearDay", "0.0", value, SettingStringBuffer, Filename);
		SE.FogNearDay = atof(value);
		GetPrivateProfileStringA(pNextSection, "FogNearNight", "0.0", value, SettingStringBuffer, Filename);
		SE.FogNearNight = atof(value);
		GetPrivateProfileStringA(pNextSection, "FogFarDay", "0.0", value, SettingStringBuffer, Filename);
		SE.FogFarDay = atof(value);
		GetPrivateProfileStringA(pNextSection, "FogFarNight", "0.0", value, SettingStringBuffer, Filename);
		SE.FogFarNight = atof(value);
		GetPrivateProfileStringA(pNextSection, "WindSpeed", "0.0", value, SettingStringBuffer, Filename);
		SE.WindSpeed = atof(value);
		GetPrivateProfileStringA(pNextSection, "CloudSpeedLower", "0.0", value, SettingStringBuffer, Filename);
		SE.CloudSpeedLower = atof(value);
		GetPrivateProfileStringA(pNextSection, "CloudSpeedUpper", "0.0", value, SettingStringBuffer, Filename);
		SE.CloudSpeedUpper = atof(value);
		GetPrivateProfileStringA(pNextSection, "TransDelta", "0.0", value, SettingStringBuffer, Filename);
		SE.TransDelta = atof(value);
		GetPrivateProfileStringA(pNextSection, "SunGlare", "0.0", value, SettingStringBuffer, Filename);
		SE.SunGlare = atof(value);
		GetPrivateProfileStringA(pNextSection, "SunDamage", "0.0", value, SettingStringBuffer, Filename);
		SE.SunDamage = atof(value);
		GetPrivateProfileStringA(pNextSection, "LowerLayer", "", SE.LowerLayer, 80, Filename);
		GetPrivateProfileStringA(pNextSection, "UpperLayer", "", SE.UpperLayer, 80, Filename);
		SettingsWeather[std::string(pNextSection)] = SE;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename, "Shadows\\Shadows.ini");
	SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapNear] = GetPrivateProfileIntA("ExteriorsNear", "Enabled", 1, Filename);
	SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapNear] = GetPrivateProfileIntA("ExteriorsNear", "AlphaEnabled", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Activators = GetPrivateProfileIntA("ExteriorsNear", "Activators", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Actors = GetPrivateProfileIntA("ExteriorsNear", "Actors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Apparatus = GetPrivateProfileIntA("ExteriorsNear", "Apparatus", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Books = GetPrivateProfileIntA("ExteriorsNear", "Books", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Containers = GetPrivateProfileIntA("ExteriorsNear", "Containers", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Doors = GetPrivateProfileIntA("ExteriorsNear", "Doors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Furniture = GetPrivateProfileIntA("ExteriorsNear", "Furniture", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Misc = GetPrivateProfileIntA("ExteriorsNear", "Misc", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Statics = GetPrivateProfileIntA("ExteriorsNear", "Statics", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapNear].Trees = GetPrivateProfileIntA("ExteriorsNear", "Trees", 1, Filename);
	SettingsShadows.Exteriors.ShadowMapSize[ShadowManager::ShadowMapTypeEnum::MapNear] = GetPrivateProfileIntA("ExteriorsNear", "ShadowMapSize", 4096, Filename);
	GetPrivateProfileStringA("ExteriorsNear", "ShadowMapRadius", "4096.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapNear] = atof(value);

	SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapFar] = GetPrivateProfileIntA("ExteriorsFar", "Enabled", 1, Filename);
	SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapFar] = GetPrivateProfileIntA("ExteriorsFar", "AlphaEnabled", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Activators = GetPrivateProfileIntA("ExteriorsFar", "Activators", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Actors = GetPrivateProfileIntA("ExteriorsFar", "Actors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Apparatus = GetPrivateProfileIntA("ExteriorsFar", "Apparatus", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Books = GetPrivateProfileIntA("ExteriorsFar", "Books", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Containers = GetPrivateProfileIntA("ExteriorsFar", "Containers", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Doors = GetPrivateProfileIntA("ExteriorsFar", "Doors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Furniture = GetPrivateProfileIntA("ExteriorsFar", "Furniture", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Misc = GetPrivateProfileIntA("ExteriorsFar", "Misc", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Statics = GetPrivateProfileIntA("ExteriorsFar", "Statics", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapFar].Trees = GetPrivateProfileIntA("ExteriorsFar", "Trees", 1, Filename);
	SettingsShadows.Exteriors.ShadowMapSize[ShadowManager::ShadowMapTypeEnum::MapFar] = GetPrivateProfileIntA("ExteriorsFar", "ShadowMapSize", 1024, Filename);
	GetPrivateProfileStringA("ExteriorsFar", "ShadowMapRadius", "8192.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapFar] = atof(value);

	SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapSkin] = GetPrivateProfileIntA("ExteriorsSkin", "Enabled", 1, Filename);
	SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapSkin] = GetPrivateProfileIntA("ExteriorsSkin", "AlphaEnabled", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Activators = GetPrivateProfileIntA("ExteriorsSkin", "Activators", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Actors = GetPrivateProfileIntA("ExteriorsSkin", "Actors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Apparatus = GetPrivateProfileIntA("ExteriorsSkin", "Apparatus", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Books = GetPrivateProfileIntA("ExteriorsSkin", "Books", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Containers = GetPrivateProfileIntA("ExteriorsSkin", "Containers", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Doors = GetPrivateProfileIntA("ExteriorsSkin", "Doors", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Furniture = GetPrivateProfileIntA("ExteriorsSkin", "Furniture", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Misc = GetPrivateProfileIntA("ExteriorsSkin", "Misc", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Statics = GetPrivateProfileIntA("ExteriorsSkin", "Statics", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapSkin].Trees = GetPrivateProfileIntA("ExteriorsSkin", "Trees", 1, Filename);
	SettingsShadows.Exteriors.ShadowMapSize[ShadowManager::ShadowMapTypeEnum::MapSkin] = GetPrivateProfileIntA("ExteriorsSkin", "ShadowMapSize", 4096, Filename);
	GetPrivateProfileStringA("ExteriorsSkin", "ShadowMapRadius", "512.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapSkin] = atof(value);

	SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapOrtho] = GetPrivateProfileIntA("ExteriorsOrtho", "Enabled", 0, Filename);
	SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapOrtho] = GetPrivateProfileIntA("ExteriorsOrtho", "AlphaEnabled", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Activators = GetPrivateProfileIntA("ExteriorsOrtho", "Activators", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Actors = GetPrivateProfileIntA("ExteriorsOrtho", "Actors", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Apparatus = GetPrivateProfileIntA("ExteriorsOrtho", "Apparatus", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Books = GetPrivateProfileIntA("ExteriorsOrtho", "Books", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Containers = GetPrivateProfileIntA("ExteriorsOrtho", "Containers", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Doors = GetPrivateProfileIntA("ExteriorsOrtho", "Doors", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Furniture = GetPrivateProfileIntA("ExteriorsOrtho", "Furniture", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Misc = GetPrivateProfileIntA("ExteriorsOrtho", "Misc", 0, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Statics = GetPrivateProfileIntA("ExteriorsOrtho", "Statics", 1, Filename);
	SettingsShadows.Exteriors.Forms[ShadowManager::ShadowMapTypeEnum::MapOrtho].Trees = GetPrivateProfileIntA("ExteriorsOrtho", "Trees", 0, Filename);
	SettingsShadows.Exteriors.ShadowMapSize[ShadowManager::ShadowMapTypeEnum::MapOrtho] = GetPrivateProfileIntA("ExteriorsOrtho", "ShadowMapSize", 256, Filename);
	GetPrivateProfileStringA("ExteriorsOrtho", "ShadowMapRadius", "2048.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapOrtho] = atof(value);

	SettingsShadows.Exteriors.UsePostProcessing = GetPrivateProfileIntA("Exteriors", "UsePostProcessing", 1, Filename);
	SettingsShadows.Exteriors.UseIntervalUpdate = GetPrivateProfileIntA("Exteriors", "UseIntervalUpdate", 1, Filename);
	GetPrivateProfileStringA("Exteriors", "Darkness", "0.2", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.Darkness = atof(value);
	GetPrivateProfileStringA("Exteriors", "ShadowMapFarPlane", "8192.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.ShadowMapFarPlane = atof(value);
	GetPrivateProfileStringA("Exteriors", "forwardNormBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.forwardNormBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "forwardFarNormBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.forwardFarNormBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "forwardConstBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.forwardConstBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "forwardFarConstBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.forwardFarConstBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "deferredNormBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.deferredNormBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "deferredFarNormBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.deferredFarNormBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "deferredConstBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.deferredConstBias = atof(value);
	GetPrivateProfileStringA("Exteriors", "deferredFarConstBias", "0.001", value, SettingStringBuffer, Filename);
	SettingsShadows.Exteriors.deferredFarConstBias = atof(value);

	SettingsShadows.Interiors.Enabled = GetPrivateProfileIntA("Interiors", "Enabled", 1, Filename);
	SettingsShadows.Interiors.UsePostProcessing = GetPrivateProfileIntA("Interiors", "UsePostProcessing", 1, Filename);
	SettingsShadows.Interiors.EnableSpecularShadow = GetPrivateProfileIntA("Interiors", "EnableSpecularShadow", 1, Filename);
	SettingsShadows.Interiors.AlphaEnabled = GetPrivateProfileIntA("Interiors", "AlphaEnabled", 1, Filename);
	SettingsShadows.Interiors.Forms.Activators = GetPrivateProfileIntA("Interiors", "Activators", 1, Filename);
	SettingsShadows.Interiors.Forms.Actors = GetPrivateProfileIntA("Interiors", "Actors", 1, Filename);
	SettingsShadows.Interiors.Forms.Apparatus = GetPrivateProfileIntA("Interiors", "Apparatus", 1, Filename);
	SettingsShadows.Interiors.Forms.Books = GetPrivateProfileIntA("Interiors", "Books", 0, Filename);
	SettingsShadows.Interiors.Forms.Containers = GetPrivateProfileIntA("Interiors", "Containers", 1, Filename);
	SettingsShadows.Interiors.Forms.Doors = GetPrivateProfileIntA("Interiors", "Doors", 0, Filename);
	SettingsShadows.Interiors.Forms.Furniture = GetPrivateProfileIntA("Interiors", "Furniture", 1, Filename);
	SettingsShadows.Interiors.Forms.Misc = GetPrivateProfileIntA("Interiors", "Misc", 0, Filename);
	SettingsShadows.Interiors.Forms.Statics = GetPrivateProfileIntA("Interiors", "Statics", 1, Filename);
	SettingsShadows.Interiors.TorchesCastShadows = GetPrivateProfileIntA("Interiors", "TorchesCastShadows", 0, Filename);
	SettingsShadows.Interiors.ShadowCubeMapSize = GetPrivateProfileIntA("Interiors", "ShadowCubeMapSize", 512, Filename);
	GetPrivateProfileStringA("Interiors", "Darkness", "1.0", value, SettingStringBuffer, Filename);
	SettingsShadows.Interiors.Darkness = atof(value);

	SettingsShadows.ExteriorsPoint.Enabled = GetPrivateProfileIntA("ExteriorsPoint", "Enabled", 1, Filename);
	SettingsShadows.ExteriorsPoint.UsePostProcessing = GetPrivateProfileIntA("ExteriorsPoint", "UsePostProcessing", 1, Filename);
	SettingsShadows.ExteriorsPoint.AlphaEnabled = GetPrivateProfileIntA("ExteriorsPoint", "AlphaEnabled", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Activators = GetPrivateProfileIntA("ExteriorsPoint", "Activators", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Actors = GetPrivateProfileIntA("ExteriorsPoint", "Actors", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Apparatus = GetPrivateProfileIntA("ExteriorsPoint", "Apparatus", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Books = GetPrivateProfileIntA("ExteriorsPoint", "Books", 0, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Containers = GetPrivateProfileIntA("ExteriorsPoint", "Containers", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Doors = GetPrivateProfileIntA("ExteriorsPoint", "Doors", 0, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Furniture = GetPrivateProfileIntA("ExteriorsPoint", "Furniture", 1, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Misc = GetPrivateProfileIntA("ExteriorsPoint", "Misc", 0, Filename);
	SettingsShadows.ExteriorsPoint.Forms.Statics = GetPrivateProfileIntA("ExteriorsPoint", "Statics", 1, Filename);
	SettingsShadows.ExteriorsPoint.TorchesCastShadows = GetPrivateProfileIntA("ExteriorsPoint", "TorchesCastShadows", 0, Filename);
	SettingsShadows.ExteriorsPoint.ShadowCubeMapSize = GetPrivateProfileIntA("ExteriorsPoint", "ShadowCubeMapSize", 512, Filename);
	GetPrivateProfileStringA("ExteriorsPoint", "Darkness", "1.0", value, SettingStringBuffer, Filename);
	SettingsShadows.ExteriorsPoint.Darkness = atof(value);
	
	ValueList FormValue;
	char Form[12] = { NULL };
	char FormType[4] = { NULL };
	char FormID[12] = { NULL };
	bool FormStatus = false;
	size_t EC = 0;
	size_t NC = 0;
	size_t IC = 0;
	GetPrivateProfileSectionA("FormIDs", Sections, 32767, Filename);
	for (int i = 0; i < 2; i++) {
		if (i == 1) {
			SettingsShadows.Exteriors.ExcludedForms.reserve(EC);
			SettingsShadows.ExteriorsPoint.ExcludedForms.reserve(NC);
			SettingsShadows.Interiors.ExcludedForms.reserve(IC);
		}
		pNextSection = Sections;
		while (*pNextSection != NULL) {
			SplitString(pNextSection, '=', FormValue);
			ValueList::iterator ix = FormValue.begin();
			strcpy(Form, ix->c_str());
			ix++;
			FormStatus = atoi(ix->c_str());
			if (!FormStatus) {
				strncpy(FormType, Form, 1);
				strcpy(FormID, Form + 1);
				if (FormType[0] == 'X' || FormType[0] == 'E') {
					if (i == 0)
						EC += 1;
					else
						SettingsShadows.Exteriors.ExcludedForms.push_back(atoi(FormID));
				}
				if (FormType[0] == 'X' || FormType[0] == 'I') {
					if (i == 0)
						IC += 1;
					else
						SettingsShadows.Interiors.ExcludedForms.push_back(atoi(FormID));
				}
				if (FormType[0] == 'X' || FormType[0] == 'N') {
					if (i == 0)
						NC += 1;
					else
						SettingsShadows.ExteriorsPoint.ExcludedForms.push_back(atoi(FormID));
				}
			}
			pNextSection = pNextSection + strlen(pNextSection) + 1;
		}
	}
	if (EC) std::sort(SettingsShadows.Exteriors.ExcludedForms.begin(), SettingsShadows.Exteriors.ExcludedForms.end());
	if (IC) std::sort(SettingsShadows.ExteriorsPoint.ExcludedForms.begin(), SettingsShadows.ExteriorsPoint.ExcludedForms.end());
	if (IC) std::sort(SettingsShadows.Interiors.ExcludedForms.begin(), SettingsShadows.Interiors.ExcludedForms.end());

}

void SettingManager::SaveSettings(const char* Item, const char* Definition, const char* Section) {

	char Filename[MAX_PATH];

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);

	if (!strcmp(Item, "Main")) {
		if (!strcmp(Definition, "Main")) {
			WritePrivateProfileStringA("Main", "FoV", ToString(SettingsMain.Main.FoV).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "MoonPhaseLumNew", ToString(SettingsMain.Main.MoonPhaseLumNew).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "MoonPhaseLumQtr", ToString(SettingsMain.Main.MoonPhaseLumQtr).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "MoonPhaseLumHalf", ToString(SettingsMain.Main.MoonPhaseLumHalf).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "MoonPhaseLumTQtr", ToString(SettingsMain.Main.MoonPhaseLumTQtr).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "MoonPhaseLumFull", ToString(SettingsMain.Main.MoonPhaseLumFull).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "InteriorDimmerCoeff", ToString(SettingsMain.Main.InteriorDimmerCoeff).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "ScreenshotKey", ToString(SettingsMain.Main.ScreenshotKey).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "FPSOverlay", ToString(SettingsMain.Main.FPSOverlay).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Main", "DirectionalLightOverride", ToString(SettingsMain.Main.DirectionalLightOverride).c_str(), SettingsMain.Main.MainFile);
			//WritePrivateProfileStringA("Main", "RenderEffectsBeforeHdr", ToString(SettingsMain.Main.RenderEffectsBeforeHdr).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("CameraMode", "NearDistanceFirst", ToString(SettingsMain.CameraMode.NearDistanceFirst).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("CameraMode", "NearDistanceThird", ToString(SettingsMain.CameraMode.NearDistanceThird).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("CameraMode", "OffsetX", ToString(SettingsMain.CameraMode.Offset.x).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("CameraMode", "OffsetY", ToString(SettingsMain.CameraMode.Offset.y).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("CameraMode", "OffsetZ", ToString(SettingsMain.CameraMode.Offset.z).c_str(), SettingsMain.Main.MainFile);
#if defined(SKYRIM)
			WritePrivateProfileStringA("ShadowMode", "NearQuality", ToString(SettingsMain.ShadowMode.NearQuality).c_str(), SettingsMain.Main.MainFile);
#endif
			WritePrivateProfileStringA("SleepingMode", "Mode", ToString(SettingsMain.SleepingMode.Mode).c_str(), SettingsMain.Main.MainFile);
			WritePrivateProfileStringA("Menu", "StepValue", ToString(SettingsMain.Menu.StepValue).c_str(), SettingsMain.Main.MainFile);
		}
	}
	else if (!strcmp(Item, "Shader")) {
		if (!strcmp(Definition, "AmbientOcclusion")) {
			WritePrivateProfileStringA("Effects", "AmbientOcclusion", ToString(SettingsMain.Effects.AmbientOcclusion).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "AmbientOcclusion\\AmbientOcclusion.ini");
			SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.begin();
			while (v != SettingsAmbientOcclusion.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "AngleBias", ToString(v->second.AngleBias).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BlurDropThreshold", ToString(v->second.BlurDropThreshold).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BlurRadiusMultiplier", ToString(v->second.BlurRadiusMultiplier).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "ClampStrength", ToString(v->second.ClampStrength).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Enabled", ToString(v->second.Enabled).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "LumThreshold", ToString(v->second.LumThreshold).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "RadiusMultiplier", ToString(v->second.RadiusMultiplier).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Range", ToString(v->second.Range).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "StrengthMultiplier", ToString(v->second.StrengthMultiplier).c_str(), Filename);
				v++;
			}
		}
		if (!strcmp(Definition, "ShadowPointLights")) {

			if (!strcmp(Section, CreateProfileString)) {
				SettingsShadowPointLightsStruct newProfile = SettingsShadowPointLightsStruct();

				if (Player->GetWorldSpace()) {
					newProfile.iShadowLightPoints = SettingsShadowPointLight["DefaultExterior"].iShadowLightPoints;
					newProfile.iShadowCullLightPoints = SettingsShadowPointLight["DefaultExterior"].iShadowCullLightPoints;
					newProfile.fShadowLightRadiusMin = SettingsShadowPointLight["DefaultExterior"].fShadowLightRadiusMin;
					newProfile.fShadowLightRadiusMax = SettingsShadowPointLight["DefaultExterior"].fShadowLightRadiusMax;
					newProfile.fShadowCullLightRadiusMin = SettingsShadowPointLight["DefaultExterior"].fShadowCullLightRadiusMin;
					newProfile.fShadowCullLightRadiusMax = SettingsShadowPointLight["DefaultExterior"].fShadowCullLightRadiusMax;
					newProfile.fShadowObjectScanRadius = SettingsShadowPointLight["DefaultExterior"].fShadowObjectScanRadius;
					newProfile.bEnabled = SettingsShadowPointLight["DefaultExterior"].bEnabled;
					if (!SettingsShadowPointLight.count(Player->GetWorldSpace()->GetEditorName())) {
						SettingsShadowPointLight.insert(std::pair<std::string, SettingsShadowPointLightsStruct>(Player->GetWorldSpace()->GetEditorName(), newProfile));
						MenuManager->ShowMessage("Profile Created.");
					}
					else {
						MenuManager->ShowMessage("Profile already exists.");
					}
				}
				else {
					newProfile.iShadowLightPoints = SettingsShadowPointLight["DefaultInterior"].iShadowLightPoints;
					newProfile.iShadowCullLightPoints = SettingsShadowPointLight["DefaultInterior"].iShadowCullLightPoints;
					newProfile.fShadowLightRadiusMin = SettingsShadowPointLight["DefaultInterior"].fShadowLightRadiusMin;
					newProfile.fShadowLightRadiusMax = SettingsShadowPointLight["DefaultInterior"].fShadowLightRadiusMax;
					newProfile.fShadowCullLightRadiusMin = SettingsShadowPointLight["DefaultInterior"].fShadowCullLightRadiusMin;
					newProfile.fShadowCullLightRadiusMax = SettingsShadowPointLight["DefaultInterior"].fShadowCullLightRadiusMax;
					newProfile.fShadowObjectScanRadius = SettingsShadowPointLight["DefaultInterior"].fShadowObjectScanRadius;
					newProfile.bEnabled = SettingsShadowPointLight["DefaultInterior"].bEnabled;
					if (!SettingsShadowPointLight.count(Player->parentCell->GetEditorName())) {
						SettingsShadowPointLight.insert(std::pair<std::string, SettingsShadowPointLightsStruct>(Player->parentCell->GetEditorName(), newProfile));
						MenuManager->ShowMessage("Profile Created.");
					}
					else {
						MenuManager->ShowMessage("Profile already exists.");
					}
				}
			}
			else {
				strcat(Filename, "Shadows\\ShadowPointLights.ini");
				SettingsShadowPointLightsList::iterator v = SettingsShadowPointLight.begin();
				while (v != SettingsShadowPointLight.end()) {
					if (strcmp(v->first.c_str(), CreateProfileString)) {
						WritePrivateProfileStringA(v->first.c_str(), "iShadowLightPoints", ToString(v->second.iShadowLightPoints).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "iShadowCullLightPoints", ToString(v->second.iShadowCullLightPoints).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "fShadowLightRadiusMin", ToString(v->second.fShadowLightRadiusMin).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "fShadowLightRadiusMax", ToString(v->second.fShadowLightRadiusMax).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "fShadowCullLightRadiusMin", ToString(v->second.fShadowCullLightRadiusMin).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "fShadowCullLightRadiusMax", ToString(v->second.fShadowCullLightRadiusMax).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "fShadowObjectScanRadius", ToString(v->second.fShadowObjectScanRadius).c_str(), Filename);
						WritePrivateProfileStringA(v->first.c_str(), "bEnabled", ToString(v->second.bEnabled).c_str(), Filename);
					}
					v++;
				}
			}


				
		}
		else if (!strcmp(Definition, "Bloom")) {
			WritePrivateProfileStringA("Effects", "Bloom", ToString(SettingsMain.Effects.Bloom).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Bloom\\Bloom.ini");
			SettingsBloomList::iterator v = SettingsBloom.begin();
			while (v != SettingsBloom.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "BloomIntensity", ToString(v->second.BloomIntensity).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BloomSaturation", ToString(v->second.BloomSaturation).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Luminance", ToString(v->second.Luminance).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "MiddleGray", ToString(v->second.MiddleGray).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "OriginalIntensity", ToString(v->second.OriginalIntensity).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "OriginalSaturation", ToString(v->second.OriginalSaturation).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "WhiteCutOff", ToString(v->second.WhiteCutOff).c_str(), Filename);
				v++;
			}

			SettingsBloomStruct newProfile = SettingsBloomStruct();

			newProfile.BloomIntensity = 1.4;
			newProfile.OriginalIntensity = 1;
			newProfile.BloomSaturation = 0.5;
			newProfile.OriginalSaturation = 1;
			newProfile.Luminance = 0.06;
			newProfile.MiddleGray = 0.18;
			newProfile.WhiteCutOff = 0.8;

			if (Player->GetWorldSpace()) {
				SettingsBloom.insert(std::pair<std::string, SettingsBloomStruct>(Player->GetWorldSpace()->GetEditorName(), newProfile));
			}
			else {
				SettingsBloom.insert(std::pair<std::string, SettingsBloomStruct>(Player->parentCell->GetEditorName(), newProfile));
			}
		}
		else if (!strcmp(Definition, "Cinema")) {
			WritePrivateProfileStringA("Effects", "Cinema", ToString(SettingsMain.Effects.Cinema).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Cinema\\Cinema.ini");
			WritePrivateProfileStringA("Default", "AspectRatio", ToString(SettingsCinema.AspectRatio).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Mode", ToString(SettingsCinema.Mode).c_str(), Filename);
			WritePrivateProfileStringA("Default", "VignetteDarkness", ToString(SettingsCinema.VignetteDarkness).c_str(), Filename);
			WritePrivateProfileStringA("Default", "VignetteRadius", ToString(SettingsCinema.VignetteRadius).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ChromaticAberrationPower", ToString(SettingsCinema.ChromaticAberrationPower).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Coloring")) {
			WritePrivateProfileStringA("Effects", "Coloring", ToString(SettingsMain.Effects.Coloring).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Coloring\\Coloring.ini");
			SettingsColoringList::iterator v = SettingsColoring.begin();
			while (v != SettingsColoring.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "BaseGamma", ToString(v->second.BaseGamma).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Bleach", ToString(v->second.Bleach).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BleachLuma", ToString(v->second.BleachLuma).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "ColorCurve", ToString(v->second.ColorCurve).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "ColorCurveB", ToString(v->second.ColorCurveB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "ColorCurveG", ToString(v->second.ColorCurveG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "ColorCurveR", ToString(v->second.ColorCurveR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Contrast", ToString(v->second.Contrast).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "EffectGamma", ToString(v->second.EffectGamma).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "EffectGammaB", ToString(v->second.EffectGammaB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "EffectGammaG", ToString(v->second.EffectGammaG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "EffectGammaR", ToString(v->second.EffectGammaR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Fade", ToString(v->second.Fade).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Linearization", ToString(v->second.Linearization).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Saturation", ToString(v->second.Saturation).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Strength", ToString(v->second.Strength).c_str(), Filename);
				v++;
			}
		}
		else if (!strcmp(Definition, "DepthOfField")) {
			WritePrivateProfileStringA("Effects", "DepthOfField", ToString(SettingsMain.Effects.DepthOfField).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "DepthOfField\\DepthOfField.ini");
			SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.begin();
			while (v != SettingsDepthOfField.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "BaseBlurRadius", ToString(v->second.BaseBlurRadius).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BlurFallOff", ToString(v->second.BlurFallOff).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Mode", ToString(v->second.Mode).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "DiameterRange", ToString(v->second.DiameterRange).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "DistantBlur", ToString(v->second.DistantBlur).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "DistantBlurEndRange", ToString(v->second.DistantBlurEndRange).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "DistantBlurStartRange", ToString(v->second.DistantBlurStartRange).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Enabled", ToString(v->second.Enabled).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "NearBlurCutOff", ToString(v->second.NearBlurCutOff).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Radius", ToString(v->second.Radius).c_str(), Filename);
				v++;
			}
		}
		else if (!strcmp(Definition, "GodRays")) {
			WritePrivateProfileStringA("Effects", "GodRays", ToString(SettingsMain.Effects.GodRays).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "GodRays\\GodRays.ini");
			WritePrivateProfileStringA("Default", "RayDensity", ToString(SettingsGodRays.RayDensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayIntensity", ToString(SettingsGodRays.RayIntensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayLength", ToString(SettingsGodRays.RayLength).c_str(), Filename);
			WritePrivateProfileStringA("Default", "GlobalMultiplier", ToString(SettingsGodRays.GlobalMultiplier).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Saturate", ToString(SettingsGodRays.Saturate).c_str(), Filename);
			WritePrivateProfileStringA("Default", "LightShaftPasses", ToString(SettingsGodRays.LightShaftPasses).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Luminance", ToString(SettingsGodRays.Luminance).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayR", ToString(SettingsGodRays.RayR).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayG", ToString(SettingsGodRays.RayG).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayB", ToString(SettingsGodRays.RayB).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SunGlareEnabled", ToString(SettingsGodRays.SunGlareEnabled).c_str(), Filename);
			WritePrivateProfileStringA("Default", "TimeEnabled", ToString(SettingsGodRays.TimeEnabled).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RayVisibility", ToString(SettingsGodRays.RayVisibility).c_str(), Filename);
		}
		else if (!strcmp(Definition, "KhajiitRays")) {
			WritePrivateProfileStringA("Effects", "KhajiitRays", ToString(SettingsMain.Effects.KhajiitRays).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "KhajiitRays\\KhajiitRays.ini");
			//Masser
			WritePrivateProfileStringA("Default", "MasserRayDensity", ToString(SettingsKhajiitRays.mRayDensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayIntensity", ToString(SettingsKhajiitRays.mRayIntensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayLength", ToString(SettingsKhajiitRays.mRayLength).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserGlobalMultiplier", ToString(SettingsKhajiitRays.mGlobalMultiplier).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserSaturate", ToString(SettingsKhajiitRays.mSaturate).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserLightShaftPasses", ToString(SettingsKhajiitRays.mLightShaftPasses).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserLuminance", ToString(SettingsKhajiitRays.mLuminance).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayR", ToString(SettingsKhajiitRays.mRayR).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayG", ToString(SettingsKhajiitRays.mRayG).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayB", ToString(SettingsKhajiitRays.mRayB).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MasserRayVisibility", ToString(SettingsKhajiitRays.mRayVisibility).c_str(), Filename);
			//Secunda
			WritePrivateProfileStringA("Default", "SecundaRayDensity", ToString(SettingsKhajiitRays.sRayDensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayIntensity", ToString(SettingsKhajiitRays.sRayIntensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayLength", ToString(SettingsKhajiitRays.sRayLength).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaGlobalMultiplier", ToString(SettingsKhajiitRays.sGlobalMultiplier).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaSaturate", ToString(SettingsKhajiitRays.sSaturate).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaLightShaftPasses", ToString(SettingsKhajiitRays.sLightShaftPasses).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaLuminance", ToString(SettingsKhajiitRays.sLuminance).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayR", ToString(SettingsKhajiitRays.sRayR).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayG", ToString(SettingsKhajiitRays.sRayG).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayB", ToString(SettingsKhajiitRays.sRayB).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SecundaRayVisibility", ToString(SettingsKhajiitRays.sRayVisibility).c_str(), Filename);
			//Other
			WritePrivateProfileStringA("Default", "PhaseLumQtr", ToString(SettingsKhajiitRays.phaseLumQtr).c_str(), Filename);
			WritePrivateProfileStringA("Default", "PhaseLumHalf", ToString(SettingsKhajiitRays.phaseLumHalf).c_str(), Filename);
			WritePrivateProfileStringA("Default", "PhaseLumTQtr", ToString(SettingsKhajiitRays.phaseLumTQtr).c_str(), Filename);
			WritePrivateProfileStringA("Default", "PhaseLumFull", ToString(SettingsKhajiitRays.phaseLumFull).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Grass")) {
			strcat(Filename, "Grass\\Grass.ini");
			WritePrivateProfileStringA("Default", "GrassDensity", ToString(SettingsGrass.GrassDensity).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MaxDistance", ToString(SettingsGrass.MaxDistance).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MinDistance", ToString(SettingsGrass.MinDistance).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MinHeight", ToString(SettingsGrass.MinHeight).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ScaleX", ToString(SettingsGrass.ScaleX).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ScaleY", ToString(SettingsGrass.ScaleY).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ScaleZ", ToString(SettingsGrass.ScaleZ).c_str(), Filename);
			WritePrivateProfileStringA("Default", "WindCoefficient", ToString(SettingsGrass.WindCoefficient).c_str(), Filename);
			WritePrivateProfileStringA("Default", "WindEnabled", ToString(SettingsGrass.WindEnabled).c_str(), Filename);
		}
		else if (!strcmp(Definition, "HDR")) {
			strcat(Filename, "HDR\\HDR.ini");
			WritePrivateProfileStringA("Default", "ToneMapping", ToString(SettingsHDR.ToneMapping).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ToneMappingBlur", ToString(SettingsHDR.ToneMappingBlur).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ToneMappingColor", ToString(SettingsHDR.ToneMappingColor).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Linearization", ToString(SettingsHDR.Linearization).c_str(), Filename);
		}
		else if (!strcmp(Definition, "MotionBlur")) {
			WritePrivateProfileStringA("Effects", "MotionBlur", ToString(SettingsMain.Effects.MotionBlur).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "MotionBlur\\MotionBlur.ini");
			SettingsMotionBlurList::iterator v = SettingsMotionBlur.begin();
			while (v != SettingsMotionBlur.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "BloomIntensity", ToString(v->second.BlurCutOff).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BloomSaturation", ToString(v->second.BlurOffsetMax).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Luminance", ToString(v->second.BlurScale).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "MiddleGray", ToString(v->second.Enabled).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "OriginalIntensity", ToString(v->second.GaussianWeight).c_str(), Filename);
				v++;
			}
		}
		else if (!strcmp(Definition, "POM")) {
			strcat(Filename, "POM\\POM.ini");
			WritePrivateProfileStringA("Default", "HeightMapScale", ToString(SettingsPOM.HeightMapScale).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MaxSamples", ToString(SettingsPOM.MaxSamples).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MinSamples", ToString(SettingsPOM.MinSamples).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ShadowSoftening", ToString(SettingsPOM.ShadowSoftening).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Skin")) {
			strcat(Filename, "Skin\\Skin.ini");
			WritePrivateProfileStringA("Default", "UseVanillaShaders", ToString(SettingsSkin.UseVanillaShaders).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Attenuation", ToString(SettingsSkin.Attenuation).c_str(), Filename);
			WritePrivateProfileStringA("Default", "CoeffBlue", ToString(SettingsSkin.CoeffBlue).c_str(), Filename);
			WritePrivateProfileStringA("Default", "CoeffGreen", ToString(SettingsSkin.CoeffGreen).c_str(), Filename);
			WritePrivateProfileStringA("Default", "CoeffRed", ToString(SettingsSkin.CoeffRed).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MaterialThickness", ToString(SettingsSkin.MaterialThickness).c_str(), Filename);
			WritePrivateProfileStringA("Default", "RimScalar", ToString(SettingsSkin.RimScalar).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SpecularPower", ToString(SettingsSkin.SpecularPower).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Shadows")) {
			strcat(Filename, "Shadows\\Shadows.ini");
			WritePrivateProfileStringA("Exteriors", "UsePostProcessing", ToString(SettingsShadows.Exteriors.UsePostProcessing).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "Darkness", ToString(SettingsShadows.Exteriors.Darkness).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "forwardNormBias", ToString(SettingsShadows.Exteriors.forwardNormBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "forwardFarNormBias", ToString(SettingsShadows.Exteriors.forwardFarNormBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "forwardConstBias", ToString(SettingsShadows.Exteriors.forwardConstBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "forwardFarConstBias", ToString(SettingsShadows.Exteriors.forwardFarConstBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "deferredNormBias", ToString(SettingsShadows.Exteriors.deferredNormBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "deferredFarNormBias", ToString(SettingsShadows.Exteriors.deferredFarNormBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "deferredConstBias", ToString(SettingsShadows.Exteriors.deferredConstBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "deferredFarConstBias", ToString(SettingsShadows.Exteriors.deferredFarConstBias).c_str(), Filename);
			WritePrivateProfileStringA("Exteriors", "ShadowMapFarPlane", ToString(SettingsShadows.Exteriors.ShadowMapFarPlane).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsNear", "Enabled", ToString(SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapNear]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsNear", "AlphaEnabled", ToString(SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapNear]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsNear", "ShadowMapRadius", ToString(SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapNear]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsFar", "Enabled", ToString(SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapFar]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsFar", "AlphaEnabled", ToString(SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapFar]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsFar", "ShadowMapRadius", ToString(SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapFar]).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsPoint", "Enabled", ToString(SettingsShadows.ExteriorsPoint.Enabled).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsPoint", "UsePostProcessing", ToString(SettingsShadows.ExteriorsPoint.UsePostProcessing).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsPoint", "AlphaEnabled", ToString(SettingsShadows.ExteriorsPoint.AlphaEnabled).c_str(), Filename);
			WritePrivateProfileStringA("ExteriorsPoint", "Darkness", ToString(SettingsShadows.ExteriorsPoint.Darkness).c_str(), Filename);
			WritePrivateProfileStringA("Interiors", "Enabled", ToString(SettingsShadows.Interiors.Enabled).c_str(), Filename);
			WritePrivateProfileStringA("Interiors", "UsePostProcessing", ToString(SettingsShadows.Interiors.UsePostProcessing).c_str(), Filename);
			WritePrivateProfileStringA("Interiors", "EnableSpecularShadow", ToString(SettingsShadows.Interiors.EnableSpecularShadow).c_str(), Filename);
			WritePrivateProfileStringA("Interiors", "AlphaEnabled", ToString(SettingsShadows.Interiors.AlphaEnabled).c_str(), Filename);
			WritePrivateProfileStringA("Interiors", "Darkness", ToString(SettingsShadows.Interiors.Darkness).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Sharpening")) {
			WritePrivateProfileStringA("Effects", "Sharpening", ToString(SettingsMain.Effects.Sharpening).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Sharpening\\Sharpening.ini");
			WritePrivateProfileStringA("Default", "Strength", ToString(SettingsSharpening.Strength).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Clamp", ToString(SettingsSharpening.Clamp).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Offset", ToString(SettingsSharpening.Offset).c_str(), Filename);
		}
		else if (!strcmp(Definition, "SMAA")) {
			WritePrivateProfileStringA("Effects", "SMAA", ToString(SettingsMain.Effects.SMAA).c_str(), SettingsMain.Main.MainFile);
		}
		else if (!strcmp(Definition, "TAA")) {
			WritePrivateProfileStringA("Effects", "TAA", ToString(SettingsMain.Effects.TAA).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "TAA\\TAA.ini");
			WritePrivateProfileStringA("Default", "BlendWeight", ToString(SettingsTAA.BlendWeight).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ClampRadius", ToString(SettingsTAA.ClampRadius).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Sharpening", ToString(SettingsTAA.Sharpening).c_str(), Filename);
		}
		else if (!strcmp(Definition, "SnowAccumulation")) {
			WritePrivateProfileStringA("Effects", "SnowAccumulation", ToString(SettingsMain.Effects.SnowAccumulation).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Precipitations\\Precipitations.ini");
			WritePrivateProfileStringA("SnowAccumulation", "Increase", ToString(SettingsPrecipitations.SnowAccumulation.Increase).c_str(), Filename);
			WritePrivateProfileStringA("SnowAccumulation", "Decrease", ToString(SettingsPrecipitations.SnowAccumulation.Decrease).c_str(), Filename);
			WritePrivateProfileStringA("SnowAccumulation", "SunPower", ToString(SettingsPrecipitations.SnowAccumulation.SunPower).c_str(), Filename);
			WritePrivateProfileStringA("SnowAccumulation", "Amount", ToString(SettingsPrecipitations.SnowAccumulation.Amount).c_str(), Filename);
			WritePrivateProfileStringA("SnowAccumulation", "BlurNormDropThreshhold", ToString(SettingsPrecipitations.SnowAccumulation.BlurNormDropThreshhold).c_str(), Filename);
			WritePrivateProfileStringA("SnowAccumulation", "BlurRadiusMultiplier", ToString(SettingsPrecipitations.SnowAccumulation.BlurRadiusMultiplier).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Specular")) {
			strcat(Filename, "Specular\\Specular.ini");
			WritePrivateProfileStringA("Default", "SpecularPower", ToString(SettingsSpecular.SpecularPower).c_str(), Filename);
			WritePrivateProfileStringA("Default", "FresnelPowerActors", ToString(SettingsSpecular.FresnelPowerActors).c_str(), Filename);
			WritePrivateProfileStringA("Default", "FresnelPowerObjects", ToString(SettingsSpecular.FresnelPowerObjects).c_str(), Filename);
			WritePrivateProfileStringA("Default", "SunPower", ToString(SettingsSpecular.SunPower).c_str(), Filename);
		}
		else if (!strcmp(Definition, "Terrain")) {
			strcat(Filename, "Terrain\\Terrain.ini");
			WritePrivateProfileStringA("Default", "DistantNoise", ToString(SettingsTerrain.DistantNoise).c_str(), Filename);
			WritePrivateProfileStringA("Default", "DistantSpecular", ToString(SettingsTerrain.DistantSpecular).c_str(), Filename);
			WritePrivateProfileStringA("Default", "MiddleSpecular", ToString(SettingsTerrain.MiddleSpecular).c_str(), Filename);
			WritePrivateProfileStringA("Default", "NearSpecular", ToString(SettingsTerrain.NearSpecular).c_str(), Filename);
		}
		else if (!strcmp(Definition, "VolumetricFog")) {
			WritePrivateProfileStringA("Effects", "VolumetricFog", ToString(SettingsMain.Effects.VolumetricFog).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Fog\\Fog.ini");
			WritePrivateProfileStringA("Default", "Amount", ToString(SettingsVolumetricFog.Amount).c_str(), Filename);
			WritePrivateProfileStringA("Default", "ColorCoeff", ToString(SettingsVolumetricFog.ColorCoeff).c_str(), Filename);
			WritePrivateProfileStringA("Default", "Exponent", ToString(SettingsVolumetricFog.Exponent).c_str(), Filename);
		}
		else if (!strcmp(Definition, "VolumetricLight")) {
			WritePrivateProfileStringA("Effects", "VolumetricLight", ToString(SettingsMain.Effects.VolumetricLight).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "VolumetricLight\\VolumetricLight.ini");
			SettingsVolumetricLightList::iterator v = SettingsVolumetricLight.begin();
			while (v != SettingsVolumetricLight.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunriseR", ToString(v->second.baseSunriseR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunriseG", ToString(v->second.baseSunriseG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunriseB", ToString(v->second.baseSunriseB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseMiddayR", ToString(v->second.baseMiddayR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseMiddayG", ToString(v->second.baseMiddayG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseMiddayB", ToString(v->second.baseMiddayB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunsetR", ToString(v->second.baseSunsetR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunsetG", ToString(v->second.baseSunsetG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseSunsetB", ToString(v->second.baseSunsetB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseNightR", ToString(v->second.baseNightR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseNightG", ToString(v->second.baseNightG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseNightB", ToString(v->second.baseNightB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BaseDistance", ToString(v->second.baseDistance).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunriseR", ToString(v->second.accumSunriseR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunriseG", ToString(v->second.accumSunriseG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunriseB", ToString(v->second.accumSunriseB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumMiddayR", ToString(v->second.accumMiddayR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumMiddayG", ToString(v->second.accumMiddayG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumMiddayB", ToString(v->second.accumMiddayB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunsetR", ToString(v->second.accumSunsetR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunsetG", ToString(v->second.accumSunsetG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumSunsetB", ToString(v->second.accumSunsetB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumNightR", ToString(v->second.accumNightR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumNightG", ToString(v->second.accumNightG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumNightB", ToString(v->second.accumNightB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumDistanceMax", ToString(v->second.accumDistanceMax).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumDistanceMin", ToString(v->second.accumDistanceMin).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumCutOff", ToString(v->second.accumCutOff).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumHeightMax", ToString(v->second.accumHeightMax).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AccumHeightMin", ToString(v->second.accumHeightMin).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "SunScatterR", ToString(v->second.sunScatterR).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "SunScatterG", ToString(v->second.sunScatterG).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "SunScatterB", ToString(v->second.sunScatterB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "FogPower", ToString(v->second.fogPower).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "BlurDistance", ToString(v->second.blurDistance).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "Randomizer", ToString(v->second.randomizer).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "AnimatedFog", ToString(v->second.animatedFogToggle).c_str(), Filename);
				v++;
			}

		}
		else if (!strcmp(Definition, "Underwater")) {
			WritePrivateProfileStringA("Effects", "Underwater", ToString(SettingsMain.Effects.Underwater).c_str(), SettingsMain.Main.MainFile);
		}
		else if (!strcmp(Definition, "Water") || !strcmp(Definition, "Underwater")) {
			strcat(Filename, "Water\\Water.ini");
			SettingsWaterList::iterator v = SettingsWater.begin();
			while (v != SettingsWater.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "causticsStrength", ToString(v->second.causticsStrength).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "causticsStrengthS", ToString(v->second.causticsStrengthS).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "choppiness", ToString(v->second.choppiness).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "depthDarkness", ToString(v->second.depthDarkness).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "reflectivity", ToString(v->second.reflectivity).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "shoreFactor", ToString(v->second.shoreFactor).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "turbidity", ToString(v->second.turbidity).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_B", ToString(v->second.inExtCoeff_B).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_G", ToString(v->second.inExtCoeff_G).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_R", ToString(v->second.inExtCoeff_R).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "inScattCoeff", ToString(v->second.inScattCoeff).c_str(), Filename);
#if defined(OBLIVION)
				WritePrivateProfileStringA(v->first.c_str(), "LODdistance", ToString(v->second.LODdistance).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "MinLOD", ToString(v->second.MinLOD).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "waveSpeed", ToString(v->second.waveSpeed).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "waveWidth", ToString(v->second.waveWidth).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "shoreMovement", ToString(v->second.shoreMovement).c_str(), Filename);
#endif
				v++;
			}
		}
		else if (!strcmp(Definition, "WaterLens")) {
			WritePrivateProfileStringA("Effects", "WaterLens", ToString(SettingsMain.Effects.WaterLens).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Water\\Water.ini");
			SettingsWaterList::iterator v = SettingsWater.begin();
			while (v != SettingsWater.end()) {
				WritePrivateProfileStringA(v->first.c_str(), "LensAmount", ToString(v->second.LensAmount).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "LensTime", ToString(v->second.LensTime).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "LensTimeMultA", ToString(v->second.LensTimeMultA).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "LensTimeMultB", ToString(v->second.LensTimeMultB).c_str(), Filename);
				WritePrivateProfileStringA(v->first.c_str(), "LensViscosity", ToString(v->second.LensViscosity).c_str(), Filename);
				v++;
			}
		}
		else if (!strcmp(Definition, "WetWorld")) {
			WritePrivateProfileStringA("Effects", "WetWorld", ToString(SettingsMain.Effects.WetWorld).c_str(), SettingsMain.Main.MainFile);
			strcat(Filename, "Precipitations\\Precipitations.ini");
			WritePrivateProfileStringA("WetWorld", "Increase", ToString(SettingsPrecipitations.WetWorld.Increase).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "Decrease", ToString(SettingsPrecipitations.WetWorld.Decrease).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "Amount", ToString(SettingsPrecipitations.WetWorld.Amount).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "PuddleCoeff_B", ToString(SettingsPrecipitations.WetWorld.PuddleCoeff_B).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "PuddleCoeff_G", ToString(SettingsPrecipitations.WetWorld.PuddleCoeff_G).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "PuddleCoeff_R", ToString(SettingsPrecipitations.WetWorld.PuddleCoeff_R).c_str(), Filename);
			WritePrivateProfileStringA("WetWorld", "PuddleSpecularMultiplier", ToString(SettingsPrecipitations.WetWorld.PuddleSpecularMultiplier).c_str(), Filename);
		}
	}
	else if (!strcmp(Item, "Weather")) {
		char Values[SettingStringBuffer];
		
		SettingsWeatherStruct* sws = GetSettingsWeather(Definition);
		if (!sws) sws = CreateSettingsWeather(Definition);
		char Section[12];
		for (int x = 0; x < TESWeather::kNumColorTypes; x++) {
			strcpy(Section, "Color");
			strcat(Section, ToString(x).c_str());
			Values[0] = '\0';
			for (int y = 0; y < TESWeather::kNumTimeOfDay; y++) {
				strcat(Values, ToString(sws->Colors[x].colors[y].r).c_str());
				strcat(Values, ",");
				strcat(Values, ToString(sws->Colors[x].colors[y].g).c_str());
				strcat(Values, ",");
				strcat(Values, ToString(sws->Colors[x].colors[y].b).c_str());
				if (y < TESWeather::kNumTimeOfDay - 1) strcat(Values, " ");
			}
			WritePrivateProfileStringA(Definition, Section, Values, SettingsMain.Main.WeatherFile);
		}
		Values[0] = '\0';
		for (int x = 0; x < 14; x++) {
			strcat(Values, ToString(sws->HDR[x]).c_str());
			if (x < 13) strcat(Values, " ");
		}
		WritePrivateProfileStringA(Definition, "HDR", Values, SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "CloudSpeedLower", ToString(sws->CloudSpeedLower).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "CloudSpeedUpper", ToString(sws->CloudSpeedUpper).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "FogFarDay", ToString(sws->FogFarDay).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "FogFarNight", ToString(sws->FogFarNight).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "FogNearDay", ToString(sws->FogNearDay).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "FogNearNight", ToString(sws->FogNearNight).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "SunDamage", ToString(sws->SunDamage).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "SunGlare", ToString(sws->SunGlare).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "TransDelta", ToString(sws->TransDelta).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "WindSpeed", ToString(sws->WindSpeed).c_str(), SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "UpperLayer", sws->UpperLayer, SettingsMain.Main.WeatherFile);
		WritePrivateProfileStringA(Definition, "LowerLayer", sws->LowerLayer, SettingsMain.Main.WeatherFile);
	}

}

DefinitionsList SettingManager::GetMenuItems() {

	DefinitionsList Definitions;

	Definitions["Main"] = "Main";
	Definitions["Shader"] = "Shaders";
	if (TheSettingManager->SettingsMain.WeatherMode.Enabled) Definitions["Weather"] = "Weathers";
	return Definitions;

}

DefinitionsList SettingManager::GetMenuDefinitions(const char* Item) {
	
	DefinitionsList Definitions;
	
	if (!strcmp(Item, "Main")) {
		Definitions["Main"] = "Main";
	}
	else if (!strcmp(Item, "Shader")) {
		Definitions["AmbientOcclusion"] = "Ambient Occlusion";
#if defined(OBLIVION)
		Definitions["Blood"] = "Blood";
#endif
		Definitions["Bloom"] = "Bloom";
		Definitions["Cinema"] = "Cinema";
		Definitions["Coloring"] = "Coloring";
		Definitions["DepthOfField"] = "Depth Of Field";
		Definitions["GodRays"] = "God Rays";
		Definitions["KhajiitRays"] = "Khajiit Rays";
#if defined(OBLIVION)
		Definitions["Grass"] = "Grass";
		Definitions["HDR"] = "High Dynamic Range";
#endif
		Definitions["MotionBlur"] = "Motion Blur";
#if defined(OBLIVION)
		Definitions["POM"] = "Parallax Occlusion Mapping";
#endif
		Definitions["Precipitations"] = "Precipitations";
#if defined(OBLIVION) || defined(NEWVEGAS)
		Definitions["Shadows"] = "Shadows";
		Definitions["ShadowPointLights"] = "ShadowPointLights";
#endif
		Definitions["Sharpening"] = "Sharpening";
		Definitions["SMAA"] = "Subpixel Morphological Anti Aliasing (SMAA)";
		Definitions["TAA"] = "Temporal Anti Aliasing (TAA)";
		Definitions["SnowAccumulation"] = "Snow Accumulation";
		Definitions["Specular"] = "Specular";
#if defined(OBLIVION)
		Definitions["Skin"] = "Skin";
		Definitions["Terrain"] = "Terrain";
#endif
		Definitions["VolumetricFog"] = "Volumetric Fog";
		Definitions["VolumetricLight"] = "Volumetric Light";
		Definitions["Underwater"] = "Underwater";
		Definitions["Water"] = "Water";
		Definitions["WaterLens"] = "Water on Lens";
		Definitions["WetWorld"] = "Wet World";
	}
	else if (!strcmp(Item, "Weather")) {
		#if defined(OBLIVION) || defined(NEWVEGAS)
		for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
			TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
		#elif defined(SKYRIM)		
		TArray<TESWeather*> Weathers = DataHandler->weathers;
		for (UInt32 I = 0; I < Weathers.count; I++) {
			TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
		#endif
			Definitions[Weather->EditorName] = Weather->EditorName;
		}
	}
	return Definitions;

}

SectionsList SettingManager::GetMenuSections(const char* Item, const char* Definition) {
	
	SectionsList Sections;

	if (!strcmp(Item, "Main")) {
		if (!strcmp(Definition, "Main")) {
			Sections[0] = "Main";
			if (SettingsMain.CameraMode.Enabled) Sections[1] = "CameraMode";
#if defined(OBLIVION)
			if (SettingsMain.EquipmentMode.Enabled) Sections[2] = "EquipmentMode";
			Sections[3] = "FrameRate";
			Sections[4] = "Gravity";
			if (SettingsMain.MountedCombat.Enabled) Sections[6] = "MountedCombat";
			Sections[7] = "Purger";
#endif
			if (SettingsMain.SleepingMode.Enabled) Sections[8] = "SleepingMode";
			if (SettingsMain.WeatherMode.Enabled) Sections[9] = "WeatherMode";
			Sections[10] = "Menu";
		}
	}
	else if (!strcmp(Item, "Shader")) {
		if (!strcmp(Definition, "AmbientOcclusion")) {
			SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.begin();
			while (v != SettingsAmbientOcclusion.end()) {
				Sections[(int)v._Ptr] = v->first;
				v++;
			}
		}
		else if (!strcmp(Definition, "Bloom")) {
			int index = 0;
			std::string cell;
			
			if (Player->GetWorldSpace()) {
				cell = Player->GetWorldSpace()->GetEditorName();
			}
			else {
				cell = Player->parentCell->GetEditorName();
			}
			 
			SettingsBloomList::iterator v = SettingsBloom.begin();

			if (SettingsBloom.count(cell)) {
				Sections[index] = cell;
				index++;
			}

			while (v != SettingsBloom.end()) {
				if (cell.compare(v->first) != 0) {
					Sections[index] = v->first;
					index++;
				}
				v++;
			}
		}
		else if (!strcmp(Definition, "ShadowPointLights")) {
			int index = 0;
			std::string cell;

			if (Player->GetWorldSpace()) {
				cell = Player->GetWorldSpace()->GetEditorName();
			}
			else {
				cell = Player->parentCell->GetEditorName();
			}

			SettingsShadowPointLightsList::iterator v = SettingsShadowPointLight.begin();

			if (SettingsShadowPointLight.count(cell)) {
				Sections[index] = cell;
				index++;
			}

			while (v != SettingsShadowPointLight.end()) {
				if (cell.compare(v->first) != 0) {
					Sections[index] = v->first;
					index++;
				}
				v++;
			}
		}
		else if (!strcmp(Definition, "Coloring")) {
			SettingsColoringList::iterator v = SettingsColoring.begin();
			while (v != SettingsColoring.end()) {
				Sections[(int)v._Ptr] = v->first;
				v++;
			}
		}
		else if (!strcmp(Definition, "DepthOfField")) {
			SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.begin();
			while (v != SettingsDepthOfField.end()) {
				Sections[(int)v._Ptr] = v->first;
				v++;
			}
		}
		else if (!strcmp(Definition, "MotionBlur")) {
			SettingsMotionBlurList::iterator v = SettingsMotionBlur.begin();
			while (v != SettingsMotionBlur.end()) {
				Sections[(int)v._Ptr] = v->first;
				v++;
			}
		}
		else if (!strcmp(Definition, "Shadows")) {
			Sections[0] = "Exteriors";
			Sections[1] = "ExteriorsNear";
			Sections[2] = "ExteriorsFar";
			Sections[3] = "ExteriorsPoint";
			Sections[4] = "Interiors";
		}
		else if (!strcmp(Definition, "VolumetricLight")) {
			int index = 0;
			for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
				TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
				Sections[index] = Weather->EditorName;
				index++;
			}
		}
		else if (!strcmp(Definition, "Water")) {
			SettingsWaterList::iterator v = SettingsWater.begin();
			while (v != SettingsWater.end()) {
				Sections[(int)v._Ptr] = v->first;
				v++;
			}
		}
		else if (!strcmp(Definition, "Blood") || !strcmp(Definition, "SMAA"))
			Sections[0] = "No settings to configure";
		else
			Sections[0] = "Default";
	}
	else if (!strcmp(Item, "Weather")) {
		Sections[TESWeather::eColor_Ambient] = "Ambient";
		Sections[TESWeather::eColor_CloudsLower] = "CloudsLower";
		Sections[TESWeather::eColor_CloudsUpper] = "CloudsUpper";
		Sections[TESWeather::eColor_Fog] = "Fog";
		Sections[TESWeather::eColor_Horizon] = "Horizon";
		Sections[TESWeather::eColor_SkyLower] = "SkyLower";
		Sections[TESWeather::eColor_SkyUpper] = "SkyUpper";
		Sections[TESWeather::eColor_Stars] = "Stars";
		Sections[TESWeather::eColor_Sun] = "Sun";
		Sections[TESWeather::eColor_Sunlight] = "Sunlight";
		Sections[10] = "HDR";
		Sections[11] = "Weather";
	}
	return Sections;

}

SettingsList SettingManager::GetMenuSettings(const char* Item, const char* Definition, const char* Section, UInt32 SelectedSectionKey) {
	
	SettingsList Settings;

	if (!strcmp(Item, "Main")) {
		if (!strcmp(Definition, "Main")) {
			if (!strcmp(Section, "Main")) {
				if (SettingsMain.Main.FarPlaneDistance) Settings["FarPlaneDistance"] = SettingsMain.Main.FarPlaneDistance;
				if (SettingsMain.Main.FoV) Settings["FoV"] = SettingsMain.Main.FoV;
				Settings["ScreenshotKey"] = SettingsMain.Main.ScreenshotKey;
				Settings["FPSOverlay"] = SettingsMain.Main.FPSOverlay;
				Settings["DirectionalLightOverride"] = SettingsMain.Main.DirectionalLightOverride;
				//Settings["RenderEffectsBeforeHdr"] = SettingsMain.Main.RenderEffectsBeforeHdr;
				Settings["MoonPhaseLumNew"] = SettingsMain.Main.MoonPhaseLumNew;
				Settings["MoonPhaseLumQtr"] = SettingsMain.Main.MoonPhaseLumQtr;
				Settings["MoonPhaseLumHalf"] = SettingsMain.Main.MoonPhaseLumHalf;
				Settings["MoonPhaseLumTQtr"] = SettingsMain.Main.MoonPhaseLumTQtr;
				Settings["MoonPhaseLumFull"] = SettingsMain.Main.MoonPhaseLumFull;
				Settings["InteriorDimmerCoeff"] = SettingsMain.Main.InteriorDimmerCoeff;				
			}
			else if (!strcmp(Section, "CameraMode")) {
				Settings["NearDistanceFirst"] = SettingsMain.CameraMode.NearDistanceFirst;
				Settings["NearDistanceThird"] = SettingsMain.CameraMode.NearDistanceThird;
				Settings["OffsetX"] = SettingsMain.CameraMode.Offset.x;
				Settings["OffsetY"] = SettingsMain.CameraMode.Offset.y;
				Settings["OffsetZ"] = SettingsMain.CameraMode.Offset.z;
			}
			else if (!strcmp(Section, "EquipmentMode")) {
				Settings["CombatEquipmentKey"] = SettingsMain.EquipmentMode.CombatEquipmentKey;
				Settings["DualBlockDelay"] = SettingsMain.EquipmentMode.DualBlockDelay;
				Settings["ShieldOnBackPosX"] = SettingsMain.EquipmentMode.ShieldOnBackPos.x;
				Settings["ShieldOnBackPosY"] = SettingsMain.EquipmentMode.ShieldOnBackPos.y;
				Settings["ShieldOnBackPosZ"] = SettingsMain.EquipmentMode.ShieldOnBackPos.z;
				Settings["ShieldOnBackRotX"] = SettingsMain.EquipmentMode.ShieldOnBackRot.x;
				Settings["ShieldOnBackRotY"] = SettingsMain.EquipmentMode.ShieldOnBackRot.y;
				Settings["ShieldOnBackRotZ"] = SettingsMain.EquipmentMode.ShieldOnBackRot.z;
				Settings["SleepingEquipment"] = SettingsMain.EquipmentMode.SleepingEquipment;
				Settings["SwimmingEquipment"] = SettingsMain.EquipmentMode.SwimmingEquipment;
				Settings["TorchOnBeltPosX"] = SettingsMain.EquipmentMode.TorchOnBeltPos.x;
				Settings["TorchOnBeltPosY"] = SettingsMain.EquipmentMode.TorchOnBeltPos.y;
				Settings["TorchOnBeltPosZ"] = SettingsMain.EquipmentMode.TorchOnBeltPos.z;
				Settings["TorchOnBeltRotX"] = SettingsMain.EquipmentMode.TorchOnBeltRot.x;
				Settings["TorchOnBeltRotY"] = SettingsMain.EquipmentMode.TorchOnBeltRot.y;
				Settings["TorchOnBeltRotZ"] = SettingsMain.EquipmentMode.TorchOnBeltRot.z;
				Settings["TorchKey"] = SettingsMain.EquipmentMode.TorchKey;
				Settings["WeaponOnBackPosX"] = SettingsMain.EquipmentMode.WeaponOnBackPos.x;
				Settings["WeaponOnBackPosY"] = SettingsMain.EquipmentMode.WeaponOnBackPos.y;
				Settings["WeaponOnBackPosZ"] = SettingsMain.EquipmentMode.WeaponOnBackPos.z;
				Settings["WeaponOnBackRotX"] = SettingsMain.EquipmentMode.WeaponOnBackRot.x;
				Settings["WeaponOnBackRotY"] = SettingsMain.EquipmentMode.WeaponOnBackRot.y;
				Settings["WeaponOnBackRotZ"] = SettingsMain.EquipmentMode.WeaponOnBackRot.z;
			}
			else if (!strcmp(Section, "WeatherMode")) {
				Settings["CoeffFogR"] = SettingsMain.WeatherMode.CoeffFog.x;
				Settings["CoeffFogG"] = SettingsMain.WeatherMode.CoeffFog.y;
				Settings["CoeffFogB"] = SettingsMain.WeatherMode.CoeffFog.z;
				Settings["CoeffNightR"] = SettingsMain.WeatherMode.CoeffNight.x;
				Settings["CoeffNightG"] = SettingsMain.WeatherMode.CoeffNight.y;
				Settings["CoeffNightB"] = SettingsMain.WeatherMode.CoeffNight.z;
				Settings["CoeffSunR"] = SettingsMain.WeatherMode.CoeffSun.x;
				Settings["CoeffSunG"] = SettingsMain.WeatherMode.CoeffSun.y;
				Settings["CoeffSunB"] = SettingsMain.WeatherMode.CoeffSun.z;
			}
			else if (!strcmp(Section, "Gravity")) {
				Settings["Enabled"] = SettingsMain.Gravity.Enabled;
				Settings["Value"] = SettingsMain.Gravity.Value;
			}
			else if (!strcmp(Section, "MountedCombat")) {
				Settings["TwoHandWeaponOnBackPosX"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.x;
				Settings["TwoHandWeaponOnBackPosY"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.y;
				Settings["TwoHandWeaponOnBackPosZ"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.z;
				Settings["TwoHandWeaponOnBackRotX"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.x;
				Settings["TwoHandWeaponOnBackRotY"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.y;
				Settings["TwoHandWeaponOnBackRotZ"] = SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.z;
				if (SettingsMain.EquipmentMode.Enabled) {
					Settings["WeaponOnBackPosX"] = SettingsMain.MountedCombat.WeaponOnBackPos.x;
					Settings["WeaponOnBackPosY"] = SettingsMain.MountedCombat.WeaponOnBackPos.y;
					Settings["WeaponOnBackPosZ"] = SettingsMain.MountedCombat.WeaponOnBackPos.z;
					Settings["WeaponOnBackRotX"] = SettingsMain.MountedCombat.WeaponOnBackRot.x;
					Settings["WeaponOnBackRotY"] = SettingsMain.MountedCombat.WeaponOnBackRot.y;
					Settings["WeaponOnBackRotZ"] = SettingsMain.MountedCombat.WeaponOnBackRot.z;
				}
			}
			else if (!strcmp(Section, "Purger")) {
				Settings["Enabled"] = SettingsMain.Purger.Enabled;
				Settings["Key"] = SettingsMain.Purger.Key;
				Settings["PurgeCells"] = SettingsMain.Purger.PurgeCells;
				Settings["PurgeTextures"] = SettingsMain.Purger.PurgeTextures;
				Settings["Time"] = SettingsMain.Purger.Time;
			}
			else if (!strcmp(Section, "SleepingMode")) {
				Settings["Mode"] = SettingsMain.SleepingMode.Mode;
			}
#if defined(SKYRIM)
			else if (!strcmp(Section, "ShadowMode")) {

				Settings["NearQuality"] = SettingsMain.ShadowMode.NearQuality;
			}
#endif
			else if (!strcmp(Section, "Menu")) {
				Settings["StepValue"] = SettingsMain.Menu.StepValue;
			}
		}
	}
	else if (!strcmp(Item, "Shader")) {
		if (!strcmp(Definition, "AmbientOcclusion")) {
			SettingsAmbientOcclusionStruct* sas = GetSettingsAmbientOcclusion(Section);
			Settings["AngleBias"] = sas->AngleBias;
			Settings["BlurDropThreshold"] = sas->BlurDropThreshold;
			Settings["BlurRadiusMultiplier"] = sas->BlurRadiusMultiplier;
			Settings["ClampStrength"] = sas->ClampStrength;
			Settings["Enabled"] = sas->Enabled;
			Settings["LumThreshold"] = sas->LumThreshold;
			Settings["RadiusMultiplier"] = sas->RadiusMultiplier;
			Settings["Range"] = sas->Range;
			Settings["StrengthMultiplier"] = sas->StrengthMultiplier;
		}
		else if (!strcmp(Definition, "Bloom")) {
			SettingsBloomStruct* sbs = GetSettingsBloom(Section);
			Settings["BloomIntensity"] = sbs->BloomIntensity;
			Settings["BloomSaturation"] = sbs->BloomSaturation;
			Settings["Luminance"] = sbs->Luminance;
			Settings["MiddleGray"] = sbs->MiddleGray;
			Settings["OriginalIntensity"] = sbs->OriginalIntensity;
			Settings["OriginalSaturation"] = sbs->OriginalSaturation;
			Settings["WhiteCutOff"] = sbs->WhiteCutOff;
		}
		else if (!strcmp(Definition, "ShadowPointLights")) {
			SettingsShadowPointLightsStruct* spls = GetSettingsShadowPointLight(Section);
			Settings["iShadowLightPoints"] = spls->iShadowLightPoints;
			Settings["iShadowCullLightPoints"] = spls->iShadowCullLightPoints;
			Settings["fShadowObjectScanRadius"] = spls->fShadowObjectScanRadius;
			Settings["fShadowLightRadiusMin"] = spls->fShadowLightRadiusMin;
			Settings["fShadowLightRadiusMax"] = spls->fShadowLightRadiusMax;
			Settings["fShadowCullLightRadiusMin"] = spls->fShadowCullLightRadiusMin;
			Settings["fShadowCullLightRadiusMax"] = spls->fShadowCullLightRadiusMax;
			Settings["bEnabled"] = spls->bEnabled;
		}
		else if (!strcmp(Definition, "Cinema")) {
			Settings["AspectRatio"] = SettingsCinema.AspectRatio;
			Settings["Mode"] = SettingsCinema.Mode;
			Settings["VignetteDarkness"] = SettingsCinema.VignetteDarkness;
			Settings["VignetteRadius"] = SettingsCinema.VignetteRadius;
			Settings["ChromaticAberrationPower"] = SettingsCinema.ChromaticAberrationPower;
		}
		else if (!strcmp(Definition, "Coloring")) {
			SettingsColoringStruct* scs = GetSettingsColoring(Section);
			Settings["BaseGamma"] = scs->BaseGamma;
			Settings["Bleach"] = scs->Bleach;
			Settings["BleachLuma"] = scs->BleachLuma;
			Settings["ColorCurve"] = scs->ColorCurve;
			Settings["ColorCurveR"] = scs->ColorCurveR;
			Settings["ColorCurveG"] = scs->ColorCurveG;
			Settings["ColorCurveB"] = scs->ColorCurveB;
			Settings["Contrast"] = scs->Contrast;
			Settings["EffectGamma"] = scs->EffectGamma;
			Settings["EffectGammaR"] = scs->EffectGammaR;
			Settings["EffectGammaG"] = scs->EffectGammaG;
			Settings["EffectGammaB"] = scs->EffectGammaB;
			Settings["Fade"] = scs->Fade;
			Settings["Linearization"] = scs->Linearization;
			Settings["Saturation"] = scs->Saturation;
			Settings["Strength"] = scs->Strength;
		}
		else if (!strcmp(Definition, "DepthOfField")) {
			SettingsDepthOfFieldStruct* sds = GetSettingsDepthOfField(Section);
			Settings["BaseBlurRadius"] = sds->BaseBlurRadius;
			Settings["BlurFallOff"] = sds->BlurFallOff;
			Settings["Mode"] = sds->Mode;
			Settings["DiameterRange"] = sds->DiameterRange;
			Settings["DistantBlur"] = sds->DistantBlur;
			Settings["DistantBlurEndRange"] = sds->DistantBlurEndRange;
			Settings["DistantBlurStartRange"] = sds->DistantBlurStartRange;
			Settings["Enabled"] = sds->Enabled;
			Settings["NearBlurCutOff"] = sds->NearBlurCutOff;
			Settings["Radius"] = sds->Radius;
		}
		else if (!strcmp(Definition, "GodRays")) {
			Settings["GlobalMultiplier"] = SettingsGodRays.GlobalMultiplier;
			Settings["LightShaftPasses"] = SettingsGodRays.LightShaftPasses;
			Settings["Luminance"] = SettingsGodRays.Luminance;
			Settings["RayDensity"] = SettingsGodRays.RayDensity;
			Settings["RayIntensity"] = SettingsGodRays.RayIntensity;
			Settings["RayLength"] = SettingsGodRays.RayLength;
			Settings["RayVisibility"] = SettingsGodRays.RayVisibility;
			Settings["RayR"] = SettingsGodRays.RayR;
			Settings["RayG"] = SettingsGodRays.RayG;
			Settings["RayB"] = SettingsGodRays.RayB;
			Settings["Saturate"] = SettingsGodRays.Saturate;
			Settings["SunGlareEnabled"] = SettingsGodRays.SunGlareEnabled;
			Settings["TimeEnabled"] = SettingsGodRays.TimeEnabled;
		}
		else if (!strcmp(Definition, "KhajiitRays")) {
			//Masser
			Settings["MasserGlobalMultiplier"] = SettingsKhajiitRays.mGlobalMultiplier;
			Settings["MasserLightShaftPasses"] = SettingsKhajiitRays.mLightShaftPasses;
			Settings["MasserLuminance"] = SettingsKhajiitRays.mLuminance;
			Settings["MasserRayDensity"] = SettingsKhajiitRays.mRayDensity;
			Settings["MasserRayIntensity"] = SettingsKhajiitRays.mRayIntensity;
			Settings["MasserRayLength"] = SettingsKhajiitRays.mRayLength;
			Settings["MasserRayVisibility"] = SettingsKhajiitRays.mRayVisibility;
			Settings["MasserRayR"] = SettingsKhajiitRays.mRayR;
			Settings["MasserRayG"] = SettingsKhajiitRays.mRayG;
			Settings["MasserRayB"] = SettingsKhajiitRays.mRayB;
			Settings["MasserSaturate"] = SettingsKhajiitRays.mSaturate;
			//Secunda
			Settings["SecundaGlobalMultiplier"] = SettingsKhajiitRays.sGlobalMultiplier;
			Settings["SecundaLightShaftPasses"] = SettingsKhajiitRays.sLightShaftPasses;
			Settings["SecundaLuminance"] = SettingsKhajiitRays.sLuminance;
			Settings["SecundaRayDensity"] = SettingsKhajiitRays.sRayDensity;
			Settings["SecundaRayIntensity"] = SettingsKhajiitRays.sRayIntensity;
			Settings["SecundaRayLength"] = SettingsKhajiitRays.sRayLength;
			Settings["SecundaRayVisibility"] = SettingsKhajiitRays.sRayVisibility;
			Settings["SecundaRayR"] = SettingsKhajiitRays.sRayR;
			Settings["SecundaRayG"] = SettingsKhajiitRays.sRayG;
			Settings["SecundaRayB"] = SettingsKhajiitRays.sRayB;
			Settings["SecundaSaturate"] = SettingsKhajiitRays.sSaturate;
			//Other
			Settings["PhaseLumQtr"] = SettingsKhajiitRays.phaseLumQtr;
			Settings["PhaseLumHalf"] = SettingsKhajiitRays.phaseLumHalf;
			Settings["PhaseLumTQtr"] = SettingsKhajiitRays.phaseLumTQtr;
			Settings["PhaseLumFull"] = SettingsKhajiitRays.phaseLumFull;

		}
		else if (!strcmp(Definition, "Grass")) {
			Settings["GrassDensity"] = SettingsGrass.GrassDensity;
			Settings["MaxDistance"] = SettingsGrass.MaxDistance;
			Settings["MinDistance"] = SettingsGrass.MinDistance;
			Settings["MinHeight"] = SettingsGrass.MinHeight;
			Settings["ScaleX"] = SettingsGrass.ScaleX;
			Settings["ScaleY"] = SettingsGrass.ScaleY;
			Settings["ScaleZ"] = SettingsGrass.ScaleZ;
			Settings["WindCoefficient"] = SettingsGrass.WindCoefficient;
			Settings["WindEnabled"] = SettingsGrass.WindEnabled;
		}
		else if (!strcmp(Definition, "HDR")) {
			Settings["ToneMapping"] = SettingsHDR.ToneMapping;
			Settings["ToneMappingBlur"] = SettingsHDR.ToneMappingBlur;
			Settings["ToneMappingColor"] = SettingsHDR.ToneMappingColor;
			Settings["Linearization"] = SettingsHDR.Linearization;
		}
		else if (!strcmp(Definition, "MotionBlur")) {
			SettingsMotionBlurStruct* sms = GetSettingsMotionBlur(Section);
			Settings["BlurCutOff"] = sms->BlurCutOff;
			Settings["BlurOffsetMax"] = sms->BlurOffsetMax;
			Settings["BlurScale"] = sms->BlurScale;
			Settings["Enabled"] = sms->Enabled;
			Settings["GaussianWeight"] = sms->GaussianWeight;
		}
		else if (!strcmp(Definition, "POM")) {
			Settings["HeightMapScale"] = SettingsPOM.HeightMapScale;
			Settings["MaxSamples"] = SettingsPOM.MaxSamples;
			Settings["MinSamples"] = SettingsPOM.MinSamples;
			Settings["ShadowSoftening"] = SettingsPOM.ShadowSoftening;
		}
		else if (!strcmp(Definition, "Shadows")) {
			if (!strcmp(Section, "Exteriors")) {
				Settings["UsePostProcessing"] = SettingsShadows.Exteriors.UsePostProcessing;
				Settings["Darkness"] = SettingsShadows.Exteriors.Darkness;
				Settings["forwardNormBias"] = SettingsShadows.Exteriors.forwardNormBias;
				Settings["forwardFarNormBias"] = SettingsShadows.Exteriors.forwardFarNormBias;
				Settings["forwardConstBias"] = SettingsShadows.Exteriors.forwardConstBias;
				Settings["forwardFarConstBias"] = SettingsShadows.Exteriors.forwardFarConstBias;
				Settings["deferredNormBias"] = SettingsShadows.Exteriors.deferredNormBias;
				Settings["deferredFarNormBias"] = SettingsShadows.Exteriors.deferredFarNormBias;
				Settings["deferredConstBias"] = SettingsShadows.Exteriors.deferredConstBias;
				Settings["deferredFarConstBias"] = SettingsShadows.Exteriors.deferredFarConstBias;
				Settings["ShadowMapFarPlane"] = SettingsShadows.Exteriors.ShadowMapFarPlane;
			}
			else if (!strcmp(Section, "ExteriorsNear")) {
				Settings["Enabled"] = SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapNear];
				Settings["AlphaEnabled"] = SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapNear];
				Settings["ShadowMapRadius"] = SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapNear];
			}
			else if (!strcmp(Section, "ExteriorsFar")) {
				Settings["Enabled"] = SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapFar];
				Settings["AlphaEnabled"] = SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapFar];
				Settings["ShadowMapRadius"] = SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapFar];
			}
			else if (!strcmp(Section, "Interiors")) {
				Settings["Enabled"] = SettingsShadows.Interiors.Enabled;
				Settings["UsePostProcessing"] = SettingsShadows.Interiors.UsePostProcessing;
				Settings["EnableSpecularShadow"] = SettingsShadows.Interiors.EnableSpecularShadow;
				Settings["AlphaEnabled"] = SettingsShadows.Interiors.AlphaEnabled;
				Settings["Darkness"] = SettingsShadows.Interiors.Darkness;
			}
			else if (!strcmp(Section, "ExteriorsPoint")) {
				Settings["Enabled"] = SettingsShadows.ExteriorsPoint.Enabled;
				Settings["UsePostProcessing"] = SettingsShadows.ExteriorsPoint.UsePostProcessing;
				Settings["AlphaEnabled"] = SettingsShadows.ExteriorsPoint.AlphaEnabled;
				Settings["Darkness"] = SettingsShadows.ExteriorsPoint.Darkness;
			}
		}
		else if (!strcmp(Definition, "Sharpening")) {
			Settings["Strength"] = SettingsSharpening.Strength;
			Settings["Clamp"] = SettingsSharpening.Clamp;
			Settings["Offset"] = SettingsSharpening.Offset;
		}
		else if (!strcmp(Definition, "TAA")) {
			Settings["BlendWeight"] = SettingsTAA.BlendWeight;
			Settings["ClampRadius"] = SettingsTAA.ClampRadius;
			Settings["Sharpening"] = SettingsTAA.Sharpening;
		}
		else if (!strcmp(Definition, "Skin")) {
			Settings["UseVanillaShaders"] = SettingsSkin.UseVanillaShaders;
			Settings["Attenuation"] = SettingsSkin.Attenuation;
			Settings["CoeffRed"] = SettingsSkin.CoeffRed;
			Settings["CoeffGreen"] = SettingsSkin.CoeffGreen;
			Settings["CoeffBlue"] = SettingsSkin.CoeffBlue;
			Settings["MaterialThickness"] = SettingsSkin.MaterialThickness;
			Settings["RimScalar"] = SettingsSkin.RimScalar;
			Settings["SpecularPower"] = SettingsSkin.SpecularPower;
		}
		else if (!strcmp(Definition, "SnowAccumulation")) {
			Settings["Amount"] = SettingsPrecipitations.SnowAccumulation.Amount;
			Settings["BlurNormDropThreshhold"] = SettingsPrecipitations.SnowAccumulation.BlurNormDropThreshhold;
			Settings["BlurRadiusMultiplier"] = SettingsPrecipitations.SnowAccumulation.BlurRadiusMultiplier;
			Settings["Decrease"] = SettingsPrecipitations.SnowAccumulation.Decrease;
			Settings["Increase"] = SettingsPrecipitations.SnowAccumulation.Increase;
			Settings["SunPower"] = SettingsPrecipitations.SnowAccumulation.SunPower;
		}
		else if (!strcmp(Definition, "Specular")) {
			Settings["SpecularPower"] = SettingsSpecular.SpecularPower;
			Settings["FresnelPowerActors"] = SettingsSpecular.FresnelPowerActors;
			Settings["FresnelPowerObjects"] = SettingsSpecular.FresnelPowerObjects;
			Settings["SunPower"] = SettingsSpecular.SunPower;
		}
		else if (!strcmp(Definition, "Terrain")) {
			Settings["DistantNoise"] = SettingsTerrain.DistantNoise;
			Settings["DistantSpecular"] = SettingsTerrain.DistantSpecular;
			Settings["MiddleSpecular"] = SettingsTerrain.MiddleSpecular;
			Settings["NearSpecular"] = SettingsTerrain.NearSpecular;
		}
		else if (!strcmp(Definition, "VolumetricFog")) {
			Settings["Amount"] = SettingsVolumetricFog.Amount;
			Settings["ColorCoeff"] = SettingsVolumetricFog.ColorCoeff;
			Settings["Exponent"] = SettingsVolumetricFog.Exponent;
		}
		else if (!strcmp(Definition, "VolumetricLight")) {
			SettingsVolumetricLightStruct* svls = GetSettingsVolumetricLight(Section);
			if (svls) {
				Settings["BaseSunriseR"] = svls->baseSunriseR;
				Settings["BaseSunriseG"] = svls->baseSunriseG;
				Settings["BaseSunriseB"] = svls->baseSunriseB;
				Settings["BaseMiddayR"] = svls->baseMiddayR;
				Settings["BaseMiddayG"] = svls->baseMiddayG;
				Settings["BaseMiddayB"] = svls->baseMiddayB;
				Settings["BaseSunsetR"] = svls->baseSunsetR;
				Settings["BaseSunsetG"] = svls->baseSunsetG;
				Settings["BaseSunsetB"] = svls->baseSunsetB;
				Settings["BaseNightR"] = svls->baseNightR;
				Settings["BaseNightG"] = svls->baseNightG;
				Settings["BaseNightB"] = svls->baseNightB;
				Settings["BaseDistance"] = svls->baseDistance;
				Settings["AccumSunriseR"] = svls->accumSunriseR;
				Settings["AccumSunriseG"] = svls->accumSunriseG;
				Settings["AccumSunriseB"] = svls->accumSunriseB;
				Settings["AccumMiddayR"] = svls->accumMiddayR;
				Settings["AccumMiddayG"] = svls->accumMiddayG;
				Settings["AccumMiddayB"] = svls->accumMiddayB;
				Settings["AccumSunsetR"] = svls->accumSunsetR;
				Settings["AccumSunsetG"] = svls->accumSunsetG;
				Settings["AccumSunsetB"] = svls->accumSunsetB;
				Settings["AccumNightR"] = svls->accumNightR;
				Settings["AccumNightG"] = svls->accumNightG;
				Settings["AccumNightB"] = svls->accumNightB;
				Settings["AccumDistanceMax"] = svls->accumDistanceMax;
				Settings["AccumDistanceMin"] = svls->accumDistanceMin;
				Settings["AccumCutOff"] = svls->accumCutOff;				
				Settings["AccumHeightMax"] = svls->accumHeightMax;
				Settings["AccumHeightMin"] = svls->accumHeightMin;
				Settings["SunScatterR"] = svls->sunScatterR;
				Settings["SunScatterG"] = svls->sunScatterG;
				Settings["SunScatterB"] = svls->sunScatterB;
				Settings["FogPower"] = svls->fogPower;
				Settings["BlurDistance"] = svls->blurDistance;
				Settings["Randomizer"] = svls->randomizer;
				Settings["AnimatedFog"] = svls->animatedFogToggle;
			}
			else				
				for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
					TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
					SettingsVolumetricLightStruct s;
					if (!strcmp(Section, Weather->EditorName)) {
						Settings["BaseSunriseR"] = s.baseSunriseR = 0.00f;
						Settings["BaseSunriseG"] = s.baseSunriseG = 0.00f;
						Settings["BaseSunriseB"] = s.baseSunriseB = 0.00f;
						Settings["BaseMiddayR"] = s.baseMiddayR = 0.00f;
						Settings["BaseMiddayG"] = s.baseMiddayG = 0.00f;
						Settings["BaseMiddayB"] = s.baseMiddayB = 0.00f;
						Settings["BaseSunsetR"] = s.baseSunsetR = 0.00f;
						Settings["BaseSunsetG"] = s.baseSunsetG = 0.00f;
						Settings["BaseSunsetB"] = s.baseSunsetB = 0.00f;
						Settings["BaseNightR"] = s.baseNightR = 0.00f;
						Settings["BaseNightG"] = s.baseNightG = 0.00f;
						Settings["BaseNightB"] = s.baseNightB = 0.00f;
						Settings["BaseDistance"] = s.baseDistance = 5000.0f;
						Settings["AccumSunriseR"] = s.accumSunriseR = 0.00f;
						Settings["AccumSunriseG"] = s.accumSunriseG = 0.00f;
						Settings["AccumSunriseB"] = s.accumSunriseB = 0.00f;
						Settings["AccumMiddayR"] = s.accumMiddayR = 0.00f;
						Settings["AccumMiddayG"] = s.accumMiddayG = 0.00f;
						Settings["AccumMiddayB"] = s.accumMiddayB = 0.00f;
						Settings["AccumSunsetR"] = s.accumSunsetR = 0.00f;
						Settings["AccumSunsetG"] = s.accumSunsetG = 0.00f;
						Settings["AccumSunsetB"] = s.accumSunsetB = 0.00f;
						Settings["AccumNightR"] = s.accumNightR = 0.00f;
						Settings["AccumNightG"] = s.accumNightG = 0.00f;
						Settings["AccumNightB"] = s.accumNightB = 0.00f;
						Settings["AccumDistanceMax"] = s.accumDistanceMax = 2001.0f;
						Settings["AccumDistanceMin"] = s.accumDistanceMin = 2000.0f;
						Settings["AccumCutOff"] = s.accumCutOff = 8500.0f;
						Settings["AccumHeightMax"] = s.accumHeightMax = 80000.0f;
						Settings["AccumHeightMin"] = s.accumHeightMin = 2500.0f;
						Settings["SunScatterR"] = s.sunScatterR = 0.0f;
						Settings["SunScatterG"] = s.sunScatterG = 0.0f;
						Settings["SunScatterB"] = s.sunScatterB = 0.0f;
						Settings["FogPower"] = s.fogPower = 1.00f;
						Settings["BlurDistance"] = s.blurDistance = 999999.0f;
						Settings["Randomizer"] = s.randomizer = 1.0f;
						Settings["AnimatedFog"] = s.animatedFogToggle = 1.0f;
						SettingsVolumetricLight.insert(std::pair<std::string, SettingsVolumetricLightStruct>(Weather->EditorName, s));
						break;
					};
				}
			}
		else if (!strcmp(Definition, "Water") || !strcmp(Definition, "Underwater")) {
			SettingsWaterStruct* sws = GetSettingsWater(Section);
			Settings["causticsStrength"] = sws->causticsStrength;
#if defined(SKYRIM)
			Settings["causticsStrengthS"] = sws->causticsStrengthS;
#endif
			Settings["choppiness"] = sws->choppiness;
			Settings["depthDarkness"] = sws->depthDarkness;
			Settings["reflectivity"] = sws->reflectivity;
			Settings["shoreFactor"] = sws->shoreFactor;
			Settings["turbidity"] = sws->turbidity;
			Settings["inExtCoeff_R"] = sws->inExtCoeff_R;
			Settings["inExtCoeff_G"] = sws->inExtCoeff_G;
			Settings["inExtCoeff_B"] = sws->inExtCoeff_B;
			Settings["inScattCoeff"] = sws->inScattCoeff;
#if defined(OBLIVION) || defined(NEWVEGAS)
			Settings["LODdistance"] = sws->LODdistance;
			Settings["MinLOD"] = sws->MinLOD;
			Settings["waveSpeed"] = sws->waveSpeed;
			Settings["waveWidth"] = sws->waveWidth;
			Settings["shoreMovement"] = sws->shoreMovement;
#endif
		}
		else if (!strcmp(Definition, "WaterLens")) {
			SettingsWaterStruct* sws = GetSettingsWater(Section);
			Settings["LensAmount"] = sws->LensAmount;
			Settings["LensTime"] = sws->LensTime;
			Settings["LensTimeMultA"] = sws->LensTimeMultA;
			Settings["LensTimeMultB"] = sws->LensTimeMultB;
			Settings["LensViscosity"] = sws->LensViscosity;
		}
		else if (!strcmp(Definition, "WetWorld")) {
			Settings["PuddleCoeff_B"] = SettingsPrecipitations.WetWorld.PuddleCoeff_B;
			Settings["PuddleCoeff_G"] = SettingsPrecipitations.WetWorld.PuddleCoeff_G;
			Settings["PuddleCoeff_R"] = SettingsPrecipitations.WetWorld.PuddleCoeff_R;
			Settings["PuddleSpecularMultiplier"] = SettingsPrecipitations.WetWorld.PuddleSpecularMultiplier;
			Settings["Amount"] = SettingsPrecipitations.WetWorld.Amount;
			Settings["Decrease"] = SettingsPrecipitations.WetWorld.Decrease;
			Settings["Increase"] = SettingsPrecipitations.WetWorld.Increase;
		}
	}
	else if (!strcmp(Item, "Weather")) {
		SettingsWeatherStruct* sws = GetSettingsWeather(Definition);
		if (sws) {
			if (!strcmp(Section, "Weather")) {
				Settings["CloudSpeedLower"] = sws->CloudSpeedLower;
				Settings["CloudSpeedUpper"] = sws->CloudSpeedUpper;
				Settings["FogFarDay"] = sws->FogFarDay;
				Settings["FogFarNight"] = sws->FogFarNight;
				Settings["FogNearDay"] = sws->FogNearDay;
				Settings["FogNearNight"] = sws->FogNearNight;
				Settings["SunDamage"] = sws->SunDamage;
				Settings["SunGlare"] = sws->SunGlare;
				Settings["TransDelta"] = sws->TransDelta;
				Settings["WindSpeed"] = sws->WindSpeed;
			}
			else if (!strcmp(Section, "HDR")) {
				#if defined(OBLIVION)
				Settings["EyeAdaptSpeed"] = sws->HDR[TESWeather::eHDR_EyeAdpat];
				Settings["BlurRadius"] = sws->HDR[TESWeather::eHDR_BlurRadius];
				Settings["BlurPasses"] = sws->HDR[TESWeather::eHDR_BlurPasses];
				Settings["EmissiveMult"] = sws->HDR[TESWeather::eHDR_EmissiveMult];
				Settings["TargetLUM"] = sws->HDR[TESWeather::eHDR_TargetLUM];
				Settings["UpperLUMClamp"] = sws->HDR[TESWeather::eHDR_UpperLUMClamp];
				Settings["BrightScale"] = sws->HDR[TESWeather::eHDR_BrightScale];
				Settings["BrightClamp"] = sws->HDR[TESWeather::eHDR_BrightClamp];
				Settings["LUMRampNoTex"] = sws->HDR[TESWeather::eHDR_LUMRampNoTex];
				Settings["LUMRampMin"] = sws->HDR[TESWeather::eHDR_LUMRampMin];
				Settings["LUMRampMax"] = sws->HDR[TESWeather::eHDR_LUMRampMax];
				Settings["SunlightDimmer"] = sws->HDR[TESWeather::eHDR_SunlightDimmer];
				Settings["GrassDimmer"] = sws->HDR[TESWeather::eHDR_GrassDimmer];
				Settings["TreeDimmer"] = sws->HDR[TESWeather::eHDR_TreeDimmer];
				#endif
			}
			else {
				Settings["DayR"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].r;
				Settings["DayG"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].g;
				Settings["DayB"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].b;
				Settings["NightR"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].r;
				Settings["NightG"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].g;
				Settings["NightB"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].b;
				Settings["SunriseR"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].r;
				Settings["SunriseG"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].g;
				Settings["SunriseB"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].b;
				Settings["SunsetR"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].r;
				Settings["SunsetG"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].g;
				Settings["SunsetB"] = sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].b;
			}
		}
		else {
			#if defined(OBLIVION) || defined(NEWVEGAS)
			for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
				TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
			#elif defined(SKYRIM)		
			TArray<TESWeather*> Weathers = DataHandler->weathers;
			for (UInt32 I = 0; I < Weathers.count; I++) {
				TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
			#endif
				if (!strcmp(Definition, Weather->EditorName)) {
					if (!strcmp(Section, "Weather")) {
						Settings["CloudSpeedLower"] = Weather->cloudSpeedLower;
						Settings["CloudSpeedUpper"] = Weather->cloudSpeedUpper;
						Settings["FogFarDay"] = Weather->WeatherFogFarDay;
						Settings["FogFarNight"] = Weather->WeatherFogFarNight;
						Settings["FogNearDay"] = Weather->WeatherFogNearDay;
						Settings["FogNearNight"] = Weather->WeatherFogNearNight;
						Settings["SunDamage"] = Weather->sunDamage;
						Settings["SunGlare"] = Weather->sunGlare;
						Settings["TransDelta"] = Weather->transDelta;
						Settings["WindSpeed"] = Weather->windSpeed;
					}
					else if (!strcmp(Section, "HDR")) {
						#if defined(OBLIVION)
						Settings["EyeAdaptSpeed"] = Weather->hdrInfo[TESWeather::eHDR_EyeAdpat];
						Settings["BlurRadius"] = Weather->hdrInfo[TESWeather::eHDR_BlurRadius];
						Settings["BlurPasses"] = Weather->hdrInfo[TESWeather::eHDR_BlurPasses];
						Settings["EmissiveMult"] = Weather->hdrInfo[TESWeather::eHDR_EmissiveMult];
						Settings["TargetLUM"] = Weather->hdrInfo[TESWeather::eHDR_TargetLUM];
						Settings["UpperLUMClamp"] = Weather->hdrInfo[TESWeather::eHDR_UpperLUMClamp];
						Settings["BrightScale"] = Weather->hdrInfo[TESWeather::eHDR_BrightScale];
						Settings["BrightClamp"] = Weather->hdrInfo[TESWeather::eHDR_BrightClamp];
						Settings["LUMRampNoTex"] = Weather->hdrInfo[TESWeather::eHDR_LUMRampNoTex];
						Settings["LUMRampMin"] = Weather->hdrInfo[TESWeather::eHDR_LUMRampMin];
						Settings["LUMRampMax"] = Weather->hdrInfo[TESWeather::eHDR_LUMRampMax];
						Settings["SunlightDimmer"] = Weather->hdrInfo[TESWeather::eHDR_SunlightDimmer];
						Settings["GrassDimmer"] = Weather->hdrInfo[TESWeather::eHDR_GrassDimmer];
						Settings["TreeDimmer"] = Weather->hdrInfo[TESWeather::eHDR_TreeDimmer];
						#endif
					}
					else {
						Settings["DayR"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Day].r;
						Settings["DayG"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Day].g;
						Settings["DayB"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Day].b;
						Settings["NightR"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Night].r;
						Settings["NightG"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Night].g;
						Settings["NightB"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Night].b;
						Settings["SunriseR"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].r;
						Settings["SunriseG"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].g;
						Settings["SunriseB"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].b;
						Settings["SunsetR"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunset].r;
						Settings["SunsetG"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunset].g;
						Settings["SunsetB"] = Weather->colorsb[SelectedSectionKey].colors[TESWeather::eTime_Sunset].b;
					}
					break;
				};
			}
		}
	}
	return Settings;

}

void SettingManager::SetMenuSetting(const char* Item, const char* Definition, const char* Section, UInt32 SelectedSectionKey, const char* Setting, float Value) {
	
	if (!strcmp(Item, "Main")) {
		if (!strcmp(Definition, "Main")) {
			if (!strcmp(Section, "Main")) {
				if (!strcmp(Setting, "FoV"))
					SettingsMain.Main.FoV = Value;
				else if (!strcmp(Setting, "ScreenshotKey"))
					SettingsMain.Main.ScreenshotKey = Value;
				else if (!strcmp(Setting, "FPSOverlay"))
					SettingsMain.Main.FPSOverlay = Value;
				else if (!strcmp(Setting, "DirectionalLightOverride"))
					SettingsMain.Main.DirectionalLightOverride = Value;
				/*else if (!strcmp(Setting, "RenderEffectsBeforeHdr"))
					SettingsMain.Main.RenderEffectsBeforeHdr = Value;*/
				else if (!strcmp(Setting, "MoonPhaseLumNew"))
					SettingsMain.Main.MoonPhaseLumNew = Value;
				else if (!strcmp(Setting, "MoonPhaseLumQtr"))
					SettingsMain.Main.MoonPhaseLumQtr = Value;
				else if (!strcmp(Setting, "MoonPhaseLumHalf"))
					SettingsMain.Main.MoonPhaseLumHalf = Value;
				else if (!strcmp(Setting, "MoonPhaseLumTQtr"))
					SettingsMain.Main.MoonPhaseLumTQtr = Value;
				else if (!strcmp(Setting, "MoonPhaseLumFull"))
					SettingsMain.Main.MoonPhaseLumFull = Value;
				else if (!strcmp(Setting, "InteriorDimmerCoeff"))
					SettingsMain.Main.InteriorDimmerCoeff = Value;
			}
			else if (!strcmp(Section, "CameraMode")) {
				if (!strcmp(Setting, "NearDistanceFirst"))
					SettingsMain.CameraMode.NearDistanceFirst = Value;
				else if (!strcmp(Setting, "NearDistanceThird"))
					SettingsMain.CameraMode.NearDistanceThird = Value;
				else if (!strcmp(Setting, "OffsetX"))
					SettingsMain.CameraMode.Offset.x = Value;
				else if (!strcmp(Setting, "OffsetY"))
					SettingsMain.CameraMode.Offset.y = Value;
				else if (!strcmp(Setting, "OffsetZ"))
					SettingsMain.CameraMode.Offset.z = Value;
			}
			else if (!strcmp(Section, "EquipmentMode")) {
				if (!strcmp(Setting, "CombatEquipmentKey"))
					SettingsMain.EquipmentMode.CombatEquipmentKey = Value;
				else if (!strcmp(Setting, "DualBlockDelay"))
					SettingsMain.EquipmentMode.DualBlockDelay = Value;
				else if (!strcmp(Setting, "ShieldOnBackPosX"))
					SettingsMain.EquipmentMode.ShieldOnBackPos.x = Value;
				else if (!strcmp(Setting, "ShieldOnBackPosY"))
					SettingsMain.EquipmentMode.ShieldOnBackPos.y = Value;
				else if (!strcmp(Setting, "ShieldOnBackPosZ"))
					SettingsMain.EquipmentMode.ShieldOnBackPos.z = Value;
				else if (!strcmp(Setting, "ShieldOnBackRotX"))
					SettingsMain.EquipmentMode.ShieldOnBackRot.x = Value;
				else if (!strcmp(Setting, "ShieldOnBackRotY"))
					SettingsMain.EquipmentMode.ShieldOnBackRot.y = Value;
				else if (!strcmp(Setting, "ShieldOnBackRotZ"))
					SettingsMain.EquipmentMode.ShieldOnBackRot.z = Value;
				else if (!strcmp(Setting, "SleepingEquipment"))
					SettingsMain.EquipmentMode.SleepingEquipment = Value;
				else if (!strcmp(Setting, "SwimmingEquipment"))
					SettingsMain.EquipmentMode.SwimmingEquipment = Value;
				else if (!strcmp(Setting, "TorchOnBeltPosX"))
					SettingsMain.EquipmentMode.TorchOnBeltPos.x = Value;
				else if (!strcmp(Setting, "TorchOnBeltPosY"))
					SettingsMain.EquipmentMode.TorchOnBeltPos.y = Value;
				else if (!strcmp(Setting, "TorchOnBeltPosZ"))
					SettingsMain.EquipmentMode.TorchOnBeltPos.z = Value;
				else if (!strcmp(Setting, "TorchOnBeltRotX"))
					SettingsMain.EquipmentMode.TorchOnBeltRot.x = Value;
				else if (!strcmp(Setting, "TorchOnBeltRotY"))
					SettingsMain.EquipmentMode.TorchOnBeltRot.y = Value;
				else if (!strcmp(Setting, "TorchOnBeltRotZ"))
					SettingsMain.EquipmentMode.TorchOnBeltRot.z = Value;
				else if (!strcmp(Setting, "TorchKey"))
					SettingsMain.EquipmentMode.TorchKey = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosX"))
					SettingsMain.EquipmentMode.WeaponOnBackPos.x = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosY"))
					SettingsMain.EquipmentMode.WeaponOnBackPos.y = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosZ"))
					SettingsMain.EquipmentMode.WeaponOnBackPos.z = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotX"))
					SettingsMain.EquipmentMode.WeaponOnBackRot.x = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotY"))
					SettingsMain.EquipmentMode.WeaponOnBackRot.y = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotZ"))
					SettingsMain.EquipmentMode.WeaponOnBackRot.z = Value;
			}
			else if (!strcmp(Section, "WeatherMode")) {
				if (!strcmp(Setting, "CoeffNightR"))
					SettingsMain.WeatherMode.CoeffNight.x = Value;
				else if (!strcmp(Setting, "CoeffNightG"))
					SettingsMain.WeatherMode.CoeffNight.y = Value;
				else if (!strcmp(Setting, "CoeffNightB"))
					SettingsMain.WeatherMode.CoeffNight.z = Value;
				else if (!strcmp(Setting, "CoeffFogR"))
					SettingsMain.WeatherMode.CoeffFog.x = Value;
				else if (!strcmp(Setting, "CoeffFogG"))
					SettingsMain.WeatherMode.CoeffFog.y = Value;
				else if (!strcmp(Setting, "CoeffFogB"))
					SettingsMain.WeatherMode.CoeffFog.z = Value;
				else if (!strcmp(Setting, "CoeffSunR"))
					SettingsMain.WeatherMode.CoeffSun.x = Value;
				else if (!strcmp(Setting, "CoeffSunG"))
					SettingsMain.WeatherMode.CoeffSun.y = Value;
				else if (!strcmp(Setting, "CoeffSunB"))
					SettingsMain.WeatherMode.CoeffSun.z = Value;
				#if defined(OBLIVION) || defined(NEWVEGAS)
				for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
					TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
				#elif defined(SKYRIM)		
				TArray<TESWeather*> Weathers = DataHandler->weathers;
				for (UInt32 I = 0; I < Weathers.count; I++) {
					TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
				#endif
					SetSettingsWeather(Weather);
				}
			}
			else if (!strcmp(Section, "Gravity")) {
				if (!strcmp(Setting, "Enabled"))
					SettingsMain.Gravity.Enabled = Value;
				else if (!strcmp(Setting, "Value"))
					SettingsMain.Gravity.Value = Value;
			}
			else if (!strcmp(Section, "MountedCombat")) {
				if (!strcmp(Setting, "TwoHandWeaponOnBackPosX"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.x = Value;
				else if (!strcmp(Setting, "TwoHandWeaponOnBackPosY"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.y = Value;
				else if (!strcmp(Setting, "TwoHandWeaponOnBackPosZ"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackPos.z = Value;
				else if (!strcmp(Setting, "TwoHandWeaponOnBackRotX"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.x = Value;
				else if (!strcmp(Setting, "TwoHandWeaponOnBackRotY"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.y = Value;
				else if (!strcmp(Setting, "TwoHandWeaponOnBackRotZ"))
					SettingsMain.MountedCombat.TwoHandWeaponOnBackRot.z = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosX"))
					SettingsMain.MountedCombat.WeaponOnBackPos.x = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosY"))
					SettingsMain.MountedCombat.WeaponOnBackPos.y = Value;
				else if (!strcmp(Setting, "WeaponOnBackPosZ"))
					SettingsMain.MountedCombat.WeaponOnBackPos.z = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotX"))
					SettingsMain.MountedCombat.WeaponOnBackRot.x = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotY"))
					SettingsMain.MountedCombat.WeaponOnBackRot.y = Value;
				else if (!strcmp(Setting, "WeaponOnBackRotZ"))
					SettingsMain.MountedCombat.WeaponOnBackRot.z = Value;
			}
			else if (!strcmp(Section, "Purger")) {
				if (!strcmp(Setting, "Enabled"))
					SettingsMain.Purger.Enabled = Value;
				else if (!strcmp(Setting, "Key"))
					SettingsMain.Purger.Key = Value;
				else if (!strcmp(Setting, "PurgeCells"))
					SettingsMain.Purger.PurgeCells = Value;
				else if (!strcmp(Setting, "PurgeTextures"))
					SettingsMain.Purger.PurgeTextures = Value;
				else if (!strcmp(Setting, "Time"))
					SettingsMain.Purger.Time = Value;
			}
			else if (!strcmp(Section, "ShadowMode")) {
				if (!strcmp(Setting, "NearQuality"))
					SettingsMain.ShadowMode.NearQuality = Value;
			}
			else if (!strcmp(Section, "SleepingMode")) {
				SettingsMain.SleepingMode.Mode = Value;
			}
			else if (!strcmp(Section, "Menu")) {
				SettingsMain.Menu.StepValue = Value;
			}
		}
	}
	else if (!strcmp(Item, "Shader")) {
		if (!strcmp(Definition, "AmbientOcclusion")) {
			SettingsAmbientOcclusionStruct* sas = GetSettingsAmbientOcclusion(Section);
			if (!strcmp(Setting, "AngleBias"))
				sas->AngleBias = Value;
			else if (!strcmp(Setting, "BlurDropThreshold"))
				sas->BlurDropThreshold = Value;
			else if (!strcmp(Setting, "BlurRadiusMultiplier"))
				sas->BlurRadiusMultiplier = Value;
			else if (!strcmp(Setting, "ClampStrength"))
				sas->ClampStrength = Value;
			else if (!strcmp(Setting, "Enabled"))
				sas->Enabled = Value;
			else if (!strcmp(Setting, "LumThreshold"))
				sas->LumThreshold = Value;
			else if (!strcmp(Setting, "RadiusMultiplier"))
				sas->RadiusMultiplier = Value;
			else if (!strcmp(Setting, "Range"))
				sas->Range = Value;
			else if (!strcmp(Setting, "StrengthMultiplier"))
				sas->StrengthMultiplier = Value;
		}
		else if (!strcmp(Definition, "Bloom")) {
			SettingsBloomStruct* sbs = GetSettingsBloom(Section);
			if (!strcmp(Setting, "BloomIntensity"))
				sbs->BloomIntensity = Value;
			else if (!strcmp(Setting, "BloomSaturation"))
				sbs->BloomSaturation = Value;
			else if (!strcmp(Setting, "Luminance"))
				sbs->Luminance = Value;
			else if (!strcmp(Setting, "MiddleGray"))
				sbs->MiddleGray = Value;
			else if (!strcmp(Setting, "OriginalIntensity"))
				sbs->OriginalIntensity = Value;
			else if (!strcmp(Setting, "OriginalSaturation"))
				sbs->OriginalSaturation = Value;
			else if (!strcmp(Setting, "WhiteCutOff"))
				sbs->WhiteCutOff = Value;
		}
		else if (!strcmp(Definition, "ShadowPointLights")) {
			SettingsShadowPointLightsStruct* spls = GetSettingsShadowPointLight(Section);
			if (!strcmp(Setting, "iShadowLightPoints"))
				spls->iShadowLightPoints = Value;
			else if (!strcmp(Setting, "iShadowCullLightPoints"))
				spls->iShadowCullLightPoints = Value;
			else if (!strcmp(Setting, "fShadowLightRadiusMin"))
				spls->fShadowLightRadiusMin = Value;
			else if (!strcmp(Setting, "fShadowLightRadiusMax"))
				spls->fShadowLightRadiusMax = Value;
			else if (!strcmp(Setting, "fShadowCullLightRadiusMin"))
				spls->fShadowCullLightRadiusMin = Value;
			else if (!strcmp(Setting, "fShadowCullLightRadiusMax"))
				spls->fShadowCullLightRadiusMax = Value;
			else if (!strcmp(Setting, "fShadowObjectScanRadius"))
				spls->fShadowObjectScanRadius = Value;
			else if (!strcmp(Setting, "bEnabled"))
				spls->bEnabled = Value;

			TheShadowManager->LoadShadowLightPointSettings();
		}
		else if (!strcmp(Definition, "Cinema")) {
			if (!strcmp(Setting, "AspectRatio"))
				SettingsCinema.AspectRatio = Value;
			else if (!strcmp(Setting, "Mode"))
				SettingsCinema.Mode = Value;
			else if (!strcmp(Setting, "VignetteDarkness"))
				SettingsCinema.VignetteDarkness = Value;
			else if (!strcmp(Setting, "VignetteRadius"))
				SettingsCinema.VignetteRadius = Value;
			else if (!strcmp(Setting, "ChromaticAberrationPower"))
				SettingsCinema.ChromaticAberrationPower = Value;
		}
		else if (!strcmp(Definition, "Coloring")) {
			SettingsColoringStruct* scs = GetSettingsColoring(Section);
			if (!strcmp(Setting, "BaseGamma"))
				scs->BaseGamma = Value;
			else if (!strcmp(Setting, "Bleach"))
				scs->Bleach = Value;
			else if (!strcmp(Setting, "BleachLuma"))
				scs->BleachLuma = Value;
			else if (!strcmp(Setting, "ColorCurve"))
				scs->ColorCurve = Value;
			else if (!strcmp(Setting, "ColorCurveB"))
				scs->ColorCurveB = Value;
			else if (!strcmp(Setting, "ColorCurveG"))
				scs->ColorCurveG = Value;
			else if (!strcmp(Setting, "ColorCurveR"))
				scs->ColorCurveR = Value;
			else if (!strcmp(Setting, "Contrast"))
				scs->Contrast = Value;
			else if (!strcmp(Setting, "EffectGamma"))
				scs->EffectGamma = Value;
			else if (!strcmp(Setting, "EffectGammaB"))
				scs->EffectGammaB = Value;
			else if (!strcmp(Setting, "EffectGammaG"))
				scs->EffectGammaG = Value;
			else if (!strcmp(Setting, "EffectGammaR"))
				scs->EffectGammaR = Value;
			else if (!strcmp(Setting, "Fade"))
				scs->Fade = Value;
			else if (!strcmp(Setting, "Linearization"))
				scs->Linearization = Value;
			else if (!strcmp(Setting, "Saturation"))
				scs->Saturation = Value;
			else if (!strcmp(Setting, "Strength"))
				scs->Strength = Value;
		}
		else if (!strcmp(Definition, "DepthOfField")) {
			SettingsDepthOfFieldStruct* sds = GetSettingsDepthOfField(Section);
			if (!strcmp(Setting, "BaseBlurRadius"))
				sds->BaseBlurRadius = Value;
			else if (!strcmp(Setting, "BlurFallOff"))
				sds->BlurFallOff = Value;
			else if (!strcmp(Setting, "Mode"))
				sds->Mode = Value;
			else if (!strcmp(Setting, "DiameterRange"))
				sds->DiameterRange = Value;
			else if (!strcmp(Setting, "DistantBlur"))
				sds->DistantBlur = Value;
			else if (!strcmp(Setting, "DistantBlurEndRange"))
				sds->DistantBlurEndRange = Value;
			else if (!strcmp(Setting, "DistantBlurStartRange"))
				sds->DistantBlurStartRange = Value;
			else if (!strcmp(Setting, "Enabled"))
				sds->Enabled = Value;
			else if (!strcmp(Setting, "NearBlurCutOff"))
				sds->NearBlurCutOff = Value;
			else if (!strcmp(Setting, "Radius"))
				sds->Radius = Value;
		}
		else if (!strcmp(Definition, "GodRays")) {
			if (!strcmp(Setting, "RayDensity"))
				SettingsGodRays.RayDensity = Value;
			else if (!strcmp(Setting, "RayIntensity"))
				SettingsGodRays.RayIntensity = Value;
			else if (!strcmp(Setting, "RayLength"))
				SettingsGodRays.RayLength = Value;
			else if (!strcmp(Setting, "GlobalMultiplier"))
				SettingsGodRays.GlobalMultiplier = Value;
			else if (!strcmp(Setting, "Saturate"))
				SettingsGodRays.Saturate = Value;
			else if (!strcmp(Setting, "LightShaftPasses"))
				SettingsGodRays.LightShaftPasses = Value;
			else if (!strcmp(Setting, "Luminance"))
				SettingsGodRays.Luminance = Value;
			else if (!strcmp(Setting, "RayR"))
				SettingsGodRays.RayR = Value;
			else if (!strcmp(Setting, "RayG"))
				SettingsGodRays.RayG = Value;
			else if (!strcmp(Setting, "RayB"))
				SettingsGodRays.RayB = Value;
			else if (!strcmp(Setting, "TimeEnabled"))
				SettingsGodRays.TimeEnabled = Value;
			else if (!strcmp(Setting, "SunGlareEnabled"))
				SettingsGodRays.SunGlareEnabled = Value;
			else if (!strcmp(Setting, "RayVisibility"))
				SettingsGodRays.RayVisibility = Value;
		}
		else if (!strcmp(Definition, "KhajiitRays")) {
			if (!strcmp(Setting, "MasserRayDensity"))
				SettingsKhajiitRays.mRayDensity = Value;
			else if (!strcmp(Setting, "MasserRayIntensity"))
				SettingsKhajiitRays.mRayIntensity = Value;
			else if (!strcmp(Setting, "MasserRayLength"))
				SettingsKhajiitRays.mRayLength = Value;
			else if (!strcmp(Setting, "MasserGlobalMultiplier"))
				SettingsKhajiitRays.mGlobalMultiplier = Value;
			else if (!strcmp(Setting, "MasserSaturate"))
				SettingsKhajiitRays.mSaturate = Value;
			else if (!strcmp(Setting, "MasserLightShaftPasses"))
				SettingsKhajiitRays.mLightShaftPasses = Value;
			else if (!strcmp(Setting, "MasserLuminance"))
				SettingsKhajiitRays.mLuminance = Value;
			else if (!strcmp(Setting, "MasserRayR"))
				SettingsKhajiitRays.mRayR = Value;
			else if (!strcmp(Setting, "MasserRayG"))
				SettingsKhajiitRays.mRayG = Value;
			else if (!strcmp(Setting, "MasserRayB"))
				SettingsKhajiitRays.mRayB = Value;
			else if (!strcmp(Setting, "MasserRayVisibility"))
				SettingsKhajiitRays.mRayVisibility = Value;
			else if (!strcmp(Setting, "SecundaRayDensity"))
				SettingsKhajiitRays.sRayDensity = Value;
			else if (!strcmp(Setting, "SecundaRayIntensity"))
				SettingsKhajiitRays.sRayIntensity = Value;
			else if (!strcmp(Setting, "SecundaRayLength"))
				SettingsKhajiitRays.sRayLength = Value;
			else if (!strcmp(Setting, "SecundaGlobalMultiplier"))
				SettingsKhajiitRays.sGlobalMultiplier = Value;
			else if (!strcmp(Setting, "SecundaSaturate"))
				SettingsKhajiitRays.sSaturate = Value;
			else if (!strcmp(Setting, "SecundaLightShaftPasses"))
				SettingsKhajiitRays.sLightShaftPasses = Value;
			else if (!strcmp(Setting, "SecundaLuminance"))
				SettingsKhajiitRays.sLuminance = Value;
			else if (!strcmp(Setting, "SecundaRayR"))
				SettingsKhajiitRays.sRayR = Value;
			else if (!strcmp(Setting, "SecundaRayG"))
				SettingsKhajiitRays.sRayG = Value;
			else if (!strcmp(Setting, "SecundaRayB"))
				SettingsKhajiitRays.sRayB = Value;
			else if (!strcmp(Setting, "SecundaRayVisibility"))
				SettingsKhajiitRays.sRayVisibility = Value;
			else if (!strcmp(Setting, "PhaseLumQtr"))
				SettingsKhajiitRays.phaseLumQtr = Value;
			else if (!strcmp(Setting, "PhaseLumHalf"))
				SettingsKhajiitRays.phaseLumHalf = Value;
			else if (!strcmp(Setting, "PhaseLumTQtr"))
				SettingsKhajiitRays.phaseLumTQtr = Value;
			else if (!strcmp(Setting, "PhaseLumFull"))
				SettingsKhajiitRays.phaseLumFull = Value;
		}
		else if (!strcmp(Definition, "Grass")) {
			if (!strcmp(Setting, "GrassDensity"))
				SettingsGrass.GrassDensity = Value;
			else if (!strcmp(Setting, "MaxDistance"))
				SettingsGrass.MaxDistance = Value;
			else if (!strcmp(Setting, "MinDistance"))
				SettingsGrass.MinDistance = Value;
			else if (!strcmp(Setting, "MinHeight"))
				SettingsGrass.MinHeight = Value;
			else if (!strcmp(Setting, "ScaleX"))
				SettingsGrass.ScaleX = Value;
			else if (!strcmp(Setting, "ScaleY"))
				SettingsGrass.ScaleY = Value;
			else if (!strcmp(Setting, "ScaleZ"))
				SettingsGrass.ScaleZ = Value;
			else if (!strcmp(Setting, "WindCoefficient"))
				SettingsGrass.WindCoefficient = Value;
			else if (!strcmp(Setting, "WindEnabled"))
				SettingsGrass.WindEnabled = Value;
		}
		else if (!strcmp(Definition, "HDR")) {
			if (!strcmp(Setting, "ToneMapping"))
				SettingsHDR.ToneMapping = Value;
			else if (!strcmp(Setting, "ToneMappingBlur"))
				SettingsHDR.ToneMappingBlur = Value;
			else if (!strcmp(Setting, "ToneMappingColor"))
				SettingsHDR.ToneMappingColor = Value;
			else if (!strcmp(Setting, "Linearization"))
				SettingsHDR.Linearization = Value;
		}
		else if (!strcmp(Definition, "MotionBlur")) {
			SettingsMotionBlurStruct* sms = GetSettingsMotionBlur(Section);
			if (!strcmp(Setting, "BlurCutOff"))
				sms->BlurCutOff = Value;
			else if (!strcmp(Setting, "BlurOffsetMax"))
				sms->BlurOffsetMax = Value;
			else if (!strcmp(Setting, "BlurScale"))
				sms->BlurScale = Value;
			else if (!strcmp(Setting, "Enabled"))
				sms->Enabled = Value;
			else if (!strcmp(Setting, "GaussianWeight"))
				sms->GaussianWeight = Value;
		}
		else if (!strcmp(Definition, "POM")) {
			if (!strcmp(Setting, "HeightMapScale"))
				SettingsPOM.HeightMapScale = Value;
			else if (!strcmp(Setting, "MaxSamples"))
				SettingsPOM.MaxSamples = Value;
			else if (!strcmp(Setting, "MinSamples"))
				SettingsPOM.MinSamples = Value;
			else if (!strcmp(Setting, "ShadowSoftening"))
				SettingsPOM.ShadowSoftening = Value;
		}
		else if (!strcmp(Definition, "Shadows")) {
			if (!strcmp(Section, "Exteriors")) {
				if (!strcmp(Setting, "Darkness")) {
					SettingsShadows.Exteriors.Darkness = Value;
				}
				else if (!strcmp(Setting, "ShadowMapFarPlane")) {
					SettingsShadows.Exteriors.ShadowMapFarPlane = Value;
				}
				else if (!strcmp(Setting, "forwardNormBias")) {
					SettingsShadows.Exteriors.forwardNormBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.x = Value;
				}
				else if (!strcmp(Setting, "forwardFarNormBias")) {
					SettingsShadows.Exteriors.forwardFarNormBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.y = Value;
				}
				else if (!strcmp(Setting, "forwardConstBias")) {
					SettingsShadows.Exteriors.forwardConstBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.z = Value;
				}
				else if (!strcmp(Setting, "forwardFarConstBias")) {
					SettingsShadows.Exteriors.forwardFarConstBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasForward.w = Value;
				}
				else if (!strcmp(Setting, "deferredNormBias")) {
					SettingsShadows.Exteriors.deferredNormBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.x = Value;
				}
				else if (!strcmp(Setting, "deferredFarNormBias")) {
					SettingsShadows.Exteriors.deferredFarNormBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.y = Value;
				}
				else if (!strcmp(Setting, "deferredConstBias")) {
					SettingsShadows.Exteriors.deferredConstBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.z = Value;
				}
				else if (!strcmp(Setting, "deferredFarConstBias")) {
					SettingsShadows.Exteriors.deferredFarConstBias = Value;
					TheShaderManager->ShaderConst.ShadowMap.ShadowBiasDeferred.w = Value;
				}
				else if (!strcmp(Setting, "UsePostProcessing")) {
					// Special case for forward or post-process shadowing
					SettingsShadows.Exteriors.UsePostProcessing = Value;
					TheShaderManager->SwitchShaderStatus("ShadowsExteriors");
				}
			}
			else if (!strcmp(Section, "ExteriorsNear")) {
				if (!strcmp(Setting, "Enabled"))
					SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapNear] = Value;
				else if (!strcmp(Setting, "AlphaEnabled"))
					SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapNear] = Value;
				else if (!strcmp(Setting, "ShadowMapRadius"))
					SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapNear] = Value;
			}
			else if (!strcmp(Section, "ExteriorsFar")) {
				if (!strcmp(Setting, "Enabled"))
					SettingsShadows.Exteriors.Enabled[ShadowManager::ShadowMapTypeEnum::MapFar] = Value;
				else if (!strcmp(Setting, "AlphaEnabled"))
					SettingsShadows.Exteriors.AlphaEnabled[ShadowManager::ShadowMapTypeEnum::MapFar] = Value;
				else if (!strcmp(Setting, "ShadowMapRadius"))
					SettingsShadows.Exteriors.ShadowMapRadius[ShadowManager::ShadowMapTypeEnum::MapFar] = Value;
			}
			else if (!strcmp(Section, "Interiors")) {
				if (!strcmp(Setting, "Enabled")) {
					SettingsShadows.Interiors.Enabled = Value;
				}
				else if (!strcmp(Setting, "AlphaEnabled")) {
					SettingsShadows.Interiors.AlphaEnabled = Value;
				}
				else if (!strcmp(Setting, "Darkness")) {
					SettingsShadows.Interiors.Darkness = Value;
				}
				else if (!strcmp(Setting, "UsePostProcessing")) {
					// Special case for forward or post-process shadowing
					SettingsShadows.Interiors.UsePostProcessing = Value;
					TheShaderManager->SwitchShaderStatus("ShadowsInteriors");
				}
				else if (!strcmp(Setting, "EnableSpecularShadow")) {
					SettingsShadows.Interiors.EnableSpecularShadow = Value;
					TheShaderManager->SwitchShaderStatus("InteriorSpecularShadow");
				}
			}
			else if (!strcmp(Section, "ExteriorsPoint")) {
				if (!strcmp(Setting, "Enabled")) {
					SettingsShadows.ExteriorsPoint.Enabled = Value;
				}
				else if (!strcmp(Setting, "AlphaEnabled")) {
					SettingsShadows.ExteriorsPoint.AlphaEnabled = Value;
				}
				else if (!strcmp(Setting, "Darkness")) {
					SettingsShadows.ExteriorsPoint.Darkness = Value;
				}
				else if (!strcmp(Setting, "UsePostProcessing")) {
					// Special case for forward or post-process shadowing
					SettingsShadows.ExteriorsPoint.UsePostProcessing = Value;
					TheShaderManager->SwitchShaderStatus("ExteriorsPoint");
				}
			}
		}
		else if (!strcmp(Definition, "Sharpening")) {
			if (!strcmp(Setting, "Clamp"))
				SettingsSharpening.Clamp = Value;
			else if (!strcmp(Setting, "Offset"))
				SettingsSharpening.Offset = Value;
			else if (!strcmp(Setting, "Strength"))
				SettingsSharpening.Strength = Value;
		}
		else if (!strcmp(Definition, "TAA")) {
			if (!strcmp(Setting, "BlendWeight"))
				SettingsTAA.BlendWeight = Value;
			else if (!strcmp(Setting, "ClampRadius"))
				SettingsTAA.ClampRadius = Value;
			else if (!strcmp(Setting, "Sharpening"))
				SettingsTAA.Sharpening = Value;
		}
		else if (!strcmp(Definition, "Skin")) {
			if (!strcmp(Setting, "Attenuation"))
				SettingsSkin.Attenuation = Value;
			else if (!strcmp(Setting, "CoeffBlue"))
				SettingsSkin.CoeffBlue = Value;
			else if (!strcmp(Setting, "CoeffGreen"))
				SettingsSkin.CoeffGreen = Value;
			else if (!strcmp(Setting, "CoeffRed"))
				SettingsSkin.CoeffRed = Value;
			else if (!strcmp(Setting, "MaterialThickness"))
				SettingsSkin.MaterialThickness = Value;
			else if (!strcmp(Setting, "RimScalar"))
				SettingsSkin.RimScalar = Value;
			else if (!strcmp(Setting, "SpecularPower"))
				SettingsSkin.SpecularPower = Value;
			else if (!strcmp(Setting, "UseVanillaShaders")) {
				SettingsSkin.UseVanillaShaders = Value;
				TheShaderManager->SwitchShaderStatus("SkinVanilla");
			}
		}
		else if (!strcmp(Definition, "SnowAccumulation")) {
			if (!strcmp(Setting, "Increase"))
				SettingsPrecipitations.SnowAccumulation.Increase = Value;
			else if (!strcmp(Setting, "Decrease"))
				SettingsPrecipitations.SnowAccumulation.Decrease = Value;
			else if (!strcmp(Setting, "SunPower"))
				SettingsPrecipitations.SnowAccumulation.SunPower = Value;
			else if (!strcmp(Setting, "Amount"))
				SettingsPrecipitations.SnowAccumulation.Amount = Value;
			else if (!strcmp(Setting, "BlurNormDropThreshhold"))
				SettingsPrecipitations.SnowAccumulation.BlurNormDropThreshhold = Value;
			else if (!strcmp(Setting, "BlurRadiusMultiplier"))
				SettingsPrecipitations.SnowAccumulation.BlurRadiusMultiplier = Value;
		}
		else if (!strcmp(Definition, "Specular")) {
			if (!strcmp(Setting, "SpecularPower"))
				SettingsSpecular.SpecularPower = Value;
			else if (!strcmp(Setting, "FresnelPowerActors"))
				SettingsSpecular.FresnelPowerActors = Value;
			else if (!strcmp(Setting, "FresnelPowerObjects"))
				SettingsSpecular.FresnelPowerObjects = Value;
			else if (!strcmp(Setting, "SunPower"))
				SettingsSpecular.SunPower = Value;
		}
		else if (!strcmp(Definition, "Terrain")) {
			if (!strcmp(Setting, "DistantNoise"))
				SettingsTerrain.DistantNoise = Value;
			else if (!strcmp(Setting, "DistantSpecular"))
				SettingsTerrain.DistantSpecular = Value;
			else if (!strcmp(Setting, "MiddleSpecular"))
				SettingsTerrain.MiddleSpecular = Value;
			else if (!strcmp(Setting, "NearSpecular"))
				SettingsTerrain.NearSpecular = Value;
		}
		else if (!strcmp(Definition, "VolumetricFog")) {
			if (!strcmp(Setting, "Amount"))
				SettingsVolumetricFog.Amount = Value;
			else if (!strcmp(Setting, "ColorCoeff"))
				SettingsVolumetricFog.ColorCoeff = Value;
			else if (!strcmp(Setting, "Exponent"))
				SettingsVolumetricFog.Exponent = Value;
		}
		else if (!strcmp(Definition, "VolumetricLight")) {
			SettingsVolumetricLightStruct* svls = GetSettingsVolumetricLight(Section);
			if (!strcmp(Setting, "BaseSunriseR"))
				svls->baseSunriseR = Value;
			else if (!strcmp(Setting, "BaseSunriseG"))
				svls->baseSunriseG = Value;
			else if (!strcmp(Setting, "BaseSunriseB"))
				svls->baseSunriseB = Value;
			else if (!strcmp(Setting, "BaseMiddayR"))
				svls->baseMiddayR = Value;
			else if (!strcmp(Setting, "BaseMiddayG"))
				svls->baseMiddayG = Value;
			else if (!strcmp(Setting, "BaseMiddayB"))
				svls->baseMiddayB = Value;
			else if (!strcmp(Setting, "BaseSunsetR"))
				svls->baseSunsetR = Value;
			else if (!strcmp(Setting, "BaseSunsetG"))
				svls->baseSunsetG = Value;
			else if (!strcmp(Setting, "BaseSunsetB"))
				svls->baseSunsetB = Value;
			else if (!strcmp(Setting, "BaseNightR"))
				svls->baseNightR = Value;
			else if (!strcmp(Setting, "BaseNightG"))
				svls->baseNightG = Value;
			else if (!strcmp(Setting, "BaseNightB"))
				svls->baseNightB = Value;
			else if (!strcmp(Setting, "BaseDistance"))
				svls->baseDistance = Value;
			else if (!strcmp(Setting, "AccumSunriseR"))
				svls->accumSunriseR = Value;
			else if (!strcmp(Setting, "AccumSunriseG"))
				svls->accumSunriseG = Value;
			else if (!strcmp(Setting, "AccumSunriseB"))
				svls->accumSunriseB = Value;
			else if (!strcmp(Setting, "AccumMiddayR"))
				svls->accumMiddayR = Value;
			else if (!strcmp(Setting, "AccumMiddayG"))
				svls->accumMiddayG = Value;
			else if (!strcmp(Setting, "AccumMiddayB"))
				svls->accumMiddayB = Value;
			else if (!strcmp(Setting, "AccumSunsetR"))
				svls->accumSunsetR = Value;
			else if (!strcmp(Setting, "AccumSunsetG"))
				svls->accumSunsetG = Value;
			else if (!strcmp(Setting, "AccumSunsetB"))
				svls->accumSunsetB = Value;
			else if (!strcmp(Setting, "AccumNightR"))
				svls->accumNightR = Value;
			else if (!strcmp(Setting, "AccumNightG"))
				svls->accumNightG = Value;
			else if (!strcmp(Setting, "AccumNightB"))
				svls->accumNightB = Value;
			else if (!strcmp(Setting, "AccumDistanceMax"))
				svls->accumDistanceMax = Value;
			else if (!strcmp(Setting, "AccumDistanceMin"))
				svls->accumDistanceMin = Value;
			else if (!strcmp(Setting, "AccumCutOff"))
				svls->accumCutOff = Value;
			else if (!strcmp(Setting, "AccumHeightMax"))
				svls->accumHeightMax = Value;
			else if (!strcmp(Setting, "AccumHeightMin"))
				svls->accumHeightMin = Value;
			else if (!strcmp(Setting, "SunScatterR"))
				svls->sunScatterR = Value;
			else if (!strcmp(Setting, "SunScatterG"))
				svls->sunScatterG = Value;
			else if (!strcmp(Setting, "SunScatterB"))
				svls->sunScatterB = Value;
			else if (!strcmp(Setting, "FogPower"))
				svls->fogPower = Value;
			else if (!strcmp(Setting, "BlurDistance"))
				svls->blurDistance = Value;
			else if (!strcmp(Setting, "Randomizer"))
				svls->randomizer = Value;
			else if (!strcmp(Setting, "AnimatedFog"))
				svls->animatedFogToggle = Value;
		}
		else if (!strcmp(Definition, "Water") || !strcmp(Definition, "Underwater")) {
			SettingsWaterStruct* sws = GetSettingsWater(Section);
			if (!strcmp(Setting, "causticsStrength"))
				sws->causticsStrength = Value;
			else if (!strcmp(Setting, "causticsStrengthS"))
				sws->causticsStrengthS = Value;
			else if (!strcmp(Setting, "choppiness"))
				sws->choppiness = Value;
			else if (!strcmp(Setting, "depthDarkness"))
				sws->depthDarkness = Value;
			else if (!strcmp(Setting, "inExtCoeff_B"))
				sws->inExtCoeff_B = Value;
			else if (!strcmp(Setting, "inExtCoeff_G"))
				sws->inExtCoeff_G = Value;
			else if (!strcmp(Setting, "inExtCoeff_R"))
				sws->inExtCoeff_R = Value;
			else if (!strcmp(Setting, "inScattCoeff"))
				sws->inScattCoeff = Value;
			else if (!strcmp(Setting, "LODdistance"))
				sws->LODdistance = Value;
			else if (!strcmp(Setting, "MinLOD"))
				sws->MinLOD = Value;
			else if (!strcmp(Setting, "reflectivity"))
				sws->reflectivity = Value;
			else if (!strcmp(Setting, "shoreFactor"))
				sws->shoreFactor = Value;
			else if (!strcmp(Setting, "shoreMovement"))
				sws->shoreMovement = Value;
			else if (!strcmp(Setting, "turbidity"))
				sws->turbidity = Value;
			else if (!strcmp(Setting, "waveSpeed"))
				sws->waveSpeed = Value;
			else if (!strcmp(Setting, "waveWidth"))
				sws->waveWidth = Value;
		}
		else if (!strcmp(Definition, "WaterLens")) {
			SettingsWaterStruct* sws = GetSettingsWater(Section);
			if (!strcmp(Setting, "LensAmount"))
				sws->LensAmount = Value;
			else if (!strcmp(Setting, "LensTime"))
				sws->LensTime = Value;
			else if (!strcmp(Setting, "LensTimeMultA"))
				sws->LensTimeMultA = Value;
			else if (!strcmp(Setting, "LensTimeMultB"))
				sws->LensTimeMultB = Value;
			else if (!strcmp(Setting, "LensViscosity"))
				sws->LensViscosity = Value;
		}
		else if (!strcmp(Definition, "WetWorld")) {
			if (!strcmp(Setting, "Increase"))
				SettingsPrecipitations.WetWorld.Increase = Value;
			else if (!strcmp(Setting, "Decrease"))
				SettingsPrecipitations.WetWorld.Decrease = Value;
			else if (!strcmp(Setting, "Amount"))
				SettingsPrecipitations.WetWorld.Amount = Value;
			else if (!strcmp(Setting, "PuddleCoeff_B"))
				SettingsPrecipitations.WetWorld.PuddleCoeff_B = Value;
			else if (!strcmp(Setting, "PuddleCoeff_G"))
				SettingsPrecipitations.WetWorld.PuddleCoeff_G = Value;
			else if (!strcmp(Setting, "PuddleCoeff_R"))
				SettingsPrecipitations.WetWorld.PuddleCoeff_R = Value;
			else if (!strcmp(Setting, "PuddleSpecularMultiplier"))
				SettingsPrecipitations.WetWorld.PuddleSpecularMultiplier = Value;
		}
	}
	else if (!strcmp(Item, "Weather")) {
		SettingsWeatherStruct* sws = GetSettingsWeather(Definition);
		if (!sws) sws = CreateSettingsWeather(Definition);
		if (!strcmp(Section, "Weather")) {
			if (!strcmp(Setting, "CloudSpeedLower"))
				sws->CloudSpeedLower = Value;
			else if (!strcmp(Setting, "CloudSpeedUpper"))
				sws->CloudSpeedUpper = Value;
			else if (!strcmp(Setting, "FogFarDay"))
				sws->FogFarDay = Value;
			else if (!strcmp(Setting, "FogFarNight"))
				sws->FogFarNight = Value;
			else if (!strcmp(Setting, "FogNearDay"))
				sws->FogNearDay = Value;
			else if (!strcmp(Setting, "FogNearNight"))
				sws->FogNearNight = Value;
			else if (!strcmp(Setting, "SunDamage"))
				sws->SunDamage = Value;
			else if (!strcmp(Setting, "SunGlare"))
				sws->SunGlare = Value;
			else if (!strcmp(Setting, "TransDelta"))
				sws->TransDelta = Value;
			else if (!strcmp(Setting, "WindSpeed"))
				sws->WindSpeed = Value;
		}
		else if (!strcmp(Section, "HDR")) {
			#if defined(OBLIVION)
			if (!strcmp(Setting, "EyeAdaptSpeed"))
				sws->HDR[TESWeather::eHDR_EyeAdpat] = Value;
			else if (!strcmp(Setting, "BlurRadius"))
				sws->HDR[TESWeather::eHDR_BlurRadius] = Value;
			else if (!strcmp(Setting, "BlurPasses"))
				sws->HDR[TESWeather::eHDR_BlurPasses] = Value;
			else if (!strcmp(Setting, "EmissiveMult"))
				sws->HDR[TESWeather::eHDR_EmissiveMult] = Value;
			else if (!strcmp(Setting, "TargetLUM"))
				sws->HDR[TESWeather::eHDR_TargetLUM] = Value;
			else if (!strcmp(Setting, "UpperLUMClamp"))
				sws->HDR[TESWeather::eHDR_UpperLUMClamp] = Value;
			else if (!strcmp(Setting, "BrightScale"))
				sws->HDR[TESWeather::eHDR_BrightScale] = Value;
			else if (!strcmp(Setting, "BrightClamp"))
				sws->HDR[TESWeather::eHDR_BrightClamp] = Value;
			else if (!strcmp(Setting, "LUMRampNoTex"))
				sws->HDR[TESWeather::eHDR_LUMRampNoTex] = Value;
			else if (!strcmp(Setting, "LUMRampMin"))
				sws->HDR[TESWeather::eHDR_LUMRampMin] = Value;
			else if (!strcmp(Setting, "LUMRampMax"))
				sws->HDR[TESWeather::eHDR_LUMRampMax] = Value;
			else if (!strcmp(Setting, "SunlightDimmer"))
				sws->HDR[TESWeather::eHDR_SunlightDimmer] = Value;
			else if (!strcmp(Setting, "GrassDimmer"))
				sws->HDR[TESWeather::eHDR_GrassDimmer] = Value;
			else if (!strcmp(Setting, "TreeDimmer"))
				sws->HDR[TESWeather::eHDR_TreeDimmer] = Value;
			#endif	
		}
		else {
			if (!strcmp(Setting, "DayR"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].r = Value;
			else if (!strcmp(Setting, "DayG"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].g = Value;
			else if (!strcmp(Setting, "DayB"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Day].b = Value;
			else if (!strcmp(Setting, "NightR"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].r = Value;
			else if (!strcmp(Setting, "NightG"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].g = Value;
			else if (!strcmp(Setting, "NightB"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Night].b = Value;
			else if (!strcmp(Setting, "SunriseR"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].r = Value;
			else if (!strcmp(Setting, "SunriseG"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].g = Value;
			else if (!strcmp(Setting, "SunriseB"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunrise].b = Value;
			else if (!strcmp(Setting, "SunsetR"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].r = Value;
			else if (!strcmp(Setting, "SunsetG"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].g = Value;
			else if (!strcmp(Setting, "SunsetB"))
				sws->Colors[SelectedSectionKey].colors[TESWeather::eTime_Sunset].b = Value;
		}
		#if defined(OBLIVION) || defined(NEWVEGAS)
		for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
			TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
		#elif defined(SKYRIM)		
		TArray<TESWeather*> Weathers = DataHandler->weathers;
		for (UInt32 I = 0; I < Weathers.count; I++) {
			TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
		#endif
			if (!strcmp(Definition, Weather->EditorName)) {
				SetSettingsWeather(Weather);
				break;
			}
		}
	}

}

void SettingManager::SetSettingsWeather(TESWeather* Weather) {

	TESWeatherEx* WeatherEx = (TESWeatherEx*)Weather;
	SettingsWeatherStruct* SettingsWeather = GetSettingsWeather(WeatherEx->EditorName);
	
	if (SettingsWeather) {
#if defined(OBLIVION)
		WeatherEx->lowerLayer.ddsPath.Set(SettingsWeather->LowerLayer);
		WeatherEx->upperLayer.ddsPath.Set(SettingsWeather->UpperLayer);
		memcpy(WeatherEx->hdrInfo, SettingsWeather->HDR, 0x38);
#endif
		WeatherEx->windSpeed = SettingsWeather->WindSpeed;
		WeatherEx->cloudSpeedLower = SettingsWeather->CloudSpeedLower;
		WeatherEx->cloudSpeedUpper = SettingsWeather->CloudSpeedUpper;
		WeatherEx->transDelta = SettingsWeather->TransDelta;
		WeatherEx->sunGlare = SettingsWeather->SunGlare;
		WeatherEx->sunDamage = SettingsWeather->SunDamage;
		WeatherEx->WeatherFogNearDay = SettingsWeather->FogNearDay;
		WeatherEx->WeatherFogFarDay = SettingsWeather->FogFarDay;
		WeatherEx->WeatherFogNearNight = SettingsWeather->FogNearNight;
		WeatherEx->WeatherFogFarNight = SettingsWeather->FogFarNight;
		memcpy(WeatherEx->colors, SettingsWeather->Colors, WeatherColorsSize);
		memcpy(WeatherEx->colorsb, SettingsWeather->Colors, WeatherColorsSize);
	}
	for (int c = 0; c < TESWeatherEx::eColor_Lighting; c++) {
		WeatherEx->colors[c].colors[TESWeatherEx::eTime_Night].r = WeatherEx->colorsb[c].colors[TESWeatherEx::eTime_Night].r * SettingsMain.WeatherMode.CoeffNight.x;
		WeatherEx->colors[c].colors[TESWeatherEx::eTime_Night].g = WeatherEx->colorsb[c].colors[TESWeatherEx::eTime_Night].g * SettingsMain.WeatherMode.CoeffNight.y;
		WeatherEx->colors[c].colors[TESWeatherEx::eTime_Night].b = WeatherEx->colorsb[c].colors[TESWeatherEx::eTime_Night].b * SettingsMain.WeatherMode.CoeffNight.z;
	}
	for (int t = 0; t < TESWeatherEx::eTime_Night; t++) {
		WeatherEx->colors[TESWeatherEx::eColor_Fog].colors[t].r = WeatherEx->colorsb[TESWeatherEx::eColor_Fog].colors[t].r * SettingsMain.WeatherMode.CoeffFog.x;
		WeatherEx->colors[TESWeatherEx::eColor_Fog].colors[t].g = WeatherEx->colorsb[TESWeatherEx::eColor_Fog].colors[t].g * SettingsMain.WeatherMode.CoeffFog.y;
		WeatherEx->colors[TESWeatherEx::eColor_Fog].colors[t].b = WeatherEx->colorsb[TESWeatherEx::eColor_Fog].colors[t].b * SettingsMain.WeatherMode.CoeffFog.z;
		WeatherEx->colors[TESWeatherEx::eColor_Sunlight].colors[t].r = WeatherEx->colorsb[TESWeatherEx::eColor_Sunlight].colors[t].r * SettingsMain.WeatherMode.CoeffSun.x;
		WeatherEx->colors[TESWeatherEx::eColor_Sunlight].colors[t].g = WeatherEx->colorsb[TESWeatherEx::eColor_Sunlight].colors[t].g * SettingsMain.WeatherMode.CoeffSun.y;
		WeatherEx->colors[TESWeatherEx::eColor_Sunlight].colors[t].b = WeatherEx->colorsb[TESWeatherEx::eColor_Sunlight].colors[t].b * SettingsMain.WeatherMode.CoeffSun.z;
		WeatherEx->colors[TESWeatherEx::eColor_Sun].colors[t].r = WeatherEx->colorsb[TESWeatherEx::eColor_Sun].colors[t].r * SettingsMain.WeatherMode.CoeffSun.x;
		WeatherEx->colors[TESWeatherEx::eColor_Sun].colors[t].g = WeatherEx->colorsb[TESWeatherEx::eColor_Sun].colors[t].g * SettingsMain.WeatherMode.CoeffSun.y;
		WeatherEx->colors[TESWeatherEx::eColor_Sun].colors[t].b = WeatherEx->colorsb[TESWeatherEx::eColor_Sun].colors[t].b * SettingsMain.WeatherMode.CoeffSun.z;
	}

}

SettingsWeatherStruct* SettingManager::CreateSettingsWeather(const char* WeatherName) {

	SettingsWeatherStruct SE;

	#if defined(OBLIVION) || defined(NEWVEGAS)
	for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
		TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
	#elif defined(SKYRIM)		
	TArray<TESWeather*> Weathers = DataHandler->weathers;
	for (UInt32 I = 0; I < Weathers.count; I++) {
		TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
	#endif
		if (!strcmp(WeatherName, Weather->EditorName)) {
			#if defined(OBLIVION)
			strcpy(SE.LowerLayer, Weather->lowerLayer.ddsPath.m_data);
			strcpy(SE.UpperLayer, Weather->upperLayer.ddsPath.m_data);
			memcpy(SE.HDR, Weather->hdrInfo, 0x38);
			#endif	
			SE.CloudSpeedLower = Weather->cloudSpeedLower;
			SE.CloudSpeedUpper = Weather->cloudSpeedUpper;
			SE.FogFarDay = Weather->WeatherFogFarDay;
			SE.FogFarNight = Weather->WeatherFogFarNight;
			SE.FogNearDay = Weather->WeatherFogNearDay;
			SE.FogNearNight = Weather->WeatherFogNearNight;
			SE.SunDamage = Weather->sunDamage;
			SE.SunGlare = Weather->sunGlare;
			SE.TransDelta = Weather->transDelta;
			SE.WindSpeed = Weather->windSpeed;
			memcpy(SE.Colors, Weather->colorsb, WeatherColorsSize);
			break;
		};
	}
	SettingsWeather[WeatherName] = SE;
	return &SettingsWeather[WeatherName];

}

bool SettingManager::GetMenuShaderEnabled(const char* Name)
{
	bool Value = false;

	if (!strcmp(Name, "AmbientOcclusion"))
		Value = SettingsMain.Effects.AmbientOcclusion;
	else if (!strcmp(Name, "Blood"))
		Value = SettingsMain.Shaders.Blood;
	else if (!strcmp(Name, "Bloom"))
		Value = SettingsMain.Effects.Bloom;
	else if (!strcmp(Name, "Cinema"))
		Value = SettingsMain.Effects.Cinema;
	else if (!strcmp(Name, "Coloring"))
		Value = SettingsMain.Effects.Coloring;
	else if (!strcmp(Name, "DepthOfField"))
		Value = SettingsMain.Effects.DepthOfField;
	else if (!strcmp(Name, "GodRays"))
		Value = SettingsMain.Effects.GodRays;
	else if (!strcmp(Name, "KhajiitRays"))
		Value = SettingsMain.Effects.KhajiitRays;
	else if (!strcmp(Name, "Grass"))
		Value = SettingsMain.Shaders.Grass;
	else if (!strcmp(Name, "HDR"))
		Value = SettingsMain.Shaders.HDR;
	else if (!strcmp(Name, "MotionBlur"))
		Value = SettingsMain.Effects.MotionBlur;
	else if (!strcmp(Name, "POM"))
		Value = SettingsMain.Shaders.POM;
	else if (!strcmp(Name, "Precipitations"))
		Value = SettingsMain.Effects.Precipitations;
	else if (!strcmp(Name, "Shadows"))
		Value = true;
	else if (!strcmp(Name, "Sharpening"))
		Value = SettingsMain.Effects.Sharpening;
	else if (!strcmp(Name, "Skin"))
		Value = SettingsMain.Shaders.Skin;
	else if (!strcmp(Name, "SMAA"))
		Value = SettingsMain.Effects.SMAA;
	else if (!strcmp(Name, "TAA"))
		Value = SettingsMain.Effects.TAA;
	else if (!strcmp(Name, "SnowAccumulation"))
		Value = SettingsMain.Effects.SnowAccumulation;
	else if (!strcmp(Name, "Specular"))
		Value = true;
	else if (!strcmp(Name, "Terrain"))
		Value = SettingsMain.Shaders.Terrain;
	else if (!strcmp(Name, "Underwater"))
		Value = SettingsMain.Effects.Underwater;
	else if (!strcmp(Name, "VolumetricFog"))
		Value = SettingsMain.Effects.VolumetricFog;
	else if (!strcmp(Name, "VolumetricLight"))
		Value = SettingsMain.Effects.VolumetricLight;
	else if (!strcmp(Name, "Water"))
		Value = SettingsMain.Shaders.Water;
	else if (!strcmp(Name, "WaterLens"))
		Value = SettingsMain.Effects.WaterLens;
	else if (!strcmp(Name, "WetWorld"))
		Value = SettingsMain.Effects.WetWorld;
	return Value;

}

SettingsWaterStruct* SettingManager::GetSettingsWater(const char* PlayerLocation) {

	 SettingsWaterList::iterator v = SettingsWater.find(std::string(PlayerLocation));
	 if (v == SettingsWater.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsDepthOfFieldStruct* SettingManager::GetSettingsDepthOfField(const char* Section) {

	SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.find(std::string(Section));
	if (v == SettingsDepthOfField.end())
		return NULL;
	else
		return &v->second;

}

SettingsAmbientOcclusionStruct* SettingManager::GetSettingsAmbientOcclusion(const char* Section) {

	SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.find(std::string(Section));
	if (v == SettingsAmbientOcclusion.end())
		return NULL;
	else
		return &v->second;

}

SettingsColoringStruct* SettingManager::GetSettingsColoring(const char* PlayerLocation) {

	 SettingsColoringList::iterator v = SettingsColoring.find(std::string(PlayerLocation));
	 if (v == SettingsColoring.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsBloomStruct* SettingManager::GetSettingsBloom(const char* PlayerLocation) {

	 SettingsBloomList::iterator v = SettingsBloom.find(std::string(PlayerLocation));
	 if (v == SettingsBloom.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsShadowPointLightsStruct* SettingManager::GetSettingsShadowPointLight(const char* PlayerLocation) {

	SettingsShadowPointLightsList::iterator v = SettingsShadowPointLight.find(std::string(PlayerLocation));
	if (v == SettingsShadowPointLight.end())
		return NULL;
	else
		return &v->second;

}

SettingsMotionBlurStruct* SettingManager::GetSettingsMotionBlur(const char* Section) {

	SettingsMotionBlurList::iterator v = SettingsMotionBlur.find(std::string(Section));
	if (v == SettingsMotionBlur.end())
		return NULL;
	else
		return &v->second;

}

SettingsWeatherStruct* SettingManager::GetSettingsWeather(const char* WeatherName) {

	SettingsWeatherList::iterator v = SettingsWeather.find(std::string(WeatherName));
	if (v == SettingsWeather.end())
		return NULL;
	else
		return &v->second;

}


SettingsVolumetricLightStruct* SettingManager::GetSettingsVolumetricLight(const char* WeatherName) {

	SettingsVolumetricLightList::iterator v = SettingsVolumetricLight.find(std::string(WeatherName));
	if (v == SettingsVolumetricLight.end())
		return NULL;
	else
		return &v->second;

}

struct GameSetting
{
#if defined(SKYRIM) || defined(NEWVEGAS)
	void*	vftbl;
#endif
	union {
		char*	pValue;
		int		iValue;
		float	fValue;
	};
	char*	Name;
};

class Settings {
	
public:
	bool TrackReadSetting(GameSetting* Setting);
	bool TrackSaveSettings();
#if defined(OBLIVION)
	bool TrackLoadGame(BSFile* GameFile, char* FileName, UInt8 Arg3);
#elif defined(SKYRIM) || defined(NEWVEGAS)
	bool TrackLoadGame(char* FileName, UInt8 Arg2);
#endif
};

bool (__thiscall Settings::* ReadSetting)(GameSetting*);
bool (__thiscall Settings::* TrackReadSetting)(GameSetting*);
bool Settings::TrackReadSetting(GameSetting* Setting) {

	bool r = (this->*ReadSetting)(Setting);

	if (!strcmp(Setting->Name, "iLocation X:Display") || !strcmp(Setting->Name, "iLocation Y:Display"))
		Setting->iValue = 0;
	else if (!strcmp(Setting->Name, "bFull Screen:Display"))
		SetWindowedMode(Setting->iValue);
	else if (!strcmp(Setting->Name, "bDoActorShadows:Display") || !strcmp(Setting->Name, "iActorShadowCountExt:Display") || !strcmp(Setting->Name, "iActorShadowCountInt:Display"))
		Setting->iValue = 0;
	else if (!strcmp(Setting->Name, "fDefaultFOV:Display")) {
		TheSettingManager->DefaultFov = Setting->fValue;
	}
#if defined(NEWVEGAS)
	else if (!strcmp(Setting->Name, "bDoCanopyShadowPass:Display"))
		Setting->iValue = 0;
#endif
	return r;

}

bool (__thiscall Settings::* SaveSettings)();
bool (__thiscall Settings::* TrackSaveSettings)();
bool Settings::TrackSaveSettings() {

	bool r = false;

	if (TheSettingManager->SettingsMain.Main.SaveSettings) r = (this->*SaveSettings)();
	return r;

}

#if defined(NEWVEGAS)
bool (__thiscall Settings::* LoadGame)(char*, UInt8);
bool (__thiscall Settings::* TrackLoadGame)(char*, UInt8);
bool Settings::TrackLoadGame(char* FileName, UInt8 Arg2) {

	bool r;

	TheSettingManager->GameLoading = true;
	r = (this->*LoadGame)(FileName, Arg2);
	TheSettingManager->GameLoading = false;
	if (r) TheShaderManager->InitializeConstants();
	return r;

}

void MainMenuMusicFix(char* DestinationBuffer, size_t Size, char* SourceBuffer) {

	char Buffer[64];

	strcpy(Buffer, "Data\\Music\\Special\\");
	strcat(Buffer, MainMenuMusic);
	strcat(Buffer, ".mp3");
	strcpy(DestinationBuffer, Buffer);

}

#elif defined(OBLIVION)
bool(__thiscall Settings::* LoadGame)(BSFile*, char*, UInt8);
bool(__thiscall Settings::* TrackLoadGame)(BSFile*, char*, UInt8);
bool Settings::TrackLoadGame(BSFile* GameFile, char* FileName, UInt8 Arg3) {

	bool r;

	TheSettingManager->GameLoading = true;
	r = (this->*LoadGame)(GameFile, FileName, Arg3);
	TheSettingManager->GameLoading = false;
	if (r) {
		TheShaderManager->InitializeConstants();
		TheShadowManager->ResetIntervals();
	}
	return r;

}
#elif defined(SKYRIM)
bool(__thiscall Settings::* LoadGame)(char*, UInt8);
bool(__thiscall Settings::* TrackLoadGame)(char*, UInt8);
bool Settings::TrackLoadGame(char* FileName, UInt8 Arg2) {

	bool r;

	TheSettingManager->GameLoading = true;
	r = (this->*LoadGame)(FileName, Arg2);
	TheSettingManager->GameLoading = false;
	if (r) TheShaderManager->InitializeConstants();
	return r;

}
#endif

void CreateSettingsHook() {

	*((int*)&ReadSetting) = kReadSetting;
	TrackReadSetting = &Settings::TrackReadSetting;
	*((int*)&LoadGame) = kLoadGame;
	TrackLoadGame = &Settings::TrackLoadGame;
	*((int*)&SaveSettings) = kSaveSettings;
	TrackSaveSettings = &Settings::TrackSaveSettings;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ReadSetting, *((PVOID*)&TrackReadSetting));
	DetourAttach(&(PVOID&)LoadGame, *((PVOID*)&TrackLoadGame));
	DetourAttach(&(PVOID&)SaveSettings, *((PVOID*)&TrackSaveSettings));
	DetourTransactionCommit();

#if defined(NEWVEGAS)
	WriteRelCall(0x007D0B48, (UInt32)MainMenuMusicFix);
	WriteRelCall(0x007D6CE7, (UInt32)MainMenuMusicFix);
#endif

}