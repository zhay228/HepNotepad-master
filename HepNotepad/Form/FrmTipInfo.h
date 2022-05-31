#pragma once
class CFrmTipInfo :public WindowImplBase
{
public:
	CFrmTipInfo();
	CFrmTipInfo(DataType _dataType, string _title);
	CFrmTipInfo(DataType _dataType,string _id, string _dataInfo, string _title="");
	~CFrmTipInfo();
	virtual void OnFinalMessage(HWND); 
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam); 
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	bool SetContent(string content);
	void AppendContent(string content,int tabIndex = 1);
	void InitData();
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI& msg);


private:
	void CloseWnd();
	void DataDeal();
	void TabRichEdit(int _index =1);
	 
	
private:
	DataType dataType;
	string  id,title,dataInfo;
	CTextUI * m_pTitle;
	CRichEditUI* m_pContent;
	CRichEditUI* m_pContentTwo;
	CRichEditUI* m_pContentTwoDown;
	CRichEditUI* m_pContentThree;
	CRichEditUI* m_pContentThreeDown;
	CRichEditUI* m_pContentFour;
	CButtonUI* m_pBtnPageOne;
	CButtonUI* m_pBtnPageTwo;
	CButtonUI* m_pBtnPageThree;
	CButtonUI* m_pBtnPageFour;
	CHorizontalLayoutUI * m_pLayoutPage;
	CTabLayoutUI* m_pTabContainer;
	int index = 1;
	int refeshCount = 0;

};

