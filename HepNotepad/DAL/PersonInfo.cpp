
#include "PersonInfo.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "Common.h"
extern UserInfo* userInfo;
CPersonInfo::CPersonInfo()
{
}


CPersonInfo::~CPersonInfo()
{ 
}


int CPersonInfo::getMaxId() {
	string sql = "select ifnull(max(id),0) as maxid from Dat_PersonInfo";
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

bool CPersonInfo::Add(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;

	string sql = _T("insert into Dat_PersonInfo(id,guid,IsEncryption,Key,Title,Content,UserId,CreateTime,UserName,UpdateTime,TypeName) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += "," + CDataTypeTool::intTostring(dataInfo->isEncryption) + "";
	sql += ",'" + userInfo->key + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "'";
	sql += ",'" + replace(dataInfo->content, "'", "''") + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userId) + "'";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->typeName) + "'";
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}


bool CPersonInfo::Update(DataInfo * dataInfo) {
	string sql = "update Dat_PersonInfo set UpdateTime = '"+dataInfo->updateTime+"'";

	if (dataInfo->typeName != "")
		sql += ",typeName =  '" + CDataTypeTool::GBKToUTF8(dataInfo->typeName) + "' ";
	if (dataInfo->title != "")
		sql += ",Title =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "' ";
	if (dataInfo->content != "")
		sql += ",Content =  '" + replace(dataInfo->content, "'", "''") + "' ";
	if (dataInfo->isEncryption > -1 )
		sql += ",IsEncryption = " + CDataTypeTool::intTostring(dataInfo->isEncryption);

	sql += " where id =" + dataInfo->id;
	 
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

int CPersonInfo::IsExist(string name) {
	string sql = "select ifnull(max(id),0) as  maxId from Dat_PersonInfo ";
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

bool CPersonInfo::Delete(string id) {
	string sql = "delete from Dat_PersonInfo";
	sql += " where id =" + id;
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

vector<DataInfo*> CPersonInfo::GetList(PageInfo * pageInfo, string type, string key) {
	string sql = "select id,key,title,content,typeName,UpdateTime,isEncryption from Dat_PersonInfo ";	 
	string whereSql = " where key  = '" + userInfo->key +"'";
	if (type != "")
		whereSql += " and typeName like '%" + CDataTypeTool::GBKToUTF8(type) + "%'";
	if (key != "")
		whereSql += " and title like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";

	sql += whereSql;
	pageInfo->whereSql = whereSql;
	CCommon * pOperation = new CCommon;
	pOperation->getPageInfo(DataType::personal, pageInfo);
	delete pOperation;
	sql += " order by  UpdateTime desc limit  " + CDataTypeTool::intTostring(pageInfo->pageSize);
	sql += " OFFSET  " + CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);
	 
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = reader.GetStringValue(3);
		dataInfo->typeName = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(5));
		dataInfo->isEncryption = reader.GetIntValue(6);
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}

vector<string> CPersonInfo::GetTypeList(){
	string sql = "select distinct typeName from Dat_PersonInfo"; 
			sql += " where key  = '" + userInfo->key + "'";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());
	vector<string> list;

	while (reader.Read()) { 
		list.push_back(CDataTypeTool::UTF8ToGBK(reader.GetStringValue(0)));
	}
	reader.Close();
	delete sqliteServer;
	return list;
}
