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

LowHSoundScript::LowHSoundScript() {

	HeartSlow = NULL;
	ElapsedTime = -1.0f;

}

void LowHSoundScript::Run() {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
		if (TheShaderManager->ShaderConst.LowHF.Data.x || ElapsedTime != -1.0f) {
			float HealthCoeff = TheShaderManager->ShaderConst.LowHF.HealthCoeff;

			if (Player->GetLifeState(0)) {
				if (HealthCoeff) {
					if (ElapsedTime == -1.0f && HealthCoeff >= 0.5) {
						Global->GetSound()->Play(HeartSlow);
						ElapsedTime = 0.0f;
					}
					else if (ElapsedTime >= 0.0f) {
						ElapsedTime += TheFrameRateManager->ElapsedTime;
						if (ElapsedTime >= 1.5f) ElapsedTime = -1.0f;
					}
				}
			}
		}
	}

}

LowFSoundScript::LowFSoundScript() {

	BreathingF = NULL;
	BreathingM = NULL;
	ElapsedTime = -1.0f;

}

void LowFSoundScript::Run() {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
		if (TheShaderManager->ShaderConst.LowHF.Data.x || ElapsedTime != -1.0f) {
			float FatigueCoeff = TheShaderManager->ShaderConst.LowHF.FatigueCoeff;

			if (Player->GetLifeState(0)) {
				if (FatigueCoeff) {
					if (ElapsedTime == -1.0f && FatigueCoeff >= 0.5) {
						if (Player->IsFemale()) {
							Global->GetSound()->Play(BreathingF);
							BreathingTime = 8.0f;
						}
						else {
							Global->GetSound()->Play(BreathingM);
							BreathingTime = 1.0f;
						}
						ElapsedTime = 0.0f;
					}
					else if (ElapsedTime >= 0.0f) {
						ElapsedTime += TheFrameRateManager->ElapsedTime;
						if (ElapsedTime >= BreathingTime) ElapsedTime = -1.0f;
					}
				}
			}
		}
	}

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

GravityScript::GravityScript() { }

void GravityScript::Run() {

	bhkWorld* HavokWorld = NULL;
	TESObjectCELL* CurrentCell = Player->parentCell;

	if (CurrentCell) {
		ExtraHavok* extraHavok = CellHavok;
		if (extraHavok) HavokWorld = extraHavok->world;
	}
	if (!HavokWorld) HavokWorld = *((bhkWorld**)HavokWorldM);
	HavokWorld->GetWorld()->gravity.z = -17.0f * TheSettingManager->SettingsMain.Gravity.Value;

}

void EquipmentSetupChoice() {

	TheScriptManager->EquipmentSetup->ConfigStep = (EquipmentSetupScript::StepType)(MenuManager->GetMessageBoxButton() + EquipmentSetupScript::StepType::Normal);
	if (TheScriptManager->EquipmentSetup->ConfigStep <= EquipmentSetupScript::StepType::Swimming)
		TheScriptManager->EquipmentSetup->EquipItems(EquipmentSetupScript::StepType::Normal, TheScriptManager->EquipmentSetup->ConfigStep);
	else
		TheScriptManager->EquipmentSetup->ConfigStep = EquipmentSetupScript::StepType::None;

}

EquipmentSetupScript::EquipmentSetupScript() {

	ConfigStep = None;
	GameStep = Normal;
	CombatState = 0;
	for (int i = 0; i < 5; i++) LeftWeapon[i] = NULL;

}

void EquipmentSetupScript::Run() {

	if (ConfigStep == None && MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
		StepType CurrentStep = GetCurrentEquipmentType();
		if (CurrentStep != GameStep) {
			if (CurrentStep == Normal || CurrentStep == Combat || (TheSettingManager->SettingsMain.EquipmentMode.SleepingEquipment && CurrentStep == Sleeping) || (TheSettingManager->SettingsMain.EquipmentMode.SwimmingEquipment && CurrentStep == Swimming)) {
				EquipItems(GameStep, CurrentStep);
				GameStep = CurrentStep;
			}
		}
	}
	else if (ConfigStep == Request) {
		if (GameStep == EquipmentSetupScript::StepType::Normal && !CombatState) {
			ConfigStep = Choice;
			MenuManager->ShowMessageBox("Equipment setup", EquipmentSetupChoice, "Combat", "Sleep", "Swim", "Cancel");
		}
		else {
			ConfigStep = EquipmentSetupScript::StepType::None;
			MenuManager->ShowMessage("You cannot use the equipment menu now.");
		}
	}
	else if (ConfigStep >= Combat && MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
		EquipItems(ConfigStep, Normal);
		ConfigStep = None;
	}

}

