#pragma once

#include<vector>;
#include<string>

using namespace std;
class CDataTypeTool
{
public:
	static string WideByteToAnsi(wstring& data);
	static wstring AnsiToWideByte(const string& data);

	static string UTF8ToANSI(string& data);
	static string ANSIToUTF8(string& data);

	static string WideByteToAcsi(wstring& data); 
	///string转wstring在其他使用UFT-8编码的地方,中文不会乱码 功能等于UTF8ToGBK();StringToWString();
	static wstring AcsiToWideByte(string& data);

	static wstring Utf8ToUnicode(const string& data);
	static string  UnicodeToUtf8(const wstring& data);
	
	static string UTF8ToASCII(string& data);
	static string ASCIIToUTF8(string& data);
	 
	static string UTF8ToGBK(const char* data);
	static string GBKToUTF8(const string& data);
	 
	///string转wstring在其他使用UFT-8编码的地方,需要先用GBKToUTF8编码中文才不会乱码
	static wstring StringToWString(const string& data);
	static string  WStringToString(const wstring &data);

	static WCHAR* CharToWchar(char* data);
	static WCHAR* CharToWchar(const char* data);
	static char* WcharToChar(const WCHAR* data);

	static string intTostring(int data);
	static char * intTochar(int data);

	static string toLower(string data);
	static string toUpper(string data);
};