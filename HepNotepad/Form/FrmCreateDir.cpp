#include "../stdafx.h"
#include "../DAL/PersonInfo.h"
#include "../Common/des.h"
#include "../DAL/SystemInfo.h"
#include "../Form/FrmTipInfo.h"
#include "FrmCreateDir.h"
#include "../Common/FileHelper.h"
#include "../FileDialogEx.h"

DUI_BEGIN_MESSAGE_MAP(CFrmCreateDir, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

extern HWND m_MainHwnd;

CFrmCreateDir::CFrmCreateDir()
{

}


CFrmCreateDir::CFrmCreateDir(int _type)
{
	type = _type;
}

CFrmCreateDir::~CFrmCreateDir()
{

}

void CFrmCreateDir::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


CDuiString CFrmCreateDir::GetSkinFile()
{
	return _T("createDir.xml");
}

LPCTSTR CFrmCreateDir::GetWindowClassName(void) const
{
	return _T("HepNotepad.FrmInfoQuery");
}


void CFrmCreateDir::InitWindow()
{
	m_pTitle = static_cast<CTextUI*>(m_pm.FindControl("txtTitle"));
	m_pTxtPath = static_cast<CEditUI*>(m_pm.FindControl("txtPath"));
	m_pRootPath = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("comRootPath")));
	m_pBtnOk = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnOK")));
	CHAR szLogicDriveStrings[1024];
	PCHAR szDrive;

	ZeroMemory(szLogicDriveStrings, 1024);

	GetLogicalDriveStrings(1023, szLogicDriveStrings);
	szDrive = (PCHAR)szLogicDriveStrings;
	vector<char*> list;
	do
	{
		list.push_back(szDrive);
		szDrive += (lstrlen(szDrive) + 1);
	} while (*szDrive != '\x00');
	for (int i = 0; i < list.size(); i++) {
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetFixedHeight(23);
		pLabel->SetText(list[i]);
		m_pRootPath->Add(pLabel);
	}
	CListLabelElementUI* pLabel;
	if (m_pRootPath->GetCount() > 1) {
		pLabel = (CListLabelElementUI*)m_pRootPath->GetItemAt(1);
	}
	else {
		pLabel = (CListLabelElementUI*)m_pRootPath->GetItemAt(0);
	}
	pLabel->Select(true);

	if (type == 2) {
		m_pBtnOk->SetText("选择");
		m_pTitle->SetText("复制文件");
		m_pTxtPath->SetText("WebRoot");
	}

}

LRESULT CFrmCreateDir::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE;
		return 0;
	}

	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmCreateDir::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}


LRESULT CFrmCreateDir::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//switch (uMsg)
	//{
	//case WM_CHAR: {//快捷键捕获

	//	switch (wParam)
	//	{
	//	case 0x1B: //Esc
	//	{
	//		Close(1); // 因为activex的原因，使用close可能会出现错误
	//	}
	//	break;
	//	}
	//}
	//			  break;

	//default:
	//	return __super::HandleMessage(uMsg, wParam, lParam);
	//}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFrmCreateDir::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	switch (uMsg)
	{
	case WM_KEYDOWN: {
		if (wParam == VK_ESCAPE) //快捷键捕获 
		{
			Close(0);
		}
	}
	break;
	default:
		return __super::MessageHandler(uMsg, wParam, lParam, bHandled);

	}
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void CFrmCreateDir::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (_tcsicmp(itemName, _T("closebtn")) == 0) {

		Close(1); // 因为activex的原因，使用close可能会出现错误
	}
	else if (_tcsicmp(itemName, _T("btnOK")) == 0) {
		__try {
			if (type == 1) {
				if (m_pTxtPath->GetText().GetLength() > 0) {
					CDuiString pathStr = m_pTxtPath->GetText();
					if (pathStr.Left(1) == "\\") {
						pathStr = pathStr.Mid(1, pathStr.GetLength() - 1);
					}
					if (pathStr.Right(1) != "\\")
						pathStr.Append("\\");
					char path[1024];
					sprintf(path, "%s%s", m_pRootPath->GetText().GetData(), pathStr.GetData());
					CFileHelper::CreateDir(path);
					ShowMessage("创建成功");
				}
			}
			else
			{
				LPCTSTR lpszFilter = "All(*.*)\0*.*;\0\0";
				CFileDialogEx fileDlg;
				fileDlg.SetFilter(lpszFilter);
				fileDlg.SetParentWnd(m_hWnd);
				fileDlg.SetMultiSel(true);
				if (fileDlg.ShowOpenFileDlg())
				{
					string file = fileDlg.GetPathName().c_str();
					POSITION pos = fileDlg.GetStartPosition();
					vector<CDuiString> fileList;

					while (pos != NULL)
					{
						fileList.push_back(fileDlg.GetNextPathName(pos).c_str());
					} 
					CDuiString pathStr = m_pTxtPath->GetText();
					if (pathStr.Left(1) == "\\") {
						pathStr = pathStr.Mid(1, pathStr.GetLength() - 1);
					} 
					char path[1024];
					sprintf(path, "%s%s\\", m_pRootPath->GetText().GetData(), pathStr.GetData());

					string newPath = path;
					if (fileList.size() > 0) {
						CDuiString file = fileList[0];
						vector<CDuiString> pathList = StrSplit(file, "\\");
						for (int i = 1; i < pathList.size() - 1; i++) {
							newPath += pathList[i].GetData();
							newPath += "\\";
							if (pathList[i].Compare(pathStr) == 0) {
								newPath = path;
							}
						}
					}
					sprintf(path, "%s", newPath);
					CFileHelper::CreateDir(path);
					string fileName = "";
					for (int i = 0; i < fileList.size(); i++) {
						int pos = fileList[i].ReverseFind('\\');
						fileName = fileList[i].Mid(pos + 1, fileList[i].GetLength() - pos - 1);
						CopyFile(fileList[i].GetData(), (newPath + fileName).c_str(), false);
					}
					sprintf(path, "复制%d个文件成功", fileList.size());
					ShowMessage(path);
				}
			}
		}
		__except (exception_filter(GetExceptionInformation())) {
			CHAR buffer[1024];
			sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
			CLog::WriteToLog(buffer);
		}
	}
}

