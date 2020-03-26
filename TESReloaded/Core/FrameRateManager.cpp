FrameRateManager::FrameRateManager() {

	Logger::Log("Starting the framerate manager...");
	TheFrameRateManager = this;

	FrameCounter = 0;
	FrameRate = 0;
	LastFrameTime = 0.0;
	ElapsedTime = 0.0;
	GridDistant = *SettingGridDistantCount;

}

void FrameRateManager::Set() {

	if (FrameRate > 0) {
		if (FrameRate < TheSettingManager->SettingsMain.FrameRate.Critical) {
			FrameCounter++;
			if (FrameCounter >= TheSettingManager->SettingsMain.FrameRate.Delay) {
				FrameCounter = 0;
				GridDistant = 1;
				*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = 1;
				*SettingLODFadeOutMultActors = 1;
				RequestType = FrameRateRequestType_None;
			}
		}
		else if (FrameRate < TheSettingManager->SettingsMain.FrameRate.Min) {
			FrameCounter++;
			if (FrameCounter >= TheSettingManager->SettingsMain.FrameRate.Delay) {
				FrameCounter = 0;
				GridDistant = TheSettingManager->SettingsMain.FrameRate.GridMin;
				RequestType = FrameRateRequestType_TurnDown;
			}
		}
		else if (FrameRate < TheSettingManager->SettingsMain.FrameRate.Average - TheSettingManager->SettingsMain.FrameRate.Gap) {
			FrameCounter++;
			if (FrameCounter >= TheSettingManager->SettingsMain.FrameRate.Delay) {
				FrameCounter = 0;
				GridDistant -= TheSettingManager->SettingsMain.FrameRate.GridStep;
				if (GridDistant < TheSettingManager->SettingsMain.FrameRate.GridMin) GridDistant = TheSettingManager->SettingsMain.FrameRate.GridMin;
				RequestType = FrameRateRequestType_TurnDown;
			}
		}
		else if (FrameRate > TheSettingManager->SettingsMain.FrameRate.Average + TheSettingManager->SettingsMain.FrameRate.Gap) {
			FrameCounter++;
			if (FrameCounter >= TheSettingManager->SettingsMain.FrameRate.Delay) {
				FrameCounter = 0;
				if (*SettingLODFadeOutMultActors == 15.0f && *SettingLODFadeOutMultObjects == 15.0f) {
					GridDistant += TheSettingManager->SettingsMain.FrameRate.GridStep;
					if (GridDistant > *SettingGridDistantCount) GridDistant = *SettingGridDistantCount;
				}
				RequestType = FrameRateRequestType_TurnUp;
			}
		}
		else {
			FrameCounter = 0;
			RequestType = FrameRateRequestType_None;
		}
		if (RequestType == FrameRateRequestType_TurnDown) {
			if (GridDistant == TheSettingManager->SettingsMain.FrameRate.GridMin) {
				if (*SettingLODFadeOutMultObjects > TheSettingManager->SettingsMain.FrameRate.FadeMinObjects) {
					*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = *SettingLODFadeOutMultObjects - TheSettingManager->SettingsMain.FrameRate.FadeStep;
					if (*SettingLODFadeOutMultObjects < TheSettingManager->SettingsMain.FrameRate.FadeMinObjects)
						*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = TheSettingManager->SettingsMain.FrameRate.FadeMinObjects;
				}
				else if (*SettingLODFadeOutMultActors > TheSettingManager->SettingsMain.FrameRate.FadeMinActors) {
					*SettingLODFadeOutMultActors = *SettingLODFadeOutMultActors - TheSettingManager->SettingsMain.FrameRate.FadeStep;
					if (*SettingLODFadeOutMultActors < TheSettingManager->SettingsMain.FrameRate.FadeMinActors)
						*SettingLODFadeOutMultActors = TheSettingManager->SettingsMain.FrameRate.FadeMinActors;
				}
			}
		}
		else if (RequestType == FrameRateRequestType_TurnUp) {
			if (*SettingLODFadeOutMultActors < 15.0f) {
				*SettingLODFadeOutMultActors = *SettingLODFadeOutMultActors + TheSettingManager->SettingsMain.FrameRate.FadeStep;
				if (*SettingLODFadeOutMultActors > 15.0f)
					*SettingLODFadeOutMultActors = 15.0f;
			}
			else if (*SettingLODFadeOutMultObjects < 15.0f) {
				*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = *SettingLODFadeOutMultObjects + TheSettingManager->SettingsMain.FrameRate.FadeStep;
				if (*SettingLODFadeOutMultObjects > 15.0f)
					*SettingLODFadeOutMultItems = *SettingLODFadeOutMultObjects = 15.0f;
			}
		}
	}
}

void FrameRateManager::SetFrameTime(double CurrentFrameTime) {
	
	ElapsedTime = CurrentFrameTime - LastFrameTime;
	LastFrameTime = CurrentFrameTime;
	FrameRate = 1.0 / ElapsedTime;

}

bool FrameRateManager::IsOutGrid(NiAVObject* Object) {
	
	float Distance = (GridDistant + *SettingGridsToLoad / 2.0f) * 4096.0f;

	if (Object->GetDistance(&Player->pos) > Distance)
		return true;
	else
		return false;

}