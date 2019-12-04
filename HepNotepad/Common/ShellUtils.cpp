// I think this code is based on the works by PJ Naughter and Igor Vigdorchik
// with some modifications by me (Abu Mami)

#include "../stdafx.h"
#include <shlobj.h>
#include "ShellUtils.h"
#include <objbase.h>
#include <shlguid.h>
#include <io.h>
#include <Shlwapi.h>


CDuiString GetSpecialFolderLocation(int nFolder)
{
	CDuiString Result;
	
	LPITEMIDLIST pidl;
	HRESULT hr = SHGetSpecialFolderLocation(NULL, nFolder, &pidl);
	if (SUCCEEDED(hr))
	{
		char szPath[_MAX_PATH];
		if (SHGetPathFromIDList(pidl, szPath))
			Result = szPath;
	}	
	return Result;
}
void CreateShortcut(const CDuiString& ExePath, const CDuiString& LinkFilename, const CDuiString& WorkingDirectory, const CDuiString& Description, int nFolder)
{
    IShellLink* psl; 
	const CDuiString PathLink = GetSpecialFolderLocation(nFolder) + _T("\\") + LinkFilename + _T(".lnk");

	// Assume all folders except folders based on CSIDL_PROGRAMS. This is because the installer
	// creates a separate folder for the "project" in which links for all executables will be
	// created. Since the project folder is not a system folder, it might have to be created.
	if(nFolder == CSIDL_PROGRAMS)
	{
		CDuiString Path = GetSpecialFolderLocation(nFolder) + _T("\\") + LinkFilename;
		if(_access(Path, 2) != 0)
		{
			if(!MakeSurePathExists(Path))
			{
				CDuiString err;
				err.Format(_T("Could not create Start menu directory %s."), Path);
				//AfxMessageBox(err);
				return;
			}
		}
	}
	
    // Get a pointer to the IShellLink interface. 
    HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl); 
    if (SUCCEEDED(hres)) 
	{ 
        IPersistFile* ppf; 
		
        // Set the path to the shortcut target and add the description. 
        psl->SetPath(ExePath); 
		psl->SetWorkingDirectory(WorkingDirectory);
        psl->SetDescription(Description); 
		
		// Query IShellLink for the IPersistFile interface for saving the 
		// shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf); 
        if (SUCCEEDED(hres)) 
		{
			WCHAR wszTemp[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, PathLink, -1, wszTemp, MAX_PATH);
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(wszTemp, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 
}
 
void Register(CDuiString dll)
{
	MessageBox(NULL, dll, _T(""), MB_OK);
	BOOL rc = FALSE;
	HMODULE hm = LoadLibraryEx(dll, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	//HMODULE hm = LoadLibrary(dll);
	if (hm)
	{
		FARPROC lpDllEntryPoint; 
		lpDllEntryPoint = GetProcAddress(hm, TEXT("DllRegisterServer")); // call function of register DllRegisterServer 
		if (lpDllEntryPoint != NULL) { 
			if (FAILED((*lpDllEntryPoint)())==false)  { 
				FreeLibrary(hm);   	
				return;
			}
			FreeLibrary(hm);
		}
	//	MessageBox(NULL, dll, _T("2"), MB_OK);
	}
	CDuiString str;
	str.Format("Regsvr32.exe /s \"%s\"",dll);
	WinExec(str, SW_HIDE);
	//MessageBox(NULL, dll, _T("3"), MB_OK);
}
 
bool DeleteDirectory(LPTSTR DirName, bool delRoot)
{
	HANDLE hFirstFile = NULL;
	WIN32_FIND_DATA FindData;

	TCHAR currdir[MAX_PATH] = { 0 };
	sprintf(currdir, _T("%s\\*.*"), DirName);

	hFirstFile = ::FindFirstFile(currdir, &FindData);
	if (hFirstFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bRes = true;

	while (bRes)
	{
		bRes = ::FindNextFile(hFirstFile, &FindData);

		if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //发现目录
		{
			if (!strcmp(FindData.cFileName, _T(".")) || !strcmp(FindData.cFileName, _T(".."))) //.或..
				continue;
			else
			{
				char tmppath[MAX_PATH] = { 0 };
				sprintf(tmppath, _T("%s\\%s"), DirName, FindData.cFileName);

				DeleteDirectory(tmppath,true);
			}
		}
		else               //发现文件
		{
			TCHAR tmppath[MAX_PATH] = { 0 };
			sprintf(tmppath, _T("%s\\%s"), DirName, FindData.cFileName);
			::DeleteFile(tmppath);
		}
	}
	::FindClose(hFirstFile);
	if (delRoot == true){
		if (!RemoveDirectory(DirName))
		{
			return false;
		}
	}
	
	return true;
}


//删除快捷方式的数据文件 (*.lnk) 
BOOL DeleteLink(CDuiString lpszShortcut)
{
	SHFILEOPSTRUCT fos;

	ZeroMemory(&fos, sizeof(fos));
	fos.hwnd = HWND_DESKTOP;
	fos.wFunc = FO_DELETE;
	fos.pFrom = lpszShortcut;
	fos.pTo = NULL;
	fos.fFlags = FOF_SILENT | FOF_ALLOWUNDO;
	//删除快捷方式（*.lnk) 
	if (0 != SHFileOperation(&fos))
		return FALSE;
	return TRUE;
}


BOOL MakeSurePathExists(CDuiString &Path, BOOL FilenameIncluded)
{
	int Pos = 0;
	while ((Pos = Path.Find('\\', Pos + 1)) != -1)
		CreateDirectory(Path.Left(Pos), NULL);
	if (!FilenameIncluded)
		CreateDirectory(Path, NULL);
	return ((!FilenameIncluded) ? !_access(Path, 0) :
		!_access(Path.Left(Path.ReverseFind('\\')), 0));
}

// 程序开机自动启动
void AutoStart(char * strExeFullDir, CDuiString szItem)
{
	HKEY hKey;
	char * strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

	//1、找到系统的启动项  
	if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
	{
		//2、得到本程序自身的全路径
		/*TCHAR strExeFullDir[MAX_PATH];
		GetModuleFileName(NULL, strExeFullDir, MAX_PATH);*/

		//3、判断注册表项是否已经存在
		TCHAR strDir[MAX_PATH] = {};
		DWORD nLength = MAX_PATH;
		long result = RegGetValue(hKey, nullptr, "GISRestart", RRF_RT_REG_SZ, 0, strDir, &nLength);

		//4、已经存在
		if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
		{
			//5、添加一个子Key,并设置值，"GISRestart"是应用程序名字（不加后缀.exe） 
			RegSetValueEx(hKey, szItem, 0, REG_SZ, (LPBYTE)strExeFullDir, (lstrlen(strExeFullDir) + 1)*sizeof(TCHAR));

			//6、关闭注册表
			RegCloseKey(hKey);
		}
	}
	else
	{

	}
}

bool DeleteRegKey(HKEY hKey, CDuiString szSubKey, CDuiString szItem){

	if (ERROR_SUCCESS == RegDeleteKey(hKey, szSubKey)){
		return TRUE;
	}
	HKEY hSubKey;
	if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_WRITE | KEY_READ, &hSubKey) != ERROR_SUCCESS){
		return false;
	}

	if (ERROR_SUCCESS != RegDeleteValue(hSubKey, szItem)){
		return false;
	}

	RegCloseKey(hSubKey);
	//RegDeleteKey(hKey, szSubKey);

	return true;
}

