#pragma once
class CShowMessageBox :
	public WindowImplBase
{
public:
	CShowMessageBox();
	~CShowMessageBox();
	CShowMessageBox(CDuiString title, CDuiString message, int _msgType=1);
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();  
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

private:
	//	BOOL IsInStaticControl(CControlUI * pControl);

private:
	CButtonUI* m_pBtnClose, *m_pBtnOK, *m_pBtnCancel;
	CLabelUI* m_pIcon;
	CTextUI*  m_pTitle;
	CTextUI*  m_pMessage;
	CDuiString m_title;
	CDuiString m_message;

	int msgType  ;
};

