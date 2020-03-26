#include <ctime>
#include <fstream>
#include "PluginVersion.h"
#include "WeatherMode.h"

#if defined(NEWVEGAS)
static char* TitleMenu = "New Vegas Reloaded - Settings";
#elif defined(OBLIVION)
static char* TitleMenu = "Oblivion Reloaded - Settings";
#elif defined(SKYRIM)
static char* TitleMenu = "Skyrim Reloaded - Settings";
#endif

#define TextColorNormal D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextColorNormal[0], TheSettingManager->SettingsMain.Menu.TextColorNormal[1], TheSettingManager->SettingsMain.Menu.TextColorNormal[2])
#define TextShadowColorNormal D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextShadowColorNormal[0], TheSettingManager->SettingsMain.Menu.TextShadowColorNormal[1], TheSettingManager->SettingsMain.Menu.TextShadowColorNormal[2])
#define TextColorSelected D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextColorSelected[0], TheSettingManager->SettingsMain.Menu.TextColorSelected[1], TheSettingManager->SettingsMain.Menu.TextColorSelected[2])
#define TextShadowColorSelected D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextShadowColorSelected[0], TheSettingManager->SettingsMain.Menu.TextShadowColorSelected[1], TheSettingManager->SettingsMain.Menu.TextShadowColorSelected[2])
#define TextColorEditing D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextColorEditing[0], TheSettingManager->SettingsMain.Menu.TextColorEditing[1], TheSettingManager->SettingsMain.Menu.TextColorEditing[2])
#define TextShadowColorEditing D3DCOLOR_XRGB(TheSettingManager->SettingsMain.Menu.TextShadowColorEditing[0], TheSettingManager->SettingsMain.Menu.TextShadowColorEditing[1], TheSettingManager->SettingsMain.Menu.TextShadowColorEditing[2])
#define TextFont TheSettingManager->SettingsMain.Menu.TextFont
#define TextSize TheSettingManager->SettingsMain.Menu.TextSize
#define TextFontStatus TheSettingManager->SettingsMain.Menu.TextFontStatus
#define TextSizeStatus TheSettingManager->SettingsMain.Menu.TextSizeStatus
#define PositionX TheSettingManager->SettingsMain.Menu.PositionX
#define PositionY TheSettingManager->SettingsMain.Menu.PositionY
#define InfoPositionX TheSettingManager->SettingsMain.Menu.InfoPositionX
#define InfoPositionY TheSettingManager->SettingsMain.Menu.InfoPositionY
#define TitleColumnSize TheSettingManager->SettingsMain.Menu.TitleColumnSize
#define MainItemColumnSize TheSettingManager->SettingsMain.Menu.MainItemColumnSize
#define ItemColumnSize TheSettingManager->SettingsMain.Menu.ItemColumnSize
#define InfoColumnSize TheSettingManager->SettingsMain.Menu.InfoColumnSize
#define RowSpace TheSettingManager->SettingsMain.Menu.RowSpace
#define RowsPerPage TheSettingManager->SettingsMain.Menu.RowsPerPage
#define IntValues "ScreenshotKey GrassDensity LightShaftPasses CombatEquipmentKey TorchKey Average Min Critical Gap Delay FadeStep FadeMinObjects FadeMinActors GridStep GridMin TimeKey DayR DayG DayB NightR NightG NightB SunriseR SunriseG SunriseB SunsetR SunsetG SunsetB CloudSpeedLower CloudSpeedUpper SunGlare SunDamage TransDelta WindSpeed Mode Quality"
#define BoolValues "Enabled DistantBlur SunGlareEnabled TimeEnabled FPSOverlay SleepingEquipment SwimmingEquipment PurgeCells PurgeTextures FatigueEnabled HealthEnabled InfoEnabled AlphaEnabled"

