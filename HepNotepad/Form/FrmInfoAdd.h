#pragma once
class CFrmInfoAdd :public WindowImplBase
{
public:
	CFrmInfoAdd();
	CFrmInfoAdd(DataInfo* _dataInfo);
	~CFrmInfoAdd();
	virtual void OnFinalMessage(HWND); 
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	 

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnItemSelect(TNotifyUI &msg);
private :
	CEditUI* m_pTxtType;
	CEditUI* m_pTitle;
	CComboUI * m_pCbxType;
	CRichEditUI* m_pContent;
	DataInfo* m_pDataInfo = nullptr;
	CButtonUI * m_pBtnAdd ;
};

