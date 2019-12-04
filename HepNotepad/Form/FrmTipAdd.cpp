#include "../stdafx.h"
#include "FrmAddTip.h"

DUI_BEGIN_MESSAGE_MAP(CFrmTipAdd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) 
DUI_END_MESSAGE_MAP()

CFrmTipAdd::CFrmTipAdd()
{
}


CFrmTipAdd::~CFrmTipAdd()
{
}

void CFrmTipAdd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd); 
}

 
CDuiString CFrmTipAdd::GetSkinFile()
{
	return _T("addTip.xml");
}

 

LPCTSTR CFrmTipAdd::GetWindowClassName(void) const
{
	return _T("ECOA04.0Login");
}


void CFrmTipAdd::InitWindow()
{
 
}
 
LRESULT CFrmTipAdd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	case SC_MAXIMIZE: /// not allow maximize, because after maximize, ui layout is wrong 

		bHandled = TRUE;
		return S_OK;
		break;
	default:
		break;
	}
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

void CFrmTipAdd::Notify(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	 
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmTipAdd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	 
	 
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}


void CFrmTipAdd::OnClick(TNotifyUI &msg)
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


