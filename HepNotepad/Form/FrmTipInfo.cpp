#include "../stdafx.h"
#include "FrmTipInfo.h"
#include "../DAL/TipInfo.h"
#include "../DAL//DataInfo.h"
#include "../DAL/Weekly.h"

DUI_BEGIN_MESSAGE_MAP(CFrmTipInfo, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()
extern UserInfo* userInfo;
extern HWND m_MainHwnd;

CFrmTipInfo::CFrmTipInfo()
{
}

CFrmTipInfo::CFrmTipInfo(DataType _dataType, string _title) {

	dataType = _dataType;
	title = _title;
}

CFrmTipInfo::CFrmTipInfo(DataType _dataType, string _id, string _dataInfo, string _title) {
	dataType = _dataType;
	id = _id;
	title = _title;
	dataInfo = _dataInfo;
}

CFrmTipInfo::~CFrmTipInfo()
{
	DestroyWindow(GetHWND());
}

void CFrmTipInfo::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CDuiString CFrmTipInfo::GetSkinFile()
{
	return _T("tipInfo.xml");
}

LPCTSTR CFrmTipInfo::GetWindowClassName(void) const
{
	return _T("HepNotepad.tipInfo");
}

void CFrmTipInfo::InitWindow()
{
	__try {
		m_pTitle = static_cast<CTextUI*>(m_pm.FindControl(_T("txtTitle")));
		m_pContent = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContent")));
		m_pContentTwo = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContentTwo")));
		m_pContentTwoDown = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContentTwoDown")));
		m_pContentThree = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContentThree")));
		m_pContentThreeDown = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContentThreeDown")));
		m_pContentFour = static_cast<CRichEditUI*>(m_pm.FindControl(_T("txtContentFour")));
		m_pBtnPageOne = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageOne")));
		m_pBtnPageTwo = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageTwo")));
		m_pBtnPageThree = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageThree")));
		m_pBtnPageFour = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageFour")));
		m_pLayoutPage = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("pageLayout")));

		m_pTitle->SetText(title.c_str());
		m_pContent->SetText(dataInfo.c_str());
		m_pTabContainer = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tabContainer")));
		m_pTabContainer->SelectItem(0);
		if (dataType == DataType::calendar) {
			m_pContent->SetReadOnly(true);
		}
		else if (dataType == DataType::tempTip) {
			m_pLayoutPage->SetVisible(true);
		}
		else {
			m_pContent->SetFocus();
		}

		SIZE size = m_pContent->GetScrollRange();
		if (size.cy > m_pContent->GetHeight())
			m_pContent->HomeUp();

		index = 1;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CFrmTipInfo::InitData() {
	if (dataType == DataType::tempTip) {
		if (OpenClipboard(GetHWND())) {//如果打开时，临时标签
			if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
			{
				HANDLE hClip;
				char* pBuf;
				//读取数据  
				hClip = GetClipboardData(CF_TEXT);
				pBuf = (char*)GlobalLock(hClip);
				GlobalUnlock(hClip);
				string info = pBuf;
				if (info.length() > 2) {
					int tabIndex = 1;
					if (m_pContent->GetTextLength() < 2) {
						tabIndex = 1;
					}
					/*else if (m_pContentTwo->GetTextLength() < 2) {
						tabIndex = 2;
					}
					else if (m_pContentThree->GetTextLength() < 2) {
						tabIndex = 3;
					}*/
					else if (m_pContentFour->GetTextLength() < 2) {
						tabIndex = 4;
					}
					AppendContent(info, tabIndex);
				}
			}
			CloseClipboard();
		}
	}
}

bool CFrmTipInfo::SetContent(string content) {
	bool show = false;
	if (m_pContentFour->GetText().GetLength() > 1) {
		if (ShowMessageConfirm(GetHWND(), _T("温馨提示"), _T("您确定要覆盖第4面板的数据吗？"))) {
			TabRichEdit(4);
			m_pContentFour->SetText(content.c_str());
			show = true;
		}
	}
	else {
		TabRichEdit(4);
		m_pContentFour->SetText(content.c_str());
		show = true;
	}
	return show;
}

void CFrmTipInfo::AppendContent(string content, int tabIndex) {
	content += "\r";
	if (dataType == DataType::copy) {
		m_pContent->AppendText(content.c_str());
	}
	else {
		if (tabIndex == 1) {
			m_pContent->AppendText(content.c_str());
		}
		else if (tabIndex == 2) {
			m_pContentTwo->AppendText(content.c_str());
		}
		else if (tabIndex == 3) {
			m_pContentThree->AppendText(content.c_str());
		}
		else if (tabIndex == 4) {
			m_pContentFour->AppendText(content.c_str());
		}
		else if (tabIndex == 5) {
			tabIndex = 2;
			m_pContentTwoDown->AppendText(content.c_str());
		}
		else if (tabIndex == 6) {
			tabIndex = 3;
			m_pContentThreeDown->AppendText(content.c_str());
		}

		TabRichEdit(tabIndex);
	}
}

LRESULT CFrmTipInfo::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmTipInfo::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmTipInfo::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CHAR: {//快捷键捕获
		switch (wParam)
		{
		case 0x06: //Ctrl+Enter
		{
			if (((m_pContentTwo != NULL) && m_pContentTwo->IsFocused()) || ((m_pContentTwo != NULL) && m_pContentTwoDown->IsFocused())) {
				CDuiString contentLeft = m_pContentTwo->GetText();
				CDuiString contentRigth = m_pContentTwoDown->GetText();
				if (contentLeft.GetLength() > 36 && contentRigth.GetLength() > 36) {
					vector<CDuiString>  listLeft = StrSplit(contentLeft, "\r");
					vector<CDuiString>  listRight = StrSplit(contentRigth, "\r");
					m_pContentTwo->SetText("");
					m_pContentTwoDown->SetText("");
					bool same = false;
					int n = 0;
					for (vector<CDuiString>::iterator it = listLeft.begin(); it != listLeft.end(); it++) {
						for (vector<CDuiString>::iterator itr = listRight.begin(); itr != listRight.end(); itr++) {
							if (*it == *itr) {
								listLeft.erase(it);
								listRight.erase(itr);
								same = true;
								it--;
								itr--;
								break;
							}
						}
						if (!same) {
							AppendContent(((CDuiString)*it).GetData(), 2);
						}
						same = false;
					}
					for (vector<CDuiString>::iterator itr = listRight.begin(); itr != listRight.end(); itr++) {
						AppendContent(((CDuiString)*itr).GetData(), 5);
					}
				}
			}
			else if ((m_pContentThree != NULL) && m_pContentThree->IsFocused())
			{
				CDuiString content = m_pContentThree->GetText();
				if (content.GetLength() > 300) {
					vector<CDuiString>  listStr = StrSplit(content, "\r");
					TCHAR paramVlaue[MAX_PATH];
					CDuiString configPath = CPaintManagerUI::GetInstancePath();
					configPath.Append(_T("Config.ini"));
					GetPrivateProfileString(_T("SystemInfo"), _T("Filter1"), _T(""), paramVlaue, MAX_PATH, configPath);
					CDuiString filter1 = paramVlaue;
					GetPrivateProfileString(_T("SystemInfo"), _T("Filter2"), _T(""), paramVlaue, MAX_PATH, configPath);
					CDuiString filter2 = paramVlaue;
					GetPrivateProfileString(_T("SystemInfo"), _T("Filter3"), _T(""), paramVlaue, MAX_PATH, configPath);
					CDuiString filter3 = paramVlaue;
					GetPrivateProfileString(_T("SystemInfo"), _T("Filter4"), _T(""), paramVlaue, MAX_PATH, configPath);
					CDuiString filter4 = paramVlaue;
					bool filter = false;
					for (int i = 0; i < listStr.size(); i++) {
						if (listStr[i].Find(filter1) != -1) {
							filter = true;
						}
						else if (listStr[i].Find(filter2) != -1) {
							filter = true;
						}
						else if (listStr[i].Find(filter3) != -1) {
							filter = true;
						}
						else if (listStr[i].Find(filter4) != -1) {
							filter = true;
						}
						if (filter) {
							AppendContent(listStr[i].GetData(), 6);
						}
						filter = false;
					}
				}
			}
		}
		break;

		case 0x0D://Enter
		{

		}
		break;
		default:
			break;
		}
	}
				  break;
	case WM_PAINT: {
		/*if (refeshCount < 3) {
			SetForegroundWindow(GetHWND());
			if (m_pContent != NULL)
				m_pContent->SetFocus();
			refeshCount++;

		}*/
	}
				   break;
	case WM_KEYDOWN: {
		if ((m_pContent != NULL) && m_pContent->IsFocused() && (wParam == 'S') && GetKeyState(VK_CONTROL) < 0)	// 发送消息框的Ctrl+V消息,图片粘贴处理
		{
			DataDeal();
			m_pContent->SetFocus();
		}
		else if ((m_pContent != NULL) && m_pContent->IsFocused() && (wParam == 'T') && GetKeyState(VK_CONTROL) < 0)	// 发送消息框的Ctrl+V消息,图片粘贴处理
		{
			if (dataType != DataType::tempTip) {
				SendMsg * msg = new SendMsg;
				msg->data = m_pContent->GetText().GetData();
				::PostMessage(m_MainHwnd, HN_GETINFOQUERY, (WPARAM)msg, (LPARAM)0);
				CloseWnd();
			}
		}
		else if (GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_TAB) < 0) {
			index++;
			TabRichEdit(index);
		}
		else if (GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_LEFT) < 0) {
			index--;
			TabRichEdit(index);
		}
		else if (GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_RIGHT) < 0) {
			index++;
			TabRichEdit(index);
		}
	}
					 break;
	case WM_KEYUP:
	{

	}
	break;
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFrmTipInfo::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (wParam == VK_ESCAPE) //快捷键捕获 
	{
		CloseWnd();
	}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void CFrmTipInfo::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (_tcsicmp(itemName, _T("closebtn")) == 0) {
			DataDeal();
			CloseWnd();
		}
		else if (_tcsicmp(itemName, _T("minbtn")) == 0) {

			if (dataType == DataType::tempTip || dataType == DataType::copy) {
				CloseWnd();
			}
			else {
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
			}

		}
		else if (_tcsicmp(itemName, _T("maxbtn")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
		}
		else if (_tcsicmp(itemName, _T("restorebtn")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
		}
		else if (_tcsicmp(itemName, _T("btnPageOne")) == 0) {
			TabRichEdit(1);
		}
		else if (_tcsicmp(itemName, _T("btnPageTwo")) == 0) {
			TabRichEdit(2);
		}
		else if (_tcsicmp(itemName, _T("btnPageThree")) == 0) {
			TabRichEdit(3);
		}
		else if (_tcsicmp(itemName, _T("btnPageFour")) == 0) {
			TabRichEdit(4);
		}
	}
	else if (msg.sType == _T("return")) {

	}
}