void EquipmentSetupScript::EquipItems(StepType From, StepType To) {

	HighProcessEx* Process = (HighProcessEx*)Player->process;
	InventoryChanges::Data* InventoryChangesData = Player->extraDataList.GetInventoryChangesData();
	InventoryChanges::EntryData* Data = NULL;
	ExtraDataList* DataList = NULL;
	TList<ExtraDataList>* ExtendData = NULL;
	bool Loop = true;

	Items[From - Normal].clear();
	while (Loop) { // InventoryChanges is re-indexed for each unequipping. Extra loop needed.
		Loop = false;
		if (InventoryChangesData) {
			for (TList<InventoryChanges::EntryData>::Iterator ItrE = InventoryChangesData->data->Begin(); !ItrE.End(); ++ItrE) {
				Data = ItrE.Get();
				if (Data && (ExtendData = Data->extendData)) {
					for (TList<ExtraDataList>::Iterator Itr = ExtendData->Begin(); !Itr.End(); ++Itr) {
						DataList = Itr.Get();
						if (DataList && DataList->IsWorn()) {
							TESForm* Form = Data->type;
							Items[From - Normal].push_back(Form);
							if (Form->formType == TESForm::FormType::kFormType_Weapon && DataList->IsWorn(1)) LeftWeapon[From - Normal] = (TESObjectWEAP*)Form;
							Player->UnequipItem(Form, 1, DataList);
							Loop = true;
							break;
						}
					}
				}
				if (Loop) break;
			}
		}
	}
	for each (TESForm* Form in Items[To - Normal]) {
		if (LeftWeapon[To - Normal] && Form == LeftWeapon[To - Normal]) {
			LeftWeapon[To - Normal] = NULL;
			Process->LeftEquippingState = HighProcessEx::State::In;
		}
		Player->EquipItem(Form, 1, NULL);
		Process->LeftEquippingState = HighProcessEx::State::StateNone;
	}
	Player->UpdateInventory();

}

EquipmentSetupScript::StepType EquipmentSetupScript::GetCurrentEquipmentType() {

	UInt8 SitSleepState = Player->GetSitSleepState();
	StepType CurrentStep = Normal;
	
	if (CombatState) CurrentStep = Combat;
	if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.EquipmentMode.CombatEquipmentKey)) {
		if (CombatState) CurrentStep = Normal; else CurrentStep = Combat;
		CombatState = !CombatState;
	}
	else if (SitSleepState >= 8 && SitSleepState <= 10) {
		if (!CombatState) CurrentStep = Sleeping;
	}
	else if (IsMoving) {
		if (!CombatState) {
			ElapsedTime = 0.0f;
			CurrentStep = Swimming;
		}
	}
	if (CurrentStep == Normal && GameStep == Swimming && ElapsedTime < 2.0f) {
		ElapsedTime += TheFrameRateManager->ElapsedTime;
		CurrentStep = Swimming;
	}
	return CurrentStep;

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
	
	LowHSound = new LowHSoundScript();
	LowFSound = new LowFSoundScript();
	Purger = new PurgerScript();
	Gravity = new GravityScript();
	EquipmentSetup = new EquipmentSetupScript();

}

void ScriptManager::Run() {

	if (TheSettingManager->SettingsMain.Purger.Enabled) Purger->Run();
	if (TheSettingManager->SettingsMain.Gravity.Enabled) Gravity->Run();
	if (TheSettingManager->SettingsMain.Effects.LowHF) {
		if (TheSettingManager->SettingsMain.LowHFSound.HealthEnabled) LowHSound->Run();
		if (TheSettingManager->SettingsMain.LowHFSound.FatigueEnabled) LowFSound->Run();
	}
	if (TheSettingManager->SettingsMain.EquipmentMode.Enabled) EquipmentSetup->Run();

}

void CreateScriptHook() {

	*((int *)&RunScripts)	= 0x004402F0;
	TrackRunScripts			= &ScriptHook::TrackRunScripts;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RunScripts,	*((PVOID *)&TrackRunScripts));
	DetourTransactionCommit();

}