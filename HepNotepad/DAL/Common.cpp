#include "../stdafx.h"
#include "Common.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "../Common/des.h"
extern UserInfo* userInfo;
CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}


void CCommon::getPageInfo(DataType dataType, PageInfo * pageInfo) {
	string table = "Dat_Tip";
	switch (dataType)
	{
	case tip:
		table = "Dat_Tip";
		break;
	case info:
		table = "Dat_Info";
		break;
	case personal:
		table = "Dat_PersonInfo";
		break;
	case calendar:
		table = "Dat_Calendar";
		break;
	case weekly:
		table = "Dat_Weekly";
		break;
	default:
		break;
	}

	string sql = "select count(id) as count,ifnull(max(id),0) as maxid from " + table + pageInfo->whereSql;

	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	while (reader.Read()) {
		pageInfo->totalCount = reader.GetIntValue(0);
		pageInfo->maxId = reader.GetIntValue(1);
	}
	pageInfo->totalPageCount = (pageInfo->totalCount + pageInfo->pageSize - 1) / pageInfo->pageSize;

	if (pageInfo->currPageIndex < 0)pageInfo->currPageIndex = 0;
	if (pageInfo->currPageIndex >= pageInfo->totalPageCount) pageInfo->currPageIndex = pageInfo->totalPageCount - 1;

	/*sql = "select ifnull(max(id),0) as maxid  from (select id from " + table + pageInfo->whereSql;
	sql += "limit  " + CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);
	sql += " OFFSET 0 )";
	SQLiteDataReader reader1 = sqliteServer->ExecuteQuery(sql.c_str());
	while (reader1.Read()) {
		pageInfo->startIndex = reader1.GetIntValue(0);
	}*/

	delete sqliteServer;
}
 
bool CCommon::DatabaseCheck(string database) {
	SQLite sqlite; 
	string key = "";
	if (sqlite.Open(database.c_str())) { 
		string sql = "select value from Sys_Info where name = '"+ userInfo->keyCol +"'";
		SQLiteDataReader reader = sqlite.ExcuteQuery(sql.c_str());
		int count = reader.ColumnCount();
		
		while (reader.Read()) {
			key = reader.GetStringValue(0); 
		}
	}
	sqlite.Close();
	if (key != "")
		key = Decrypt(key);
	return key == userInfo->key;
}

int CCommon::getMaxId(string tableName) {
	string sql = "select ifnull(max(id),0) as maxid from "+tableName;
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	int index = 0;
	while (reader.Read()) {
		index = reader.GetIntValue(0);
	}
	reader.Close();
	index++;
	delete sqliteServer;
	return index;
}

bool CCommon::IsExist(string tableName,string guid) {
	string sql = "select ifnull(max(TypeId),0) as  maxId from Dat_Type  ";
	sql += " where guid = '" + guid + "'";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());
	int count = 0;
	while (reader.Read()) {
		count = reader.GetIntValue(0);
		break;
	}
	reader.Close();
	delete sqliteServer;
	return count > 0;
}

void CCommon::DataInput(string database) {
	SQLite sqlite; 
	SQLite localSqlite;
	string path = CPaintManagerUI::GetInstancePath();
	path += "data.db";
	bool localOpen = localSqlite.Open(path.c_str());
	if (sqlite.Open(database.c_str())) { 
		string sqlTable = "select name,sql from sqlite_master where type='table'  order by name";
		vector<string> tableList;
		
		SQLiteDataReader reader = sqlite.ExcuteQuery(sqlTable.c_str());
		int count = reader.ColumnCount(); 
		while (reader.Read()) {
			tableList.push_back(reader.GetStringValue(0));
		}
		reader.Close();
		 
		char sqlexec[99068];
	 
		string type = "", columnName = "", tempValue = "";
		for (int i = 0; i < tableList.size(); i++) {
			
			int id = getMaxId(tableList[i]);
			sprintf(sqlexec, "PRAGMA table_info('%s')", tableList[i].c_str());
			string columnquery = "",columnInsert, value = "";;
			int count = reader.ColumnCount();
			int columnCount = 0;
			vector<string> typeList; 
			SQLiteDataReader readerColumn = sqlite.ExcuteQuery(sqlexec);
		
			while (readerColumn.Read()) {
				columnName = readerColumn.GetStringValue(1);
				type = readerColumn.GetStringValue(2);
				if (type != "INT") {
					columnquery += "ifnull(" + columnName + ",'') as " + columnName;
				}
				else {
					columnquery += columnName;
				}
				columnInsert += columnName + ",";
				typeList.push_back(type);
				columnquery += ",";
				columnCount++;
			} 
			readerColumn.Close();
			if (columnquery != "") {
				columnquery = columnquery.substr(0, columnquery.size() - 1);
				columnInsert = columnInsert.substr(0, columnInsert.size() - 1);
			}

			sprintf(sqlexec, "select %s from %s", columnquery.c_str(), tableList[i].c_str());
			 
		 
			SQLiteDataReader readerData = sqlite.ExcuteQuery(sqlexec);
			while (readerData.Read()) {
				value = CDataTypeTool::intTostring(id);
				for (int i = 1; i < columnCount; i++) {
					if (typeList[i] == "INT") {
						value += ",";
						value += CDataTypeTool::intTostring(readerData.GetIntValue(i));
					}
					else { 						 
						tempValue = CDataTypeTool::UTF8ToGBK(readerData.GetStringValue(i));
						value +=",'" + tempValue +  "'";
					} 
				}
				sprintf(sqlexec, "insert into %s(%s) values(%s)", tableList[i].c_str(), columnInsert.c_str(),value.c_str());
			
				if (localOpen) {
					CLog::WriteToLog(sqlexec);
					if (localSqlite.ExcuteNonQuery(sqlexec)) {
						
					}
					else {

					}
				}
				
				id++; 
			} 
			readerData.Close();
		} 
	} 
	sqlite.Close();
	localSqlite.Close();
}