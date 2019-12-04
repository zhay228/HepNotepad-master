#pragma once
#ifndef __Main_Frame_SHOW_H__
#define __Main_Frame_SHOW_H__


#include<sstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <deque>
#include "Form\FrmTipInfo.h"
#include "Form\FrmLogo.h"

class CMainFrame : public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
public:
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnSelectedChanged(TNotifyUI &msg);
	virtual void        OnClick(TNotifyUI& msg);
	virtual void OnItemSelect(TNotifyUI &msg);

	virtual LPCTSTR GetWindowClassName(void) const; 
	CDuiString GetSkinFile();
	virtual void InitWindow(); 
	
	virtual LRESULT     HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT     OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void        OnFinalMessage(HWND hWnd);
	virtual void        Notify(TNotifyUI & msg);
	 
private: 
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void CustomInital();
	void DataLoad();
	void LoadTipData(int tab=0);
	void LoadInfoData(int tab = 0);
	void LoadPersonInfoData(int tab = 0);
	void LoadRecent();
	void LoadTool();
	void LoadCalendar(int tab = 0);
	void LoadHistory(int tab = 0);
	void LoadWeekly(int tab = 0);
	void LoadSystemInfo();
	void EditCalendar(CListTextElementUI * item);
	void Unlock();
	void ChangeTab();
	void GotoPageIndex(GotoPage page, int index = 0);
	void SetPageInfo(PageInfo * pageInfo);
	void DeleteData(CListUI* pList);
	void SetHotKey(CDuiString hotText, CDuiString configkey, __in int hotKey, bool inital = true);
	void ConnectClipboard();    // connects as a clipboard viewer
	void DisconnectClipboard(bool bSendPing = true); // disconnects as a clipboard viewer
	 
private:
	CMenuWnd* m_pMenu;	 
	CHorizontalLayoutUI *m_pNavMain;
	CTabLayoutUI * m_pMainContainer;
	COptionUI * m_pOptMainNavInfo;
	COptionUI * m_pOptMainNavTool;
	COptionUI * m_pOptMainNavCalendar;
	COptionUI * m_pOptMainNavSet;

	CHorizontalLayoutUI * m_pLayoutPage;
	CTextUI * m_pPageInfo;
	CEditUI * m_pGotoPage;
	CButtonUI * m_pBtnFirst;
	CButtonUI * m_pBtnPrev;
	CButtonUI * m_pBtnNext;
	CButtonUI * m_pBtnEnd;

	CHorizontalLayoutUI *m_pNavData;
	CTabLayoutUI * m_pDataContainer;
	COptionUI * m_pOptNavTip;
	COptionUI * m_pOptNavInfo;
	COptionUI * m_pOptNavPersonal;

	CHorizontalLayoutUI *m_pNavTool;
	CTabLayoutUI * m_pToolContainer;

	CHorizontalLayoutUI *m_pNavCalendar;
	CTabLayoutUI * m_pCalendarContainer;
	COptionUI * m_pOptNavCalendar;
	COptionUI * m_pOptNavHistory;
	COptionUI * m_pOptNavWeekly;

	CHorizontalLayoutUI *m_pNavSystem;
	CTabLayoutUI * m_pSystemContainer;
	 
	CListUI* m_pTipList;
	CEditUI * m_pTipKey;

	CEditUI * m_pInfoKey;
	CComboUI * m_pInfoType;
	CListUI* m_pDataList;

	CEditUI * m_pPersonInfoKey;
	CComboUI * m_pPersonInfoType;
	CListUI * m_pPersonInfoList;

	CVerticalLayoutUI * m_pLayoutPerson;
	CVerticalLayoutUI * m_pLayoutEncryt;
	CEditUI* m_pTxtPwd;


	CListUI* m_pRecentList;
	CListUI* m_pDevelopToolList;

	CListUI* m_pCalendarList;
	CListUI* m_pHistoryList;
	CListUI* m_pWeeklyList;

	CComboUI * m_pCalendarType;
	CEditUI * m_pCalendarKey;
	CComboUI * m_pHistoryType;
	CEditUI * m_pHistoryKey;

	CCheckBoxUI * m_pChkAutoStart;
	CCheckBoxUI * m_pChkCreateLink;
	CCheckBoxUI * m_pChkAutoSaveTempTip;
	CCheckBoxUI * m_pChkPersonEnable;
	CCheckBoxUI * m_pChkWeeklyEnable;
	CHotKeyUI * m_pHotShowMain;
	CHotKeyUI * m_pHotAddTip;
	CHotKeyUI * m_pHotInfoQuery;
	CHotKeyUI * m_pHotProgramQuery;
	CHotKeyUI * m_pHotTempTip;
	CHotKeyUI * m_pHotCopySaveTip;
	CHotKeyUI * m_pGetCopy;


	CDuiString configPath; 
	CFrmTipInfo* m_pTempTip;
	CFrmTipInfo* wndCopy;
	CFrmLogo * m_pLogo;
	DataType currentType; 
	CComboUI * m_pCbxHour;
	CComboUI * m_pCbxMin;

	CTextUI * m_pVersion;

	HWND m_hwndNextViewer; 
	bool init = false;
	bool copySaveTip = false; 
};
#endif // #ifndef __Main_Frame_SHOW_H__

