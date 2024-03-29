#include "../stdafx.h"
#include "FrmInfoAdd.h"
#include "../DAL/DataInfo.h"

DUI_BEGIN_MESSAGE_MAP(CFrmInfoAdd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) 
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_END_MESSAGE_MAP()
 
extern HWND m_MainHwnd;

CFrmInfoAdd::CFrmInfoAdd()
{
	 
}

CFrmInfoAdd::CFrmInfoAdd(DataInfo* _dataInfo){
	m_pDataInfo = _dataInfo;
}

CFrmInfoAdd::~CFrmInfoAdd()
{ 
}

void CFrmInfoAdd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd); 
	delete this;
}

 
CDuiString CFrmInfoAdd::GetSkinFile()
{
	return _T("infoAdd.xml");
}

LPCTSTR CFrmInfoAdd::GetWindowClassName(void) const
{
	return _T("HepNotepad.infoAdd");
}

void CFrmInfoAdd::InitWindow()
{
	m_pTitle = static_cast<CEditUI*>(m_pm.FindControl("txtTitle"));
	m_pTxtType = static_cast<CEditUI*>(m_pm.FindControl("txtType"));
	m_pContent = static_cast<CRichEditUI*>(m_pm.FindControl("txtContent"));
	m_pCbxType = static_cast<CComboUI*>(m_pm.FindControl("cbxType"));
	m_pBtnAdd = static_cast<CButtonUI*>(m_pm.FindControl("btnAdd"));

	CDataInfo* pData = new CDataInfo;
	vector<string> dataTypeList = pData->GetTypeList();
	m_pCbxType->RemoveAll();
	for (int i = 0; i < dataTypeList.size(); i++) {
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetFixedHeight(23);
		pLabel->SetText(dataTypeList[i].c_str());
		m_pCbxType->Add(pLabel);
	}
	delete pData;

	if (m_pDataInfo != NULL || m_pDataInfo != nullptr){
		m_pBtnAdd->SetText("更新");
		m_pTitle->SetText(m_pDataInfo->title.c_str());
		m_pTxtType->SetText(m_pDataInfo->typeName.c_str());
		m_pContent->SetText(m_pDataInfo->content.c_str());
		m_pContent->SetFocus();
	}
	else{
		m_pTitle->SetFocus();
	}
}
 
LRESULT CFrmInfoAdd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmInfoAdd::Notify(TNotifyUI &msg)
{ 
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmInfoAdd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	 
	switch (uMsg)
	{
	 
	 
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFrmInfoAdd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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


void CFrmInfoAdd::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
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
		else if (_tcsicmp(itemName, _T("btnAdd")) == 0) {
			if (m_pDataInfo == NULL || m_pDataInfo == nullptr)
				m_pDataInfo = new DataInfo;
			
			if (m_pTitle->GetText().GetLength()<1){
				ShowMessage("请输入标题");
				return;
			}
			if (m_pTxtType->GetText().GetLength()<1){
				ShowMessage("请输入类型");
				return;
			}
			
			if (m_pContent->GetText().GetLength()<1){
				ShowMessage("请输入内容");
				return;
			}
			m_pDataInfo->content = m_pContent->GetText().GetData();
			m_pDataInfo->title = m_pTitle->GetText().GetData();
			m_pDataInfo->typeName = m_pTxtType->GetText().GetData();
			m_pDataInfo->createTime = GetTimeInfo();
			CDataInfo* pData = new CDataInfo;
			if (m_pDataInfo->id == ""){
				if (pData->Add(m_pDataInfo)){
					ShowMessage("添加成功");
					::SendMessage(m_MainHwnd, HN_MAINLOADINFO, (WPARAM)0, (LPARAM)0);					
					delete m_pDataInfo;
					Close(1);
				}
				else{
					ShowMessage("添加失败");
				}
			}
			else{
				if (pData->Update(m_pDataInfo)){
					ShowMessage("更新成功");
					::SendMessage(m_MainHwnd, HN_MAINLOADINFO, (WPARAM)0, (LPARAM)0);
				 
					Close(1);
				}
				else{
					ShowMessage("更新失败");
				}
			} 
			delete pData;
		}
		else if (_tcsicmp(itemName, _T("btnCancel")) == 0) {
			Close(1); // 因为activex的原因，使用close可能会出现错误
		}
	}
}

void CFrmInfoAdd::OnItemSelect(TNotifyUI &msg)
{
	if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("cbxType"))
		{ 
			m_pTxtType->SetText(m_pCbxType->GetText());
		}
	}
}