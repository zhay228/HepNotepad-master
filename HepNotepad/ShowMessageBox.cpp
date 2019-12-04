#include "stdafx.h"
#include "ShowMessageBox.h"

DUI_BEGIN_MESSAGE_MAP(CShowMessageBox, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CShowMessageBox::CShowMessageBox()
{
	m_title.Assign(_T(""));
	m_message.Assign(_T(""));
}

CShowMessageBox::CShowMessageBox(CDuiString title, CDuiString message, int _msgType)
{
	m_title.Assign(title);
	m_message.Assign(message);
	msgType = _msgType;
} 

CShowMessageBox::~CShowMessageBox()
{
}

void CShowMessageBox::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


CDuiString CShowMessageBox::GetSkinFile()
{
	return _T("messagebox.xml");
}
 
LPCTSTR CShowMessageBox::GetWindowClassName(void) const
{
	return _T("HepNotepad.0MessageWnd");
}
void CShowMessageBox::OnClick(TNotifyUI &msg)
{
	if (msg.pSender == m_pBtnClose || msg.pSender->GetName().Compare(_T("btnCancal")) == 0)
	{
		Close(0); // 因为activex的原因，使用close可能会出现错误
		return;
	}else if (msg.pSender->GetName().Compare(_T("btnOk"))==0)
	{
		Close(1);
		return;
	}
}


void CShowMessageBox::Notify(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (itemName.IsEmpty() == false && msg.sType == _T("return"))
	{
		
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CShowMessageBox::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CShowMessageBox::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
	return 0;
}

LRESULT CShowMessageBox::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CShowMessageBox::InitWindow()
{
	m_pBtnClose = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnClose")));
	m_pBtnOK = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnOk")));
	m_pBtnCancel = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnCancal")));
	m_pIcon = static_cast<CLabelUI*>(m_pm.FindControl(_T("icon")));
	m_pTitle = static_cast<CTextUI*>(m_pm.FindControl(_T("title")));
	m_pMessage = static_cast<CTextUI*>(m_pm.FindControl(_T("message")));
	m_pBtnClose->SetFocus();
	m_pTitle->SetText(m_title);
	m_pMessage->SetText(m_message); 
	if (msgType == 2){
		RECT rc;
		rc.left = 84;
		rc.right = 0;
		rc.top = 15;
		rc.bottom = 5;
		m_pBtnOK->SetPadding(rc);
		m_pBtnCancel->SetVisible(true);
	}
	int msgLength = m_message.GetLength();
	if (msgLength > 68) {
		int count = 15;
		int width = count * 15;
		int height = (msgLength / (34 + count) - 2) * 20;
		RECT rc = m_pBtnOK->GetPadding();
		rc.left += width / 2;
		m_pBtnOK->SetPadding(rc);
		::SetWindowPos(GetHWND(), NULL, 0, 0, 360 + width, 190 + height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

CControlUI* CShowMessageBox::CreateControl(LPCTSTR pstrClass)
{
	return __super::CreateControl(pstrClass);
}