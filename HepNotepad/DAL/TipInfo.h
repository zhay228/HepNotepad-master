#pragma once
#include "../stdafx.h"
 
class CTipInfo
{
public:
	CTipInfo();
	~CTipInfo();
	int getMaxId();
	bool IsExist(string name);
	bool Add(DataInfo * dataType);
	bool Delete(string id);
	bool Update(DataInfo * dataType);
	vector<DataInfo*> GetList(PageInfo * pageInfo, string key = "");
	vector<DataInfo*> GetListRecent(string key = "", int orderType = 0);
};

