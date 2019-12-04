#pragma once
#include "../stdafx.h"
class CDataInfo
{
public:
	CDataInfo();
	~CDataInfo();
	int getMaxId();
	int IsExist(string name);
	bool Add(DataInfo * dataInfo);
	bool Delete(string id);
	bool Update(DataInfo * dataInfo);
	vector<DataInfo*> GetList(PageInfo * pageInfo, string type = "", string key = "");
	vector<DataInfo*> GetListRecent(string key = "", int orderType = 0);
	vector<string> GetTypeList();
	vector<DataInfo*> Query(string key, int orderType = 0);
	DataInfo* GetModel(string id);
	void UpdateCount(string id, DataType type);
};

