#include "../stdafx.h"
#include "../DAL/Calendar.h"
#include "FrmCalendarAdd.h"
#include "../Common/des.h"

DUI_BEGIN_MESSAGE_MAP(CFrmCalendarAdd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_END_MESSAGE_MAP()

extern HWND m_MainHwnd;

CFrmCalendarAdd::CFrmCalendarAdd()
{

}

CFrmCalendarAdd::CFrmCalendarAdd(DataInfo* _dataInfo){
	m_pDataInfo = _dataInfo;
}

CFrmCalendarAdd::~CFrmCalendarAdd()
{

}

void CFrmCalendarAdd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	DestroyWindow(hWnd);
}


CDuiString CFrmCalendarAdd::GetSkinFile()
{
	return _T("calendarInfoAdd.xml");
}

LPCTSTR CFrmCalendarAdd::GetWindowClassName(void) const
{
	return _T("HepNotepad.personInfoAdd");
}


void CFrmCalendarAdd::InitWindow()
{
	m_pStatus = static_cast<CCheckBoxUI*>(m_pm.FindControl("chkStatus"));
	m_pContent = static_cast<CRichEditUI*>(m_pm.FindControl("txtContent"));
	m_pCbxType = static_cast<CComboUI*>(m_pm.FindControl("cbxType"));
	m_pRemindDate =  static_cast<CDateTimeUI*>(m_pm.FindControl("dtRemindDate"));
	m_pWeekType = static_cast<CComboUI*>(m_pm.FindControl("cbxRemindDate"));
	m_pCbxHour = static_cast<CComboUI*>(m_pm.FindControl("cbxHour")); 
	m_pCbxMin = static_cast<CComboUI*>(m_pm.FindControl("cbxMin"));
	m_pBtnAdd = static_cast<CButtonUI*>(m_pm.FindControl("btnAdd")); 
	string text = "", hour="" ,min="";
	if (m_pDataInfo != NULL || m_pDataInfo != nullptr) {
		m_pBtnAdd->SetText("更新");
		if (m_pDataInfo->status == 1)
			m_pStatus->Selected(true,true);
		else
			m_pStatus->Selected(false, true);
		m_pCbxType->SelectItem(m_pDataInfo->remindType, false);
		if (m_pDataInfo->remindType == 0) {
			m_pRemindDate->SetText(m_pDataInfo->remindDate.c_str());
		}
		else if (m_pDataInfo->remindType == 1) {
			m_pWeekType->SelectItem(m_pDataInfo->remindDateIndex, false);

		}
		string time = m_pDataInfo->remindTime;
		int pos = time.find(":");
		hour = time.substr(0, pos);
		min = time.substr(pos + 1);
		m_pCbxHour->SetText(hour.c_str());
		m_pCbxMin->SetText(min.c_str());
		m_pContent->SetText(m_pDataInfo->content.c_str());
		m_pContent->SetFocus(); 
	}
	 
	for (int i = 0; i < 24; i++) {
	
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		if (i < 10) { 
			text = "0" + CDataTypeTool::intTostring(i);
		}
		else {
			text = CDataTypeTool::intTostring(i);
		}
		
		pLabel->SetFixedHeight(23);
		pLabel->SetText(text.c_str());
		m_pCbxHour->Add(pLabel);
		if (i == 0)
			pLabel->Select(true);
		if (text == hour) {
			pLabel->Select(true);
		} 
	}
	 
	for (int i = 0; i < 60; i+=5) {
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		if (i < 10) {
			text = "0" + CDataTypeTool::intTostring(i);
		}
		else {
			text = CDataTypeTool::intTostring(i);
		}
		
		pLabel->SetFixedHeight(23);
		pLabel->SetText(text.c_str());
		m_pCbxMin->Add(pLabel);  
		if (i == 0)
			pLabel->Select(true);
		if (text == min) {
			pLabel->Select(true);
		}
	} 
}

LRESULT CFrmCalendarAdd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		
		Close();
		bHandled = TRUE;
		return 0;
	}
	DWORD   dwMsgSn = 0;
	dwMsgSn = wParam & 0xfff0;
	switch (dwMsgSn)
	{
		/*case SC_MAXIMIZE: /// not allow maximize, because after maximize, ui layout is wrong

			bHandled = TRUE;
			return S_OK;
			break;*/
	default:
		break;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmCalendarAdd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmCalendarAdd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	 
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CFrmCalendarAdd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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

void CFrmCalendarAdd::OnClick(TNotifyUI &msg)
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
	else if (_tcsicmp(itemName, _T("btnAdd")) == 0) {
		if (m_pDataInfo == NULL || m_pDataInfo == nullptr)
			m_pDataInfo = new DataInfo;
		 
		if (m_pContent->GetText().GetLength()<1) {
			ShowMessage("请输入内容");
			return;
		}
		if (m_pStatus->IsSelected())
			m_pDataInfo->status = 1;
		else 
			m_pDataInfo->status = 0;
		m_pDataInfo->content = m_pContent->GetText().GetData();
		m_pDataInfo->remindType = m_pCbxType->GetCurSel();
		if (m_pDataInfo->remindType==0) {
			m_pDataInfo->remindDate = m_pRemindDate->GetText(); 
		}
		else if (m_pDataInfo->remindType == 1) {
			m_pDataInfo->remindDate = CDataTypeTool::intTostring(m_pWeekType->GetCurSel());
		}
		else if (m_pDataInfo->remindType == 2) {
			m_pDataInfo->remindDate = "everyDay";
		}
		
		string hour = m_pCbxHour->GetText().GetData();
		string min = m_pCbxMin->GetText().GetData();
		m_pDataInfo->remindTime = hour + ":" + min; 
		m_pDataInfo->createTime = GetTimeInfo(); 
		CCalendar* pOperation = new CCalendar;
		if (m_pDataInfo->id == ""){
			if (pOperation->Add(m_pDataInfo)){
				ShowMessage("添加成功");
				delete m_pDataInfo; 
				Close(1);
			}
			else{
				ShowMessage("添加失败");
			}
		}
		else{ 
			if (pOperation->Update(m_pDataInfo)){
				ShowMessage("更新成功");  
				Close(1);
			}
			else{
				ShowMessage("更新失败");
			}
		}
		delete pOperation;
		::SendMessage(m_MainHwnd, HN_MAINLOADCALENDARDATA, (WPARAM)0, (LPARAM)0);

	}
	else if (_tcsicmp(itemName, _T("btnCancel")) == 0) {
		Close(1); // 因为activex的原因，使用close可能会出现错误
	}	
}

void CFrmCalendarAdd::OnItemSelect(TNotifyUI &msg)
{
	if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("cbxType"))
		{
			int index = m_pCbxType->GetCurSel();
			if(0 == index){
				m_pRemindDate->SetEnabled(true);
				m_pRemindDate->SetVisible(true);
				m_pWeekType->SetVisible(false);
			}
			else if (1 == index) {
				m_pRemindDate->SetVisible(false);
				m_pWeekType->SetVisible(true);
			}
			else if (2 == index) {
				m_pRemindDate->SetVisible(true);
				m_pRemindDate->SetText("");
				m_pRemindDate->SetEnabled(false);
				m_pWeekType->SetVisible(false);
			}
		 
		}
	}
}