#pragma once
#ifndef _BASE64_H_
#define _BASE64_H_

#include "Base64.h"
#include <tchar.h>
#include <windows.h>
#include <iostream> 
#include <string>
using namespace std;
class CBase64
{
public:
	CBase64();
	~CBase64();

	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	*/
	static std::string Encode(const char* Data, int DataByte);

	/*����
	DataByte
	[in]��������ݳ���,���ֽ�Ϊ��λ
	OutByte
	[out]��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ����
	������ݵĳ���
	*/
	static std::string Decode(const char* Data, int DataByte, int& OutByte);
	//>->19.4.18��ȡͼƬ��������base64����
	static bool ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData);

	//>->19.4.18����ͼƬ�����������ɶ�ά��
	static bool WritePhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData);
};

#endif//_BASE64_H_
