#include "../stdafx.h"
#include "DemoFrame.h"

DUI_BEGIN_MESSAGE_MAP(CDemoFrame, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) 
DUI_END_MESSAGE_MAP()

CDemoFrame::CDemoFrame()
{
}


CDemoFrame::~CDemoFrame()
{
}

void CDemoFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd); 
}

 
CDuiString CDemoFrame::GetSkinFile()
{
	return _T("demo.xml");
}

 

LPCTSTR CDemoFrame::GetWindowClassName(void) const
{
	return _T("HepNotepad.demo");
}


void CDemoFrame::InitWindow()
{
 
}
 
LRESULT CDemoFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	DWORD   dwMsgSn = 0;
	dwMsgSn = wParam & 0xfff0;
	switch (dwMsgSn)
	{
	/*case SC_MAXIMIZE: /// not allow maximize, because after maximize, ui layout is wrong 

		bHandled = TRUE;
		return S_OK;
		break;*/
	default:
		break;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CDemoFrame::Notify(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	 
	return WindowImplBase::Notify(msg);
}

LRESULT CDemoFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{ 
	 
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDemoFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (wParam == VK_ESCAPE) //快捷键捕获 
	{
		Close(0);
	}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void CDemoFrame::OnClick(TNotifyUI &msg)
{
	if (msg.pSender->GetName().Compare(_T("btnClose")) == 0)
	{
		Close(0); // 因为activex的原因，使用close可能会出现错误
		return;
	}
	else if (msg.pSender->GetName().Compare(_T("btnMin")) == 0){
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}	 
}


