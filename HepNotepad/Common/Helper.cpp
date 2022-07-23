 
#include "../stdafx.h"
#include <winver.h>
#pragma comment(lib, "Version.lib")
#include "Helper.h"
//#include<iostream>
//#include<atltime.h>
  
std::string  GenerateGuid()
{
	GUID guid;
	CoCreateGuid(&guid);
	char cBuffer[64] = { 0 };
	sprintf_s(cBuffer, sizeof(cBuffer),
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2,
		guid.Data3, guid.Data4[0],
		guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4],
		guid.Data4[5], guid.Data4[6],
		guid.Data4[7]);
	return string(cBuffer);
}

string gettime(string _date, string _time, bool full){
	char info[1000];
	string date = "";
	if (_date.size() < 3){
		SYSTEMTIME stime;
		GetLocalTime(&stime);
		sprintf(info, "%02d:%02d:%02d", stime.wHour, stime.wMinute, stime.wSecond);
		date = info;
	}
	else{
		SYSTEMTIME stime;
		GetLocalTime(&stime);
		sprintf(info, "%02d-%02d-%02d", stime.wYear, stime.wMonth, stime.wDay);
		date = info;
		if (date == _date) //判断是否是当前日期
		{
			date = _time.c_str();
		}
		else {
			CDuiString tempDate = _date.c_str();
			tempDate = tempDate.Mid(5, tempDate.GetLength()); //去年份
			tempDate.Replace("-", "/");
			date = tempDate.GetData();
			if (full){
				date += " ";
				date += _time;
			}
		}
	}
	return date;
}

char * getParam(cJSON *data, string  msg, char* paramName){
	try	{
		if (msg.find(paramName) != msg.npos)
			return cJSON_GetObjectItem(data, paramName)->valuestring;
		else
			return _T("");
	}
	catch (exception const &exception) {
		return _T("");
	}
}

int GetGmtTime(char* szGmtTime)
{
	if (szGmtTime == NULL)
	{
		return -1;
	}
	time_t rawTime;
	struct tm* timeInfo;
	char szTemp[30] = { 0 };
	time(&rawTime);
	timeInfo = gmtime(&rawTime);
	strftime(szTemp, sizeof(szTemp), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
	strcpy_s(szGmtTime, strlen(szTemp) + 1, szTemp);//必须这样，避免内存释放，内容没有的问题。 
	return strlen(szGmtTime);
}

double difTime(time_t start, time_t end){ 
	double dif; 
	time(&end);
	dif = difftime(end, start);
	return dif;
}

time_t StringToDatetime(std::string str)
{
	char *cha = (char*)str.data();             // 将string转换成char*。
	tm tm_;                                    // 定义tm结构体。
	int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
	sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
	tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
	tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
	tm_.tm_mday = day;                         // 日。
	tm_.tm_hour = hour;                        // 时。
	tm_.tm_min = minute;                       // 分。
	tm_.tm_sec = second;                       // 秒。
	tm_.tm_isdst = 0;                          // 非夏令时。
	time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
	return t_;                                 // 返回值。
}
  
void GetTimeInfo(string & date, string & timeInfo){

	time_t timep ;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y-%m-%d", dateInfo);
	strftime(szTime, sizeof(szTime), "%H:%M:%S", dateInfo);
	date = szDate;
	timeInfo = szTime; 
}

string GetTimeInfo() {
	 
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y-%m-%d %H:%M:%S", dateInfo); 
	return szDate;

}

string GetTimeInfoNumber() {
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y%m%d%H%M%S", dateInfo);
	return szDate;
}

STDAPI_(int) exception_filter(LPEXCEPTION_POINTERS p_exinfo)
{
	if (p_exinfo->ExceptionRecord->ExceptionFlags == 0)
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else{
		return EXCEPTION_EXECUTE_HANDLER; //告诉except处理这个异常
	}
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}
	return ret;
}

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size() * 4;
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}
int GetCharCount(string str){
	char info[30720];
	sprintf(info, "%s", str.c_str());
	return GetCharCount(info);
}
 