void CFrmTipInfo::TabRichEdit(int _index) {
	index = _index;
	if (index > 4)index = 1;
	if (index < 1)index = 4;
	if (dataType != DataType::tempTip)return;
	m_pBtnPageOne->SetNormalImage("ico/page1_1.png");
	m_pBtnPageTwo->SetNormalImage("ico/page2_1.png");
	m_pBtnPageThree->SetNormalImage("ico/page3_1.png");
	m_pBtnPageFour->SetNormalImage("ico/page4_1.png");
	m_pTabContainer->SelectItem(index - 1);

	switch (index)
	{
	case 1: {
		m_pBtnPageOne->SetNormalImage("ico/page1.png");
		m_pContent->SetFocus();
	}
			break;
	case 2: {
		m_pBtnPageTwo->SetNormalImage("ico/page2.png");
		m_pContentFour->SetFocus();
	}
			break;
	case 3: {
		m_pBtnPageThree->SetNormalImage("ico/page3.png");
		m_pContentFour->SetFocus();
	}
			break;
	case 4: {
		m_pBtnPageFour->SetNormalImage("ico/page4.png");
		m_pContentFour->SetFocus();
	}
			break;
	default:
		break;
	}
}

void CFrmTipInfo::DataDeal() {
	__try {
		string content = m_pContent->GetText().GetData();
		if (dataInfo == content)return;
		DataInfo * dataInfo = new DataInfo;
		if (dataType == DataType::tip || (dataType == DataType::tempTip && userInfo->saveTempTip)) {
			CTipInfo* pOperation = new CTipInfo;
			dataInfo->content = m_pContent->GetText();
			if (id == "") {
				dataInfo->createTime = GetTimeInfo();
				pOperation->Add(dataInfo);
				id = dataInfo->id;
			}
			else {
				dataInfo->id = id;
				dataInfo->updateTime = GetTimeInfo();
				pOperation->Update(dataInfo);
			}
			delete pOperation;
			::SendMessage(m_MainHwnd, HN_MAINLOADTIP, (WPARAM)0, (LPARAM)0);
		}
		else if (dataType == DataType::info) {
			CDataInfo* pOperation = new CDataInfo;

			dataInfo->content = m_pContent->GetText();
			dataInfo->id = id;
			dataInfo->updateTime = GetTimeInfo();
			pOperation->Update(dataInfo);
			delete pOperation;
			::SendMessage(m_MainHwnd, HN_MAINLOADINFO, (WPARAM)0, (LPARAM)0);
		}
		else if (dataType == DataType::weekly) {
			CWeekly * pOperation = new CWeekly;
			dataInfo->content = m_pContent->GetText();
			if (id == "") {
				dataInfo->createTime = GetTimeInfo();
				pOperation->AddWeekDetail(dataInfo);
				id = dataInfo->id;
			}
			else {
				pOperation->Update(id, dataInfo->content);
			}
			delete pOperation;
			::SendMessage(m_MainHwnd, HN_MAINLOADWEEKDATA, (WPARAM)0, (LPARAM)0);
		}
		content = dataInfo->content;
		delete dataInfo;
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CFrmTipInfo::CloseWnd() {
	if (dataType == DataType::tempTip || dataType == DataType::copy) {
		::ShowWindow(GetHWND(), SW_HIDE);
	}
	else {
		Close(1);
	}
}
