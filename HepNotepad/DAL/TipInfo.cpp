 
#include "TipInfo.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "Common.h"
CTipInfo::CTipInfo()
{
}


CTipInfo::~CTipInfo()
{
}


int CTipInfo::getMaxId() {
	string sql = "select ifnull(max(id),0) as maxid from Dat_Tip";
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

bool CTipInfo::Add(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;
	if (IsExist(dataInfo->content))return false;

	CDuiString str = dataInfo->content.c_str();
	vector<CDuiString>  listStr = StrSplit(str, "\r");
	if (listStr.size() > 0)
		dataInfo->title = listStr[0].GetData();
	else
		dataInfo->title = str.GetData();

	string sql = _T("insert into Dat_Tip(id,guid,Key,Title,Content,UserId,CreateTime,UserName,UpdateTime,Grade) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id); 
	sql += ",'" + GenerateGuid()+ "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->key) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content,"'","''")) + "'";
	sql += "," + CDataTypeTool::GBKToUTF8(dataInfo->userId) + "";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::intTostring(dataInfo->index) + "'";
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	if (result)
		dataInfo->id = CDataTypeTool::intTostring(id);
	delete sqliteServer;
	return result;
}


bool CTipInfo::Update(DataInfo * dataInfo) {

	CDuiString str = dataInfo->content.c_str();
	vector<CDuiString>  listStr = StrSplit(str, "\r");
	if (listStr.size() > 0)
		dataInfo->title = listStr[0].GetData();
	else
		dataInfo->title = str.GetData();

	string sql = "update Dat_Tip set "; 
	sql += "Content =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "' ";
	if (dataInfo->title != "")
		sql += ",Title =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "' ";
	sql += ",UpdateTime =  '" + dataInfo->updateTime + "' ";
	if(dataInfo->index > -1)
		sql += ",Grade = " + CDataTypeTool::intTostring(dataInfo->index); 

	sql += " where id =" + dataInfo->id;
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);

	delete sqliteServer;
	return result;
}

bool CTipInfo::IsExist(string content) {
	string sql = "select ifnull(max(id),0) as  maxId from Dat_Tip  ";
	sql += " where Content = '" + CDataTypeTool::GBKToUTF8(content) + "'";
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

bool CTipInfo::Delete(string id) {
	string sql = "delete from Dat_Tip";	 
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;

}

vector<DataInfo*> CTipInfo::GetList(PageInfo * pageInfo, string key) {
	string sql = "select id,key,title,content,UpdateTime,Grade from Dat_Tip ";
	string whereSql = " where 1 = 1 ";
	if (key != "")
		whereSql += " and Title like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";
	sql += whereSql;
	if (pageInfo != nullptr) {
		pageInfo->whereSql = whereSql;
		CCommon * pOperation = new CCommon;
		pOperation->getPageInfo(DataType::tip, pageInfo);
		delete pOperation;
		sql += " order by UpdateTime desc  limit  " + CDataTypeTool::intTostring(pageInfo->pageSize);
		sql += " OFFSET  " + CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);

	}
	
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->dataType = DataType::tip;
		dataInfo->index = reader.GetIntValue(5);
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}



vector<DataInfo*> CTipInfo::GetListRecent(string key, int orderType) {
	string sql = "select id,key,title,content,UpdateTime,Grade from Dat_Tip ";
	string whereSql = " where 1 = 1 ";
	if (key != "")
		whereSql += " and Title like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";
	sql += whereSql;
	
	if (orderType == 0) {
		sql += " order by usecount  desc  limit  " + CDataTypeTool::intTostring(10);
	}
	else {
		sql += " order by useTime desc  limit  " + CDataTypeTool::intTostring(10);
	}
	sql += " OFFSET 0 ";
	

	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->dataType = DataType::tip;
		dataInfo->index = reader.GetIntValue(5);
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}
