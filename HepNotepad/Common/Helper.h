#pragma once

std::string GenerateGuid();
char * getParam(cJSON *data, string  msg, char* paramName);
string gettime(string date, string time, bool full = false);
int GetGmtTime(char* szGmtTime);
STDAPI_(int) exception_filter(LPEXCEPTION_POINTERS p_exinfo);
 
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);

int GetCharCount(string str); 
int GetCharCount(char * buffer);

int GetStringWidth(HDC hDC, string str, int _windth = 100);
int GetStringWidth(HDC hDC, string str, int _windth, int fontsize);
int SubStr(string str, int start, int len, std::string &strData);

string GetTimeInfo();
string GetTimeInfoNumber();
void GetTimeInfo(string & date, string & timeInfo); 
double difTime(time_t start, time_t end);
time_t StringToDatetime(std::string str);

int GetHour();
int GetMinute();
string GetYear();
string GetMonday();
string GetSunday();
int GetWeekIndex();// 每年第几周
int GetDayinWeekIndex(); //获取今天是这周几天星期为:1
void WaitMsg(int msg);
string intToDaXie(int index);
string replace(string str, string old, string newStr);
BOOL IsVista();
string GetProductVersion(const char* lpszFileName);