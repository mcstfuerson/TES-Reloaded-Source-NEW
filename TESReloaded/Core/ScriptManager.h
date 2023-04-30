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

class ScriptManager // Never disposed
{
public:
	ScriptManager();

	void					Run();

	PurgerScript*			Purger;
};

void CreateScriptHook();