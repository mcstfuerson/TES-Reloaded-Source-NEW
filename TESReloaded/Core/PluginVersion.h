#pragma once

class PluginVersion {

public:
	static void CreateVersionString();
	static char VersionString[128];

private:
	static void AddVersion(char* FileName);

};