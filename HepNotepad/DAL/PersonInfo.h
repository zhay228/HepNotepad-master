#pragma once
#include "../stdafx.h"
class CPersonInfo
{
public:
	CPersonInfo();
	~CPersonInfo();
	int getMaxId();
	int IsExist(string name);
	bool Add(DataInfo * dataInfo);
	bool Delete(string id);
	bool Update(DataInfo * dataInfo);
	vector<DataInfo*> GetList(PageInfo * pageInfo, string type="",string key="");
	vector<string> GetTypeList();
};

