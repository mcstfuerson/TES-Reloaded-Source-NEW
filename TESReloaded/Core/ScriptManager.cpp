#include "ScriptManager.h"

#if defined(NEWVEGAS)
#define HavokWorldM 0x011CA0D8
#define CellHavok (ExtraHavok*)CurrentCell->extraDataList.GetExtraData(BSExtraData::ExtraDataType::kExtraData_Havok)
#define IsMoving Player->actorMover->GetMovementFlags() & 0x800
#elif defined(OBLIVION)
#define HavokWorldM 0x00B35C24
#define CellHavok (ExtraHavok*)CurrentCell->extraDataList.GetExtraData(BSExtraData::ExtraDataType::kExtraData_Havok)
#define IsMoving Player->process->GetMovementFlags() & 0x800
#elif defined(SKYRIM)
// NOTE: THE SCRIPTMANAGER IS NOT USED IN SKYRIM, FUNCTIONS ARE DECLARED ONLY FOR COMPATIBILITY
#define HavokWorldM 0x00000000
#define CellHavok (ExtraHavok*)CurrentCell->extraDataList->GetByType(BSExtraData::ExtraDataType::kExtraData_Havok)
#define IsMoving 0
#define process processManager->process
#endif

BaseScript::BaseScript() {

	ElapsedTime = 0.0f;

}

PurgerScript::PurgerScript() { }

void PurgerScript::Run() {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
		int PurgerTime = TheSettingManager->SettingsMain.Purger.Time;
		bool PurgerKeyPressed = TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Purger.Key);

		if (PurgerTime || PurgerKeyPressed) {
			ElapsedTime += TheFrameRateManager->ElapsedTime;
			if (ElapsedTime >= PurgerTime || PurgerKeyPressed) {
				if (TheSettingManager->SettingsMain.Purger.PurgeTextures) { TheRenderManager->device->EvictManagedResources(); }
				if (TheSettingManager->SettingsMain.Purger.PurgeCells) { Tes->PurgeCells(); Global->PurgeModels(); }
				ElapsedTime = 0.0f;
			}
		}
	}

}

class ScriptHook {
public:
	void TrackRunScripts();
};

void (__thiscall ScriptHook::* RunScripts)();
void (__thiscall ScriptHook::* TrackRunScripts)();
void ScriptHook::TrackRunScripts() {

	(this->*RunScripts)();
	TheScriptManager->Run();

}

ScriptManager::ScriptManager() {

	Logger::Log("Starting the script manager...");
	TheScriptManager = this;

	Purger = new PurgerScript();

}

void ScriptManager::Run() {

	if (TheSettingManager->SettingsMain.Purger.Enabled) Purger->Run();

}

void CreateScriptHook() {

	*((int *)&RunScripts)	= 0x004402F0;
	TrackRunScripts			= &ScriptHook::TrackRunScripts;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RunScripts,	*((PVOID *)&TrackRunScripts));
	DetourTransactionCommit();

}