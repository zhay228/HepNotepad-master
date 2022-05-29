#include "../stdafx.h"
#include "FrmLogo.h"
#include "../DAL/TipInfo.h"
#include "../DAL//DataInfo.h"
#include "../DAL/Weekly.h"

DUI_BEGIN_MESSAGE_MAP(CFrmLogo, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()
extern UserInfo* userInfo;
extern HWND m_MainHwnd;

CFrmLogo::CFrmLogo()
{
}

CFrmLogo::CFrmLogo(string _id, string _dataInfo)
{
	 
}


CFrmLogo::~CFrmLogo()
{ 
	DestroyWindow(GetHWND());
}

void CFrmLogo::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd); 
	delete this;
}


CDuiString CFrmLogo::GetSkinFile()
{
	return _T("logo.xml");
}
 
LPCTSTR CFrmLogo::GetWindowClassName(void) const
{
	return _T("HepNotepad.tipInfo");
}

void CFrmLogo::InitWindow()
{
	__try { 
		  
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}
 

LRESULT CFrmLogo::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
 
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmLogo::Notify(TNotifyUI &msg)
{ 
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmLogo::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{	 
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFrmLogo::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (wParam == VK_ESCAPE) //¿ì½Ý¼ü²¶»ñ 
	{
		Close(0);
	}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}


void CFrmLogo::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (_tcsicmp(itemName, _T("closebtn")) == 0) {
			Close(1);
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
	}
}
 