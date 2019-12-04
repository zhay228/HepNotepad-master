#pragma once
#include "../stdafx.h"
class CSystemInfo
{
public:
	CSystemInfo();
	~CSystemInfo();
	int getMaxId();
	int IsExist(string name);
	bool Add(DataInfo * dataInfo);
	bool Delete(string id);
	bool Update(DataInfo * dataInfo);
	bool UpdateValue(string name, string value = "", string dataValue = "");
	vector<DataInfo*> GetList(string name = ""); 
};

