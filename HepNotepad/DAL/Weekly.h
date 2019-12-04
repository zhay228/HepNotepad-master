#pragma once
#include "../stdafx.h"
class CWeekly
{
public:
	CWeekly();
	~CWeekly();
	int getMaxId(int type = 1); 
	bool AddWeek(DataInfo * dataInfo);
	bool AddWeekDetail(DataInfo * dataInfo);
	bool Delete(string id);
	bool Update(string id,string  content);
	vector<DataInfo*> GetList(PageInfo* pageInfo);
	string GetCurrentWeekId();
	DataInfo* GetTodayData();
};

