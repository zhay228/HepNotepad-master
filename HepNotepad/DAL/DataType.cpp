#include "../stdafx.h"
#include "DataType.h" 
#include  "../Common/SQLite.h"
#include "../Common/SqliteServer.h" 
#include "../Common/DataTypeTool.h"
CDataType::CDataType()
{
}


CDataType::~CDataType()
{
}
 
void CDataType::DataOperator(){
	/*string dataFile = "F:\\data.db";
	CSqliteServer sqlite;
	if (sqlite.Open(dataFile)){
	wstring wsql = L"begin;\n INSERT INTO Dat_Type values(15, 'test', '中国');\n commit;";
	string sql = Unicode2Utf8(wsql);
	if (sqlite.ExcuteNonQuery(sql))
	{
	MessageBox(NULL,_T("添加数据成功"),_T("提示"),MB_OK);
	}
	}

	sqlite.Close();*/

	SQLite sqlite;
	string database = "data.db";
	if (sqlite.Open(database.c_str())){
		char *sql = "INSERT INTO Dat_Type values(11, 'test', '完全')";
		/*if (sqlite.ExcuteNonQuery(sql)){
		MessageBox(NULL, "添加数据成功", "提示", MB_OK);
		}*/
		/*sql = "delete from dat_Type where Id=10 ";
		if (sqlite.ExcuteNonQuery(sql)){
		MessageBox(NULL, "删除数据成功", "提示", MB_OK);
		}*/
		sql = "select * from dat_Type  ";
		SQLiteDataReader reader = sqlite.ExcuteQuery(sql);
		int count = reader.ColumnCount();
		wstring wdata = L"";
		while (reader.Read()){
			string data = reader.GetStringValue(1);
			wdata = CDataTypeTool::Utf8ToUnicode(data);			
			string d = CDataTypeTool::WStringToString(wdata);

		}
	}

	sqlite.Close();
}
 
int CDataType::getMaxId() {
	string sql = "select ifnull(max(Id),0) as maxid from Dat_Type";
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

bool CDataType::Add(DataTypeInfo * dataType) {
	if (dataType == nullptr)	return false;

	string sql = _T("insert into Dat_Type(Id,guid,Name,Remark) values (");
	int id = getMaxId();
	sql += CDataTypeTool::intTostring(id);
	sql += ",'" + GenerateGuid()+ "'";
	sql += "," + CDataTypeTool::GBKToUTF8(dataType->name) + "";
	sql += "," + CDataTypeTool::GBKToUTF8(dataType->remark) + "";
	sql += ")";
	CSqliteServer * sqliteServer = new CSqliteServer;
	bool result = sqliteServer->ExecuteNoneQuery(sql);
	delete sqliteServer;
	return result;
}

int CDataType::IsExist(string name) {
	string sql = "select ifnull(max(Id),0) as  maxId from Dat_Type  ";
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
 
bool CDataType::Delete(int id) {

	return true;

}
 
vector<DataTypeInfo*> CDataType::GetList() {
	string sql = "select Id,Name,Remark  from Dat_Type ";	
	CSqliteServer * sqliteServer = new CSqliteServer;
	SQLiteDataReader reader = sqliteServer->ExecuteQuery(sql.c_str());

	vector<DataTypeInfo*> list;
	while (reader.Read()) {
		DataTypeInfo * dataType = new DataTypeInfo;
		dataType->typeId = CDataTypeTool::intTostring(reader.GetIntValue(0));
		dataType->name = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(1));
		dataType->remark = CDataTypeTool::UTF8ToGBK(reader.GetStringValue(2));
		 
		list.push_back(dataType);
	}
	reader.Close();
	delete sqliteServer;
	return list;
}