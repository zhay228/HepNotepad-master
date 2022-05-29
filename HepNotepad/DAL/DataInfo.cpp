
#include "DataInfo.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "Common.h";
#include "TipInfo.h"
CDataInfo::CDataInfo()
{
}


CDataInfo::~CDataInfo()
{
}


int CDataInfo::getMaxId() {
	string sql = "select ifnull(max(id),0) as maxid from Dat_Info";
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

bool CDataInfo::Add(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;

	string sql = _T("insert into Dat_Info(id,guid,isCtrl,isAlt,IsShift,Key,Title,Content,UserId,CreateTime,UserName,UpdateTime,TypeName) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += "," + CDataTypeTool::intTostring(dataInfo->isCtrl) + "";
	sql += "," + CDataTypeTool::intTostring(dataInfo->isAlt) + "";
	sql += "," + CDataTypeTool::intTostring(dataInfo->isShift) + "";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->key) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userId) + "'";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + dataInfo->createTime + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->typeName) + "'";
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	if (result)
		dataInfo->id = CDataTypeTool::intTostring(id);
	delete sqliteServer;
	return result;
}


bool CDataInfo::Update(DataInfo * dataInfo) {
	string sql = "update Dat_Info set UpdateTime = '" + dataInfo->updateTime + "'";

	if (dataInfo->typeName != "")
		sql += ",typeName =  '" + CDataTypeTool::GBKToUTF8(dataInfo->typeName) + "'";
	if (dataInfo->title != "")
		sql += ",Title =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "' ";
	if (dataInfo->content != "")
		sql += ",Content =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "'";
	sql += " where id =" + dataInfo->id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

int CDataInfo::IsExist(string name) {
	string sql = "select ifnull(max(id),0) as  maxId from Dat_Info  ";
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

bool CDataInfo::Delete(string id) {
	string sql = "delete from Dat_Info";
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

bool CDataInfo::DataClear() {
	string sql = "delete from Dat_Info"; 

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_Calendar";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_Tip";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_Data";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_PersonInfo";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_Type";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_WeekDetail";
	result = sqliteServer->ExecuteNoneQuery(sql);
	sql = "delete from Dat_Weekly";
	result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

vector<DataInfo*> CDataInfo::GetList(PageInfo * pageInfo, string type, string key) {
	string sql = "select id,key,title,content,typeName,UpdateTime from Dat_Info ";
	string whereSql = " where 1 = 1 "; 
	if (type != "")
		whereSql += " and typeName like '%" + CDataTypeTool::GBKToUTF8(replace(type, "'", "''")) + "%'";
	if (key != "")
		whereSql += " and title like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'"; 
	sql += whereSql;
	if (pageInfo != nullptr) {
		pageInfo->whereSql = whereSql;
		CCommon * pOperation = new CCommon;
		pOperation->getPageInfo(DataType::info, pageInfo);
		delete pOperation;
		 
		sql += " order by UpdateTime desc  limit  " + CDataTypeTool::intTostring(pageInfo->pageSize);
		sql += " OFFSET  "+ CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);
		 
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
		dataInfo->typeName = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(5));
		dataInfo->dataType = DataType::info;
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}

vector<DataInfo*> CDataInfo::GetListRecent(string key,  int orderType){
	string sql = "select id,key,title,content,typeName,UpdateTime from Dat_Info ";
	string whereSql = " where 1 = 1 ";
	if (key != "")
		whereSql += " and title like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";
	sql += whereSql;
	
	if (orderType == 0) {
		sql += " order by usecount desc  limit  " + CDataTypeTool::intTostring(10);
	}
	else{
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
		dataInfo->typeName = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(5));
		dataInfo->dataType = DataType::info;
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}

vector<string> CDataInfo::GetTypeList() {
	string sql = "select distinct typeName from Dat_Info ";
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


vector<DataInfo*> CDataInfo::Query(string key, int orderType) {

	vector<DataInfo*> infoList = GetListRecent( key,orderType);
	CTipInfo * pType = new CTipInfo;
	vector<DataInfo*> tipList = pType->GetListRecent(key, orderType);

	vector<DataInfo*> resultList;
	for (int i = 0; i < tipList.size(); i++) {
		/*if (infoList.size() >10 && i > 9 && orderType == 1) {
			delete tipList[i];
			tipList[i] = nullptr;
		}
		else {*/
			resultList.push_back(tipList[i]);
		//}
	}

	for (int i = 0; i < infoList.size(); i++) {
		if (resultList.size() > 15 && orderType == 1) {
			delete infoList[i];
			infoList[i] = nullptr;
		}
		else {
			resultList.push_back(infoList[i]);
		}
	}

	return resultList;
}

DataInfo* CDataInfo::GetModel(string id) {
	string sql = "select id,key,title,content,typeName,UpdateTime from Dat_Info ";
	sql += " where id = " + id;
	 
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());
	 
	DataInfo * dataInfo = new DataInfo;
	while (reader.Read()) {
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->typeName = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->updateTime = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(5));
		dataInfo->dataType = DataType::info;

	}
	reader.Close();
	delete sqliteServer;
	return dataInfo;
}


void CDataInfo::UpdateCount(string id, DataType type) {
	string table = "Dat_Info";
	if (type == DataType::tip)
		table = "Dat_Tip";
	else if (type == DataType::personal)
		table = "Dat_PersonInfo";
	string sql = "update " + table + " set useTime = '" + GetTimeInfo() + "'";
	sql += ",useCount =  ifnull(useCount,0)+1 ";
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer; 
}