#pragma once

#include <string>
#include "direct.h"
#include <io.h>
  
class CFileHelper
{
public:
	CFileHelper(void);
	~CFileHelper(void);

	static tstring GetAppPath();	// ��ȡӦ�ó���ִ��·��
	static tstring GetCurDir();	// ��ȡӦ�ó���ǰĿ¼
	static tstring GetTempPath();	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
	static tstring GetTempFileName(LPCTSTR lpszFileName);	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// ��ȡ����ļ���(ȫ·��)

	static BOOL IsDirectory(LPCTSTR lpszPath);	// ���ָ��·���Ƿ�Ŀ¼
	static BOOL IsFileExist(LPCTSTR lpszFileName);	// ���ָ���ļ��Ƿ����
	static BOOL IsDirectoryExist(LPCTSTR lpszPath);	// ���ָ��Ŀ¼�Ƿ����
	static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	static void CreateDir(LPSTR fileName);
	static void CreateDir(string fileName);

	static tstring GetPathRoot(LPCTSTR lpszPath);	// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
	static tstring GetDirectoryName(LPCTSTR lpszPath);	// ����ָ��·���ַ�����Ŀ¼��Ϣ
	static tstring GetFileName(LPCTSTR lpszPath); // ����ָ��·���ַ������ļ�������չ��
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);	// ���ز�������չ����·���ַ������ļ���
	static tstring GetExtension(LPCTSTR lpszPath);	// ����ָ����·���ַ�������չ��
	static tstring GetFullPath(LPCTSTR lpszPath);	// ����ָ�������·����ȡ����·��
	 
	static int GetFileSize(char* filename);
	static string GetFileSizeText(int fileSize);

	static tstring GetFileTypeIco(LPCTSTR fileName);
	static HICON FileIcon(std::string extention);
	static HICON FolderIcon();
	static string FileType(std::string extention);
	static string FolderType();

	 
	static vector<char*> getRootPath();
	//currentIndex��ǰĿ¼�㼶��max���㼶
	static vector<string> GetFiles(string dir, vector<string> typeList, int max = 1, int currentIndex = 1);

	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
 
	static bool SaveHIcon2PngFile(HICON hIcon, LPCTSTR lpszPicFileName);

	static BOOL  SaveBitmap(const  BITMAP   &bm, HDC   hDC, HBITMAP   hBitmap, LPCTSTR   szFileName);

	static HRESULT SaveIcon(HICON hIcon, const char* path);

	static bool DeleteDirFile(CDuiString path);
 
};

