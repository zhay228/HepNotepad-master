
#include "stdafx.h"
#include "resource.h"
#include "FrameWindowWnd.h"

CFrameWindowWnd::CFrameWindowWnd() { };

CFrameWindowWnd::~CFrameWindowWnd() { };

LPCTSTR CFrameWindowWnd::GetWindowClassName() const { return _T("UIMainFrame"); };

UINT CFrameWindowWnd::GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

void CFrameWindowWnd::OnFinalMessage(HWND /*hWnd*/) { delete this; };

void CFrameWindowWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) {
			Close();
		}
	}
}

LRESULT CFrameWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		return 0;
	}
	else if (uMsg == WM_DESTROY) {
		::PostQuitMessage(0);
	}
	else if (uMsg == WM_NCACTIVATE) {
		if (!::IsIconic(m_hWnd)) {
			return (wParam == 0) ? TRUE : FALSE;
		}
	}
	else if (uMsg == WM_NCCALCSIZE) {
		return 0;
	}
	else if (uMsg == WM_NCPAINT) {
		return 0;
	}
	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	 
}
