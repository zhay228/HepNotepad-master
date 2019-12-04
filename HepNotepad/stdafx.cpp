// stdafx.cpp : source file that includes just the standard includes
// HepNotepad.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "ShowMessageBox.h"
#include "DAL\SystemInfo.h"
#include "Common\des.h"
extern UserInfo* userInfo;
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

BOOL AddNotificationIcon(HWND hwnd)
{
	NOTIFYICONDATA nid = { sizeof(nid) };
	nid.hWnd = hwnd;
	nid.uID = IDI_SMALL;
	// add the icon, setting the icon, tooltip, and callback message.
	// the icon will be identified with the GUID
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
	nid.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_SMALL)); 
	Shell_NotifyIcon(NIM_ADD, &nid);

	// NOTIFYICON_VERSION_4 is prefered
	return Shell_NotifyIcon(NIM_SETVERSION, &nid);
}
void ShowMessageBox(string msg, string tip){
	MessageBox(NULL, msg.c_str(), tip.c_str(), MB_OK);
}

void ShowMessage(CDuiString info){//普通提示框
	//MessageBox(NULL,info.GetData(),_T("提示"),MB_OK); 
	CShowMessageBox* messagebox = new CShowMessageBox(_T("提示"), info);
	messagebox->Create(NULL, _T("温馨提示"), UI_WNDSTYLE_FRAME, 0L, 0, 0);
	messagebox->CenterWindow();
	messagebox->ShowModal();
}

bool ShowMessageConfirm(HWND hwnd, CDuiString title, CDuiString info){//确认提示框
	//MessageBox(NULL,info.GetData(),_T("提示"),MB_OK); 
	CShowMessageBox* messagebox = new CShowMessageBox(_T("提示"), info, 2);
	messagebox->Create(hwnd, title, UI_WNDSTYLE_FRAME, 0L, 0, 0);
	messagebox->CenterWindow();
	return messagebox->ShowModal() == 1;
}


void ShowQueryResult(LPVOID param){

}

void InitalSystemInfo(){
	CSystemInfo * system = new CSystemInfo;
	vector<DataInfo*> dataList = system->GetList();
	for (int i = 0; i < dataList.size(); i++){
		if (dataList[i]->name == userInfo->keyCol){
			if (dataList[i]->value != "")
				userInfo->key = Decrypt(dataList[i]->value);
		}
		else if (dataList[i]->name == userInfo->pwdCol){
			userInfo->pwd = dataList[i]->value;
		}
		else if (dataList[i]->name == userInfo->saveTempTipCol) {
			userInfo->saveTempTip = dataList[i]->value == "1" ;
		}
		else if (dataList[i]->name == userInfo->personDataEnableCol) {
			userInfo->personDataEnable = dataList[i]->value == "1" ;
		}
		else if (dataList[i]->name == userInfo->WeekTimeCol) {
			userInfo->WeekEnable=dataList[i]->value == "1" ;
			userInfo->WeekTime = dataList[i]->content;
		}
	}
	if (userInfo->key == ""){
		string guid = GenerateGuid();
		userInfo->key = guid;
		system->UpdateValue(userInfo->keyCol,Encrypt(guid));
	}
	delete system;
	/*userInfo->personDataEnable = true;*/
}

