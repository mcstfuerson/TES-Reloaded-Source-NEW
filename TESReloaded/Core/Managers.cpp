SettingManager*		TheSettingManager = NULL;
CommandManager*		TheCommandManager = NULL;
TextureManager*		TheTextureManager = NULL;
ShaderManager*		TheShaderManager = NULL;
RenderManager*		TheRenderManager = NULL;
FrameRateManager*	TheFrameRateManager = NULL;
GameMenuManager*	TheGameMenuManager = NULL;
KeyboardManager*	TheKeyboardManager = NULL;
ShadowManager*		TheShadowManager = NULL;
ScriptManager*		TheScriptManager = NULL;

void InitializeManagers() {

	new TextureManager();
	new ShaderManager();
	new FrameRateManager();
	new GameMenuManager();
	new KeyboardManager();
	new ShadowManager();
	new ScriptManager();

}