int GetCharCount(char * buffer){
	int count = 0; 
	while (*buffer++ != '\0')
	{
		count++;
		//当前字符是中文字符，则让指针多移一位，因为中文字符占两个字节
		if (!(*buffer >= 0 && *buffer <= 127))
			buffer++;
	}
	return count;
}

int GetStringWidth(HDC hDC, string str, int _windth ){
	LOGFONTA lfText = { -16, 0, 0, 0, FW_NORMAL, 0, 0, 0, 134, 0, 0, 0, FF_SWISS, "微软雅黑" };
	int width = _windth, dis_num = 100;//区域内可显示的字符个数，及区域大小（像素点的范围）
	HFONT hfontText, hfontOld;
	HDC hMemDC = CreateCompatibleDC(hDC);
	hfontText = CreateFontIndirectA(&lfText);
	hfontOld = (HFONT)SelectObject(hMemDC, hfontText);
	SIZE str_size = { 0 };
	GetTextExtentExPoint(hMemDC, str.c_str(), lstrlen(str.c_str()), width, &dis_num, NULL, &str_size);
	hfontText = (HFONT)SelectObject(hMemDC, hfontOld);
	DeleteObject(hfontText);
	return str_size.cx;
}

int GetStringWidth(HDC hDC, string str, int _windth, int fontsize){
	LOGFONTA lfText = { -(fontsize * 20 / 15), 0, 0, 0, FW_NORMAL, 0, 0, 0, 134, 0, 0, 0, FF_SWISS, "微软雅黑" };
	int width = _windth, dis_num = 100;//区域内可显示的字符个数，及区域大小（像素点的范围）
	HFONT hfontText, hfontOld;
	HDC hMemDC = CreateCompatibleDC(hDC);
	hfontText = CreateFontIndirectA(&lfText);
	hfontOld = (HFONT)SelectObject(hMemDC, hfontText);
	SIZE str_size = { 0 };
	GetTextExtentExPoint(hMemDC, str.c_str(), lstrlen(str.c_str()), width, &dis_num, NULL, &str_size);
	hfontText = (HFONT)SelectObject(hMemDC, hfontOld);
	DeleteObject(hfontText);
	return str_size.cx;
}
 
int SubStr(string str, int start, int len, std::string &strData){
	string left, destStr;
	int i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (i < start){
			if (str[i] < 255 && str[i]>0)//扩充的ASCII字符范围为0-255,如是,处理一个字节
			{
				left.append(str.substr(i, 1));
			}
			else//<0,>255的是汉字,处理两个字节
			{
				left.append(str.substr(i, 2));
				++i;
			}
		}
		else if (i >= start&&i - start <= len){
			if (str[i] < 255 && str[i]>0)//扩充的ASCII字符范围为0-255,如是,处理一个字节
			{
				destStr.append(str.substr(i, 1));
			}
			else//<0,>255的是汉字,处理两个字节
			{
				destStr.append(str.substr(i, 2));
				++i;
			}
		}
		else
		{
			break;
		}
	}
	strData = destStr;
	return i;
}

void WaitMsg(int msg){
	MSG Message;
	while (TRUE)
	{
		if (GetMessage(&Message, NULL, 0, 0))
		{
			if (Message.message == msg)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else break;
	}
}

string intToDaXie(int index)
{
	vector<string> list = {"零", "一","二","三","四","五","六","七","八","九","十" };
	if (index > -1 && index < list.size())
		return list[index];
	return "";
}

string GetYear(){
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);

	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y", dateInfo);
	return szDate;
}

string GetMonday() {
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);

	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y-%m-%d 00:00:00", dateInfo);
	time_t today = StringToDatetime(szDate);

	char tmp[64];
	strftime(tmp, sizeof(tmp), "%u", dateInfo); 
	today -= (atoi(tmp)-1) * 86400;
	dateInfo = localtime(&today);
	strftime(szDate, sizeof(szDate), "%Y-%m-%d", dateInfo);
	return szDate; 
}

