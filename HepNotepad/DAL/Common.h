#pragma once

struct  TableInfo
{
	string Name;
	string columns;
};

class CCommon
{
public:
	CCommon();
	~CCommon(); 
	int getMaxId(string tableName);
	bool IsExist(string tableName, string guid);
	void getPageInfo(DataType dataType, PageInfo * pageInfo);
	bool DatabaseCheck(string database);
	void DataInput(string database);
};

