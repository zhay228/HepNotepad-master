#pragma once 
#include "../DAL/DataInfo.h"
class CFrmInfoQuery :public WindowImplBase
{
public:
	CFrmInfoQuery(); 
	CFrmInfoQuery(int _type);
	~CFrmInfoQuery();
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
protected:
	void OpendWnd(CListContainerElementUI* pElem);
	void LoadData(int type = 0, string key = "");
	void LoadLink(string key = "");

private :
	CListUI * m_pInfoList;
	CEditUI* m_pTxtQuery; 
	CDataInfo* pData;
	CTextUI * m_pTitle;
	int type = 1;
	int refeshCount = 0;
 
};

