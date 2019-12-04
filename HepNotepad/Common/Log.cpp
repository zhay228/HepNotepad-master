 #include <windows.h>
#include "direct.h" 

// C RunTime Header Files  
#include <io.h>
#include <time.h>
#include "Log.h" 

void CLog::CreateDir(LPSTR fileName)
{
	char *tag;
	for (tag = fileName; *tag; tag++)
	{
		if (*tag == '\\')
		{
			char buf[9999], path[9999];
			strcpy(buf, fileName);
			buf[strlen(fileName) - strlen(tag) + 1] = NULL;
			strcpy(path, buf);
			if (access(path, 6) == -1)
			{
				mkdir(path);
			}
		}
	}
}

void CLog::WriteToLog(char* LOGFILE, char* logInfo){
	FILE* log;
	log = fopen(LOGFILE, "ab+");
	if (log == NULL)
	{
		CreateDir(LOGFILE);
		log = fopen(LOGFILE, "ab+");
		if (log == NULL)
		{
			return;
		}
	}
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	fprintf(log, "%d年%d月%d日 %d点%d分%d秒 %s\r\n", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond, logInfo);
	fclose(log);

}
 
void CLog::WriteToLog(char* logInfo) {
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	string path = szFilePath;
	int pos = path.find_last_of('\\', path.length());
	path = path.substr(0, pos + 1);

	time_t t = time(NULL);
	char date[64] = { 0 };
	strftime(date, sizeof(date) - 1, "%Y-%m-%d", localtime(&t));
	string dateStr = date;
	TCHAR fileName[600];
	path += +"log\\" + dateStr + ".log";
	sprintf(fileName, "%s", path);
	WriteToLog(fileName, logInfo);
}

void CLog::WriteToLog(string logInfo) {
	TCHAR msg[30720];
	strcpy(msg, logInfo.c_str());
	WriteToLog(msg);
}

