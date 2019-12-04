#pragma once

std::wstring Utf82Unicode(const std::string& utf8string);
std::string WideByte2Acsi(std::wstring& wstrcode);
std::string UTF_82ASCII(std::string& strUtf8Code);
std::wstring Acsi2WideByte(std::string& strascii);
std::string Unicode2Utf8(const std::wstring& widestring);
std::string ASCII2UTF_8(std::string& strAsciiCode);
std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring &wstr);