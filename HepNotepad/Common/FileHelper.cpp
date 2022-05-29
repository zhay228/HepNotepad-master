#include <algorithm> 
#include "../stdafx.h"
#include "Helper.h"
#include "FileHelper.h"
#include <shellapi.h>//SHGetFileInfo
#include <ShObjIdl.h>//IShellFolder::GetAttributesOf
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
CFileHelper::CFileHelper(void)
{
}

CFileHelper::~CFileHelper(void)
{
}

// 获取应用程序执行路径
tstring CFileHelper::GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = {0};

	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		tstring strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}

	return szPath;
}

// 获取应用程序当前目录
tstring CFileHelper::GetCurDir()
{
	TCHAR szCurDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurDir);

	if (szCurDir[_tcslen(szCurDir) - 1] != _T('\\'))
		_tcscat(szCurDir, _T("\\"));

	return szCurDir;
}

// 获取当前系统的临时文件夹的路径
tstring CFileHelper::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = {0};

	::GetTempPath(MAX_PATH, szTempPath);

	if (szTempPath[_tcslen(szTempPath) - 1] != _T('\\'))
		_tcscat(szTempPath, _T("\\"));

	return szTempPath;
}

// 获取当前系统的临时文件夹的路径下的唯一命名的临时文件名(全路径)
tstring CFileHelper::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
}

// 获取随机文件名(全路径)
tstring CFileHelper::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	tstring strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = {0};

	if (!IsDirectoryExist(lpszPath))
		strPath = GetCurDir();
	else
		strPath = lpszPath;

	strFileName = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (int i = 2; i < 10000; i++)
	{
		if (strExtFileName.empty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}
		
		if (!IsFileExist(strFullPath.c_str()))
			return strFullPath;
	}

	return _T("");
}

// 检测指定路径是否目录
BOOL CFileHelper::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

// 检测指定文件是否存在
BOOL CFileHelper::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	if(::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)
		return TRUE;
	else
		return FALSE;
}

// 检测指定目录是否存在
BOOL CFileHelper::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

BOOL CFileHelper::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR cPath[MAX_PATH] = {0};
	TCHAR cTmpPath[MAX_PATH] = {0};
	TCHAR * lpPos = NULL;
	TCHAR cTmp = _T('\0');

	if (NULL == lpPathName || NULL == *lpPathName)
		return FALSE;

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (int i = 0; i < (int)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
			cPath[i] = _T('/');
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (lpPos != NULL)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp = *lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}

	return TRUE;
}

void CFileHelper::CreateDir(string fileName) {
	char path[MAX_PATH];
	sprintf(path, "%s", fileName.c_str());
	CreateDir(path);
}

void CFileHelper::CreateDir(LPSTR fileName)
{
	char *tag;
	for (tag = fileName; *tag; tag++)
	{
		if (*tag == '\\')
		{
			char buf[9999], path[9999];
			strcpy(buf, fileName);
			buf[strlen(fileName) - strlen(tag) + 1] = NULL;
			strcpy(path, buf);
			if (access(path, 6) == -1)
			{
				mkdir(path);
			}
		}
	}
}


// 获取指定路径的根目录信息
tstring CFileHelper::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.find(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// 返回指定路径字符串的目录信息
tstring CFileHelper::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// 返回指定路径字符串的文件名和扩展名
tstring CFileHelper::GetFileName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// 返回不具有扩展名的路径字符串的文件名
tstring CFileHelper::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(nPos+1);

	nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos);

	return strPath;
}

// 返回指定的路径字符串的扩展名
tstring CFileHelper::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::size_type nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// 根据指定的相对路径获取绝对路径
tstring CFileHelper::GetFullPath(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	TCHAR cFirstChar = strPath.at(0);
	TCHAR cSecondChar = strPath.at(1);

	if (cFirstChar == _T('/'))
	{
		tstring strCurDir = GetAppPath();
		tstring strRootPath = GetPathRoot(strCurDir.c_str());
		return strRootPath + strPath;
	}
	else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
	{
		return strPath;
	}
	else
	{
		tstring strCurDir = GetAppPath();
		return strCurDir + strPath;
	}
}


int CFileHelper::GetFileSize(char* filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp) return -1;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);

	return size;
}

