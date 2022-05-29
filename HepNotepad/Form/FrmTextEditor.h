#pragma once
class CFrmTextEditor :public WindowImplBase
{
public:
	CFrmTextEditor();
	CFrmTextEditor(DataType _dataType,string _id, string _dataInfo, string _title="");
	~CFrmTextEditor();
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


private:
	void InitEditor();
	sptr_t SendEditor(unsigned int iMessage, uptr_t wParam = 0, sptr_t lParam = 0){
		return m_fnDirect(m_ptrDirect, iMessage, wParam, lParam);
	}


private:
	SciFnDirect m_fnDirect; 
	sptr_t m_ptrDirect;
	DataType dataType;
	string  id,title,dataInfo;
	CTextUI * m_pTitle;
	
	CButtonUI* m_pBtnPageOne;
	CButtonUI* m_pBtnPageTwo;
	CButtonUI* m_pBtnPageThree;
	CButtonUI* m_pBtnPageFour;
	CHorizontalLayoutUI * m_pLayoutPage;
	CHorizontalLayoutUI * m_pEditorContainer;	
	HWND hwndEditor;
	bool init = false;
};

