#pragma once
#include "../stdafx.h"
class CCalendar
{
public:
	CCalendar();
	~CCalendar();
	int getMaxId();
	int IsExist(string name);
	bool Add(DataInfo * dataInfo);
	bool Delete(string id);
	bool ClearHistory(string key, int type);
	bool Update(DataInfo * dataInfo);
	bool Update(string id);
	vector<DataInfo*> GetList(PageInfo* pageInfo, string key, int type, int isRemind); 
	vector<DataInfo*> GetList();

};

