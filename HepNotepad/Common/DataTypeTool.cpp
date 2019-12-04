#include <windows.h> 
#include "DataTypeTool.h"
  
#pragma region  --- 编码转码  ---
//unicode -> ansi 
string CDataTypeTool::WideByteToAnsi(wstring& data){
	int asciisize = ::WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw exception("Error in conversion.");
	}
	vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize)
	{
		throw exception("La falla!");
	}
	return string(&resultstring[0]);
}

//unicode <- ansi 
wstring CDataTypeTool::AnsiToWideByte(const string& data)
{
	int len = data.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, NULL, 0);
	if (unicodeLen == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw exception("Invalid UTF-8 sequence.");
	}
	if (unicodeLen == 0)
	{
		throw exception("Error in conversion.");
	}
	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	int convresult = ::MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	if (convresult != unicodeLen)
	{
		throw exception("La falla!");
	}
	wstring rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode; 
	return rt;
} 

//utf-8 -> ansi
string CDataTypeTool::UTF8ToANSI(string& data){
	using namespace std;
	string strRet = "";
	//先把 utf8 转为 unicode 
	wstring wstr = Utf8ToUnicode(data);
	//最后把 unicode 转为 ascii 
	strRet = WideByteToAnsi(wstr);
	return strRet;
}

//utf-8 <- ansi
string CDataTypeTool::ANSIToUTF8(string& data){
	using namespace std;
	 wstring wstr = L"";
	 //先把 ansi 转为 unicode 
	 wstr = AnsiToWideByte(data);
	 //最后把 unicode 转为 utf-8
	 string strRet = UnicodeToUtf8(wstr); 
	return strRet;
}
 
//unicode -> ascii 
string CDataTypeTool::WideByteToAcsi(wstring& data){
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, data.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw exception("Error in conversion.");
	}
	vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, data.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);
	if (convresult != asciisize)
	{
		throw exception("La falla!");
	}
	return string(&resultstring[0]);
}
 
//unicode <- ascii
wstring CDataTypeTool::AcsiToWideByte(string& data){
	using namespace std;
	int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)data.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw exception("Error in conversion.");
	}
	vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)data.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw exception("La falla!");
	}
	return wstring(&resultstring[0]);
}
 
//utf-8 -> unicode 
wstring CDataTypeTool::Utf8ToUnicode(const string& data) {
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw exception("Error in conversion.");
	}
	vector<wchar_t> resultstring(widesize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw exception("La falla!");
	}
	return wstring(&resultstring[0]);
}

//utf-8 <- unicode
string CDataTypeTool::UnicodeToUtf8(const wstring& data){
	using namespace std;
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, data.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw exception("Error in conversion.");
	}
	vector<char> resultstring(utf8size);
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, data.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);
	if (convresult != utf8size)
	{
		throw exception("La falla!");
	}
	return string(&resultstring[0]);
}
  
//utf-8 <- ascii
string CDataTypeTool::ASCIIToUTF8(string& data) {
	using namespace std;
	string strRet("");
	//先把 ascii 转为 unicode 
	wstring wstr = AcsiToWideByte(data);
	//最后把 unicode 转为 utf8 
	strRet = UnicodeToUtf8(wstr);
	return strRet;
}
    
//utf-8 -> ascii 
string CDataTypeTool::UTF8ToASCII(string& data){
	using namespace std;
	string strRet = "";
	//先把 utf8 转为 unicode 
	wstring wstr = Utf8ToUnicode(data);
	//最后把 unicode 转为 ascii 
	strRet = WideByteToAcsi(wstr);
	return strRet;
}
 
//utf-8 -> gbk
string CDataTypeTool::UTF8ToGBK(const char* data)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, data, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, data, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

//utf-8 <- gbk
string CDataTypeTool::GBKToUTF8(const string& data)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}
#pragma endregion

//string 转 wstring
wstring CDataTypeTool::StringToWString(const string& data)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, wide, num);
	wstring w_str(wide);
	delete[] wide;
	return w_str;
}

//string 转 wstring
string CDataTypeTool::WStringToString(const wstring& data)
{

	string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = data.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
 
WCHAR* CDataTypeTool::CharToWchar(char* data){

	int iSize;
	wchar_t* pwszUnicode;

	//返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
	iSize = MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0); //iSize =wcslen(pwsUnicode)+1=6
	pwszUnicode = (wchar_t *)malloc(iSize*sizeof(wchar_t)); //不需要 pwszUnicode = (wchar_t *)malloc((iSize+1)*sizeof(wchar_t))
	MultiByteToWideChar(CP_ACP, 0, data, -1, pwszUnicode, iSize);
	return pwszUnicode;
}

char* CDataTypeTool::WcharToChar(const WCHAR* data)
{
	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, data, wcslen(data), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, data, wcslen(data), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

WCHAR* CDataTypeTool::CharToWchar(const char* data)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, data, strlen(data), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, data, strlen(data), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

string CDataTypeTool::intTostring(int data){
	char itc[10];
	sprintf(itc, "%d", data);
	string str = itc;
	return str;
}

char * CDataTypeTool::intTochar(int data){
	char itc[10];
	sprintf(itc, "%d", data);
	return itc;
}
