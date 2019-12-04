// HepNotepad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "HepNotepad.h"
#include "MainFrame.h"
#include "FrameWindowWnd.h"
#include "resource.h"

#include "Form\FrmTextEditor.h"


HINSTANCE g_hInst;
HWND m_MainHwnd;
UserInfo* userInfo;
SystemInfo * systemInfo;

void InitResource()
{
	using namespace DuiLib;
	CPaintManagerUI::SetResourceType(DuiLib::UILIB_ZIPRESOURCE);
	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(IDR_ZIPRES1), _T("ZIPRES"));
	if (hResource != NULL) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if (hGlobal != NULL) {
			dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
			if (dwSize > 0) {
				CPaintManagerUI::SetResourceZip(::LockResource(hGlobal), dwSize);
			}
		}
		::FreeResource(hResource);
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
 

	g_hInst = hInstance;
	CPaintManagerUI::SetInstance(hInstance);

	HANDLE handle = ::CreateMutex(NULL, FALSE, (LPCSTR)"hepnotepad");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "程序已经启动", "", MB_OK);
		return FALSE;
	}

	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	//CPaintManagerUI::SetResourceZip("skin.zip");
	InitResource();

	LoadLibrary(_T("SciLexer.dll"));
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	userInfo = new UserInfo;
	systemInfo = new SystemInfo;

	/*CFrmTextEditor* pTest = new CFrmTextEditor();
	if (pTest == NULL) return 0;
	pTest->Create(NULL, _T("测试"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	::ShowWindow(pTest->GetHWND(), SW_SHOW);*/

	InitalSystemInfo();
	CMainFrame* pFrame = new CMainFrame();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("记事管理"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(pFrame->GetHWND(), SW_HIDE);

	CPaintManagerUI::MessageLoop();
	delete userInfo;
	delete systemInfo;
	return 0;
}

 