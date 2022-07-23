#include "../stdafx.h"
#include "../DAL/PersonInfo.h"
#include "../Common/des.h"
#include "../DAL/SystemInfo.h"
#include "FrmInfoQuery.h"
#include "../Form/FrmTipInfo.h"
#include "../Common/FileHelper.h"

DUI_BEGIN_MESSAGE_MAP(CFrmInfoQuery, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

extern HWND m_MainHwnd;

CFrmInfoQuery::CFrmInfoQuery()
{

}

CFrmInfoQuery::CFrmInfoQuery(int _type)
{
	type = _type;
}


CFrmInfoQuery::~CFrmInfoQuery()
{
	delete pData;
	DestroyWindow(GetHWND());
}

void CFrmInfoQuery::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


CDuiString CFrmInfoQuery::GetSkinFile()
{
	return _T("query.xml");
}

LPCTSTR CFrmInfoQuery::GetWindowClassName(void) const
{
	return _T("HepNotepad.FrmInfoQuery");
}

void CFrmInfoQuery::InitWindow()
{
	m_pTxtQuery = static_cast<CEditUI*>(m_pm.FindControl("txtQuery"));
	m_pInfoList = static_cast<CListUI*>(m_pm.FindControl("InfoList"));
	m_pTitle = static_cast<CTextUI*>(m_pm.FindControl("txtTitle"));
	pData = new CDataInfo;
	if (type == 2) {
		m_pTitle->SetText("程序/文档");
		LoadLink("");
	}
	else {
		m_pTitle->SetText("便签/知识点");
		LoadData(1, "");
	}
	m_pTxtQuery->SetFocus();
}

LRESULT CFrmInfoQuery::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE;
		return 0;
	}

	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmInfoQuery::LoadData(int type, string key) {
	__try {
		vector<DataInfo*> list;
		list = pData->Query(key, type);
		m_pInfoList->RemoveAll();
		for (int i = 0; i < list.size(); i++) {
			CListContainerElementUI* pElem = nullptr;
			CDialogBuilder builder;
			pElem = static_cast<CListContainerElementUI*>(builder.Create("queryItem.xml"));
			pElem->SetName("queryItem");
			CTextUI* pInfo = static_cast<CTextUI*>(pElem->FindSubControl("txtInfo"));
			if (list[i]->dataType == DataType::info) {
				pInfo->SetText(list[i]->title.c_str());
				pInfo->SetAttribute("type", "info");
			}
			else if (list[i]->dataType == DataType::tip) {

				CDuiString str = list[i]->content.c_str();
				vector<CDuiString>  listStr = StrSplit(str, "\r");
				if (listStr.size() > 0)
					pInfo->SetText(listStr[0].GetData());
				else
					pInfo->SetText(str.GetData());

				pInfo->SetAttribute("type", "tip");
			}
			pInfo->SetAttribute("id", list[i]->id.c_str());
			pInfo->SetAttribute("content", list[i]->content.c_str());
			m_pInfoList->Add(pElem);
			delete list[i];
		}
		if (m_pInfoList->GetCount() > 0) {
			CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(0);
			pElem->Select(true);
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CFrmInfoQuery::LoadLink(string key) {
	__try {
		m_pInfoList->RemoveAll();
		string path = CPaintManagerUI::GetInstancePath().GetData();
		path += "shortcut\\";
		vector<string> typeList1 = { ".docx", ".doc", ".xlsx", ".xls", ".txt", ".pdf",".lnk" };
		vector<string> dList = CFileHelper::GetFiles(path, typeList1, 2); 
		for (int i = 0; i < dList.size(); i++) {
			if (dList[i].find(key) != string::npos) {
				CDialogBuilder builder;
				CListContainerElementUI* pElem = nullptr;
				pElem = static_cast<CListContainerElementUI*>(builder.Create("queryItem.xml"));
				pElem->SetName("queryItem");
				CTextUI* pInfo = static_cast<CTextUI*>(pElem->FindSubControl("txtInfo"));
				int nPos = dList[i].rfind(".");
				string	fileName = dList[i].substr(0, nPos);
				/*nPos = fileName.rfind(".");
				fileName = fileName.substr(0, nPos);*/
				pInfo->SetText(fileName.c_str());

				pElem->SetAttribute("filePath", (path + dList[i]).c_str());
				m_pInfoList->Add(pElem);
			}
		}

		vector<string> typeList = { ".exe", ".udl" };
		path = CPaintManagerUI::GetInstancePath();
		path += "tool\\";
		vector<string> toolList = CFileHelper::GetFiles(path, typeList, 2); 
		for (int i = 0; i < toolList.size(); i++) {
			if (toolList[i].find(key) != string::npos) {
				if (m_pInfoList->GetCount() < 15) {
					CDialogBuilder builder;
					CListContainerElementUI* pElem = nullptr;
					pElem = static_cast<CListContainerElementUI*>(builder.Create("queryItem.xml"));
					pElem->SetName("queryItem");
					CTextUI* pInfo = static_cast<CTextUI*>(pElem->FindSubControl("txtInfo"));
					pInfo->SetText(toolList[i].c_str());
					pElem->SetAttribute("filePath", (path + toolList[i]).c_str());
					m_pInfoList->Add(pElem);
				} 
			}
		}
		path = CPaintManagerUI::GetInstancePath();
		path += "CustomTool\\";
		vector<string> cusList = CFileHelper::GetFiles(path, typeList, 2);
		for (int i = 0; i < cusList.size(); i++) {
			if (cusList[i].find(key) != string::npos) {
				if (m_pInfoList->GetCount() < 15) {
					CDialogBuilder builder;
					CListContainerElementUI* pElem = nullptr;
					pElem = static_cast<CListContainerElementUI*>(builder.Create("queryItem.xml"));
					pElem->SetName("queryItem");
					CTextUI* pInfo = static_cast<CTextUI*>(pElem->FindSubControl("txtInfo"));
					pInfo->SetText(cusList[i].c_str());
					pElem->SetAttribute("filePath", (path + cusList[i]).c_str());
					m_pInfoList->Add(pElem);
				}
			}
		}
		if (m_pInfoList->GetCount() > 0) {
			CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(0);
			pElem->Select(true);
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

void CFrmInfoQuery::Notify(TNotifyUI &msg)
{
	__try {
		CDuiString itemName = msg.pSender->GetName();
		if (itemName.IsEmpty() == false && itemName == _T("txtQuery") && msg.sType == _T("return"))
		{
			if (m_pInfoList->GetCount() > 0) {
				CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(0);
				if (type == 1) {
					OpendWnd(pElem);
				}
				else {
					string filePath = pElem->GetCustomAttribute("filePath");
					if (CFileHelper::IsFileExist(filePath.c_str()))
						ShellExecute(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
				}
				Close(1);
			}
		}
		else if (_tcsicmp(msg.sType, _T("textchanged")) == 0) {
			if (_tcsicmp(itemName, _T("txtQuery")) == 0) {				
				string key = m_pTxtQuery->GetText().GetData();
				if (type == 1) {
					LoadData(0, key);
				}
				else {
					LoadLink(key);
				}				
			}
		}
		else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)//双击
		{
			if (_tcsicmp(msg.pSender->GetClass(), DUI_CTR_LISTCONTAINERELEMENT) == 0)
			{
				CDuiString itemName = msg.pSender->GetName();
				if (itemName.IsEmpty() == false && _tcsicmp(itemName, _T("queryItem")) == 0)
				{
					CListContainerElementUI* pElem = (CListContainerElementUI*)msg.pSender;
					if (type == 1) {
						OpendWnd(pElem);
					}
					else {
						string filePath = pElem->GetCustomAttribute("filePath");
						if (CFileHelper::IsFileExist(filePath.c_str()))
							ShellExecute(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
					}
					Close(1);
				}
			}
		}
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
	return WindowImplBase::Notify(msg);
}

void CFrmInfoQuery::OpendWnd(CListContainerElementUI* pElem) {
	__try {
		CTextUI* pInfo = static_cast<CTextUI*>(pElem->FindSubControl("txtInfo"));
		DataType dType = DataType::info;
		string title = "信息";
		string type = pInfo->GetCustomAttribute("type");
		if (type == "tip") {
			title = "便签";
			dType = DataType::tip;
		}
		string content = pInfo->GetCustomAttribute("content");
		string id = pInfo->GetCustomAttribute("id");

		CDataInfo *pOperation = new CDataInfo;
		pOperation->UpdateCount(id, dType);
		delete pOperation;
		int  cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
		CFrmTipInfo* m_pTempTip = new CFrmTipInfo(dType, id, content, title); 
		m_pTempTip->Create(NULL, _T("信息内容"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE, cx - 375, cy - 232);
		m_pTempTip->ResizeClient(360, 240);
	    m_pTempTip->InitData();
		::SetWindowPos(m_pTempTip->GetHWND(), HWND_TOPMOST, cx - 361, cy - 218, 360, 240, SWP_SHOWWINDOW);
		::SetActiveWindow(m_pTempTip->GetHWND());
		::ShowWindow(m_pTempTip->GetHWND(), SW_SHOW);
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}

LRESULT CFrmInfoQuery::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYUP: {
		__try {
			if (GetKeyState(VK_UP) < 0) {
				int index = m_pInfoList->GetCurSel();
				if (index == 0) {
					CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(m_pInfoList->GetCount() - 1);
					pElem->Select(true);
				}
				return false;
			}
			else if (GetKeyState(VK_DOWN) < 0) {

				int index = m_pInfoList->GetCurSel();
				if (index == (m_pInfoList->GetCount() - 1)) {
					CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(0);
					pElem->Select(true);
				}
				return false;

			}
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
				   break;

	
	case WM_PAINT: {
		if (refeshCount < 3) {

			if (m_pTxtQuery != NULL)
				m_pTxtQuery->SetFocus();
			refeshCount++;
		}
	}
				   break;
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}
 
LRESULT CFrmInfoQuery::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	switch (uMsg)
	{
	case WM_KEYDOWN: {
		if (wParam == VK_ESCAPE) //快捷键捕获 
		{
			Close(0);
		}
		if (GetKeyState(VK_DOWN) < 0) {
			int index = m_pInfoList->GetCurSel();
			if (index < (m_pInfoList->GetCount() - 1)) {
				CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(index+1);
				pElem->Select(true);
			}
			else {
				CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(0);
				pElem->Select(true);
			}
		}
		else if (GetKeyState(VK_UP) < 0) {
			int index = m_pInfoList->GetCurSel();
			if (index > 0) {				
				CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(index - 1);
				pElem->Select(true);
			}
			else {
				CListContainerElementUI* pElem = (CListContainerElementUI*)m_pInfoList->GetItemAt(m_pInfoList->GetCount() - 1);
				pElem->Select(true);
			}
		}
	}
					 break;	 
	default:
		return __super::MessageHandler(uMsg, wParam, lParam, bHandled);

	}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void CFrmInfoQuery::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (_tcsicmp(itemName, _T("closebtn")) == 0) {

		Close(1); // 因为activex的原因，使用close可能会出现错误
	}
	else if (_tcsicmp(itemName, _T("minbtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (_tcsicmp(itemName, _T("maxbtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (_tcsicmp(itemName, _T("restorebtn")) == 0) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
}
