#include "PluginVersion.h"
#include "WeatherMode.h"
#include "FormHook.h"

#if defined(NEWVEGAS)
#define CommandPrefix "NVR"
#elif defined(OBLIVION)
#define CommandPrefix "OR"
#elif defined(SKYRIM)
#define CommandPrefix "SR"
#endif

CommandParam SetCustomConstantParams[5] = {

	{ "string",	CommandParam::ParamType::kParamType_String, 0 },
	{ "float",  CommandParam::ParamType::kParamType_Float,  0 },
	{ "float",  CommandParam::ParamType::kParamType_Float,  0 },
	{ "float",  CommandParam::ParamType::kParamType_Float,  0 },
	{ "float",  CommandParam::ParamType::kParamType_Float,  0 },

};

CommandParam SetExtraEffectEnabledParams[2] = {

	{ "string",	CommandParam::ParamType::kParamType_String,  0 },
	{ "int",	CommandParam::ParamType::kParamType_Integer, 0 },

};

static bool CommandExecuter_GetVersion(CommandArgs Args) {

	TheCommandManager->Commands.GetVersion(Args.result);
	return true;

}
static CommandInfo CommandInfo_GetVersion = { CommandPrefix"GetVersion", "", 0, "Returns the plugin version (console command only)", 0, 0, NULL, CommandExecuter_GetVersion, NULL, NULL, 0 };

static bool CommandExecuter_GetLocationName(CommandArgs Args) {

	TheCommandManager->Commands.GetLocationName(Args.result);
	return true;

}
static CommandInfo CommandInfo_GetLocationName = { CommandPrefix"GetLocationName", "", 0, "Gets the name of the current worldspace and current cell (console command only)", 0, 0, NULL, CommandExecuter_GetLocationName, NULL, NULL, 0 };

static bool CommandExecuter_SetExtraEffectEnabled(CommandArgs Args) {

	char Name[80];
	int Value;

	if (ExtractArgs(Args.paramInfo, Args.arg1, Args.opcodeOffset, Args.thisObj, Args.Obj, Args.scriptObj, Args.eventList, &Name, &Value)) TheCommandManager->Commands.SetExtraEffectEnabled(Args.result, Name, Value);
	return true;

}
static CommandInfo CommandInfo_SetExtraEffectEnabled = { CommandPrefix"SetExtraEffectEnabled", "", 0, "Enables or disables a custom effect", 0, 2, SetExtraEffectEnabledParams, CommandExecuter_SetExtraEffectEnabled, NULL, NULL, 0 };

static bool CommandExecuter_SetCustomConstant(CommandArgs Args) {

	char Name[80];
	float Value[4];
	
	if (ExtractArgs(Args.paramInfo, Args.arg1, Args.opcodeOffset, Args.thisObj, Args.Obj, Args.scriptObj, Args.eventList, &Name, &Value[0], &Value[1], &Value[2], &Value[3])) TheCommandManager->Commands.SetCustomConstant(Args.result, Name, Value);
	return true;

}
static CommandInfo CommandInfo_SetCustomConstant = { CommandPrefix"SetCustomConstant", "", 0, "Sets the value of a custom constant", 0, 5, SetCustomConstantParams, CommandExecuter_SetCustomConstant, NULL, NULL, 0 };

static bool CommandExecuter_GetWeatherName(CommandArgs Args) {

	TheCommandManager->Commands.GetWeatherName(Args.result);
	return true;

}
static CommandInfo CommandInfo_GetWeatherName = { CommandPrefix"GetWeatherName", "", 0, "Returns the plugin version (console command only)", 0, 0, NULL, CommandExecuter_GetWeatherName, NULL, NULL, 0 };

CommandManager::CommandManager() {

	Logger::Log("Starting the command manager...");
	TheCommandManager = this;

}

#if defined(OBLIVION) || defined(NEWVEGAS)
void CommandManager::AddCommands(const PluginInterface* Interface) {
	
	Interface->SetOpcodeBase(0x3000);
	Interface->RegisterCommand(&CommandInfo_GetVersion);
	Interface->RegisterCommand(&CommandInfo_GetLocationName);
	Interface->RegisterCommand(&CommandInfo_SetExtraEffectEnabled);
	Interface->RegisterCommand(&CommandInfo_SetCustomConstant);
	Interface->RegisterCommand(&CommandInfo_GetWeatherName);

}
#elif defined (SKYRIM)
CommandTable	commandTable;

