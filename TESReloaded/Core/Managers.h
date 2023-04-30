#pragma once

#include "ScriptManager.h"
#include "SettingManager.h"
#include "CommandManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "FrameRateManager.h"
#include "GameEventManager.h"
#include "GameMenuManager.h"
#include "KeyboardManager.h"
#include "ShadowManager.h"

extern SettingManager*		TheSettingManager;
extern CommandManager*		TheCommandManager;
extern TextureManager*		TheTextureManager;
extern ShaderManager*		TheShaderManager;
extern RenderManager*		TheRenderManager;
extern FrameRateManager*	TheFrameRateManager;
extern GameMenuManager*		TheGameMenuManager;
extern KeyboardManager*		TheKeyboardManager;
extern ShadowManager*		TheShadowManager;
extern ScriptManager*		TheScriptManager;

void InitializeManagers();