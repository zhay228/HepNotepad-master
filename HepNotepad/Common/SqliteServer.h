#pragma once 
#include "../Common/SQLite.h"
class CSqliteServer
{
public:
	CSqliteServer();
	~CSqliteServer();
	bool Open(string dataFile = "");	
	void Close();
	bool ExecuteNoneQuery(string sql); 
	SQLiteDataReader ExecuteQuery(string sql);
	string GetAppPath();
	string GetDirectoryName(LPCTSTR lpszPath);

private:
	sqlite3 *pDatabase;
	SQLite sqlite;
	string databaseFilePath;
	bool sqliteStatus;
};

