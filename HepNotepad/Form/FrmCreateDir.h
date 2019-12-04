#pragma once 
class CFrmCreateDir :public WindowImplBase
{
public:
	CFrmCreateDir(); 
	CFrmCreateDir::CFrmCreateDir(int _type);
	~CFrmCreateDir();
	virtual void OnFinalMessage(HWND); 
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	 

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg); 
protected:
	BOOL GetDriverInfo(LPSTR szDrive);

private :
	CTextUI * m_pTitle;
	CEditUI* m_pTxtPath;
	CComboUI * m_pRootPath;
	CButtonUI * m_pBtnOk;
	double freeSize;
	string driverName;
	int type;
};

