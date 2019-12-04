#include "stdafx.h"
#include "SqliteServer.h"
#include "sqlite3.h"
#include<string>
using namespace std;

CSqliteServer::CSqliteServer()
{
	

	
}


CSqliteServer::~CSqliteServer()
{
}


bool CSqliteServer::Open(string dataFile){
	pDB = NULL;
	int nRes = sqlite3_open(dataFile.c_str(), &pDB);
	if (nRes != SQLITE_OK)
	{
		return false;
	}
	return true;
}
bool CSqliteServer::Close(){
	sqlite3_close(pDB);
	return true;

}

bool CSqliteServer::ExcuteNonQuery(string sql){
	char* errmsg;

	int nResult = sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errmsg);
	if (nResult != SQLITE_OK)
	{
		sqlite3_close(pDB);
	 
		sqlite3_free(errmsg);
		return false;
	}
	return true;
}