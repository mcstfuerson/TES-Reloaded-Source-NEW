#pragma once

class BaseScript
{
public:
	BaseScript();

	virtual void	Run() = 0;

	float			ElapsedTime;
};

class PurgerScript : public BaseScript // Never disposed
{
public:
	PurgerScript();

	void			Run();
};

class GravityScript : public BaseScript // Never disposed
{
public:
	GravityScript();

	void			Run();
};

class EquipmentSetupScript : public BaseScript // Never disposed
{
public:
	EquipmentSetupScript();

	enum StepType
	{
		None,
		Request,
		Choice,
		Normal,
		Combat,
		Sleeping,
		Swimming,
	};

	void					Run();
	void					EquipItems(StepType From, StepType To);
	StepType				GetCurrentEquipmentType();

	std::vector<TESForm*>	Items[4];
	TESObjectWEAP*			LeftWeapon[4];
	StepType				ConfigStep;
	StepType				GameStep;
	bool					CombatState;

};

class ScriptManager // Never disposed
{
public:
	ScriptManager();

	void					Run();

	PurgerScript*			Purger;
	GravityScript*			Gravity;
	EquipmentSetupScript*	EquipmentSetup;
};

void CreateScriptHook();