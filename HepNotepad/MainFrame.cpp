#include "stdafx.h"
#include "MainFrame.h"
#include <comutil.h>
#include <commdlg.h>
#include <mshtml.h>
#include <wininet.h>
#include <sys/stat.h> 
#include "Common\des.h"
#include "Common\FileHelper.h"
#include "Common\ShellUtils.h"
#include "SqliteServer.h"
#include "DAL\Common.h"
#include "DAL\DataType.h"
#include "DAL\DataInfo.h"
#include "DAL\TipInfo.h"
#include "DAL\PersonInfo.h"
#include "DAL\SystemInfo.h"
#include "DAL\Calendar.h"
#include "DAL\Weekly.h"
#include "Form\FrmInfoAdd.h"

#include "Form\FrmPersonInfoAdd.h" 
#include "Form\FrmPersonInfoPwdSet.h"
#include "Form\FrmInfoQuery.h"
#include "Form\FrmCreateDir.h"
#include "Form\FrmCalendarAdd.h"
#include "FileDialogEx.h"
#include "Form\FrmTextEditor.h"



extern UserInfo* userInfo;
extern HWND m_MainHwnd;


DUI_BEGIN_MESSAGE_MAP(CMainFrame, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectedChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_END_MESSAGE_MAP()

std::vector<std::string> titleList;
std::vector<std::string> contentList;

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
	UnregisterHotKey(GetHWND(), WM_SHOWMAINHOTKEY);
	UnregisterHotKey(GetHWND(), WM_ADDTIPHOTKEY);
	UnregisterHotKey(GetHWND(), WM_QUERYHOTKEY);
	UnregisterHotKey(GetHWND(), WM_OFTENTOOLHOTKEY);
	UnregisterHotKey(GetHWND(), WM_TEMPINFOHOTKEY);
	UnregisterHotKey(GetHWND(), WM_COPYSAVETIPHOTKEY);
	UnregisterHotKey(GetHWND(), WM_GETCOPY);
	delete wndCopy;
	delete m_pTempTip;
	delete m_pLogo;
}

CDuiString CMainFrame::GetSkinFile()
{
	return _T("main.xml");
}

LPCTSTR CMainFrame::GetWindowClassName(void) const
{
	return _T("HepNotepad.main");
}

string GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	string strPath(lpszPath);

	string::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}
	string::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != string::npos)
		strPath = strPath.substr(0, nPos + 1);
	return strPath;
}

string GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = { 0 };
	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		string strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}
	return szPath;
}

