#pragma once

struct  DataTypeInfo
{
	string typeId;
	string name; 
	string remark;
};

class CDataType
{
public:
	CDataType();
	~CDataType();
	void DataOperator();
	int getMaxId();
	int IsExist(string name);
	bool Add(DataTypeInfo * dataType);	
	bool Delete(int id); 
	vector<DataTypeInfo*> GetList();
};