namespace SRPapyrus {

	const char* SRClass = "SRCommands";

	bool SetExtraEffectEnabled(BSFixedString Name, bool Value) {

		double result;

		TheCommandManager->Commands.SetExtraEffectEnabled(&result, Name.m_data, Value);
		return result;

	}

	bool SetCustomConstant(BSFixedString Name, float Value1, float Value2, float Value3, float Value4) {

		double result;
		float Value[4] = { Value1, Value2, Value3, Value4 };

		TheCommandManager->Commands.SetCustomConstant(&result, Name.m_data, Value);
		return result;

	}

	bool RegisterCommands(VMClassRegistry* registry) {

		registry->RegisterFunction(new PapyrusFunction2<BSFixedString, bool>("SetExtraEffectEnabled", SRClass, SetExtraEffectEnabled, registry));
		registry->RegisterFunction(new PapyrusFunction5<BSFixedString, float, float, float, float>("SetCustomConstant", SRClass, SetCustomConstant, registry));
		return true;

	}

}

void (__cdecl* ToggleConsole)() = (void (__cdecl*)())0x00847210;
void __cdecl TrackToggleConsole() {

	if (!commandTable.Patched) {
		commandTable.Initialize();
		commandTable.Add(&CommandInfo_GetVersion);
		commandTable.Add(&CommandInfo_GetLocationName);
		commandTable.Add(&CommandInfo_SetExtraEffectEnabled);
		commandTable.Add(&CommandInfo_SetCustomConstant);
		commandTable.Add(&CommandInfo_GetWeatherName);
		commandTable.Set();
		commandTable.Patched = true;
	}
	ToggleConsole();

}

void CommandManager::AddCommands(const PluginInterface* Interface) {

	PapyrusInterface* PInterface = (PapyrusInterface*)Interface->QueryInterface(PluginInterface::InterfaceType::kInterface_Papyrus);
	PInterface->Register(SRPapyrus::RegisterCommands);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ToggleConsole, &TrackToggleConsole);
	DetourTransactionCommit();

}
#endif

void CommandManager::PluginCommands::GetVersion(double* result) {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_Console)) PrintToConsole(PluginVersion::VersionString);
	*result = 1;

}

void CommandManager::PluginCommands::GetLocationName(double* result) {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_Console)) {
		const char* worldName = NULL;
		const char* regionName = NULL;
		const char* cellName = Player->parentCell->GetEditorName();

		std::string s = "Worldspace: ";
		if (TESWorldSpace* currentWS = Player->GetWorldSpace()) worldName = currentWS->GetEditorName();
		if (worldName != NULL)
			s += worldName;
		else
			s += "(no worldspace)";
		s += ", Region: ";
		if (TESRegionEx* currentRE = (TESRegionEx*)Player->GetRegion()) regionName = currentRE->EditorName;
		if (regionName != NULL)
			s += regionName;
		else
			s += "(no region)";
		s += ", Cell: ";
		s += cellName;
		PrintToConsole(s.c_str());
	}
	*result = 1;

}

void CommandManager::PluginCommands::SetExtraEffectEnabled(double* result, const char* Name, bool Value) {

	TheShaderManager->SetExtraEffectEnabled(Name, Value);
	*result = 1;

}

void CommandManager::PluginCommands::SetCustomConstant(double* result, const char* Name, D3DXVECTOR4 Value) {

	TheShaderManager->SetCustomConstant(Name, Value);
	*result = 1;

}

void CommandManager::PluginCommands::GetWeatherName(double* result) {

	if (MenuManager->IsActive(Menu::MenuType::kMenuType_Console)) {
		const char* weatherName = NULL;

		if (TESWeatherEx* currentWE = (TESWeatherEx*)TheShaderManager->ShaderConst.pWeather) weatherName = currentWE->EditorName;
		std::string s = "Weather: ";
		if (weatherName != NULL)
			s += weatherName;
		else
			s += "(no weather)";
		PrintToConsole(s.c_str());
	}
	*result = 1;

}
