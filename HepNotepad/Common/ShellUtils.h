#ifndef __SHELLUTILS_H__
#define __SHELLUTILS_H__
CDuiString GetSpecialFolderLocation(int nFolder);
void CreateShortcut(const CDuiString& ExePath, const CDuiString& LinkFilename,const CDuiString& WorkingDirectory, const CDuiString& Description,int nFolder);
void Register(CDuiString dll); 
bool DeleteDirectory(LPTSTR DirName,bool delRoot=false);
BOOL DeleteLink(CDuiString lpszShortcut);
BOOL MakeSurePathExists(CDuiString &Path, BOOL FilenameIncluded = TRUE);
void AutoStart(char * strExeFullDir, CDuiString szItem);//开机自动启动
bool DeleteRegKey(HKEY hKey, CDuiString szSubKey, CDuiString szItem);//删除注册表
#endif
