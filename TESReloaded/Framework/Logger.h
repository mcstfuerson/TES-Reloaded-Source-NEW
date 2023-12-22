#pragma once

class Logger {
public:
	static void CreateLog(char* FileName);
	static void Log(char* Message, ...);
	
	static char			MessageBuffer[2048];
	static FILE*		LogFile;

};