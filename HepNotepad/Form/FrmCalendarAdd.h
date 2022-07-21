#pragma once
class CFrmCalendarAdd :public WindowImplBase
{
public:
	CFrmCalendarAdd();
	CFrmCalendarAdd(DataInfo* _dataInfo);
	~CFrmCalendarAdd();
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
	virtual void OnItemSelect(TNotifyUI &msg); 

private :
	CCheckBoxUI* m_pStatus;
	CComboUI * m_pCbxType;
	CComboUI * m_pWeekType;
	CRichEditUI* m_pContent; 
	DataInfo* m_pDataInfo = nullptr;
	CButtonUI * m_pBtnAdd ; 
	CDateTimeUI * m_pRemindDate;
	CComboUI * m_pCbxHour;
	CComboUI * m_pCbxMin;

};

