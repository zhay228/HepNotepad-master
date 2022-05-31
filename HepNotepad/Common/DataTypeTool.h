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
	///stringתwstring������ʹ��UFT-8����ĵط�,���Ĳ������� ���ܵ���UTF8ToGBK();StringToWString();
	static wstring AcsiToWideByte(string& data);

	static wstring Utf8ToUnicode(const string& data);
	static string  UnicodeToUtf8(const wstring& data);
	
	static string UTF8ToASCII(string& data);
	static string ASCIIToUTF8(string& data);
	 
	static string UTF8ToGBK(const char* data);
	static string GBKToUTF8(const string& data);
	 
	///stringתwstring������ʹ��UFT-8����ĵط�,��Ҫ����GBKToUTF8�������ĲŲ�������
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