void CMainFrame::InitWindow()
{
	m_pNavMain = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("navMain")));
	m_pMainContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabMainContainer")));
	m_pOptMainNavInfo = static_cast<COptionUI*>(m_pm.FindControl(_T("navOptInfo")));
	m_pOptMainNavTool = static_cast<COptionUI*>(m_pm.FindControl(_T("navOptTool")));
	m_pOptMainNavCalendar = static_cast<COptionUI*>(m_pm.FindControl(_T("navOptCalendar")));
	m_pOptMainNavSet = static_cast<COptionUI*>(m_pm.FindControl(_T("navOptSet")));

	m_pNavData = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("navData")));
	m_pDataContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabDataContainer")));
	m_pOptNavTip = static_cast<COptionUI*>(m_pm.FindControl(_T("optTip")));
	m_pOptNavInfo = static_cast<COptionUI*>(m_pm.FindControl(_T("optInfo")));
	m_pOptNavPersonal = static_cast<COptionUI*>(m_pm.FindControl(_T("optPerson")));

	m_pNavTool = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("navTool")));
	m_pToolContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabToolContainer")));

	m_pNavCalendar = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("navCalendar")));
	m_pCalendarContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabCalendarContainer")));
	m_pOptNavCalendar = static_cast<COptionUI*>(m_pm.FindControl(_T("optCalendar")));
	m_pOptNavHistory = static_cast<COptionUI*>(m_pm.FindControl(_T("optHistory")));
	m_pOptNavWeekly = static_cast<COptionUI*>(m_pm.FindControl(_T("optWeekly")));

	m_pNavSystem = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("navSystem")));
	m_pSystemContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabSystemContainer")));

	m_pTipKey = static_cast<CEditUI*>(m_pm.FindControl(_T("txtTipKey")));
	m_pTipList = static_cast<CListUI*>(m_pm.FindControl(_T("listTip")));

	m_pInfoKey = static_cast<CEditUI*>(m_pm.FindControl(_T("txtInfoKey")));
	m_pInfoType = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("comInfoType")));
	m_pDataList = static_cast<CListUI*>(m_pm.FindControl(_T("listData")));

	m_pLayoutPerson = static_cast<CVerticalLayoutUI*>(m_pm.FindControl("layoutPerson"));
	m_pLayoutEncryt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl("layoutEncryt"));
	m_pPersonInfoKey = static_cast<CEditUI*>(m_pm.FindControl("txtPersonInfoKey"));
	m_pPersonInfoType = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("comPersonInfoType")));
	m_pPersonInfoList = static_cast<CListUI*>(m_pm.FindControl(_T("listPersonInfo")));
	m_pTxtPwd = static_cast<CEditUI*>(m_pm.FindControl(_T("txtPwd")));

	m_pRecentList = static_cast<CListUI*>(m_pm.FindControl("recentList"));
	m_pDevelopToolList = static_cast<CListUI*>(m_pm.FindControl("developList"));

	m_pCalendarList = static_cast<CListUI*>(m_pm.FindControl("listCalendar"));
	m_pHistoryList = static_cast<CListUI*>(m_pm.FindControl("listCalendarHistory"));
	m_pWeeklyList = static_cast<CListUI*>(m_pm.FindControl("listWeeklyData"));

	m_pCalendarType = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbxCalendarType")));
	m_pCalendarKey = static_cast<CEditUI*>(m_pm.FindControl(_T("txtCalendarKey")));

	m_pHistoryType = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbxHistoryType")));
	m_pHistoryKey = static_cast<CEditUI*>(m_pm.FindControl(_T("txtHistoryKey")));

	m_pChkAutoStart = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("chkAutoStart")));
	m_pChkCreateLink = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("chkCreateDeskLink")));
	m_pChkAutoSaveTempTip = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("chkAutoSaveTempTip")));
	m_pChkPersonEnable = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("chkPersonEnable")));
	m_pChkWeeklyEnable = static_cast<CCheckBoxUI*>(m_pm.FindControl(_T("chkWeeklyEnable")));

	m_pCbxMin = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbxWeekMin")));
	m_pCbxHour = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbxWeekHour")));

	m_pLayoutPage = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("pageLayout")));
	m_pPageInfo = static_cast<CTextUI*>(m_pm.FindControl(_T("txtPageInfo")));
	m_pGotoPage = static_cast<CEditUI*>(m_pm.FindControl(_T("txtGotoPage")));
	m_pBtnFirst = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageFirst")));
	m_pBtnPrev = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPagePrev")));
	m_pBtnNext = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageNext")));
	m_pBtnEnd = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageEnd")));

	m_pHotShowMain = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotShowMain")));
	m_pHotAddTip = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotAddTip")));
	m_pHotInfoQuery = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotInfoQuery")));
	m_pHotProgramQuery = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotProgramQuery")));
	m_pHotTempTip = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotTempTip")));
	m_pHotCopySaveTip = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtHotCopySaveTip")));
	m_pGetCopy = static_cast<CHotKeyUI*>(m_pm.FindControl(_T("txtGetCopy")));
	m_pVersion = static_cast<CTextUI*>(m_pm.FindControl(_T("txtVersion")));

	/*RegisterHotKey(GetHWND(), WM_QUERYHOTKEY, MOD_CONTROL | MOD_SHIFT, 'Q');
	RegisterHotKey(GetHWND(), WM_OFTENTOOLHOTKEY, MOD_CONTROL | MOD_SHIFT, 'W');
	RegisterHotKey(GetHWND(), WM_TEMPINFOHOTKEY, MOD_CONTROL | MOD_SHIFT, 'E');*/
	CustomInital();
	DataLoad();
}

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	__try {
		CCalendar * pOperation = new CCalendar;
		vector<DataInfo*> list = pOperation->GetList();
		for (int i = 0; i < list.size(); i++) {
			list[i]->isRemind = 1;

			if (list[i]->remindType == 0) {
				pOperation->Update(list[i]->id);
			}
			else {
				list[i]->remindDateTime = GetTimeInfo();
				//list[i]->remindType = 0;
				pOperation->Add(list[i]);
			}

			int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
			int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
			CFrmTipInfo* wnd = new CFrmTipInfo(DataType::calendar, "", list[i]->content, "温馨提醒");
			wnd->Create(NULL, _T("温馨提醒"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
			wnd->ResizeClient(360, 240);
			::SetWindowPos(wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			::ShowWindow(wnd->GetHWND(), SW_SHOW);
		}
		delete pOperation;

		if (userInfo->WeekEnable) { //周记事提示
			time_t timep;
			time(&timep);
			struct tm* dateInfo;
			char tmp[64];
			dateInfo = localtime(&timep);
			strftime(tmp, sizeof(tmp), "%H:%M", dateInfo);
			string now = tmp;

			timep += 330;

			dateInfo = localtime(&timep);
			strftime(tmp, sizeof(tmp), "%H:%M", dateInfo);
			string nexttime = tmp;

			string s = userInfo->WeekTime;
			if (strcmp(now.c_str(), userInfo->WeekTime.c_str()) < 0 && strcmp(userInfo->WeekTime.c_str(), nexttime.c_str()) < 0) {
				CWeekly * pWeekly = new CWeekly;
				DataInfo * dataInfo = pWeekly->GetTodayData();
				CFrmTipInfo* wnd = new CFrmTipInfo(DataType::weekly, dataInfo->id, dataInfo->content, "今日记事");
				delete dataInfo;
				delete pWeekly;
				wnd->Create(NULL, _T("今日记事"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
				wnd->CenterWindow();
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
			}
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::SetHotKey(CDuiString hotText, CDuiString configkey, __in int hotKey, bool inital) {
	__try {
		vector<CDuiString> list = StrSplit(hotText, " + ");
		bool shift = false, ctrl = false, alt = false;
		string keyStr = "";
		for (int i = 0; i < list.size(); i++) {
			if (list[i] == _T("Shift")) {
				shift = true;
			}
			else if (list[i] == _T("Ctrl")) {
				ctrl = true;
			}
			else if (list[i] == _T("Alt")) {
				alt = true;
			}
			else {
				keyStr = list[i];
			}
		}
		if (keyStr != "") {
			char hotkey = keyStr[0];
			DWORD h = MOD_CONTROL;
			if (!ctrl)
				h &= ~MOD_CONTROL;
			if (shift)
				h |= MOD_SHIFT;
			if (alt)
				h |= MOD_ALT;
			RegisterHotKey(GetHWND(), hotKey, h, hotkey);

		}
		else {
			if (!inital)
				ShowMessage("无效热键");
		}
		if (!inital) {
			WritePrivateProfileString(_T("SystemInfo"), configkey, hotText, configPath);
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::CustomInital() {
	__try {
		m_pMenu = new CMenuWnd();
		AddNotificationIcon(GetHWND());
		m_MainHwnd = GetHWND();

		configPath = CPaintManagerUI::GetInstancePath();
		configPath.Append(_T("Config.ini"));

		m_pNavData->GetItemAt(2)->SetVisible(userInfo->personDataEnable);  //个人记事显示
		m_pLayoutEncryt->SetVisible(userInfo->personDataEnable);
		m_pChkWeeklyEnable->SetCheck(userInfo->WeekEnable);
		string exePath = CPaintManagerUI::GetInstancePath();
		exePath += _T("HepNotepad.exe");
		string version = "{c #386382}版本：" + GetProductVersion(exePath.c_str()) + " 2019 by hep{/c}";
		m_pVersion->SetText(version.c_str());

		string text = "", hour = "", min = "";
		for (int i = 0; i < 24; i++) {
			CListLabelElementUI* pLabel = new CListLabelElementUI;
			if (i < 10) {
				text = "0" + CDataTypeTool::intTostring(i);
			}
			else {
				text = CDataTypeTool::intTostring(i);
			}
			pLabel->SetFixedHeight(23);
			pLabel->SetText(text.c_str());
			m_pCbxHour->Add(pLabel);
		}

		for (int i = 0; i < 60; i += 5) {
			CListLabelElementUI* pLabel = new CListLabelElementUI;
			if (i < 10) {
				text = "0" + CDataTypeTool::intTostring(i);
			}
			else {
				text = CDataTypeTool::intTostring(i);
			}
			pLabel->SetFixedHeight(23);
			pLabel->SetText(text.c_str());
			m_pCbxMin->Add(pLabel);
		}

		string time = userInfo->WeekTime;
		int pos = time.find(":");
		hour = time.substr(0, pos);
		min = time.substr(pos + 1);
		m_pCbxHour->SetText(hour.c_str());
		m_pCbxMin->SetText(min.c_str());
		if (userInfo->WeekEnable) {
			m_pCbxHour->SetVisible(true);
			m_pCbxMin->SetVisible(true);
		}

		TCHAR paramVlaue[MAX_PATH];
		GetPrivateProfileString(_T("SystemInfo"), _T("HotShowMain"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotShowMain"), WM_SHOWMAINHOTKEY);
		m_pHotShowMain->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotHotAddTip"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotHotAddTip"), WM_ADDTIPHOTKEY);
		m_pHotAddTip->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotInfoQuery"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotInfoQuery"), WM_QUERYHOTKEY);
		m_pHotInfoQuery->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotProgramQuery"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotProgramQuery"), WM_OFTENTOOLHOTKEY);
		m_pHotProgramQuery->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotTempTip"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotTempTip"), WM_TEMPINFOHOTKEY);
		m_pHotTempTip->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotCopySaveTip"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotCopySaveTip"), WM_COPYSAVETIPHOTKEY);
		m_pHotCopySaveTip->SetText(paramVlaue);
		GetPrivateProfileString(_T("SystemInfo"), _T("HotGetCopy"), _T(""), paramVlaue, MAX_PATH, configPath);
		SetHotKey(paramVlaue, _T("HotGetCopy"), WM_GETCOPY);
		m_pGetCopy->SetText(paramVlaue);

		m_pTempTip = new CFrmTipInfo(DataType::tempTip, "", "", "临时便签");
		m_pTempTip->Create(NULL, _T("临时便签"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		SetWindowLong(m_pTempTip->GetHWND(), GWL_EXSTYLE, WS_EX_TOOLWINDOW); //不在任务栏显示
		::ShowWindow(m_pTempTip->GetHWND(), SW_HIDE);

		m_pLogo = new CFrmLogo();
		m_pLogo->Create(NULL, _T("随手记"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		SetWindowLong(m_pLogo->GetHWND(), GWL_EXSTYLE, WS_EX_TOOLWINDOW); //不在任务栏显示
		::ShowWindow(m_pLogo->GetHWND(), SW_HIDE);

		wndCopy = new CFrmTipInfo(DataType::copy, "获取复制内容");
		wndCopy->Create(NULL, "复制内容", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		SetWindowLong(wndCopy->GetHWND(), GWL_EXSTYLE, WS_EX_TOOLWINDOW); //不在任务栏显示
		::ShowWindow(wndCopy->GetHWND(), SW_HIDE);

		SetTimer(0, 0, 300000, &TimerProc);
		init = true;

		/*GetPrivateProfileString(_T("SystemInfo"), _T("AutoStart"), _T("false"), paramVlaue, MAX_PATH, configPath);
		string pValue = paramVlaue;
		if (pValue == _T("true")) {
			string exePath = CPaintManagerUI::GetInstancePath();
			exePath += _T("HepNotepad.exe");
			TCHAR path[MAX_PATH];
			strcpy(path, exePath.c_str());
			AutoStart(path, _T("HepNotepad_22820191023"));

		}*/

		m_hwndNextViewer = SetClipboardViewer(GetHWND());

	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::DataLoad() {
	currentType = DataType::tip;
	LoadTipData();
}

void CMainFrame::LoadTipData(int tab) {
	__try {
		if (!m_pOptNavTip->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pTipList->GetCustomAttribute("pageIndex");
		string pageCount = m_pTipList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());
		if (tab != 1 || m_pTipList->GetCount() < 1) {
			if (tab == 1 && m_pTipList->GetCount() > 0) return;
			string key = m_pTipKey->GetText().GetData();
			CTipInfo* pTipInfo = new CTipInfo;
			vector<DataInfo*> tipInfoList;
			tipInfoList = pTipInfo->GetList(pageInfo, key);
			m_pTipList->RemoveCustomAttribute("pageCount");
			m_pTipList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());

			m_pTipList->RemoveAll();
			for (int i = 0; i < tipInfoList.size(); i++) {
				tipInfoList[i]->dataType = DataType::tip;
				tipInfoList[i]->wndTitle = "便签详情";
				CListTextElementUI* pElem = new CListTextElementUI;
				pElem->SetOwner(m_pTipList);
				pElem->SetName("ItemInfo");
				pElem->SetTag((UINT_PTR)tipInfoList[i]);
				pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
				CDuiString str = tipInfoList[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pElem->SetText(1, listStr[0].GetData());
				else
					pElem->SetText(1, str.GetData());
				pElem->SetText(2, (tipInfoList[i]->updateTime).c_str());
				m_pTipList->Add(pElem);
			}
			delete pTipInfo;
		}
		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadInfoData(int tab) {
	__try {
		if (!m_pOptNavInfo->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pDataList->GetCustomAttribute("pageIndex");
		string pageCount = m_pDataList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());
		if (tab != 1 || m_pDataList->GetCount() < 1) {
			string type = m_pInfoType->GetText();
			string key = m_pInfoKey->GetText();
			CDataInfo* pDataInfo = new CDataInfo;
			vector<DataInfo*> dataInfoList;
			dataInfoList = pDataInfo->GetList(pageInfo, type, key);
			m_pDataList->RemoveCustomAttribute("pageCount");
			m_pDataList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());
			DeleteData(m_pDataList);
			vector<string> tpyeList;
			for (int i = 0; i < dataInfoList.size(); i++) {
				dataInfoList[i]->dataType = DataType::info;
				dataInfoList[i]->wndTitle = "信息详情";
				CListTextElementUI* pElem = new CListTextElementUI;
				//pElem->SetManager(&m_pm, m_pDataList, true);
				pElem->SetOwner(m_pDataList);
				pElem->SetName("ItemInfo");
				pElem->SetTag((UINT_PTR)dataInfoList[i]);
				pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
				pElem->SetText(1, (dataInfoList[i]->typeName).c_str());
				pElem->SetText(2, dataInfoList[i]->title.c_str());
				CDuiString str = dataInfoList[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pElem->SetText(3, listStr[0].GetData());
				else
					pElem->SetText(3, str.GetData());
				pElem->SetText(4, dataInfoList[i]->updateTime.c_str());
				m_pDataList->Add(pElem);
			}

			string text = m_pInfoType->GetText();
			vector<string> dataTypeList = pDataInfo->GetTypeList();
			m_pInfoType->RemoveAll();

			bool select = false;
			for (int i = 0; i < dataTypeList.size(); i++) {
				CListLabelElementUI* pLabel = new CListLabelElementUI;
				pLabel->SetFixedHeight(23);
				pLabel->SetText(dataTypeList[i].c_str());
				m_pInfoType->Add(pLabel);
				if (text == dataTypeList[i]) {
					pLabel->Select(true);
					select = true;
				}
			}
			CListLabelElementUI* pLabel = new CListLabelElementUI;
			pLabel->SetFixedHeight(23);
			pLabel->SetText("");
			m_pInfoType->Add(pLabel);
			if (!select)
				pLabel->Select(true);
			delete pDataInfo;
		}

		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadPersonInfoData(int tab) {
	__try {
		if (!m_pOptNavPersonal->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pPersonInfoList->GetCustomAttribute("pageIndex");
		string pageCount = m_pPersonInfoList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());
		DeleteData(m_pPersonInfoList);
		if ((tab != 1 || m_pPersonInfoList->GetCount() < 1) && !m_pLayoutEncryt->IsVisible()) {
			string type = m_pPersonInfoType->GetText();
			string key = m_pPersonInfoKey->GetText();
			CPersonInfo* pPerson = new CPersonInfo;
			vector<DataInfo*> dataInfoList;
			dataInfoList = pPerson->GetList(pageInfo, type, key);
			m_pPersonInfoList->RemoveCustomAttribute("pageCount");
			m_pPersonInfoList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());

			vector<string> tpyeList;
			for (int i = 0; i < dataInfoList.size(); i++) {
				dataInfoList[i]->dataType = DataType::personal;
				dataInfoList[i]->wndTitle = "个人信息详情";
				CListTextElementUI* pElem = new CListTextElementUI;
				pElem->SetOwner(m_pPersonInfoList);
				pElem->SetName("ItemPersonInfo");
				pElem->SetTag((UINT_PTR)dataInfoList[i]);
				pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
				pElem->SetText(1, (dataInfoList[i]->typeName).c_str());
				pElem->SetText(2, dataInfoList[i]->title.c_str());
				CDuiString str = dataInfoList[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pElem->SetText(3, listStr[0].GetData());
				else
					pElem->SetText(3, str.GetData());
				pElem->SetText(4, dataInfoList[i]->updateTime.c_str());
				m_pPersonInfoList->Add(pElem);
			}
			string text = m_pPersonInfoType->GetText();
			vector<string> dataTypeList = pPerson->GetTypeList();
			m_pPersonInfoType->RemoveAll();
			bool select = false;
			for (int i = 0; i < dataTypeList.size(); i++) {
				CListLabelElementUI* pLabel = new CListLabelElementUI;
				pLabel->SetFixedHeight(23);
				pLabel->SetText(dataTypeList[i].c_str());
				m_pPersonInfoType->Add(pLabel);
				if (text == dataTypeList[i]) {
					pLabel->Select(true);
					select = true;
				}
			}
			CListLabelElementUI* pLabel = new CListLabelElementUI;
			pLabel->SetFixedHeight(23);
			pLabel->SetText("");
			m_pPersonInfoType->Add(pLabel);
			if (!select)
				pLabel->Select(true);
			delete pPerson;
		}
		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadRecent() {
	__try {
		string path = CPaintManagerUI::GetInstancePath();
		path += "recent\\";
		vector<string> typeList = { ".docx", ".doc", ".xlsx", ".xls", ".txt", ".pdf", ".lnk" };
		vector<string> dList = CFileHelper::GetFiles(path, typeList, 2);
		m_pRecentList->RemoveAll();
		for (int i = 0; i < dList.size(); i++) {
			CListContainerElementUI* pElem = nullptr;
			CDialogBuilder builder;
			pElem = static_cast<CListContainerElementUI*>(builder.Create(_T("recentItem.xml")));

			int nPos = dList[i].rfind(".");
			string	fileName = dList[i].substr(0, nPos);
			nPos = fileName.rfind(".");
			fileName = fileName.substr(0, nPos);

			CControlUI* pHeaderImage = static_cast<CControlUI*>(pElem->FindSubControl("fileImage"));
			CTextUI* pName = static_cast<CTextUI*>(pElem->FindSubControl("name"));
			pName->SetText(fileName.c_str());
			pElem->SetAttribute("filePath", (path + dList[i]).c_str());

			RECT rectClient = pHeaderImage->GetClientPos();
			RECT rect;
			rect.top = rectClient.top;
			rect.left = rectClient.left;
			rect.right = rectClient.right;
			rect.bottom = rectClient.bottom;

			HICON hIcon = CFileHelper::FileIcon((path + dList[i]).c_str());
			string pngPath = path + fileName + ".png";
			if (!CFileHelper::IsFileExist(pngPath.c_str())) {
				CFileHelper::SaveHIcon2PngFile(hIcon, CDataTypeTool::GBKToUTF8(pngPath).c_str());
			}
			pHeaderImage->SetBkImage(pngPath.c_str());
			CButtonUI* pBtnRemove = static_cast<CButtonUI*>(pElem->FindSubControl("btnRecentRemove"));
			pBtnRemove->SetAttribute("filePath", (path + dList[i]).c_str());
			pBtnRemove->SetAttribute("type", "recent");
			m_pRecentList->Add(pElem);
		}
		CListContainerElementUI* pElemAdd = nullptr;
		CDialogBuilder builder;
		pElemAdd = static_cast<CListContainerElementUI*>(builder.Create(_T("recentItem.xml")));
		pElemAdd->SetName("btnAddRecent");
		CButtonUI* pBtnRemove = static_cast<CButtonUI*>(pElemAdd->FindSubControl("btnRecentRemove"));
		pBtnRemove->SetVisible(false);
		m_pRecentList->Add(pElemAdd);
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadTool() {
	__try {
		string path = CPaintManagerUI::GetInstancePath();
		path += "tool\\";
		vector<string> typeList = { ".exe", ".udl" };
		vector<string> toolList = CFileHelper::GetFiles(path, typeList, 2);
		for (int i = 0; i < m_pDevelopToolList->GetCount(); i++) {
			CListContainerElementUI* pElem = (CListContainerElementUI*)m_pDevelopToolList->GetItemAt(i);
			string name = pElem->GetName();
			if (name != "sysTooltem") {
				m_pDevelopToolList->RemoveAt(i);
				i--;
			}
		}

		for (int i = 0; i < toolList.size(); i++) {
			CListContainerElementUI* pElem = nullptr;
			CDialogBuilder builder;
			pElem = static_cast<CListContainerElementUI*>(builder.Create(_T("toolItem.xml")));

			int nPos = toolList[i].rfind(".");
			string	toolName = toolList[i].substr(0, nPos);

			CControlUI* pHeaderImage = static_cast<CControlUI*>(pElem->FindSubControl("fileImage"));
			CTextUI* pName = static_cast<CTextUI*>(pElem->FindSubControl("name"));
			pName->SetText(toolName.c_str());
			pElem->SetAttribute("filePath", (path + toolList[i]).c_str());

			RECT rectClient = pHeaderImage->GetClientPos();
			RECT rect;
			rect.top = rectClient.top;
			rect.left = rectClient.left;
			rect.right = rectClient.right;
			rect.bottom = rectClient.bottom;

			HICON hIcon = CFileHelper::FileIcon((path + toolList[i]).c_str());
			string pngPath = path + toolName + ".png";
			if (!CFileHelper::IsFileExist(pngPath.c_str())) {
				CFileHelper::SaveHIcon2PngFile(hIcon, CDataTypeTool::GBKToUTF8(pngPath).c_str());
			}
			pHeaderImage->SetBkImage(pngPath.c_str());
			m_pDevelopToolList->Add(pElem);
		}

		string customPath = CPaintManagerUI::GetInstancePath();
		customPath += "CustomTool\\";
		if (!CFileHelper::IsDirectoryExist(customPath.c_str())) {
			CFileHelper::CreateDir(customPath);
		}
		vector<string> cusTometoolList = CFileHelper::GetFiles(customPath, typeList, 2);

		for (int i = 0; i < cusTometoolList.size(); i++) {
			CListContainerElementUI* pElem = nullptr;
			CDialogBuilder builder;
			pElem = static_cast<CListContainerElementUI*>(builder.Create(_T("recentItem.xml")));

			int nPos = cusTometoolList[i].rfind(".");
			string	toolName = cusTometoolList[i].substr(0, nPos);

			CControlUI* pHeaderImage = static_cast<CControlUI*>(pElem->FindSubControl("fileImage"));
			CTextUI* pName = static_cast<CTextUI*>(pElem->FindSubControl("name"));
			pName->SetText(toolName.c_str());
			pElem->SetAttribute("filePath", (customPath + cusTometoolList[i]).c_str());

			RECT rectClient = pHeaderImage->GetClientPos();
			RECT rect;
			rect.top = rectClient.top;
			rect.left = rectClient.left;
			rect.right = rectClient.right;
			rect.bottom = rectClient.bottom;

			HICON hIcon = CFileHelper::FileIcon((customPath + cusTometoolList[i]).c_str());
			string pngPath = customPath + toolName + ".png";
			if (!CFileHelper::IsFileExist(pngPath.c_str())) {
				CFileHelper::SaveHIcon2PngFile(hIcon, CDataTypeTool::GBKToUTF8(pngPath).c_str());
			}
			pHeaderImage->SetBkImage(pngPath.c_str());
			CButtonUI* pBtnRemove = static_cast<CButtonUI*>(pElem->FindSubControl("btnRecentRemove"));
			pBtnRemove->SetAttribute("filePath", (customPath + cusTometoolList[i]).c_str());
			pBtnRemove->SetAttribute("type", "tool");
			m_pDevelopToolList->Add(pElem);
		}

		CListContainerElementUI* pElemAdd = nullptr;
		CDialogBuilder builder;
		pElemAdd = static_cast<CListContainerElementUI*>(builder.Create(_T("toolItem.xml")));
		pElemAdd->SetName("btnAddTool");
		m_pDevelopToolList->Add(pElemAdd);
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadCalendar(int tab) {
	__try {
		if (!m_pOptNavCalendar->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pCalendarList->GetCustomAttribute("pageIndex");
		string pageCount = m_pCalendarList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());
		if (tab != 1 || m_pCalendarList->GetCount() < 1) {
			int type = m_pCalendarType->GetCurSel();
			if (type > 2)type = -1;
			string key = m_pCalendarKey->GetText();
			CCalendar* pOperation = new CCalendar;
			vector<DataInfo*> dataInfoList;
			dataInfoList = pOperation->GetList(pageInfo, key, type, 0);
			m_pCalendarList->RemoveCustomAttribute("pageCount");
			m_pCalendarList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());
			DeleteData(m_pCalendarList);
			vector<string> tpyeList;
			for (int i = 0; i < dataInfoList.size(); i++) {
				dataInfoList[i]->dataType = DataType::personal;
				dataInfoList[i]->wndTitle = "个人信息详情";
				CListTextElementUI* pElem = new CListTextElementUI;
				pElem->SetOwner(m_pCalendarList);
				pElem->SetName("ItemCalendarInfo");
				pElem->SetTag((UINT_PTR)dataInfoList[i]);
				pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
				pElem->SetText(1, (dataInfoList[i]->typeName).c_str());
				pElem->SetText(2, (dataInfoList[i]->remindDate).c_str());
				pElem->SetText(3, (dataInfoList[i]->remindTime).c_str());
				pElem->SetText(4, dataInfoList[i]->statusText.c_str());
				CDuiString str = dataInfoList[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pElem->SetText(5, listStr[0].GetData());
				else
					pElem->SetText(5, str.GetData());
				m_pCalendarList->Add(pElem);
			}
			delete pOperation;
		}
		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadHistory(int tab) {
	__try {
		if (!m_pOptNavHistory->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pHistoryList->GetCustomAttribute("pageIndex");
		string pageCount = m_pHistoryList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());
		if (tab != 1 || m_pHistoryList->GetCount() < 1) {
			int type = m_pHistoryType->GetCurSel();
			if (type > 2)type = -1;
			string key = m_pHistoryKey->GetText();
			CCalendar* pOperation = new CCalendar;
			vector<DataInfo*> dataInfoList;
			dataInfoList = pOperation->GetList(pageInfo, key, type, 1);
			m_pHistoryList->RemoveCustomAttribute("pageCount");
			m_pHistoryList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());
			DeleteData(m_pHistoryList);
			vector<string> tpyeList;
			for (int i = 0; i < dataInfoList.size(); i++) {
				dataInfoList[i]->dataType = DataType::personal;
				dataInfoList[i]->wndTitle = "个人信息详情";
				CListTextElementUI* pElem = new CListTextElementUI;
				pElem->SetOwner(m_pHistoryList);
				pElem->SetName("ItemCalendarInfo");
				pElem->SetTag((UINT_PTR)dataInfoList[i]);
				pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
				pElem->SetText(1, (dataInfoList[i]->typeName).c_str());
				pElem->SetText(2, (dataInfoList[i]->remindDateTime).c_str());
				//pElem->SetText(3, dataInfoList[i]->title.c_str());
				CDuiString str = dataInfoList[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pElem->SetText(3, listStr[0].GetData());
				else
					pElem->SetText(3, str.GetData());
				m_pHistoryList->Add(pElem);
			}
			delete pOperation;
		}
		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::LoadWeekly(int tab) {
	__try {

		if (!m_pOptNavWeekly->IsSelected())return;
		PageInfo * pageInfo = new PageInfo;
		string index = m_pWeeklyList->GetCustomAttribute("pageIndex");
		string pageCount = m_pWeeklyList->GetCustomAttribute("pageCount");
		pageInfo->currPageIndex = atoi(index.c_str());
		pageInfo->totalPageCount = atoi(pageCount.c_str());

		CWeekly* pOperation = new CWeekly;
		vector<DataInfo*> dataInfoList;
		dataInfoList = pOperation->GetList(pageInfo);
		m_pWeeklyList->RemoveCustomAttribute("pageCount");
		m_pWeeklyList->SetAttribute("pageCount", CDataTypeTool::intTostring(pageInfo->totalPageCount).c_str());
		DeleteData(m_pWeeklyList);
		vector<string> tpyeList;
		for (int i = 0; i < dataInfoList.size(); i++) {
			dataInfoList[i]->dataType = DataType::personal;
			dataInfoList[i]->wndTitle = "今日记事";
			CListTextElementUI* pElem = new CListTextElementUI;
			pElem->SetOwner(m_pWeeklyList);
			pElem->SetName("ItemWeekInfo");
			pElem->SetTag((UINT_PTR)dataInfoList[i]);
			pElem->SetText(0, (CDataTypeTool::intTostring(i + 1 + pageInfo->currPageIndex*pageInfo->pageSize)).c_str());
			pElem->SetText(1, (dataInfoList[i]->pId).c_str());
			pElem->SetText(2, (dataInfoList[i]->title).c_str());
			pElem->SetText(3, (dataInfoList[i]->remark).c_str());
			/*pElem->SetText(2, (dataInfoList[i]->monday).c_str());
			pElem->SetText(3, (dataInfoList[i]->tuesday).c_str());
			pElem->SetText(4, (dataInfoList[i]->wednesday).c_str());
			pElem->SetText(5, (dataInfoList[i]->thursday).c_str());
			pElem->SetText(6, (dataInfoList[i]->friday).c_str());
			pElem->SetText(7, (dataInfoList[i]->saturday).c_str());
			pElem->SetText(8, (dataInfoList[i]->sunday).c_str());*/
			string content = "周一：" + dataInfoList[i]->monday;
			content += "\r\n周二：" + dataInfoList[i]->tuesday;
			content += "\r\n周三：" + dataInfoList[i]->wednesday;
			content += "\r\n周四：" + dataInfoList[i]->thursday;
			content += "\r\n周五：" + dataInfoList[i]->friday;
			content += "\r\n周六：" + dataInfoList[i]->saturday;
			content += "\r\n周日：" + dataInfoList[i]->sunday;
			pElem->SetAttribute("content", content.c_str());
			CDuiString str = content.c_str();
			vector<CDuiString>  listStr = StrSplit(str, "\r");
			if (listStr.size() > 0)
				pElem->SetText(4, listStr[0].GetData());
			else
				pElem->SetText(4, str.GetData());
			m_pWeeklyList->Add(pElem);
		}
		delete pOperation;
		SetPageInfo(pageInfo);
		delete pageInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::DeleteData(CListUI* pList) {
	for (int i = 0; i < pList->GetCount(); i++) {
		CListTextElementUI* pElem = (CListTextElementUI*)pList->GetItemAt(i);
		DataInfo* dataInfo = (DataInfo*)pElem->GetTag();
		delete dataInfo;
		dataInfo = nullptr;
	}
	pList->RemoveAll();
}

void CMainFrame::LoadSystemInfo() {
	TCHAR paramVlaue[MAX_PATH];
	GetPrivateProfileString(_T("SystemInfo"), _T("AutoStart"), _T("false"), paramVlaue, MAX_PATH, configPath);
	string pValue = paramVlaue;
	if (pValue == _T("true")) {
		m_pChkAutoStart->SetCheck(true);
	}

	string programDir = GetSpecialFolderLocation(CSIDL_DESKTOP);
	programDir += "\\随手记.lnk";
	if (CFileHelper::IsFileExist(programDir.c_str())) {
		m_pChkCreateLink->SetCheck(true);
	}
	m_pChkAutoSaveTempTip->SetCheck(userInfo->saveTempTip);
	m_pChkPersonEnable->SetCheck(userInfo->personDataEnable);
}

void  CMainFrame::SetPageInfo(PageInfo * pageInfo) {
	if (pageInfo->totalPageCount < 2) {
		m_pLayoutPage->SetVisible(false);
	}
	else {
		m_pLayoutPage->SetVisible(true);
	}
	m_pBtnFirst->SetEnabled(!(pageInfo->currPageIndex == 0));
	m_pBtnPrev->SetEnabled(!(pageInfo->currPageIndex == 0));
	m_pBtnNext->SetEnabled(!(pageInfo->currPageIndex == pageInfo->totalPageCount - 1));
	m_pBtnEnd->SetEnabled(!(pageInfo->currPageIndex == pageInfo->totalPageCount - 1));
	string text = "当前页：" + CDataTypeTool::intTostring(pageInfo->currPageIndex + 1) + "/" + CDataTypeTool::intTostring(pageInfo->totalPageCount) + "页";
	m_pPageInfo->SetText(text.c_str());
}

void  CMainFrame::ConnectClipboard() {
	bool useSetClipboardWnd = true;
	if (IsVista())
	{
		HMODULE hUser32 = LoadLibrary(_T("USER32.dll"));
		if (hUser32)
		{
			typedef BOOL(__stdcall *AddClipFormatListener)(HWND hwnd);

			AddClipFormatListener addListener = (AddClipFormatListener)GetProcAddress(hUser32, "AddClipboardFormatListener");
			if (addListener)
			{
				useSetClipboardWnd = false;
				addListener(m_hWnd);
			}
		}
	}
}

void  CMainFrame::DisconnectClipboard(bool bSendPing) {
	bool removeOldWay = true;

	if (IsVista())
	{
		HMODULE hUser32 = LoadLibrary(_T("USER32.dll"));
		if (hUser32)
		{
			typedef BOOL(__stdcall *RemoveClipFormatListener)(HWND hwnd);

			RemoveClipFormatListener removeListener = (RemoveClipFormatListener)GetProcAddress(hUser32, "RemoveClipboardFormatListener");
			if (removeListener)
			{
				removeOldWay = false;
				removeListener(m_hWnd);
			}
		}
	}

	if (removeOldWay)
	{
		BOOL bRet = ::ChangeClipboardChain(m_hWnd, m_hwndNextViewer);
		if (!bRet)
		{
			bRet = ::ChangeClipboardChain(m_hWnd, m_hwndNextViewer);
			if (!bRet)
			{

			}
		}
	}
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT: /// not allow maximize, because after maximize, ui layout is wrong 
	{
		if (init) {
			RECT rect = m_pLayoutPerson->GetPos();
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			if (width > 100) {
				if (m_pLayoutEncryt->IsVisible()) {
					m_pLayoutEncryt->SetFixedHeight(height);
					m_pLayoutEncryt->SetMinHeight(height);
					m_pLayoutEncryt->SetFixedWidth(width);
					m_pLayoutEncryt->SetMinWidth(width);
					::InvalidateRect(GetHWND(), NULL, TRUE);
				}
			}
		}
	}
	break;
	case WM_DRAWCLIPBOARD: {
		if (copySaveTip) {
			if (OpenClipboard(GetHWND())) {
				if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
				{
					HANDLE hClip;
					char* pBuf;
					//读取数据  
					hClip = GetClipboardData(CF_TEXT);
					pBuf = (char*)GlobalLock(hClip);
					GlobalUnlock(hClip);
					string content = pBuf;
					content += "\r\n";
					if (content.length() > 1) {
						wndCopy->AppendContent(content);
					}
				}
				CloseClipboard();
			}
		}
		if (m_hwndNextViewer) {
			::SendMessage(m_hwndNextViewer, uMsg, wParam, lParam);
		}
		InvalidateRect(GetHWND(), NULL, TRUE);
		return 0;
	}
						   break;
	case WM_CHANGECBCHAIN: {
		if ((HWND)wParam == m_hwndNextViewer) {
			m_hwndNextViewer = (HWND)lParam;
		}
		else if (m_hwndNextViewer) {
			::SendMessage(m_hwndNextViewer, uMsg, wParam, lParam);
		}
		return 0;
	}
						   break;
	case WM_DESTROY: {
		ChangeClipboardChain(GetHWND(), m_hwndNextViewer);
	}
					 break;
	case HN_MAINLOADTIP: //重新加载数据
		LoadTipData();
		break;
	case HN_MAINLOADINFO:
		LoadInfoData();
		break;
	case HN_MAINLOADPERSONDATA:
		LoadPersonInfoData();
		break;
	case HN_MAINLOADCALENDARDATA:
		LoadCalendar();
		break;
	case HN_MAINLOADWEEKDATA:
		LoadWeekly();
		break;
	case HN_GETINFOQUERY:
	{
		__try {
			SendMsg * msg = (SendMsg *)wParam;
			if (m_pTempTip != NULL) {
				bool show = m_pTempTip->SetContent(msg->data);
				int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
				int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
				if (show) {
					if (m_pTempTip == NULL || m_pTempTip == nullptr) {
						m_pTempTip = new CFrmTipInfo(DataType::tempTip, "", "", "临时便签");

						m_pTempTip->Create(NULL, _T("临时便签"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
						m_pTempTip->ResizeClient(360, 240);
					}
					::SetWindowPos(m_pTempTip->GetHWND(), HWND_TOPMOST, cx - 361, cy - 218, 360, 240, SWP_SHOWWINDOW);
					::ShowWindow(m_pTempTip->GetHWND(), SW_SHOW);
				}
			}
			delete msg;
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
	break;
	case WMAPP_NOTIFYCALLBACK:
	{
		if (wParam != IDI_SMALL)
			return 0;
		if (LOWORD(lParam) == WM_LBUTTONUP) {
			if (IsWindowVisible(GetHWND()) == false) {
				::ShowWindow(GetHWND(), SW_RESTORE);
				return false;
			}
		}
		if (LOWORD(lParam) == WM_RBUTTONUP)
		{
			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
				m_pMenu = new CMenuWnd();
			}
			CDuiPoint point;
			::GetCursorPos(&point);
			point.y -= 195;
			//point.x -= 160;
			m_pMenu->Init(NULL, _T("notificationMenu.xml"), point, &m_pm);
			// 动态添加后重新设置菜单的大小
			m_pMenu->ResizeMenu();
		}
		else if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
		{
			/*if (IsWindowVisible(GetHWND()) == false){

			::ShowWindow(GetHWND(), SW_RESTORE);
			isTip = false;
			return false;
			} */
		}
		else if (LOWORD(lParam) == WM_MOUSEMOVE) { //鼠标移动上去显示消息

		}
	}
	break;
	case WM_HOTKEY:
	{
		int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
		__try {
			//if (!IsWindowVisible(GetHWND())) {
			if (wParam == WM_SHOWMAINHOTKEY) {
				::ShowWindow(GetHWND(), SW_SHOW);
			}
			else if (wParam == WM_ADDTIPHOTKEY) {

				CFrmTipInfo* wnd = new CFrmTipInfo(DataType::tip, "", "", "添加便签信息");
				wnd->Create(NULL, _T("添加便签信息"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
				wnd->CenterWindow();
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
			}
			else if (wParam == WM_QUERYHOTKEY) {
				CFrmInfoQuery * wnd = new CFrmInfoQuery();
				wnd->Create(GetHWND(), _T("信息查询"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 301, cy - 310);
				::SetWindowPos(wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				//::SendMessage(wnd->GetHWND(), WM_QUERYHOTRESULTKEY, (WPARAM)0, 0);
				::SetActiveWindow(wnd->GetHWND());
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
				::SetCursorPos(cx - 151, cy - 275);//更改鼠标坐标 
				Sleep(10);//控制移动时间间隔
				mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			}
			else if (wParam == WM_OFTENTOOLHOTKEY) {
				CFrmInfoQuery * wnd = new CFrmInfoQuery(2);
				wnd->Create(GetHWND(), _T("快捷方式"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 301, cy - 310);
				::SetWindowPos(wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				//::SendMessage(wnd->GetHWND(), WM_QUERYHOTRESULTKEY, (WPARAM)0, 0);
				::SetActiveWindow(wnd->GetHWND());
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
				::SetCursorPos(cx - 150, cy - 275);//更改鼠标坐标 
				Sleep(10);//控制移动时间间隔
				mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

			}
			else if (wParam == WM_TEMPINFOHOTKEY) {
				if (m_pTempTip == NULL || m_pTempTip == nullptr) {
					m_pTempTip = new CFrmTipInfo(DataType::tempTip, "", "", "临时便签");

					m_pTempTip->Create(NULL, _T("临时便签"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
					m_pTempTip->ResizeClient(360, 240);
				}

				::SetWindowPos(m_pTempTip->GetHWND(), HWND_TOPMOST, cx - 361, cy - 218, 360, 240, SWP_SHOWWINDOW);
				::ShowWindow(m_pTempTip->GetHWND(), SW_SHOW);
			}
			else if (wParam == WM_COPYSAVETIPHOTKEY) {

				if (OpenClipboard(GetHWND())) {
					if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
					{
						HANDLE hClip;
						char* pBuf;
						//读取数据  
						hClip = GetClipboardData(CF_TEXT);
						pBuf = (char*)GlobalLock(hClip);
						GlobalUnlock(hClip);
						string d = pBuf;
						if (d.length() > 5) {
							DataInfo * dataInfo = new DataInfo;
							CTipInfo* pOperation = new CTipInfo;
							dataInfo->createTime = GetTimeInfo();
							dataInfo->content = pBuf;
							dataInfo->updateTime = GetTimeInfo();
							pOperation->Add(dataInfo);
							delete pOperation;
							delete dataInfo;
							LoadTipData();
						}
					}
					CloseClipboard();
				}
				/*copySaveTip = !copySaveTip;
				if (copySaveTip) {
					if (m_pLogo == NULL || m_pTempTip == nullptr) {
						m_pLogo = new CFrmLogo();
						m_pLogo->Create(NULL, _T("临时便签"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 24,24);
					}
					::SetWindowPos(m_pLogo->GetHWND(), HWND_TOPMOST, cx - 48, 16, 24, 24, SWP_SHOWWINDOW);
					::ShowWindow(m_pLogo->GetHWND(), SW_SHOW);
				}
				else {
					::ShowWindow(m_pLogo->GetHWND(), SW_HIDE);
				}*/

			}
			else if (wParam == WM_GETCOPY) {
				if (!IsWindowVisible(wndCopy->GetHWND())) {
					copySaveTip = true;
					if (wndCopy == NULL || wndCopy == nullptr) {
						wndCopy = new CFrmTipInfo(DataType::copy, "获取复制内容");
						wndCopy->Create(NULL, _T("获取复制内容"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
						wndCopy->ResizeClient(360, 240);
					}
					::SetWindowPos(wndCopy->GetHWND(), HWND_TOPMOST, cx - 361, cy - 600, 360, 240, SWP_SHOWWINDOW);
					::ShowWindow(wndCopy->GetHWND(), SW_SHOW);
				}
				else {
					copySaveTip = false;
					::ShowWindow(wndCopy->GetHWND(), SW_HIDE);
				}
			}
			//}
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
	break;
	case WM_CHAR: {//快捷键捕获

		switch (wParam)
		{
		case 0x1B: //Esc
		{
			::ShowWindow(GetHWND(), SW_HIDE);
		}
		break;
		}
	}
				  break;

	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
		break;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	//delete this; ///< 在这自杀不好, 还是要通知主窗体将我的指针压入队列, 找一个合适的时机删除.
}

//选择事件
void CMainFrame::OnSelectedChanged(TNotifyUI &msg) {
	__try {
		if (_tcsicmp(msg.pSender->GetClass(), "OptionUI") == 0) {
			CDuiString itemType = msg.pSender->GetCustomAttribute(_T("navType"));
			if (itemType.IsEmpty() == false && itemType == _T("navMainBtn")) {
				int position = m_pNavMain->GetItemIndex(msg.pSender);
				m_pMainContainer->SelectItem(position);

			}
			else if (itemType.IsEmpty() == false && itemType == _T("navDataBtn")) {
				int position = m_pNavData->GetItemIndex(msg.pSender);
				m_pDataContainer->SelectItem(position);
			}
			else if (itemType.IsEmpty() == false && itemType == _T("navToolBtn")) {
				int position = m_pNavTool->GetItemIndex(msg.pSender);

				m_pToolContainer->SelectItem(position);

			}
			else if (itemType.IsEmpty() == false && itemType == _T("navCalendarBtn")) {
				int position = m_pNavCalendar->GetItemIndex(msg.pSender);
				m_pCalendarContainer->SelectItem(position);
			}
			else if (itemType.IsEmpty() == false && itemType == _T("navSystemBtn")) {
				int position = m_pNavSystem->GetItemIndex(msg.pSender);
				m_pSystemContainer->SelectItem(position);
			}
			ChangeTab();
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CMainFrame::ChangeTab() {
	if (m_pOptMainNavInfo->IsSelected()) {  //信息面板 
		if (m_pOptNavPersonal->IsSelected())
		{
			m_pLayoutEncryt->SetVisible(true);
			LoadPersonInfoData(1);
			currentType = DataType::personal;
		}
		else {
			m_pLayoutEncryt->SetVisible(false);
			if (m_pOptNavTip->IsSelected()) {
				LoadTipData(1);
				currentType = DataType::tip;
			}
			else if (m_pOptNavInfo->IsSelected()) {
				LoadInfoData(1);
				currentType = DataType::info;
			}
		}
	}
	else if (m_pOptMainNavTool->IsSelected()) { //工具栏
		LoadRecent();
		LoadTool();
		m_pLayoutPage->SetVisible(false);
	}
	else if (m_pOptMainNavCalendar->IsSelected()) { //日历

		if (m_pOptNavCalendar->IsSelected()) {
			LoadCalendar(1);
			currentType = DataType::calendar;
		}
		else if (m_pOptNavHistory->IsSelected()) {
			LoadHistory();
			currentType = DataType::history;
		}
		if (m_pOptNavWeekly->IsSelected()) {
			LoadWeekly(1);
			currentType = DataType::weekly;
		}
	}
	else if (m_pOptMainNavSet->IsSelected()) { //设置
		LoadSystemInfo();
		m_pLayoutPage->SetVisible(false);
	}
}

bool isDegital(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str.at(i) == '-' && str.size() > 1)  // 有可能出现负数
			continue;
		if (str.at(i) > '9' || str.at(i) < '0')
			return false;
	}
	return true;
}

void CMainFrame::Notify(TNotifyUI & msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (msg.sType == _T("return"))
	{
		if (itemName.IsEmpty() == false && itemName == _T("txtPwd"))
			Unlock();
		else if (itemName.IsEmpty() == false && itemName == _T("txtTipKey"))
			LoadTipData();
		else if (itemName.IsEmpty() == false && itemName == _T("txtInfoKey"))
			LoadInfoData();
		else if (itemName.IsEmpty() == false && itemName == _T("txtPersonInfoKey"))
			LoadPersonInfoData();
		else if (itemName.IsEmpty() == false && itemName == _T("txtGotoPage"))
		{
			string index = m_pGotoPage->GetText();
			if (isDegital(index)) {
				GotoPageIndex(GotoPage::Goto, atoi(index.c_str()));
			}
			else {
				ShowMessage("不是有效页数");
			}
		}
		else if (_tcsicmp(itemName, _T("txtHotShowMain")) == 0) {
			CDuiString text = m_pHotShowMain->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotShowMain"), WM_SHOWMAINHOTKEY, false);
		}
		else if (_tcsicmp(itemName, _T("txtHotAddTip")) == 0) {
			CDuiString text = m_pHotAddTip->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotHotAddTip"), WM_ADDTIPHOTKEY, false);
		}
		else if (_tcsicmp(itemName, _T("txtHotInfoQuery")) == 0) {
			CDuiString text = m_pHotInfoQuery->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotInfoQuery"), WM_QUERYHOTKEY, false);
		}
		else if (_tcsicmp(itemName, _T("txtHotProgramQuery")) == 0) {
			CDuiString text = m_pHotProgramQuery->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotProgramQuery"), WM_OFTENTOOLHOTKEY, false);

		}
		else if (_tcsicmp(itemName, _T("txtHotTempTip")) == 0) {
			CDuiString text = m_pHotTempTip->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotTempTip"), WM_TEMPINFOHOTKEY, false);
		}
		else if (_tcsicmp(itemName, _T("txtHotCopySaveTip")) == 0) {
			CDuiString text = m_pHotCopySaveTip->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotCopySaveTip"), WM_COPYSAVETIPHOTKEY, false);
		}
		else if (_tcsicmp(itemName, _T("txtGetCopy")) == 0) {
			CDuiString text = m_pGetCopy->GetText();
			if (text.GetLength() == 0 || text == "无") {
				ShowMessage("热键冲突");
			}
			SetHotKey(text, _T("HotGetCopy"), WM_GETCOPY, false);
		}
	}
	else if (msg.sType == _T("killfocus")) {
		TCHAR paramVlaue[MAX_PATH];
		if (_tcsicmp(itemName, _T("txtHotShowMain")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotShowMain"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotShowMain"), WM_SHOWMAINHOTKEY);
			m_pHotShowMain->SetText(paramVlaue);
		}
		if (_tcsicmp(itemName, _T("txtHotAddTip")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotHotAddTip"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotHotAddTip"), WM_ADDTIPHOTKEY);
			m_pHotAddTip->SetText(paramVlaue);
		}
		if (_tcsicmp(itemName, _T("txtHotInfoQuery")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotInfoQuery"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotInfoQuery"), WM_QUERYHOTKEY);
			m_pHotInfoQuery->SetText(paramVlaue);

		}
		else if (_tcsicmp(itemName, _T("txtHotProgramQuery")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotProgramQuery"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotProgramQuery"), WM_OFTENTOOLHOTKEY);
			m_pHotProgramQuery->SetText(paramVlaue);
		}
		else if (_tcsicmp(itemName, _T("txtHotTempTip")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotTempTip"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotTempTip"), WM_TEMPINFOHOTKEY);
			m_pHotTempTip->SetText(paramVlaue);
		}
		else if (_tcsicmp(itemName, _T("txtHotCopySaveTip")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotCopySaveTip"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("HotCopySaveTip"), WM_COPYSAVETIPHOTKEY);
			m_pHotCopySaveTip->SetText(paramVlaue);
		}
		else if (_tcsicmp(itemName, _T("txtHotCopy")) == 0) {
			GetPrivateProfileString(_T("SystemInfo"), _T("HotGetCopy"), _T(""), paramVlaue, MAX_PATH, configPath);
			SetHotKey(paramVlaue, _T("txtGetCopy"), WM_GETCOPY);
			m_pGetCopy->SetText(paramVlaue);
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)//单击
	{
		__try {
			if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("btnAddRecent")) == 0) {
				LPCTSTR lpszFilter = "All(*.*)\0*.*;\0\0";
				CFileDialogEx fileDlg;
				fileDlg.SetFilter(lpszFilter);
				fileDlg.SetParentWnd(m_hWnd);

				if (fileDlg.ShowOpenFileDlg())
				{
					string filePath = fileDlg.GetPathName().c_str();
					string path = CPaintManagerUI::GetInstancePath();
					path += "recent\\";
					if (!CFileHelper::IsDirectoryExist(path.c_str())) {
						CFileHelper::CreateDir(path);
					}
					int pos = filePath.rfind("\\");
					string fileDir = filePath.substr(0, pos + 1);
					string fileName = filePath.substr(pos + 1);
					pos = fileName.rfind(".");
					string name = fileName;
					name.substr(0, pos - 1);
					CreateShortcut(filePath.c_str(), fileName.c_str(), fileDir.c_str(), name.c_str(), CSIDL_PROGRAMS);
					string programDir = GetSpecialFolderLocation(CSIDL_PROGRAMS);
					programDir += "\\" + fileName + ".lnk";
					MoveFile(programDir.c_str(), (path + fileName + ".lnk").c_str());
					LoadRecent();
				}
			}
			else if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("btnAddTool")) == 0) {
				LPCTSTR lpszFilter = "All(*.exe)\0*.exe;\0\0";
				CFileDialogEx fileDlg;
				fileDlg.SetFilter(lpszFilter);
				fileDlg.SetParentWnd(m_hWnd);

				if (fileDlg.ShowOpenFileDlg())
				{
					string file = fileDlg.GetPathName().c_str();

					string path = CPaintManagerUI::GetInstancePath();
					path += "CustomTool\\";
					int pos = file.rfind("\\");
					string fileName = file.substr(pos + 1);
					CopyFile(file.c_str(), (path + fileName).c_str(), true);

					LoadTool();
				}
			}
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)//双击
	{
		__try {
			if (_tcsicmp(msg.pSender->GetClass(), "ListTextElementUI") == 0)
			{
				CDuiString itemName = msg.pSender->GetName();
				if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("ItemInfo")) == 0)
				{
					DataInfo* pDataInfo = (DataInfo*)msg.pSender->GetTag();

					CFrmTipInfo* wnd = new CFrmTipInfo(pDataInfo->dataType, pDataInfo->id, pDataInfo->content, pDataInfo->wndTitle);
					wnd->Create(NULL, pDataInfo->wndTitle.c_str(), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
					wnd->CenterWindow();
					::ShowWindow(wnd->GetHWND(), SW_SHOW);
				}
				else if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("ItemPersonInfo")) == 0)
				{
					DataInfo* pDataInfo = (DataInfo*)msg.pSender->GetTag();
					string content = pDataInfo->content;
					if (pDataInfo->isEncryption == 1) {
						content = Decrypt(pDataInfo->content);
					}
					CFrmTipInfo* wnd = new CFrmTipInfo(pDataInfo->dataType, pDataInfo->id, content, pDataInfo->wndTitle);

					wnd->Create(NULL, pDataInfo->wndTitle.c_str(), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
					wnd->CenterWindow();
					::ShowWindow(wnd->GetHWND(), SW_SHOW);
				}
				else if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("ItemCalendarInfo")) == 0) {
					EditCalendar((CListTextElementUI*)msg.pSender);
				}
				else if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("ItemWeekInfo")) == 0) {

					CListTextElementUI * pElem = (CListTextElementUI*)msg.pSender;
					string content = pElem->GetCustomAttribute("content");
					CFrmTipInfo* wnd = new CFrmTipInfo(DataType::none, "", content, "今日记事");
					wnd->Create(NULL, _T("今日记事"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
					::ShowWindow(wnd->GetHWND(), SW_SHOW);
				}
			}
			else if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0) {
				if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("sysTooltem")) == 0)
				{
					CListContainerElementUI* pElem = (CListContainerElementUI*)msg.pSender;
					string tool = pElem->GetCustomAttribute("tool");
					if (tool == "createDir") {
						CFrmCreateDir * wnd = new CFrmCreateDir(1);
						wnd->Create(GetHWND(), _T("创建目录"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
						wnd->CenterWindow();
						::ShowWindow(wnd->GetHWND(), SW_SHOW);
					}
					else if (tool == "copyFile") {
						CFrmCreateDir * wnd = new CFrmCreateDir(2);
						wnd->Create(GetHWND(), _T("复制文件"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
						wnd->CenterWindow();
						::ShowWindow(wnd->GetHWND(), SW_SHOW);
					}
					else if (tool == "dealText") {
						CFrmTextEditor * wnd = new CFrmTextEditor();
						wnd->Create(GetHWND(), _T("文本编辑器"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
						::SetWindowPos(wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
						wnd->CenterWindow();
						::ShowWindow(wnd->GetHWND(), SW_SHOW);
					}
				}
				else if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("openItem")) == 0)
				{
					CListContainerElementUI* pElem = (CListContainerElementUI*)msg.pSender;
					string filePath = pElem->GetCustomAttribute("filePath");
					if (CFileHelper::IsFileExist(filePath.c_str()))
						ShellExecute(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
				}
			}
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
	return __super::Notify(msg);
}

void CMainFrame::OnClick(TNotifyUI& msg)
{
	CDuiString itemName = msg.pSender->GetName();

	if (_tcsicmp(itemName, _T("closebtn")) == 0) {
		::ShowWindow(GetHWND(), SW_HIDE);
	}
	else if (_tcsicmp(itemName, _T("minbtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (_tcsicmp(itemName, _T("maxbtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (_tcsicmp(itemName, _T("restorebtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	else if (msg.pSender->GetName().Compare(_T("btnTipAdd")) == 0) {
		CFrmTipInfo* wnd = new CFrmTipInfo(DataType::tip, "", "", "添加便签信息");
		wnd->Create(NULL, _T("添加便签信息"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnTipSearch")) == 0) {
		LoadTipData();
	}
	else if (msg.pSender->GetName().Compare(_T("btnTipDel")) == 0) {
		int nIndex = m_pTipList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pTipList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CTipInfo* pTipInfo = new CTipInfo;
			bool result = pTipInfo->Delete(pDataInfo->id);
			delete pTipInfo;
			if (result) {
				LoadTipData();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnInfoAdd")) == 0) {
		CFrmInfoAdd* wnd = new CFrmInfoAdd();
		wnd->Create(NULL, _T("信息添加"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnInfoDel")) == 0) {
		int nIndex = m_pDataList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pDataList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CDataInfo* pData = new CDataInfo;
			bool result = pData->Delete(pDataInfo->id);
			delete pData;
			if (result) {
				LoadInfoData();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnInfoUpdate")) == 0) {
		int nIndex = m_pDataList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		CListTextElementUI * item = (CListTextElementUI*)m_pDataList->GetItemAt(nIndex);
		DataInfo* pDataInfo = (DataInfo*)item->GetTag();
		CFrmInfoAdd* wnd = new CFrmInfoAdd(pDataInfo);
		wnd->Create(NULL, _T("信息"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnInfoSearch")) == 0) {
		LoadInfoData();
	}
	else if (msg.pSender->GetName().Compare(_T("btnPersonInfoAdd")) == 0) {
		CFrmPersonInfoAdd * wnd = new CFrmPersonInfoAdd();
		wnd->Create(NULL, _T("个人记事添加"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPersonInfoUpdate")) == 0) {
		int nIndex = m_pPersonInfoList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		CListTextElementUI * item = (CListTextElementUI*)m_pPersonInfoList->GetItemAt(nIndex);
		DataInfo* pDataInfo = (DataInfo*)item->GetTag();
		CFrmPersonInfoAdd* wnd = new CFrmPersonInfoAdd(pDataInfo);
		wnd->Create(NULL, _T("个人记事修改"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPersonInfoDel")) == 0) {
		int nIndex = m_pPersonInfoList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pPersonInfoList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CPersonInfo* pPerson = new CPersonInfo;
			bool result = pPerson->Delete(pDataInfo->id);
			delete pPerson;
			if (result) {
				LoadPersonInfoData();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnPersonInfoSearch")) == 0) {
		LoadPersonInfoData();
	}
	else if (msg.pSender->GetName().Compare(_T("btnPwdSet")) == 0) {
		CFrmPersonInfoPwdSet * wnd = new CFrmPersonInfoPwdSet();
		wnd->Create(GetHWND(), _T("密码设置"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnLockScreen")) == 0) {
		m_pPersonInfoList->RemoveAll();
		m_pLayoutEncryt->SetVisible(true);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPwdOK")) == 0) {
		Unlock();
	}
	else if (msg.pSender->GetName().Compare(_T("btnRecentRemove")) == 0) {

		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CButtonUI * btnRemove = (CButtonUI*)msg.pSender;
			string type = btnRemove->GetCustomAttribute("type");
			string fileName = btnRemove->GetCustomAttribute("filePath");
			if (type == "recent") {
				remove(fileName.c_str());
				LoadRecent();
			}
			else {
				remove(fileName.c_str());
				int nPos = fileName.rfind(".");
				fileName = fileName.substr(0, nPos + 1);
				fileName += "png";
				remove(fileName.c_str());
				LoadTool();
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarAdd")) == 0) {
		CFrmCalendarAdd * wnd = new CFrmCalendarAdd();
		wnd->Create(NULL, _T("提醒设置"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarUpdate")) == 0) {
		int nIndex = m_pCalendarList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		EditCalendar((CListTextElementUI*)m_pCalendarList->GetItemAt(nIndex));
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarDel")) == 0) {
		int nIndex = m_pCalendarList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pCalendarList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CCalendar* pOperation = new CCalendar;
			bool result = pOperation->Delete(pDataInfo->id);
			delete pOperation;
			if (result) {
				LoadCalendar();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarSearch")) == 0) {
		LoadCalendar();
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarHistoryDel")) == 0) {
		int nIndex = m_pHistoryList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pHistoryList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CCalendar* pOperation = new CCalendar;
			bool result = pOperation->Delete(pDataInfo->id);
			delete pOperation;
			if (result) {
				LoadHistory();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnCalendarHistoryClear")) == 0) {

		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除下列所有数据吗？"))) {
			int type = m_pHistoryType->GetCurSel();
			if (type > 2)type = -1;
			CCalendar* pOperation = new CCalendar;
			string key = m_pHistoryKey->GetText();
			bool result = pOperation->ClearHistory(key, type);
			delete pOperation;
			if (result) {
				LoadHistory();
				ShowMessage("清理成功");
			}
			else {
				ShowMessage("清理失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnHistorySearch")) == 0) {
		LoadHistory();
	}
	else if (msg.pSender->GetName().Compare(_T("btnWeeklyAdd")) == 0) {
		CWeekly * pOperation = new CWeekly;
		DataInfo * dataInfo = pOperation->GetTodayData();
		CFrmTipInfo* wnd = new CFrmTipInfo(DataType::weekly, dataInfo->id, dataInfo->content, "今日记事");
		delete dataInfo;
		delete pOperation;
		wnd->Create(NULL, _T("今日记事"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
		wnd->CenterWindow();
		::ShowWindow(wnd->GetHWND(), SW_SHOW);
	}
	else if (msg.pSender->GetName().Compare(_T("btnWeeklyDel")) == 0) {
		int nIndex = m_pWeeklyList->GetCurSel();
		if (nIndex < 0) {
			ShowMessage("请选择一条数据");
			return;
		}
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要删除选中数据吗？"))) {
			CListTextElementUI * item = (CListTextElementUI*)m_pWeeklyList->GetItemAt(nIndex);
			DataInfo* pDataInfo = (DataInfo*)item->GetTag();
			CWeekly* pOperation = new CWeekly;
			bool result = pOperation->Delete(pDataInfo->id);
			delete pOperation;
			if (result) {
				LoadWeekly();
				ShowMessage("删除成功");
			}
			else {
				ShowMessage("删除失败");
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("chkAutoStart")) == 0) { //开机启动
		CDuiString checked = m_pChkAutoStart->GetCheck() ? _T("false") : _T("true");
		if (checked == _T("true")) {
			string exePath = CPaintManagerUI::GetInstancePath();
			exePath += _T("HepNotepad.exe");
			TCHAR path[MAX_PATH];
			strcpy(path, exePath.c_str());
			AutoStart(path, _T("HepNotepad_22820191023"));
		}
		else {
			char * officePath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
			DeleteRegKey(HKEY_CURRENT_USER, officePath, _T("HepNotepad_22820191023"));
		}
		WritePrivateProfileString(_T("SystemInfo"), _T("AutoStart"), checked, configPath);
	}
	else if (msg.pSender->GetName().Compare(_T("chkCreateDeskLink")) == 0) { //创建桌面快捷方式
		CDuiString checked = m_pChkCreateLink->GetCheck() ? _T("false") : _T("true");
		if (checked == _T("true")) {
			string exePath = CPaintManagerUI::GetInstancePath();
			exePath += _T("HepNotepad.exe");
			TCHAR path[MAX_PATH];
			strcpy(path, exePath.c_str());
			CreateShortcut(exePath.c_str(), "随手记", CPaintManagerUI::GetInstancePath(), "随手记", CSIDL_DESKTOP);
		}
		else {
			string programDir = GetSpecialFolderLocation(CSIDL_DESKTOP);
			programDir += "\\随手记.lnk";
			remove(programDir.c_str());
		}
		WritePrivateProfileString(_T("SystemInfo"), _T("AutoStart"), checked, configPath);
	}
	else if (msg.pSender->GetName().Compare(_T("chkAutoSaveTempTip")) == 0) {  //自动保存临时便签
		CDuiString checked = m_pChkAutoSaveTempTip->GetCheck() ? _T("false") : _T("true");
		CSystemInfo * pOperation = new CSystemInfo;
		DataInfo * pDataInfo = new DataInfo;
		string value = "0";
		if (checked == _T("true")) {
			userInfo->saveTempTip = true;
			value = "1";
		}
		else {
			userInfo->saveTempTip = false;
		}
		pOperation->UpdateValue(userInfo->saveTempTipCol, value);
		delete pOperation;
	}
	else if (msg.pSender->GetName().Compare(_T("chkPersonEnable")) == 0) { //个人记事启用
		CDuiString checked = m_pChkPersonEnable->GetCheck() ? _T("false") : _T("true");
		CSystemInfo * pOperation = new CSystemInfo;
		DataInfo * pDataInfo = new DataInfo;
		string value = "0";
		if (checked == _T("true")) {
			userInfo->personDataEnable = true;
			value = "1";
		}
		else {
			userInfo->personDataEnable = false;
		}

		COptionUI * option = (COptionUI *)m_pNavData->GetItemAt(2);
		if (option->IsSelected())
		{
			((COptionUI *)m_pNavData->GetItemAt(0))->Selected(true, true);
		}

		m_pNavData->GetItemAt(2)->SetVisible(userInfo->personDataEnable);
		pOperation->UpdateValue(userInfo->personDataEnableCol, value);
		delete pOperation;
	}
	else if (msg.pSender->GetName().Compare(_T("chkWeeklyEnable")) == 0) {
		CDuiString checked = m_pChkWeeklyEnable->GetCheck() ? _T("false") : _T("true");
		if (checked == _T("true")) {
			m_pCbxMin->SetVisible(true);
			m_pCbxHour->SetVisible(true);
		}
		else {
			m_pCbxMin->SetVisible(false);
			m_pCbxHour->SetVisible(false);
		}
		string enable = "1";
		if (m_pChkWeeklyEnable->GetCheck())
			enable = "0";
		userInfo->WeekEnable = checked == _T("true");
		CSystemInfo * pOperation = new CSystemInfo;
		pOperation->UpdateValue(userInfo->WeekTimeCol, enable, "");
		delete pOperation;
	}
	else if (msg.pSender->GetName().Compare(_T("btnPageFirst")) == 0) { //首页
		GotoPageIndex(GotoPage::First);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPagePrev")) == 0) {  //上一页
		GotoPageIndex(GotoPage::Prev);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPageNext")) == 0) {  //下一页
		GotoPageIndex(GotoPage::Next);
	}
	else if (msg.pSender->GetName().Compare(_T("btnPageEnd")) == 0) {  //末页
		GotoPageIndex(GotoPage::End);
	}
	else if (msg.pSender->GetName().Compare(_T("btnDataBackup")) == 0) {
		LPCTSTR lpszFilter = "db(*.db)\0*.db;\0\0";
		CFileDialogEx fileDlg;
		fileDlg.SetFilter(lpszFilter);
		fileDlg.SetParentWnd(m_hWnd);
		fileDlg.SetMultiSel(true);
		if (fileDlg.ShowSaveFileDlg())
		{
			string file = fileDlg.GetPathName().c_str();
			string path = CPaintManagerUI::GetInstancePath();
			path += "data.db";
			CLog::WriteToLog(path.c_str());
			CopyFile(path.c_str(), file.c_str(), false);
			ShowMessage("备份成功");
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnDataRevert")) == 0) {
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("还愿操作将覆盖所有数据，您确定要还愿吗？"))) {
			LPCTSTR lpszFilter = "db(*.db)\0*.db;\0\0";
			CFileDialogEx fileDlg;
			fileDlg.SetFilter(lpszFilter);
			fileDlg.SetParentWnd(m_hWnd);
			fileDlg.SetMultiSel(true);
			if (fileDlg.ShowOpenFileDlg())
			{
				string file = fileDlg.GetPathName().c_str();
				CCommon * pOperation = new CCommon;
				if (pOperation->DatabaseCheck(file)) {
					string path = CPaintManagerUI::GetInstancePath();
					path += "data.db";
					string backpath = CPaintManagerUI::GetInstancePath();

					backpath += "backup\\";
					if (!CFileHelper::IsDirectoryExist(backpath.c_str())) {
						CFileHelper::CreateDir(backpath);
					}
					backpath += "data" + GetTimeInfoNumber() + ".db";
					CopyFile(path.c_str(), backpath.c_str(), false);
					CLog::WriteToLog(path.c_str());
					CopyFile(file.c_str(), path.c_str(), false);
					ShowMessage("还原成功，将重新启动");
					string exec = CPaintManagerUI::GetInstancePath();
					exec += "HepNotepad.exe";
					ShellExecute(NULL, "open", exec.c_str(), NULL, NULL, SW_SHOWNORMAL);
					Close();
					PostQuitMessage(0);
				}
				else {
					ShowMessage("不是有效备份数据或同不是一个用户备份文件数据");
				}
			}
		}
	}
	else if (msg.pSender->GetName().Compare(_T("btnDataInput")) == 0) {

		LPCTSTR lpszFilter = "db(*.db)\0*.db;\0\0";
		CFileDialogEx fileDlg;
		fileDlg.SetFilter(lpszFilter);
		fileDlg.SetParentWnd(m_hWnd);
		fileDlg.SetMultiSel(true);
		if (fileDlg.ShowOpenFileDlg())
		{
			string file = fileDlg.GetPathName().c_str();
			CCommon * pOperation = new CCommon;
			if (pOperation->DatabaseCheck(file)) {
				pOperation->DataInput(file);
				ShowMessage("导入成功");
			}
			else {
				ShowMessage("不是有效备份数据或同不是一个用户备份文件数据");
			}
		}
	}
}

void CMainFrame::GotoPageIndex(GotoPage page, int index) {
	string pageIndex = "0", pageCount = "0";
	switch (currentType) {
	case tip:
	{
		pageIndex = m_pTipList->GetCustomAttribute("pageIndex");
		pageCount = m_pTipList->GetCustomAttribute("pageCount");
	}
	break;
	case info: {
		pageIndex = m_pDataList->GetCustomAttribute("pageIndex");
		pageCount = m_pDataList->GetCustomAttribute("pageCount");
	}
			   break;
	case personal: {
		pageIndex = m_pPersonInfoList->GetCustomAttribute("pageIndex");
		pageCount = m_pPersonInfoList->GetCustomAttribute("pageCount");
	}
				   break;
	case calendar: {
		pageIndex = m_pCalendarList->GetCustomAttribute("pageIndex");
		pageCount = m_pCalendarList->GetCustomAttribute("pageCount");
	}
				   break;
	case history: {
		pageIndex = m_pHistoryList->GetCustomAttribute("pageIndex");
		pageCount = m_pHistoryList->GetCustomAttribute("pageCount");
	}
				  break;
	case weekly: {
		pageIndex = m_pWeeklyList->GetCustomAttribute("pageIndex");
		pageCount = m_pWeeklyList->GetCustomAttribute("pageCount");
	}
				 break;
	default:
		break;
	}

	int currentPage = atoi(pageIndex.c_str());
	int totalPageCount = atoi(pageCount.c_str());
	switch (page)
	{
	case First:
		currentPage = 0;
		break;
	case Prev:
		currentPage--;
		break;
	case Next:
		currentPage++;
		break;
	case End:
		currentPage = totalPageCount;
		break;
	case Goto:
		if (index > totalPageCount || index < 0) {
			ShowMessage("不是有效页数");
			return;
		}
		else {
			currentPage = index - 1;
		}
		break;
	default:
		break;
	}

	switch (currentType) {
	case tip:
		m_pTipList->RemoveCustomAttribute("pageIndex");
		m_pTipList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadTipData();
		break;
	case info:
		m_pDataList->RemoveCustomAttribute("pageIndex");
		m_pDataList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadInfoData();
		break;
	case personal:
		m_pPersonInfoList->RemoveCustomAttribute("pageIndex");
		m_pPersonInfoList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadPersonInfoData();
		break;
	case calendar:
		m_pCalendarList->RemoveCustomAttribute("pageIndex");
		m_pCalendarList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadCalendar();
		break;
	case history:
		m_pHistoryList->RemoveCustomAttribute("pageIndex");
		m_pHistoryList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadHistory();
		break;
	case weekly:
		m_pWeeklyList->RemoveCustomAttribute("pageIndex");
		m_pWeeklyList->SetAttribute("pageIndex", CDataTypeTool::intTostring(currentPage).c_str());
		LoadWeekly();
		break;
	default:
		break;
	}
}

//>->19.4.24自定义消息处理--托盘菜单事件消息
LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		if (pMenuCmd != NULL)
		{
			CDuiString sMenuName = pMenuCmd->szName;
			if (sMenuName.CompareNoCase(_T("btnExit")) == 0)
			{
				Close();
				PostQuitMessage(0);
			}
			else if (sMenuName.CompareNoCase(_T("btnShow")) == 0)
			{
				if (IsWindowVisible(GetHWND()) == false) {
					::ShowWindow(GetHWND(), SW_RESTORE);
					return false;
				}
			}
			else if (sMenuName.CompareNoCase(_T("btnCreateFloder")) == 0)
			{
				CFrmCreateDir * wnd = new CFrmCreateDir(1);
				wnd->Create(GetHWND(), _T("创建目录"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
				wnd->CenterWindow();
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
			}
			else if (sMenuName.CompareNoCase(_T("btnCopy")) == 0)
			{
				CFrmCreateDir * wnd = new CFrmCreateDir(2);
				wnd->Create(GetHWND(), _T("复制文件"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
				wnd->CenterWindow();
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
			}
			else if (sMenuName.CompareNoCase(_T("btnTempTip")) == 0)
			{
				int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
				int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
				if (m_pTempTip == NULL || m_pTempTip == nullptr) {

					m_pTempTip = new CFrmTipInfo(DataType::tempTip, "", "", "临时便签");
					m_pTempTip->Create(NULL, _T("临时便签"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
					m_pTempTip->ResizeClient(360, 240);
				}
				::SetWindowPos(m_pTempTip->GetHWND(), HWND_TOPMOST, cx - 361, cy - 218, 360, 240, SWP_SHOWWINDOW);
				::ShowWindow(m_pTempTip->GetHWND(), SW_SHOW);
			}
			else if (sMenuName.CompareNoCase(_T("btnTextEditor")) == 0)
			{
				CFrmTextEditor * wnd = new CFrmTextEditor();
				wnd->Create(GetHWND(), _T("文本编辑器"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
				::SetWindowPos(wnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				wnd->CenterWindow();
				::ShowWindow(wnd->GetHWND(), SW_SHOW);
			}
		}
		bHandled = TRUE;

	}
	return 0;
}

void CMainFrame::Unlock() {
	CSystemInfo * system = new CSystemInfo;
	vector<DataInfo*> list = system->GetList(userInfo->pwdCol);
	delete system;
	string guid = GenerateGuid();
	CLog::WriteToLog(guid);
	CLog::WriteToLog(Encrypt(guid));
	string pwd = "";
	for (int i = 0; i < list.size(); i++) {
		pwd = list[i]->value;
	}
	pwd = Decrypt(pwd);
	string txtpwd = m_pTxtPwd->GetText().GetData();
	if (pwd == txtpwd) {
		m_pTxtPwd->SetText("");
		m_pLayoutEncryt->SetVisible(false);
		LoadPersonInfoData();
	}
	else {
		ShowMessage("密码不正确");
	}
}

void CMainFrame::EditCalendar(CListTextElementUI * item) {
	DataInfo* pDataInfo = (DataInfo*)item->GetTag();
	CFrmCalendarAdd* wnd = new CFrmCalendarAdd(pDataInfo);
	wnd->Create(NULL, _T("提醒设置"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, 0, 0);
	wnd->CenterWindow();
	::ShowWindow(wnd->GetHWND(), SW_SHOW);
}

void CMainFrame::OnItemSelect(TNotifyUI &msg) {
	if (msg.sType == _T("itemselect")) {

		if (msg.pSender->GetName() == _T("cbxWeekMin") || msg.pSender->GetName() == _T("cbxWeekHour"))
		{
			string hour = m_pCbxHour->GetText();
			string min = m_pCbxMin->GetText();
			userInfo->WeekTime = hour + ":" + min;
			CSystemInfo * pOperation = new CSystemInfo;
			pOperation->UpdateValue(userInfo->WeekTimeCol, "", userInfo->WeekTime);
			delete pOperation;
		}
	}
}