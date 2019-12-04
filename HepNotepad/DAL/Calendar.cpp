
#include "Calendar.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "Common.h"
extern UserInfo* userInfo;
CCalendar::CCalendar()
{
}


CCalendar::~CCalendar()
{
}


int CCalendar::getMaxId() {
	string sql = "select ifnull(max(id),0) as maxid from Dat_Calendar";
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

bool CCalendar::Add(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;

	string sql = _T("insert into Dat_Calendar(id,guid,Key,Title,Content,UserId,CreateTime,UserName,UpdateTime,RemindType,RemindDate,RemindTime,IsRemind,RemindDateTime,status) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += ",'" + userInfo->key + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userId) + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += "," + CDataTypeTool::intTostring(dataInfo->remindType);
	sql += ",'" + dataInfo->remindDate + "'";
	sql += ",'" + dataInfo->remindTime + "'";
	sql += ",'" + CDataTypeTool::intTostring(dataInfo->isRemind) + "'";
	sql += ",'" + dataInfo->remindDateTime + "'";
	sql += ",'" + CDataTypeTool::intTostring(dataInfo->status) + "'";
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}


bool CCalendar::Update(DataInfo * dataInfo) {
	string sql = "update Dat_Calendar set UpdateTime = '" + GetTimeInfo() + "'";

	if (dataInfo->title != "")
		sql += ",Title =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->title, "'", "''")) + "' ";

	sql += ",Content =  '" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "' ";
	sql += ",RemindType = " + CDataTypeTool::intTostring(dataInfo->remindType);
	sql += ",RemindDate = '" + dataInfo->remindDate + "'";
	sql += ",RemindTime = '" + dataInfo->remindTime + "'";
	sql += ",status = '" + CDataTypeTool::intTostring(dataInfo->status) + "'";
	if (dataInfo->status == 1) {
		sql += ",IsRemind = '0'";
	}
	sql += " where id =" + dataInfo->id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

bool CCalendar::Update(string id) {
	string sql = "update Dat_Calendar set UpdateTime = '" + GetTimeInfo() + "'";
	sql += ",IsRemind = 1";
	sql += ",RemindDateTime = '" + GetTimeInfo() + "' ,status = 0";
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

int CCalendar::IsExist(string name) {
	string sql = "select ifnull(max(id),0) as  maxId from Dat_Calendar ";
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

bool CCalendar::Delete(string id) {
	string sql = "delete from Dat_Calendar";
	sql += " where id =" + id;

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

bool CCalendar::ClearHistory(string key, int type) {
	string sql = "delete from Dat_Calendar where IsRemind = '1'";
	if (type > -1)
		sql += " and RemindType  = '" + CDataTypeTool::intTostring(type) + "'";
	if (key != "")
		sql += " and Content like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";

	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

vector<DataInfo*> CCalendar::GetList(PageInfo* pageInfo, string key, int type, int isRemind) {
	string sql = "select id,Key,Title,Content,RemindType,RemindDate,RemindTime,updateTime,ifnull(RemindDateTime,'') as RemindDateTime,status from Dat_Calendar ";
	string whereSql = " where 1 = 1";
	if (isRemind > -1)
		whereSql += " and IsRemind  = '" + CDataTypeTool::intTostring(isRemind) + "'";

	if (type > -1)
		whereSql += " and RemindType  = '" + CDataTypeTool::intTostring(type) + "'";
	if (key != "")
		whereSql += " and Content like '%" + CDataTypeTool::GBKToUTF8(replace(key, "'", "''")) + "%'";

	pageInfo->whereSql = whereSql;
	sql += whereSql;
	CCommon * pOperation = new CCommon;
	pOperation->getPageInfo(DataType::calendar, pageInfo);
	delete pOperation;

	sql += " order by updateTime desc limit  " + CDataTypeTool::intTostring(pageInfo->pageSize);
	sql += " OFFSET  " + CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);

	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->remindType = reader.GetIntValue(4);
		dataInfo->remindDate = reader.GetStringValue(5);
		if (dataInfo->remindType == 0) {
			dataInfo->typeName = "一次提醒";
		}
		else if (dataInfo->remindType == 1) {
			dataInfo->typeName = "每周提醒";

			dataInfo->remindDateIndex = atoi(dataInfo->remindDate.c_str());
			if (dataInfo->remindDateIndex == 0)
				dataInfo->remindDate = "每周日";
			else
				dataInfo->remindDate = "每周" + intToDaXie(dataInfo->remindDateIndex);
		}
		else if (dataInfo->remindType == 2) {
			dataInfo->remindDate = "";
			dataInfo->typeName = "每天提醒";
		}

		dataInfo->remindTime = reader.GetStringValue(6);
		dataInfo->updateTime = reader.GetStringValue(7);
		dataInfo->remindDateTime = reader.GetStringValue(8);
		dataInfo->status = reader.GetIntValue(9);
		if (dataInfo->status == 0) {
			dataInfo->statusText = "停止";
		}
		else
		{
			dataInfo->statusText = "启动";
		}
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}

vector<DataInfo*> CCalendar::GetList() {

	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y-%m-%d", dateInfo);
	string date = szDate;
	 
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%w", dateInfo);
	string week = tmp;

	strftime(tmp, sizeof(tmp), "%H:%M", dateInfo);
	string time = tmp;

	timep -= 330;
	dateInfo = localtime(&timep);
	strftime(tmp, sizeof(tmp), "%H:%M", dateInfo);
	string prevtime = tmp;

	string sql = "select Id,guid,[Key],Title,Content,UserId,UserName,RemindType,RemindDate,RemindTime from Dat_Calendar where ";
	sql += " (( RemindType = 0 and remindTime > '" + prevtime + "' and remindDate <= '" + date + "' and isRemind = 0 ) or";
	sql += "  ( RemindType = 1 and remindTime > '" + prevtime + "' and remindTime <= '" + time + "' and remindDate = '" + week + "') or";
	sql += "  ( RemindType = 2 and remindTime > '" + prevtime + "' and remindTime <= '" + time + "'))  ";
	sql += " and status = 1 order by updateTime desc  ";


	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->key = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->userId = CDataTypeTool::intTostring(reader.GetIntValue(5));
		dataInfo->userName = reader.GetStringValue(6);
		dataInfo->remindType = reader.GetIntValue(7);
		dataInfo->remindDate = reader.GetStringValue(8);
		dataInfo->remindTime = reader.GetStringValue(9);
		list.push_back(dataInfo);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}
