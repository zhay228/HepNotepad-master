// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <objbase.h>
#include "wininet.h"
#ifndef SQLITE3
#define SQLITE3
#include "Library\sqlite3.h"
#endif

 
#include <shellapi.h>
#include "Library\cJSON.h"
#include<iostream>
#include<atltime.h>
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

#include <fstream>
#include "Common\Log.h"
#include "Common\DataTypeTool.h"
#include "Common\Helper.h"

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

extern HINSTANCE g_hInst;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   endif
#endif

#include "Library/Scintilla.h"
#include "Library/SciLexer.h"


#include "Resource.h"


#define  DUI_CTR_CANVAS                          (_T("Canvas"))
#define  DUI_CTR_CANVASCONTAINER                 (_T("CanvasContainer"))

#define MENUCMD_ADDRECTANGE		200
#define MENUCMD_ADDELLIPSE		201
#define MENUCMD_ADDARROW		202
#define MENUCMD_ADDBRUSH		203
#define MENUCMD_ADDTEXT			204
#define MENUCMD_UNDO			205
#define MENUCMD_RECHOICE		206
#define MENUCMD_SAVEANDQUIT		207
#define MENUCMD_SAVE			208
#define MENUCMD_HIDETOOLBAR		209
#define MENUCMD_QUIT			210
#define WMAPP_NOTIFYCALLBACK	 (WM_USER + 300) //NOTIFYCALLBACK
#define WM_SHOWMAINHOTKEY (WM_USER + 301)
#define WM_ADDTIPHOTKEY (WM_USER + 303)
#define WM_QUERYHOTKEY (WM_USER + 306)
#define WM_OFTENTOOLHOTKEY (WM_USER + 308)
#define WM_TEMPINFOHOTKEY (WM_USER + 310)
#define WM_COPYSAVETIPHOTKEY (WM_USER + 312)
#define WM_GETCOPY (WM_USER + 314)
#define WM_STOPGETCOPY (WM_USER + 315)
#define WM_QUERYHOTRESULTKEY (WM_USER + 316)
#define WM_SAVETIPHOTTKEY (WM_USER + 318) 

#define WM_RIGHTHOTTKEY (WM_USER + 320)
#define WM_LEFTHOTTKEY (WM_USER + 322)

#define HN_MAINLOADTIP	 (WM_USER + 400) //
#define HN_MAINLOADINFO	 (WM_USER + 401) //
#define HN_MAINLOADPERSONDATA	 (WM_USER + 402) 
#define HN_MAINLOADCALENDARDATA	 (WM_USER + 404) 
#define HN_MAINLOADWEEKDATA	 (WM_USER + 405) 
#define HN_SETFOUCSQUERY	 (WM_USER + 407) 
#define HN_CLOSEQUERY	 (WM_USER + 409) 
#define HN_GETINFOQUERY	 (WM_USER + 412) 
// TODO: reference additional headers your program requires here

  
enum  DataType {
	none,
	tempTip,
	tip,
	info,
	personal,
	calendar,
	history,
	weekly,
	copy
};

enum GotoPage {
	First,
	Prev,
	Next,
	End,
	Goto
};

struct  SystemInfo
{
	DataType dataType;
	 
};
  
struct UserInfo{
	string pwdCol = "systemApp";
	string pwd = "";
	string keyCol = "systemKey";
	string key = "";
	string saveTempTipCol = "autoSaveTempTip";
	bool saveTempTip;
	string personDataEnableCol = "personDataEnable";
	bool personDataEnable;
	string WeekTimeCol = "WeekTime";
	bool WeekEnable;
	string WeekTime = "";
};
enum RemindType {
	Once,
	Weekly,
	Dayly
};

struct  DataInfo
{
	DataType dataType;
	string id;
	int isCtrl = 0;
	int isAlt = 0;
	int isShift = 0;
	int isEncryption = -1;
	string key;
	string title;
	string content;
	string userId="1";
	string createTime;
	string updateTime;
	string typeName = "";
	string userName = "admin";
	string wndTitle;
	string name = "";
	string value = "";
	string remark = "";
	int remindType; 
	int remindDateIndex = -1;
	string remindDate;
	string remindTime;  
	string remindDateTime;
	int isRemind = 0;
	int index = -1;
	string pId; //父级IP
	int status;
	string statusText;
	string monday;
	string tuesday;
	string wednesday;
	string thursday;
	string friday;
	string saturday;
	string sunday;
};

struct WeeklyData
{
	string id;
	string monday;
	string tuesday;
	string wednesday;
	string thursday;
	string friday;
	string saturday;
	string sunday;
	string summary;
};

struct PageInfo{ 
	string type = "";
	string key = "";
	string date;
	string content;
	string whereSql="";
	int startIndex = -1;
	int endIndex;
	int currPageIndex = 0;
	int pageSize = 50;
	int pageCount = 1; //实际查询数量
	int totalCount;
	int totalPageCount;//从0开始计算,首页是0	
	int maxId = 0;
	string currentId; //查询消息，打开前后消息，传递ID
	 
};

struct  SendMsg
{
	HWND	m_hwnd;
	CControlUI* pSender;
	DataType dataType;
	string key;
	string data;
};

BOOL AddNotificationIcon(HWND hwnd);
void ShowMessage(CDuiString info);
bool ShowMessageConfirm(HWND hwnd, CDuiString title, CDuiString info);
void ShowQueryResult(LPVOID param);
void InitalSystemInfo();