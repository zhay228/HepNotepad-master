#pragma once

#include <string>
#include "direct.h"
#include <io.h>
  
class CFileHelper
{
public:
	CFileHelper(void);
	~CFileHelper(void);

	static tstring GetAppPath();	// 获取应用程序执行路径
	static tstring GetCurDir();	// 获取应用程序当前目录
	static tstring GetTempPath();	// 获取当前系统的临时文件夹的路径
	static tstring GetTempFileName(LPCTSTR lpszFileName);	// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// 获取随机文件名(全路径)

	static BOOL IsDirectory(LPCTSTR lpszPath);	// 检测指定路径是否目录
	static BOOL IsFileExist(LPCTSTR lpszFileName);	// 检测指定文件是否存在
	static BOOL IsDirectoryExist(LPCTSTR lpszPath);	// 检测指定目录是否存在
	static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	static void CreateDir(LPSTR fileName);
	static void CreateDir(string fileName);

	static tstring GetPathRoot(LPCTSTR lpszPath);	// 获取指定路径的根目录信息
	static tstring GetDirectoryName(LPCTSTR lpszPath);	// 返回指定路径字符串的目录信息
	static tstring GetFileName(LPCTSTR lpszPath); // 返回指定路径字符串的文件名和扩展名
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);	// 返回不具有扩展名的路径字符串的文件名
	static tstring GetExtension(LPCTSTR lpszPath);	// 返回指定的路径字符串的扩展名
	static tstring GetFullPath(LPCTSTR lpszPath);	// 根据指定的相对路径获取绝对路径
	 
	static int GetFileSize(char* filename);
	static string GetFileSizeText(int fileSize);

	static tstring GetFileTypeIco(LPCTSTR fileName);
	static HICON FileIcon(std::string extention);
	static HICON FolderIcon();
	static string FileType(std::string extention);
	static string FolderType();

	 
	static vector<char*> getRootPath();
	//currentIndex当前目录层级，max最大层级
	static vector<string> GetFiles(string dir, vector<string> typeList, int max = 1, int currentIndex = 1);

	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
 
	static bool SaveHIcon2PngFile(HICON hIcon, LPCTSTR lpszPicFileName);

	static BOOL  SaveBitmap(const  BITMAP   &bm, HDC   hDC, HBITMAP   hBitmap, LPCTSTR   szFileName);

	static HRESULT SaveIcon(HICON hIcon, const char* path);

	static bool DeleteDirFile(CDuiString path);
 
};

