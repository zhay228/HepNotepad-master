#pragma once
#ifndef _BASE64_H_
#define _BASE64_H_
 
#include <tchar.h> 
#include <string>
using namespace std;
class CBase64Image
{
public:
	CBase64Image();
	~CBase64Image();

	/*编码
	DataByte
	[in]输入的数据长度,以字节为单位
	*/
	static std::string Encode(const char* Data, int DataByte);

	/*解码
	DataByte
	[in]输入的数据长度,以字节为单位
	OutByte
	[out]输出的数据长度,以字节为单位,请不要通过返回值计算
	输出数据的长度
	*/
	static std::string Decode(const char* Data, int DataByte, int& OutByte);
	//>->19.4.18读取图片数据生成base64数据
	static bool ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData);

	//>->19.4.18生成图片，可用作生成二维码
	static bool WritePhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData);
};

#endif//_BASE64_H_
