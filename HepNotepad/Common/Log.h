#pragma once
#include <tchar.h> 
#include <string>
using namespace std;
class CLog
{
public:
	static void CreateDir(LPSTR fileName);
	static void WriteToLog(string logInfo);
	static void WriteToLog(char* logInfo);
	static void WriteToLog(char* LOGFILE, char* logInfo);
};
