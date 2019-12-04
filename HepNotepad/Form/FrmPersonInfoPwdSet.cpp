#include "../stdafx.h"
#include "../DAL/PersonInfo.h"
#include "FrmPersonInfoPwdSet.h"
#include "../Common/des.h"
#include "../DAL/SystemInfo.h"

DUI_BEGIN_MESSAGE_MAP(CFrmPersonInfoPwdSet, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick) 
DUI_END_MESSAGE_MAP()

extern HWND m_MainHwnd;
extern UserInfo* userInfo;
CFrmPersonInfoPwdSet::CFrmPersonInfoPwdSet()
{

}
 

CFrmPersonInfoPwdSet::~CFrmPersonInfoPwdSet()
{
}

void CFrmPersonInfoPwdSet::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	DestroyWindow(hWnd);
}


CDuiString CFrmPersonInfoPwdSet::GetSkinFile()
{
	return _T("personInfoPwdSet.xml");
}

LPCTSTR CFrmPersonInfoPwdSet::GetWindowClassName(void) const
{
	return _T("HepNotepad.personInfoAdd");
}


void CFrmPersonInfoPwdSet::InitWindow()
{
	m_pOldPwd = static_cast<CEditUI*>(m_pm.FindControl("txtOldPwd"));
	m_pPwd = static_cast<CEditUI*>(m_pm.FindControl("txtNewPwd"));
	m_pPwd2 = static_cast<CEditUI*>(m_pm.FindControl("txtPwd2"));
}

LRESULT CFrmPersonInfoPwdSet::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE; 
		return 0;
	}
	
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmPersonInfoPwdSet::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmPersonInfoPwdSet::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CFrmPersonInfoPwdSet::OnClick(TNotifyUI &msg)
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
	else if (_tcsicmp(itemName, _T("btnPersonInfoAdd")) == 0) {

		if (m_pOldPwd->GetText().GetLength() < 1){
			ShowMessage("请输入旧密码");
			return;
		}
		CSystemInfo * system = new CSystemInfo;	 
		vector<DataInfo*> list = system->GetList(userInfo->pwdCol);
		string pwd = "";
		for (int i = 0; i < list.size(); i++){
			pwd = list[i]->value;
		}
		string txtpwd = m_pOldPwd->GetText().GetData();
		pwd = Decrypt(pwd);
		if (txtpwd != pwd){
			ShowMessage("旧密码不正确");
			delete system;
			return;
		}
		if (m_pPwd->GetText().GetLength() < 1){
			ShowMessage("请输入密码");
			delete system;
			return;
		}
		string newpwd = m_pPwd->GetText().GetData();
		string pwd2 = m_pPwd2->GetText().GetData();
		if (newpwd != pwd2){
			ShowMessage("两次密码不一致");
			delete system;
			return;
		} 		 
		if (system->UpdateValue(userInfo->pwdCol, Encrypt(newpwd))){
			ShowMessage("设置成功");
			Close(1);
		}
		
	}
	else if (_tcsicmp(itemName, _T("btnPersonInfoCancel")) == 0) {
		Close(1); // 因为activex的原因，使用close可能会出现错误
	} 
}
 