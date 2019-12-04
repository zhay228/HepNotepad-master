#pragma once
class CSqliteServer
{
public:
	CSqliteServer();
	~CSqliteServer();
	bool Open(string dataFile);
	bool Close();
	bool ExcuteNonQuery(string sql);

private:
	sqlite3 * pDB;
	
};

