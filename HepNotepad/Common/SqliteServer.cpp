#include "stdafx.h" 
#include "SqliteServer.h"

CSqliteServer::CSqliteServer()
{
	Open();	
}


CSqliteServer::~CSqliteServer()
{
}

// 返回指定路径字符串的目录信息
string CSqliteServer::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	string strPath(lpszPath);

	string::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	string::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != string::npos)
		strPath = strPath.substr(0, nPos + 1);

	return strPath;
}

// 获取应用程序执行路径
string CSqliteServer::GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = { 0 };

	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		string strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}

	return szPath;
}
 

bool CSqliteServer::Open(string dataFile){
	pDatabase = NULL;
	if (dataFile == "")
		dataFile = GetAppPath() + "data.db";
	sqliteStatus = sqlite.Open(dataFile.c_str());
	return sqliteStatus;
}
void CSqliteServer::Close(){
	sqlite.Close();

}

bool CSqliteServer::ExecuteNoneQuery(string sql){
	bool result = true;
	if (sqliteStatus)
		result = sqlite.ExcuteNonQuery(sql.c_str());

	return result;
}

SQLiteDataReader CSqliteServer::ExecuteQuery(string sql)
{
	return sqlite.ExcuteQuery(sql.c_str());
}