#pragma once
class CFrmPersonInfoPwdSet :public WindowImplBase
{
public:
	CFrmPersonInfoPwdSet(); 
	~CFrmPersonInfoPwdSet();
	virtual void OnFinalMessage(HWND); 
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	 
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg); 

private :

	CEditUI* m_pOldPwd;
	CEditUI* m_pPwd;
	CEditUI* m_pPwd2;  
};