GameMenuManager::GameMenuManager() {

	Logger::Log("Starting the menu manager...");
	TheGameMenuManager = this;

	SelectedColumn = 0;
	SelectedRow[0] = SelectedRow[1] = SelectedRow[2] = SelectedRow[3] = 0;
	SelectedPage[0] = SelectedPage[1] = SelectedPage[2] = SelectedPage[3] = 0;
	Enabled = false;
	EditingMode = false;
	FrameRateText = 0;
	LastFrameRateTextTime = 0;
	SelectedSetting[0] = '\0';
	InfoText[0] = '\0';
	SetRect(&RectInfo, InfoPositionX, InfoPositionY, InfoPositionX + InfoColumnSize, InfoPositionY + RowSpace + TextSize * 2);
	SetRect(&RectInfoShadow, RectInfo.left + 1, RectInfo.top + 1, RectInfo.right + 1, RectInfo.bottom + 1);
	D3DXCreateFontA(TheRenderManager->device, TextSize, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, TextFont, &FontNormal);
	D3DXCreateFontA(TheRenderManager->device, TextSize, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, TextFont, &FontSelected);
	D3DXCreateFontA(TheRenderManager->device, TextSizeStatus, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, TextFontStatus, &FontStatus);

}

void GameMenuManager::Render() {
	
	if (MenuManager->IsActive(Menu::MenuType::kMenuType_Main)) {
		SetRect(&Rect, 0, TheRenderManager->height - TextSize - 10, TheRenderManager->width, TheRenderManager->height + TextSize);
		SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
		FontNormal->DrawTextA(NULL, PluginVersion::VersionString, -1, &RectShadow, DT_CENTER, TextShadowColorNormal);
		FontNormal->DrawTextA(NULL, PluginVersion::VersionString, -1, &Rect, DT_CENTER, TextColorNormal);
	}
	else {
		if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyEnable)) {
			Enabled = !Enabled;
			EditingMode = false;
		}
		if (!MenuManager->IsActive(Menu::MenuType::kMenuType_None)) {
			Enabled = false;
			EditingMode = false;
		}
		if (Enabled) {
			const char* Text = NULL;
			char SettingFirst[40];
			size_t ListSize = 0;

			if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyEditing) && SelectedColumn == 3) {
				if (!EditingMode) {
					if (strstr(IntValues, SelectedSetting) || strstr(BoolValues, SelectedSetting))
						sprintf(EditingValue, "%.0f", SelectedValue);
					else
						sprintf(EditingValue, TheSettingManager->SettingsMain.Menu.ValueFormat, SelectedValue);
				}
				else
					TheSettingManager->SetMenuSetting(SelectedItem, SelectedDefinition, SelectedSection, SelectedSectionKey, SelectedSetting, atof(EditingValue));
				EditingMode = !EditingMode;
			}
			if (!EditingMode) {
				if (SelectedColumn == 0) {
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyDown)) SelectedColumn = 1;
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyLeft) && SelectedRow[SelectedColumn] > 0) SelectedRow[SelectedColumn] -= 1;
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyRight) && SelectedRow[SelectedColumn] < Rows[SelectedColumn] - 1) SelectedRow[SelectedColumn] += 1;
					SelectedPage[1] = SelectedPage[2] = SelectedPage[3] = 0;
				}
				else {
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyUp)) {
						if (SelectedRow[SelectedColumn] > 0) SelectedRow[SelectedColumn] -= 1; else SelectedColumn = 0;
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyDown) && SelectedRow[SelectedColumn] < Rows[SelectedColumn] - 1) SelectedRow[SelectedColumn] += 1;
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyLeft)) {
						if (SelectedColumn > 0) SelectedColumn -= 1;
						SelectedRow[SelectedColumn + 1] = 0;
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyRight) && SelectedColumn < 3) SelectedColumn += 1;
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyPageUp) && SelectedPage[SelectedColumn] > 0) {
						SelectedPage[SelectedColumn] -= 1;
						SelectedRow[SelectedColumn] = 0;
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyPageDown) && SelectedPage[SelectedColumn] < Pages[SelectedColumn]) {
						SelectedPage[SelectedColumn] += 1;
						SelectedRow[SelectedColumn] = 0;
					}
					if (!memcmp(SelectedItem, "Shader", 6)) {
						bool ShaderEnabled = TheSettingManager->GetMenuShaderEnabled(SelectedDefinition);
						if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyAdd) && !ShaderEnabled && SelectedColumn == 1) TheShaderManager->SwitchShaderStatus(SelectedDefinition);
						if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeySubtract) && ShaderEnabled && SelectedColumn == 1) TheShaderManager->SwitchShaderStatus(SelectedDefinition);
					}
					else if (!memcmp(SelectedItem, "Weather", 7)) {
						if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyAdd) && SelectedColumn == 1) {
							#if defined(OBLIVION) || defined(NEWVEGAS)
							for (TList<TESWeather>::Iterator Itr = DataHandler->weathers.Begin(); !Itr.End() && Itr.Get(); ++Itr) {
								TESWeatherEx* Weather = (TESWeatherEx*)Itr.Get();
							#elif defined(SKYRIM)
							TArray<TESWeather*> Weathers = DataHandler->weathers;
							for (UInt32 I = 0; I < Weathers.count; I++) {
								TESWeatherEx* Weather = (TESWeatherEx*)Weathers.data[I];
							#endif
								if (!strcmp(SelectedDefinition, Weather->EditorName)) {
									Tes->sky->ForceWeather(Weather);
									break;
								}
							}
						}
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeyAdd) && SelectedColumn == 3) {
						if (strstr(BoolValues, SelectedSetting))
							SelectedValue = !SelectedValue;
						else if (strstr(IntValues, SelectedSetting))
							SelectedValue += 1.0f;
						else
							SelectedValue += TheSettingManager->SettingsMain.Menu.StepValue;
						TheSettingManager->SetMenuSetting(SelectedItem, SelectedDefinition, SelectedSection, SelectedSectionKey, SelectedSetting, SelectedValue);
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeySubtract) && SelectedColumn == 3) {
						if (strstr(BoolValues, SelectedSetting))
							SelectedValue = !SelectedValue;
						else if (strstr(IntValues, SelectedSetting))
							SelectedValue -= 1.0f;
						else
							SelectedValue -= TheSettingManager->SettingsMain.Menu.StepValue;
						TheSettingManager->SetMenuSetting(SelectedItem, SelectedDefinition, SelectedSection, SelectedSectionKey, SelectedSetting, SelectedValue);
					}
					if (TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.Menu.KeySave)) {
						TheSettingManager->SaveSettings(SelectedItem, SelectedDefinition);
						MenuManager->ShowMessage("Settings saved.");
					}
				}
			}
			else {
				if (strlen(EditingValue) < 19) {
					if (TheKeyboardManager->OnKeyDown(82))
						strcat(EditingValue, "0");
					else if (TheKeyboardManager->OnKeyDown(79))
						strcat(EditingValue, "1");
					else if (TheKeyboardManager->OnKeyDown(80))
						strcat(EditingValue, "2");
					else if (TheKeyboardManager->OnKeyDown(81))
						strcat(EditingValue, "3");
					else if (TheKeyboardManager->OnKeyDown(75))
						strcat(EditingValue, "4");
					else if (TheKeyboardManager->OnKeyDown(76))
						strcat(EditingValue, "5");
					else if (TheKeyboardManager->OnKeyDown(77))
						strcat(EditingValue, "6");
					else if (TheKeyboardManager->OnKeyDown(71))
						strcat(EditingValue, "7");
					else if (TheKeyboardManager->OnKeyDown(72))
						strcat(EditingValue, "8");
					else if (TheKeyboardManager->OnKeyDown(73))
						strcat(EditingValue, "9");
					else if (TheKeyboardManager->OnKeyDown(83))
						strcat(EditingValue, ".");
					else if (TheKeyboardManager->OnKeyDown(74))
						strcat(EditingValue, "-");
				}
				if (strlen(EditingValue) > 0 && TheKeyboardManager->OnKeyDown(14)) EditingValue[strlen(EditingValue) - 1] = NULL;
			}

			Text = TitleMenu;
			SetRect(&Rect, PositionX, PositionY, PositionX + TitleColumnSize, PositionY + TextSize);
			SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
			FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
			FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);

			SetRect(&Rect, Rect.left, Rect.bottom + RowSpace, Rect.right, Rect.bottom + RowSpace + 2);
			TheRenderManager->device->Clear(1L, (D3DRECT*)&Rect, D3DCLEAR_TARGET, TextColorNormal, 0.0f, 0L);

			Rows[0] = 0;
			SetRect(&Rect, Rect.left + MainItemColumnSize * 0, Rect.bottom + RowSpace, Rect.left + MainItemColumnSize * 1, Rect.bottom + RowSpace + TextSize);
			DefinitionsList Items = TheSettingManager->GetMenuItems();
			ListSize = Items.size();
			Pages[0] = ListSize / RowsPerPage;
			DefinitionsList::iterator Item = Items.begin();
			for (int i = 0; i < ListSize; i++) {
				Text = Item->second.c_str();
				SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
				if (SelectedRow[0] == Rows[0]) {
					strcpy(SelectedItem, Item->first.c_str());
					FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
					FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
				}
				else {
					FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
					FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
				}
				Rect.left += MainItemColumnSize;
				Rect.right += MainItemColumnSize;
				Rows[0]++;
				Item++;
			}

			SetRect(&Rect, PositionX, Rect.bottom + RowSpace, PositionX + TitleColumnSize, Rect.bottom + RowSpace + 2);
			TheRenderManager->device->Clear(1L, (D3DRECT*)&Rect, D3DCLEAR_TARGET, TextColorNormal, 0.0f, 0L);

			int MenuRectX = PositionX;
			int MenuRectY = Rect.bottom + RowSpace;
			Rows[1] = 0;
			SetRect(&Rect, MenuRectX + ItemColumnSize * 0, MenuRectY, MenuRectX + ItemColumnSize * 1, MenuRectY + TextSize);
			DefinitionsList Definitions = TheSettingManager->GetMenuDefinitions(SelectedItem);
			ListSize = Definitions.size();
			Pages[1] = ListSize / RowsPerPage;
			DefinitionsList::iterator Definition = Definitions.begin();
			for (int i = 0; i < ListSize; i++) {
				if (i >= RowsPerPage * SelectedPage[1] && i < RowsPerPage * (SelectedPage[1] + 1)) {
					Text = Definition->second.c_str();
					Rect.top += TextSize + RowSpace;
					Rect.bottom += TextSize + RowSpace;
					SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
					if (SelectedRow[1] == Rows[1]) {
						strcpy(SelectedDefinition, Definition->first.c_str());
						if (SelectedColumn >= 1) {
							FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
							FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
						}
						else {
							FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
							FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
						}
					}
					else {
						FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
						FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
					}
					if (!memcmp(SelectedItem, "Shader", 6)) {
						if (SelectedRow[1] == Rows[1] && SelectedColumn >= 1)
							FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_CALCRECT, TextColorSelected);
						else
							FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_CALCRECT, TextColorNormal);
						Rect.left = Rect.right;
						Rect.right += 100;
						SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
						if (TheSettingManager->GetMenuShaderEnabled(Definition->first.c_str())) Text = "ENABLED"; else Text = "DISABLED";
						if (SelectedRow[1] == Rows[1] && SelectedColumn >= 1) {
							FontStatus->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
							FontStatus->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
						}
						else {
							FontStatus->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
							FontStatus->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
						}
						Rect.left = MenuRectX + ItemColumnSize * 0;
						Rect.right = MenuRectX + ItemColumnSize * 1;
					}
					Rows[1]++;
				}
				Definition++;
			}
			Rows[2] = 0;
			SetRect(&Rect, MenuRectX + ItemColumnSize * 1, MenuRectY, MenuRectX + ItemColumnSize * 2, MenuRectY + TextSize);
			SectionsList Sections = TheSettingManager->GetMenuSections(SelectedItem, SelectedDefinition);
			ListSize = Sections.size();
			Pages[2] = ListSize / RowsPerPage;
			SectionsList::iterator Section = Sections.begin();
			for (int i = 0; i < ListSize; i++) {
				if (i >= RowsPerPage * SelectedPage[2] && i < RowsPerPage * (SelectedPage[2] + 1)) {
					Text = Section->second.c_str();
					Rect.top += TextSize + RowSpace;
					Rect.bottom += TextSize + RowSpace;
					SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
					if (SelectedRow[2] == Rows[2]) {
						strcpy(SelectedSection, Text);
						SelectedSectionKey = Section->first;
						if (SelectedColumn >= 2) {
							FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
							FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
						}
						else {
							FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
							FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
						}
					}
					else {
						FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
						FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
					}
					Rows[2]++;
				}
				Section++;
			}
			Rows[3] = 0;
			SetRect(&Rect, MenuRectX + ItemColumnSize * 2, MenuRectY, MenuRectX + ItemColumnSize * 3, MenuRectY + TextSize);
			SettingsList Settings = TheSettingManager->GetMenuSettings(SelectedItem, SelectedDefinition, SelectedSection, SelectedSectionKey);
			ListSize = Settings.size();
			Pages[3] = ListSize / RowsPerPage;
			SettingsList::iterator Setting = Settings.begin();
			for (int i = 0; i < ListSize; i++) {
				if (i >= RowsPerPage * SelectedPage[3] && i < RowsPerPage * (SelectedPage[3] + 1)) {
					strcpy(SettingFirst, Setting->first.c_str());
					if (strstr(IntValues, SettingFirst) || strstr(BoolValues, SettingFirst))
						sprintf(SettingValue, "%.0f", Setting->second);
					else
						sprintf(SettingValue, TheSettingManager->SettingsMain.Menu.ValueFormat, Setting->second);
					std::string T = Setting->first + " = " + SettingValue;
					Text = T.c_str();
					Rect.top += TextSize + RowSpace;
					Rect.bottom += TextSize + RowSpace;
					SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
					if (SelectedRow[3] == Rows[3]) {
						if (TheSettingManager->SettingsMain.Menu.InfoEnabled && strcmp(SelectedSetting, SettingFirst)) {
							char FileName[MAX_PATH];
							strcpy(FileName, DocFile);
							std::ifstream InfoFile(FileName, std::ios::in);
							if (InfoFile.is_open()) {
								bool F = false;
								size_t P = 0;
								std::string L;
								char V[80] = { 'T', 'A', 'G', '-', '\0' };
								if (!strcmp(SelectedDefinition, "Main"))
									strcat(V, SelectedSection);
								else
									strcat(V, SelectedDefinition);
								while (InfoFile.good()) {
									getline(InfoFile, L);
									if (!F) {
										P = L.find(V);
										if (P != std::string::npos) F = true;
									}
									else {
										P = L.find("<var>" + Setting->first);
										if (P != std::string::npos) {
											getline(InfoFile, L);
											strcpy_s(InfoText, L.c_str());
											break;
										}
									}
								}
								InfoFile.close();
							}
							else {
								strcat(FileName, " (ERROR OPENING)");
								strcpy_s(InfoText, FileName);
							}
						}
						strcpy(SelectedSetting, SettingFirst);
						SelectedValue = Setting->second;
						if (SelectedColumn >= 3) {
							if (EditingMode) {
								T = Setting->first + " = " + EditingValue;
								Text = T.c_str();
								FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorEditing);
								FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorEditing);
								FontNormal->DrawTextA(NULL, InfoText, -1, &RectInfoShadow, DT_CENTER | DT_WORDBREAK, TextShadowColorNormal);
								FontNormal->DrawTextA(NULL, InfoText, -1, &RectInfo, DT_CENTER | DT_WORDBREAK, TextColorNormal);
							}
							else {
								FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
								FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
								FontNormal->DrawTextA(NULL, InfoText, -1, &RectInfoShadow, DT_CENTER | DT_WORDBREAK, TextShadowColorNormal);
								FontNormal->DrawTextA(NULL, InfoText, -1, &RectInfo, DT_CENTER | DT_WORDBREAK, TextColorNormal);
							}
						}
						else {
							FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
							FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
						}
					}
					else {
						FontNormal->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorNormal);
						FontNormal->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorNormal);
					}
					Rows[3]++;
				}
				Setting++;
			}
		}
		if (TheSettingManager->SettingsMain.Main.FPSOverlay && TheFrameRateManager->FrameRate > 0) {
			char Text[4];
			time_t CurrentTime = time(NULL);

			if (difftime(CurrentTime, LastFrameRateTextTime) >= 1.0) {
				FrameRateText = TheFrameRateManager->FrameRate;
				LastFrameRateTextTime = CurrentTime;
			}
			sprintf(Text, "%d", FrameRateText);
			SetRect(&Rect, 9, 6, 109, 6 + TextSize);
			SetRect(&RectShadow, Rect.left + 1, Rect.top + 1, Rect.right + 1, Rect.bottom + 1);
			FontSelected->DrawTextA(NULL, Text, -1, &RectShadow, DT_LEFT, TextShadowColorSelected);
			FontSelected->DrawTextA(NULL, Text, -1, &Rect, DT_LEFT, TextColorSelected);
		}
	}

}