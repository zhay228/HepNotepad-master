#include "Weekly.h"
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h"
#include "../Common/DataTypeTool.h"
#include "Common.h"

extern UserInfo* userInfo;
CWeekly::CWeekly()
{
}


CWeekly::~CWeekly()
{
}
 
int CWeekly::getMaxId(int type) {
	string sql = "select ifnull(max(id),0) as maxid from Dat_Weekly";
	if(type==2)
		sql = "select ifnull(max(id),0) as maxid from Dat_WeekDetail";
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

bool CWeekly::AddWeek(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;

	string sql = _T("insert into Dat_Weekly(id,guid,Key,year,Monday,Sunday,WeekIndex,UserId,UserName,CreateTime,UpdateTime) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += ",'" + userInfo->key + "'";
	sql += ",'" + GetYear() + "'";
	sql += ",'" + dataInfo->monday + "'";
	sql += ",'" + dataInfo->sunday + "'";
	sql += ",'" + CDataTypeTool::intTostring(GetWeekIndex()) + "'";
	sql += ",'" + dataInfo->userId + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(dataInfo->userName) + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += ")";
	dataInfo->id = CDataTypeTool::intTostring(id);
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

string CWeekly::GetCurrentWeekId() {
	string monday = GetMonday();
	string sql = "select id from Dat_Weekly where monday = '" + monday + "'";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	int wId = 0;
	while (reader.Read()) {
		wId = reader.GetIntValue(0);
	}
	reader.Close();
	if (wId < 1) {
		string sunDay = GetSunday();
		DataInfo* weekly = new DataInfo;
		weekly->monday = monday;
		weekly->sunday = sunDay;
		AddWeek(weekly);
		wId = atoi(weekly->id.c_str());
	}
	
	delete sqliteServer;
	return CDataTypeTool::intTostring(wId);
}

bool CWeekly::AddWeekDetail(DataInfo * dataInfo) {
	if (dataInfo == nullptr)	return false;
	 
	string dayIndex = CDataTypeTool::intTostring(GetDayinWeekIndex());

	string wId = GetCurrentWeekId();
	string sql = _T("insert into Dat_WeekDetail(id,guid,year,weekIndex,dayIndex,Content,CreateTime,UpdateTime) values (");
	int id = getMaxId(2);
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += ",'" + GetYear() + "'";
	sql += ",'" + CDataTypeTool::intTostring(GetWeekIndex()) + "'";
	sql += ",'" + dayIndex + "'";
	sql += ",'" + CDataTypeTool::GBKToUTF8(replace(dataInfo->content, "'", "''")) + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += ",'" + GetTimeInfo() + "'";
	sql += ")";
	dataInfo->id = CDataTypeTool::intTostring(id);
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}


bool CWeekly::Update(string id,string  content) {
	 
	string sql = "update Dat_WeekDetail set UpdateTime = '"+ GetTimeInfo() +"'";
	sql += ",Content =  '" + CDataTypeTool::GBKToUTF8(replace(content, "'", "''")) + "' ";
	sql += " where id =" + id;
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}
 
bool CWeekly::Delete(string id) {

	string sql = "delete from Dat_WeekDetail where exists(select * from Dat_Weekly W where W.year = year and W.weekIndex = weekIndex and W.id =" + id+" )";	 
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);

	sql = "delete from Dat_Weekly";
	sql += " where id =" + id;
	result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

DataInfo* CWeekly::GetTodayData() {
	string dayIndex = CDataTypeTool::intTostring(GetDayinWeekIndex()); 
	string sql = "select id,Content  from Dat_WeekDetail where dayIndex = "+ dayIndex;
	sql += " and weekIndex = '" + CDataTypeTool::intTostring(GetWeekIndex()) + "' and year = '" + GetYear() + "'";
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	DataInfo* dataInfo = new DataInfo;
	while (reader.Read()) { 
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->content = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1)); 
	}
	reader.Close();
	delete sqliteServer;
	return dataInfo;
}

vector<DataInfo*> CWeekly::GetList(PageInfo* pageInfo) {
	string sql = "select W.id,w.monday as startDay";
	sql += ",( select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =1 ) monday";
	sql += ",(select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =2 ) Tuesday";
	sql += ",(select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =3) wednesday";
	sql += ",(select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =4) thursday";
	sql += ",(select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =5) friday";
	sql += ", (select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =6) saturday";
	sql += ", (select Content from  Dat_WeekDetail where weekIndex = W.weekIndex and year = W.year and dayIndex =7) sunday";
	sql += ", w.sunday ,w.WeekIndex from Dat_Weekly W order by monday desc";

	CCommon * pOperation = new CCommon;
	pOperation->getPageInfo(DataType::weekly, pageInfo);
	delete pOperation;

	sql += " limit  " + CDataTypeTool::intTostring(pageInfo->pageSize);
	sql += " OFFSET  " + CDataTypeTool::intTostring(pageInfo->currPageIndex * pageInfo->pageSize);

	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataInfo*> list;
	while (reader.Read()) {
		DataInfo * dataInfo = new DataInfo;
		dataInfo->id = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataInfo->title = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1)); 
		dataInfo->monday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		dataInfo->tuesday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(3));
		dataInfo->wednesday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(4));
		dataInfo->thursday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(5));
		dataInfo->friday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(6));
		dataInfo->saturday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(7));
		dataInfo->sunday = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(8));
		dataInfo->remark = reader.GetStringValue(9);
		dataInfo->pId = CDataTypeTool::intTostring(reader.GetIntValue(10));
		list.push_back(dataInfo);
	} 
	reader.Close();
	delete sqliteServer;
	return list;
}
 