string GetSunday() {
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);

	char szDate[30] = { 0 }, szTime[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%Y-%m-%d 00:00:00", dateInfo);
	time_t today = StringToDatetime(szDate);

	char tmp[64];
	strftime(tmp, sizeof(tmp), "%u", dateInfo);
	today -= (atoi(tmp) - 1) * 86400;
	today += 6 * 86400;
	dateInfo = localtime(&today);
	strftime(szDate, sizeof(szDate), "%Y-%m-%d", dateInfo);

	return szDate;
}

int GetWeekIndex() {
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);

	char szDate[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%U", dateInfo);
	return atoi(szDate);
}

int GetDayinWeekIndex() {
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%u", dateInfo);
	return atoi(szDate);
}

int GetHour(){
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%H", dateInfo);
	return atoi(szDate);
}
int GetMinute(){
	time_t timep;
	time(&timep);
	struct tm* dateInfo;
	dateInfo = localtime(&timep);
	char szDate[30] = { 0 };
	strftime(szDate, sizeof(szDate), "%M", dateInfo);
	return atoi(szDate);
}
string replace(string str, string old, string newStr) {
	CDuiString content = str.c_str();
	content.Replace("'", "''");
	str = content.GetData();
	return str;
}

BOOL IsVista()
{
	OSVERSIONINFO osver;

	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (::GetVersionEx(&osver) &&
		osver.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		(osver.dwMajorVersion >= 6))
	{
		return TRUE;
	}

	return FALSE;
}

string GetProductVersion(const char* lpszFileName)
{
	TCHAR szFileName[MAX_PATH] = { 0 }; // 要获取信息的文件  
	if (lpszFileName == NULL)
	{
		::GetModuleFileName(NULL, szFileName, sizeof(szFileName));
	}
	else
	{
		lstrcpy(szFileName, lpszFileName);
	}

	//得到容纳文件版本信息需要缓冲区的大小,如文件不包含版本信息,则返回0.
	string sVerInfo;
	DWORD dwVerHnd = 0;
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szFileName, &dwVerHnd);
	if (0 != dwVerInfoSize)
	{

		HANDLE hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		LPVOID lpMem = ::GlobalLock(hMem);
		if (NULL == lpMem)
		{
			return "";
		}

		//获取文件版本信息,保存到lpvMem中,非0表示成功,0表示失败
		if (::GetFileVersionInfo(szFileName, dwVerHnd, dwVerInfoSize, lpMem))
		{
			//从版本资源中获取查询的信息复制到一个缓冲区pFileInfo里
			//非零表示成功,如请求的信息不存在,或pBlock不属于有效版本信息,那就返回一个零
			const char* lpSubBlock = TEXT("\\");	//"\"标示获取文件的VS_FIXEDFILEINFO结构
			VS_FIXEDFILEINFO *pFileInfo = NULL;	//用于装载请求到的版本信息
			unsigned int uInfoSize = 0;			//请求到的版本信息的长度
			if (::VerQueryValue(lpMem, lpSubBlock, (void**)&pFileInfo, &uInfoSize))
			{
				//获取"产品版本"(区分与文件版本)
				WORD nVer[4] = { 0 };
				nVer[0] = HIWORD(pFileInfo->dwProductVersionMS);
				nVer[1] = LOWORD(pFileInfo->dwProductVersionMS);
				nVer[2] = HIWORD(pFileInfo->dwProductVersionLS);
				nVer[3] = LOWORD(pFileInfo->dwProductVersionLS);
				char szVerInfo[256] = { 0 };
				sprintf_s(szVerInfo, _T("%d.%d.%d"), nVer[0], nVer[1], nVer[2]);
				sVerInfo = szVerInfo;
			}
		}
		::GlobalUnlock(hMem);
		::GlobalFree(hMem);
	}

	return  sVerInfo;
}
 