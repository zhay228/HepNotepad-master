
#include "SystemInfo.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
CSystemInfo::CSystemInfo()
{
}


CSystemInfo::~CSystemInfo()
{
}


int CSystemInfo::getMaxId() {
	string sql = "select ifnull(max(id),0) as maxid from Sys_Info";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	int index = 0;
	while (reader.Read()) {
		index = reader.GetIntValue(0);
	}
	reader.Close();
	delete sqliteServer;
	index++;
	return index;
}

bool CSystemInfo::Add(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;

	string sql = _T("insert into Sys_Info(id,guid,name,value,CreateTime,UserName,UpdateTime) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id); 
	sql += ",'" + GenerateGuid()+ "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->name) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->value) + "'";	 
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + dataInfo->createTime + "'"; 
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}


bool CSystemInfo::UpdateValue(string name,string value, string dataValue) {
	string sql = "update Sys_Info set UpdateTime = '" + GetTimeInfo() + "'";
	if (value != "")
		sql += ",value =  '" + CDataTypeTool::GBKToUTF8(value) + "' ";
	if (dataValue != "")
		sql += ",dataValue =  '" + CDataTypeTool::GBKToUTF8(dataValue) + "' ";
	sql += " where name ='" + name+"'";

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

bool CSystemInfo::Update(DataInfo * dataInfo) {
	string sql = "update Sys_Info set UpdateTime = '"+dataInfo->updateTime+"'";

	if (dataInfo->value != "")
		sql += ",value =  '" + CDataTypeTool::GBKToUTF8(dataInfo->value) + "' ";	 
	if (dataInfo->remark != "")
		sql += ",remark =  '" + CDataTypeTool::GBKToUTF8(dataInfo->remark) + "' ";
	sql += " where id =" + dataInfo->id;
	 
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

int CSystemInfo::IsExist(string name) {
	string sql = "select ifnull(max(id),0) as  maxId from Sys_Info ";
	sql += " where name = '" + CDataTypeTool::GBKToUTF8(name) + "'";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());
	int count = 0;
	while (reader.Read()) {
		count = reader.GetIntValue(0);
		break;
	}
	reader.Close();
	delete sqliteServer;
	return count;
}

bool CSystemInfo::Delete(string id) {
	string sql = "delete from Sys_Info";
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

vector<DataInfo*> CSystemInfo::GetList(string name) {
	string sql = "select id,name,value,ifnull(dataValue,'') as dataValue from Sys_Info";
	  
	if (name != "")
		sql += " where name = '" + CDataTypeTool::GBKToUTF8(name) + "'";

	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0)); 
		dataInfo->name = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->value = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}
 