string CFileHelper::GetFileSizeText(int fileSize){
 
	string sizeType="字节";
	double size = 0;
	const double s = 1024;
	string result = "";
	if (fileSize / s < 1024){ //KB
		size = fileSize / s;
		sizeType = "KB";
	}
	else if (fileSize / s / s < 1024){//MB
		size = fileSize / s / s;
		sizeType = "MB";
	}
	else if (fileSize / s / s / s < 1024){//GB
		size = fileSize / s / s / s;
		sizeType = "GB";
	} 
	char fileInfo[100];
	sprintf(fileInfo, "%.2f%s", size, sizeType.c_str());
	result = fileInfo;
	return result;
}

tstring CFileHelper::GetFileTypeIco(LPCTSTR fileName){
	tstring fileStr = "{\".xls\":\"xlsm\",\".xlsx\":\"xlsm\",\".csv\":\"xlsm\",\".xlsm\":\"xlsm\",\".xlt\":\"xlsm\",\".xltx\":\"xlsm\",\".xltm\":\"xlsm\",\".doc\":\"docx\",\".docx\":\"docx\",\".dot\":\"docx\",\".dotm\":\"docx\",\".dotx\":\"docx\",\".docm\":\"docx\",\".ppt\":\"ppt\",\".pptx\":\"ppt\",\".pot\":\"ppt\",\".potm\":\"ppt\",\".potx\":\"ppt\",\".pps\":\"ppt\",\".ppsx\":\"ppt\",\".pptm\":\"ppt\",\".7z\":\"7z\",\".rar\":\"rar\",\".cab\":\"CAB\",\".jar\":\"rar\",\".arj\":\"rar\",\".tar\":\"rar\",\".gz\":\"rar\",\".tgz\":\"rar\",\".taz\":\"rar\",\".cpgz\":\"rar\",\".zip\":\"zip\",\".war\":\"zip\",\".ear\":\"zip\",\".z\":\"zip\",\".bz\":\"zip\",\".bz2\":\"zip\",\".tbz\":\"zip\",\".tbz2\":\"zip\",\".cpio\":\"zip\",\".ar\":\"zip\",\".crx\":\"rar\"," \
		"\".img\":\"img\",\".nrg\":\"img\",\".eml\":\"eml\",\".htm\":\"html\",\".html\":\"html\",\".shtml\":\"html\",\".xhtml\":\"html\",\".mht\":\"html\",\".js\":\"JS\",\".css\":\"CSS\",\".exe\":\"exe\",\".app\":\"exe\",\".cmd\":\"cmd\",\".ttf\":\"ttf\",\".pdf\":\"PDF\",\".psd\":\"PS\",\".pdd\":\"PS\",\".ps\":\"PS\",\".ai\":\"AI\",\".fla\":\"fla\",\".swf\":\"swf\",\".flv\":\"fla\",\".txt\":\"TXT\",\".java\":\"Java\",\".jsp\":\"html\",\".rtf\":\"RTF\",\".reg\":\"reg\",\".ram\":\"ram\",\".rpm\":\"ram\",\".rmx\":\"ram\",\".rm\":\"ram\",\".rmvb\":\"ram\",\".wm\":\"wma\",\".wma\":\"wma\",\".wmv\":\"wma\",\".asf\":\"wma\",\".wmp\":\"wma\",\".mov\":\"mov\",\".qt\":\"mov\",\".3gp\":\"mov\",\".3gpp\":\"mov\",\".amr\":\"mov\",\".avi\":\"mp4\",\".mkv\":\"mp4\"," \
		"\".mpg\":\"mp4\",\".mpeg\":\"mp4\",\".vob\":\"mp4\",\".dat\":\"mp4\",\".ts\":\"mp4\",\".tp\":\"mp4\",\".m2ts\":\"mp4\",\".evo\":\"mp4\",\".pmp\":\"mp4\",\".vp6\":\"mp4\",\".ivf\":\"mp4\",\".dsm\":\"mp4\",\".dsv\":\"mp4\",\".dsa\":\"mp4\",\".dss\":\"mp4\",\".fli\":\"mp4\",\".flc\":\"mp4\",\".flic\":\"mp4\",\".roq\":\"mp4\",\".mpa\":\"mp3\",\".m1a\":\"mp3\",\".m2a\":\"mp3\",\".mp2\":\"mp3\",\".mp3\":\"mp3\",\".ac3\":\"mp3\",\".dts\":\"mp3\",\".ddp\":\"mp3\",\".flac\":\"mp3\",\".ape\":\"mp3\",\".mac\":\"mp3\",\".apl\":\"mp3\",\".shn\":\"mp3\",\".tta\":\"mp3\",\".wv\":\"mp3\",\".cda\":\"mp3\",\".wav\":\"mp3\",\".aac\":\"mp3\",\".m4a\":\"mp3\",\".mka\":\"mp3\",\".ogg\":\"mp3\",\".mpc\":\"mp3\",\".au\":\"mp3\",\".aif\":\"mp3\"," \
		"\".aifc\":\"mp3\",\".aiff\":\"mp3\",\".mid\":\"mp3\",\".midi\":\"mp3\",\".rmi\":\"mp3\",\".todo\":\"todo\",\".jpg\":\"JPG\",\".jpeg\":\"JPG\",\".gif\":\"GIF\",\".png\":\"PNG\",\".bmp\":\"BMP\",\".dib\":\"BMP\",\".rle\":\"TIF\",\".tif\":\"TIF\",\".tiff\":\"TIF\",\".xif\":\"TIF\",\".emf\":\"EMF\",\".wmf\":\"WMF\",\".ico\":\"ico\",\".raw\":\"RAW\",\".mos\":\"RAW\",\".fff\":\"RAW\",\".bay\":\"RAW\",\".cdr\":\"CDR\",\".torrent\":\"torrent\",\".wps\":\"wps\",\".wpt\":\"wps\",\".dps\":\"dps\",\".et\":\"et\",\".msi\":\"exe\",\".iso\":\"iso\",\".ini\":\"edit\",\".inf\":\"edit\",\".xml\":\"edit\",\".hlp\":\"edit\",\".chm\":\"edit\",\".asp\":\"edit\",\".aspx\":\"edit\",\".c\":\"edit\",\".cpp\":\"edit\",\".h\":\"edit\",\".hpp\":\"edit\",\".py\":\"edit\",\".cs\":\"edit\",\".sh\":\"edit\",\".ocx\":\"dll\",\".chk\":\"edit\",\".manifest\":\"edit\",\".com\":\"dll\",\".bat\":\"cmd\",\".dll\":\"dll\"," \
		"\".mdf\":\"data\",\".ldf\":\"data\",\".log\":\"edit\"}";
	tstring fileType = GetExtension(fileName);
	char fileInfo[100];
	sprintf(fileInfo, ".%s", fileType.c_str());
	cJSON * icoJson = NULL;//cjson对象
	icoJson = cJSON_Parse(fileStr.c_str());
	tstring fileIco = getParam(icoJson, fileStr.c_str(), fileInfo);
	if (fileIco.size() < 1) fileIco = "help";
	fileIco += ".png";
	return fileIco;
}

