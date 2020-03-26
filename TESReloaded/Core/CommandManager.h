#pragma once

class CommandManager { // Never disposed

public:
	CommandManager();

	class PluginCommands {
	public:
		void GetVersion(double* result);
		void GetLocationName(double* result);
		void SetExtraEffectEnabled(double* result, const char* Name, bool Value);
		void SetCustomConstant(double* result, const char* Name, D3DXVECTOR4 Value);
		void GetWeatherName(double* result);
	}; 

	void							AddCommands(const PluginInterface* Interface);

	PluginCommands					Commands;

};