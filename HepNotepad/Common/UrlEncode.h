 //>->18.11.10 GB2312±àÂë£¬½âÂë
#pragma once
 #include <string>
using namespace  std;

unsigned char FromHex(unsigned char x);

unsigned char ToHex(unsigned char x);

std::string UrlEncode(const std::string& str);

std::string UrlDecode(const std::string& str);

const std::string ws2s(const std::wstring& ws);