// 获取文件图标
HICON CFileHelper::FileIcon(tstring extention)
{
	HICON icon = NULL;
	if (extention.length() > 0)
	{
		LPCSTR name = extention.c_str();

		SHFILEINFOA info;
		if (SHGetFileInfoA(name,
			FILE_ATTRIBUTE_NORMAL,
			&info,
			sizeof(info),
			SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
		{
			icon = info.hIcon;
		}
	}

	return icon;
}

// 获取文件类型
std::string CFileHelper::FileType(tstring extention)
{
	std::string type = "";
	if (extention.length() > 0)
	{
		LPCSTR name = extention.c_str();

		SHFILEINFOA info;
		if (SHGetFileInfoA(name,
			FILE_ATTRIBUTE_NORMAL,
			&info,
			sizeof(info),
			SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
		{
			type = info.szTypeName;
		}
	}

	return type;
}

// 获取文件夹图标
HICON CFileHelper::FolderIcon()
{
	std::string str = "folder";
	LPCSTR name = str.c_str();

	HICON icon = NULL;

	SHFILEINFOA info;
	if (SHGetFileInfoA(name,
		FILE_ATTRIBUTE_DIRECTORY,
		&info,
		sizeof(info),
		SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
	{
		icon = info.hIcon;
	}

	return icon;
}

// 获取文件夹类型
string CFileHelper::FolderType()
{
	std::string str = "folder";
	LPCSTR name = str.c_str();

	std::string type;

	SHFILEINFOA info;
	if (SHGetFileInfoA(name,
		FILE_ATTRIBUTE_DIRECTORY,
		&info,
		sizeof(info),
		SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
	{
		type = info.szTypeName;
	}

	return type;
}
 

vector<char*> CFileHelper::getRootPath(){
	CHAR szLogicDriveStrings[1024];
	PCHAR szDrive;

	ZeroMemory(szLogicDriveStrings, 1024);

	GetLogicalDriveStrings(1023, szLogicDriveStrings);
	szDrive = (PCHAR)szLogicDriveStrings;
	vector<char*> list;
	do
	{
		list.push_back(szDrive);
		szDrive += (lstrlen(szDrive) + 1);
	} while (*szDrive != '\x00');
	return list;
}

vector<string> CFileHelper::GetFiles(string dir, vector<string> typeList, int max, int grade){
	 
	if (dir.substr(dir.size()-2, 1) != "\\");
		dir += _T('\\');
	WIN32_FIND_DATA wfd;
	char data[MAX_PATH];
	sprintf(data,"%s*.*",dir);
	HANDLE hFind = FindFirstFile(data, &wfd);
	vector <string> fileList;
	if (hFind == INVALID_HANDLE_VALUE){
		return fileList;
	}
	do{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if (stricmp(_T(wfd.cFileName), ".") != 0 &&
				stricmp(_T(wfd.cFileName), "..") != 0)
			{
				/*if (grade < max){
					vector<string> tempList = GetFiles(dir + _T(wfd.cFileName), type);
					for (int i = 0; i < tempList.size(); i++){
						fileList.push_back(tempList[i]);
					}
					grade++;
				}*/
			}
		}
		else{
			string strPath = wfd.cFileName;
			int nPos = strPath.rfind(".");
			string	suffixStr = strPath.substr(nPos);
			transform(suffixStr.begin(), suffixStr.end(), suffixStr.begin(), ::tolower);
			 
			if (typeList.size()>0 ){
				for (int j = 0; j < typeList.size(); j++){
					if (typeList[j] == suffixStr)
						fileList.push_back(strPath);
				} 
			}
			else{
				fileList.push_back(strPath);
			}
			
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	return fileList;
}
 

int CFileHelper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0, size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix)
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return found; 
}


bool CFileHelper::SaveHIcon2PngFile(HICON hIcon, LPCTSTR  lpszPicFileName)
{
	if (hIcon == NULL)
	{
		return false;
	}

	ICONINFO icInfo = { 0 };
	if (!::GetIconInfo(hIcon, &icInfo))
	{
		return false;
	}

	BITMAP bitmap;
	GetObject(icInfo.hbmColor, sizeof(BITMAP), &bitmap);

	Gdiplus::Bitmap* pBitmap = NULL;
	Gdiplus::Bitmap* pWrapBitmap = NULL;
	if (bitmap.bmBitsPixel != 32)
	{
		pBitmap = Gdiplus::Bitmap::FromHICON(hIcon);
	}
	else
	{
		pWrapBitmap = Gdiplus::Bitmap::FromHBITMAP(icInfo.hbmColor, NULL);
		if (!pWrapBitmap)
			return false;

		Gdiplus::BitmapData bitmapData;
		Gdiplus::Rect rcImage(0, 0, pWrapBitmap->GetWidth(), pWrapBitmap->GetHeight());
		pWrapBitmap->LockBits(&rcImage, Gdiplus::ImageLockModeRead, pWrapBitmap->GetPixelFormat(), &bitmapData);
		pBitmap = new Gdiplus::Bitmap(bitmapData.Width, bitmapData.Height, bitmapData.Stride, PixelFormat32bppARGB, (BYTE*)bitmapData.Scan0);
		pWrapBitmap->UnlockBits(&bitmapData);
	}

	CLSID encoderCLSID;
	GetEncoderClsid(L"image/png", &encoderCLSID);
	Gdiplus::Status st = pBitmap->Save(CDataTypeTool::StringToWString(lpszPicFileName).c_str(), &encoderCLSID, NULL/*&encoderParameters*/);
	if (st != Gdiplus::Ok)
		return false;

	delete pBitmap;
	if (pWrapBitmap)
		delete pWrapBitmap;
	DeleteObject(icInfo.hbmColor);
	DeleteObject(icInfo.hbmMask);

	return true;
}


//保存图片到本地
BOOL  CFileHelper::SaveBitmap(const  BITMAP   &bm, HDC   hDC, HBITMAP   hBitmap, LPCTSTR   szFileName)
{
	int   nW = bm.bmWidth, nH = bm.bmHeight;
	int   nBitPerPixel = bm.bmBitsPixel;//获得颜色模式 
	int   nPalItemC = bm.bmPlanes; //调色板项的个数 
	int   nBmpInfSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nPalItemC;//位图信息的大小 
	int   nDataSize = (nBitPerPixel*nW + 31) / 32 * 4 * nH;//位图数据的大小 
	//初始化位图信息 
	BITMAPFILEHEADER   bfh = { 0 };//位图文件头 
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + nBmpInfSize;//sizeof(BITMAPINFOHEADER);//到位图数据的偏移量 
	bfh.bfSize = bfh.bfOffBits + nDataSize;//文件总的大小 
	bfh.bfType = (WORD)0x4d42;//位图标志 
	char   *   p = new   char[nBmpInfSize + nDataSize];//申请内存位图数据空间(包括信息头) 
	memset(p, 0, nBmpInfSize);//将信息头的数据初始化为0 
	LPBITMAPINFOHEADER   pBih = (LPBITMAPINFOHEADER)p;//位图信息头 
	pBih->biCompression = BI_RGB;
	pBih->biBitCount = nBitPerPixel;//每个图元像素使用的位数 
	pBih->biHeight = nH;//高度 
	pBih->biWidth = nW;//宽度 
	pBih->biPlanes = 1;
	pBih->biSize = sizeof(BITMAPINFOHEADER);
	pBih->biSizeImage = nDataSize;//图像数据大小 
	char   *pData = p + nBmpInfSize;
	//DDB转换为DIB 
	::GetDIBits(hDC, hBitmap, 0, nH, pData, (LPBITMAPINFO)pBih, DIB_RGB_COLORS);//获取位图数据 
	std::ofstream   ofs(szFileName, ios::binary);

	if (ofs.fail()) return FALSE;

	ofs.write((const   char*)&bfh, sizeof(BITMAPFILEHEADER));//写入位图文件头 
	ofs.write((const   char*)pBih, nBmpInfSize);//写入位图信息数据 
	ofs.write((const   char*)pData, nDataSize);//写入位图数据 
	return   TRUE;
}



HRESULT CFileHelper::SaveIcon(HICON hIcon, const char* path) {
	// Create the IPicture intrface
	PICTDESC desc = { sizeof(PICTDESC) };
	desc.picType = PICTYPE_ICON;
	desc.icon.hicon = hIcon;
	IPicture* pPicture = 0;
	HRESULT hr = OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (void**)&pPicture);
	if (FAILED(hr)) return hr;

	// Create a stream and save the image
	IStream* pStream = 0;
	CreateStreamOnHGlobal(0, TRUE, &pStream);
	LONG cbSize = 0;
	hr = pPicture->SaveAsFile(pStream, TRUE, &cbSize);

	// Write the stream content to the file
	if (!FAILED(hr)) {
		HGLOBAL hBuf = 0;
		GetHGlobalFromStream(pStream, &hBuf);
		void* buffer = GlobalLock(hBuf);
		HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if (!hFile) hr = HRESULT_FROM_WIN32(GetLastError());
		else {
			DWORD written = 0;
			WriteFile(hFile, buffer, cbSize, &written, 0);
			CloseHandle(hFile);
		}
		GlobalUnlock(buffer);
	}
	// Cleanup
	pStream->Release();
	pPicture->Release();
	return hr;

}


bool CFileHelper::DeleteDirFile(CDuiString path)
{
	CDuiString strDir = path;
	if (strDir.GetAt(strDir.GetLength() - 1) != '\\');
	strDir += _T('\\');
	
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(_T(strDir + "*.*"), &wfd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	do {
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (stricmp(_T(wfd.cFileName), ".") != 0 &&
				stricmp(_T(wfd.cFileName), "..") != 0)
				DeleteDirFile((strDir + _T(wfd.cFileName)));
		}
		else {
			DeleteFile((_T(strDir) + wfd.cFileName));
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	RemoveDirectory(_T(path));

	return true;
}