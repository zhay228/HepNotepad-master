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
