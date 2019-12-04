#include "../stdafx.h"
#include "FrmTextEditor.h"
#include "../DAL/TipInfo.h"
#include "../DAL//DataInfo.h"
#include "../DAL/Weekly.h"
#include "../minus.xpm"
#include "../plus.xpm"

DUI_BEGIN_MESSAGE_MAP(CFrmTextEditor, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()


#define SCINT_ID 1010
extern HINSTANCE g_hInst;
extern UserInfo* userInfo;
extern HWND m_MainHwnd;

CFrmTextEditor::CFrmTextEditor()
{
	title = "文本编辑器";
}

CFrmTextEditor::CFrmTextEditor(DataType _dataType, string _id, string _dataInfo, string _title)
{
	dataType = _dataType;
	id = _id;
	title = _title;
	dataInfo = _dataInfo;
}

CFrmTextEditor::~CFrmTextEditor()
{
}

void CFrmTextEditor::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	DestroyWindow(hWnd);
}

CDuiString CFrmTextEditor::GetSkinFile()
{
	return _T("textEditor.xml");
}

LPCTSTR CFrmTextEditor::GetWindowClassName(void) const
{
	return _T("HepNotepad.textEditor");
}

void CFrmTextEditor::InitWindow()
{
	__try {
		m_pTitle = static_cast<CTextUI*>(m_pm.FindControl(_T("txtTitle")));

		m_pBtnPageOne = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageOne")));
		m_pBtnPageTwo = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageTwo")));
		m_pBtnPageThree = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageThree")));
		m_pBtnPageFour = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnPageFour")));
		m_pLayoutPage = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("pageLayout")));
		m_pLayoutPage->SetVisible(false);
		m_pEditorContainer = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("editorContainer")));
		m_pTitle->SetText(title.c_str());
		InitEditor();
	}
	__except (exception_filter(GetExceptionInformation())) {
		CHAR buffer[1024];
		sprintf(buffer, "Exception: %s on line %d code:%d\r\n", __FILE__, __LINE__, GetExceptionCode());
		CLog::WriteToLog(buffer);
	}
}
typedef long Colour;
inline Colour ColourRGB(unsigned int red, unsigned int green, unsigned int blue) {
	return red | (green << 8) | (blue << 16);
}

void CFrmTextEditor::InitEditor() {
	hwndEditor = ::CreateWindowEx(0, _T("Scintilla"), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		2, 27, 100, 100, GetHWND(), (HMENU)SCINT_ID, g_hInst, NULL);

	m_fnDirect = (SciFnDirect)::SendMessage(hwndEditor, SCI_GETDIRECTFUNCTION, 0, 0);
	m_ptrDirect = (sptr_t)::SendMessage(hwndEditor, SCI_GETDIRECTPOINTER, 0, 0);
	const char* szKeywords1 =
		"asm auto break case catch class const "
		"const_cast continue default delete do double "
		"dynamic_cast else enum explicit extern false "
		"for friend goto if inline mutable "
		"namespace new operator private protected public "
		"register reinterpret_cast return signed "
		"sizeof static static_cast struct switch template "
		"this throw true try typedef typeid typename "
		"union unsigned using virtual volatile while";
	const char* szKeywords2 =
		"bool char float int long short void wchar_t";
	// 设置全局风格  
	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Courier New");
	SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
	SendEditor(SCI_STYLECLEARALL);
	//C++语法解析  
	SendEditor(SCI_SETLEXER, SCLEX_CPP);
	SendEditor(SCI_SETKEYWORDS, 0, (sptr_t)szKeywords1);//设置关键字  
	SendEditor(SCI_SETKEYWORDS, 1, (sptr_t)szKeywords2);//设置关键字  
														// 下面设置各种语法元素风格  
	SendEditor(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //关键字  
	SendEditor(SCI_STYLESETFORE, SCE_C_WORD2, 0x00800080);   //关键字  
	SendEditor(SCI_STYLESETBOLD, SCE_C_WORD2, TRUE);   //关键字  
	SendEditor(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //字符串  
	SendEditor(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x001515A3); //字符  
	SendEditor(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);//预编译开关  
	SendEditor(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//块注释  
	SendEditor(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//行注释  
	SendEditor(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x00008000);//文档注释（/**开头）  
	SendEditor(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
	SendEditor(SCI_SETCARETLINEBACK, 0xd0efff);

	init = true;

	//// 0号页边，宽度为9，显示0号标记(0..0001B)  
	//SendEditor(SCI_SETMARGINTYPEN, 0, SC_MARGIN_SYMBOL);
	//SendEditor(SCI_SETMARGINWIDTHN, 0, 9);
	//SendEditor(SCI_SETMARGINMASKN, 0, 0x01);
	//// 1号页边，宽度为9，显示1,2号标记(0..0110B)  
	//SendEditor(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	//SendEditor(SCI_SETMARGINWIDTHN, 1, 9);
	//SendEditor(SCI_SETMARGINMASKN, 1, 0x06);
	// 2号页边，宽度为20，显示行号  
	SendEditor(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	SendEditor(SCI_SETMARGINWIDTHN, 0, 20);

	// 设置标记的前景色  
	SendEditor(SCI_MARKERSETFORE, 0, 0xdddddd);//0-红色  
	SendEditor(SCI_MARKERSETFORE, 1, 0x00ff00);//1-绿色  
	SendEditor(SCI_MARKERSETFORE, 2, 0xff0000);//2-蓝色 

	SendEditor(SCI_SETPROPERTY, (sptr_t)"fold", (sptr_t)"1");
	SendEditor(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);//页边类型  
	SendEditor(SCI_SETMARGINMASKN, 1, SC_MASK_FOLDERS); //页边掩码  
	SendEditor(SCI_SETMARGINWIDTHN, 1, 18); //页边宽度  
	SendEditor(SCI_SETMARGINSENSITIVEN, 1, TRUE); //响应鼠标消息  

	// 折叠标签样式  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);

	// 折叠标签颜色  
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);

	// 折叠标签样式  
	//SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PIXMAP);
	//SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_PIXMAP);
	/*SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_PIXMAP);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_PIXMAP);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);*/

	//  
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDER, (sptr_t)plus_xpm);
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPEN, (sptr_t)minus_xpm);
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEREND, (sptr_t)plus_xpm);
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPENMID, (sptr_t)minus_xpm);
	//SendEditor(SCI_SETFOLDFLAGS, 16 | 4, 0); //如果折叠就在折叠行的上下各画一条横线  
}

LRESULT CFrmTextEditor::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		Close();
		bHandled = TRUE;
		return 0;
	}
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

void CFrmTextEditor::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CFrmTextEditor::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE: {
	}
					  break;
	case WM_CHAR: {//快捷键捕获
		switch (wParam)
		{
		case 0x1B: //Esc
		{
			Close(0);
		}
		break;

		}
	}
				  break;

	case WM_SIZE: {
		if (init) {
			RECT rc = { 0 };
			::GetClientRect(GetHWND(), &rc);
			if (rc.right - rc.left > 10) {
				::SendMessage(hwndEditor, SCI_SETSCROLLWIDTH, rc.right - 31 - 38, 5);
				::SetWindowPos(hwndEditor, HWND_TOP, 2, 28, rc.right - 5, rc.bottom - 56, SWP_NOZORDER | SWP_NOACTIVATE);
				::InvalidateRect(hwndEditor, NULL, TRUE);
			}
		}
	}
				  break;
	case WM_NOTIFY: {
		//在CharAdded事件中记录最后输入的字符  
		SCNotification* notify = (SCNotification*)lParam;
		if (notify->nmhdr.code == SCN_MARGINCLICK &&
			notify->nmhdr.idFrom == SCINT_ID) {
			// 确定是页边点击事件  
			const int line_number = SendEditor(SCI_LINEFROMPOSITION, notify->position);
			SendEditor(SCI_TOGGLEFOLD, line_number);
		}

		// 处理自动缩进  
		static int LastProcessedChar = 0;
		if (notify->nmhdr.code == SCN_CHARADDED)
		{
			LastProcessedChar = notify->ch;
		}
		// 在UpdateUI事件中处理缩进  
		if (notify->nmhdr.code == SCN_UPDATEUI && LastProcessedChar != 0)
		{
			int pos = SendEditor(SCI_GETCURRENTPOS); //取得当前位置  
			int line = SendEditor(SCI_LINEFROMPOSITION, pos); //取得当前行  
															  //如果最后输入的字符是右括号的话就自动让当前行缩进和它匹配的左括号所在行一致  
			if (strchr("})>]", LastProcessedChar) &&
				isspace(SendEditor(SCI_GETCHARAT, pos - 2)) && //要求右括号左边是空白字符  
				LastProcessedChar != 0)
			{
				//找前一个单词起始位置，这里用它来确定右括号左边是否全是空白字符  
				int startpos = SendEditor(SCI_WORDSTARTPOSITION, pos - 1, false);
				int linepos = SendEditor(SCI_POSITIONFROMLINE, line); //当前行起始位置  
				if (startpos == linepos) //这样相当于判断右括号左边是否全是空白字符  
				{
					int othpos = SendEditor(SCI_BRACEMATCH, pos - 1); //得到对应的左括号所在的位置  
					int othline = SendEditor(SCI_LINEFROMPOSITION, othpos);   //左括号所在行  
					int nIndent = SendEditor(SCI_GETLINEINDENTATION, othline);//左括号所在行的缩进值  
																			  // 替换右括号前面的空白字符，使之与左括号缩进一致  
					char space[1024];
					memset(space, ' ', 1024);
					SendEditor(SCI_SETTARGETSTART, startpos);
					SendEditor(SCI_SETTARGETEND, pos - 1);
					SendEditor(SCI_REPLACETARGET, nIndent, (sptr_t)space);
				}
			}
			// 如果输入的是回车，则保持与上一行缩进一致  
			// 如果上一行最后有效字符为左括号，就多缩进四个空格  
			if (LastProcessedChar == ' ')
			{
				if (line > 0)
				{
					// 得到上一行缩进设置  
					int nIndent = SendEditor(SCI_GETLINEINDENTATION, line - 1);
					// 查找上一行最后一个有效字符（非空白字符）  
					int nPrevLinePos = SendEditor(SCI_POSITIONFROMLINE, line - 1);
					int c = ' ';
					for (int p = pos - 2;
						p >= nPrevLinePos && isspace(c);
						p--, c = SendEditor(SCI_GETCHARAT, p));
					// 如果是左括号，就多缩进四格  
					if (c && strchr("{([<", c)) nIndent += 4;
					// 缩进...  
					char space[1024];
					memset(space, ' ', 1024);
					space[nIndent] = 0;
					SendEditor(SCI_REPLACESEL, 0, (sptr_t)space);
				}
			}
			LastProcessedChar = 0;
		}
	}
					break;
	default:
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

BOOL InitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	响应WM_INITDIALOG消息时，如果函数调用SetFocus设置对话
	框中控制中的一个焦点, 则对话框应用程序应该返回零值，否则对
	话框应用程序应该返回非零值在,这种情况下系统对能够有焦点的
	对话框中的第一个控制设置焦点。
	*/
	return TRUE;
}

BOOL CALLBACK FindDlg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return InitDialog(hWnd, uMsg, wParam, lParam);

	case WM_KEYDOWN:
		break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK)
		{
			//	MessageBox(hWnd, "确定", "", MB_OK);
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			DestroyWindow(hWnd);
		}
		return TRUE;
	}
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return TRUE;
	}
	return FALSE;	// 如果函数不处理消息，则对话框应用程序应该返回零值。
}

void CFrmTextEditor::OnClick(TNotifyUI &msg)
{
	CDuiString itemName = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (_tcsicmp(itemName, _T("closebtn")) == 0) {
			Close(0);
		}
		else if (_tcsicmp(itemName, _T("minbtn")) == 0) {
			PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (_tcsicmp(itemName, _T("maxbtn")) == 0) {
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if (_tcsicmp(itemName, _T("restorebtn")) == 0) {
			PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else if (_tcsicmp(itemName, _T("btnPageOne")) == 0) {
			HWND  hWndDlg = ::CreateDialogParamW(g_hInst,
				(LPCWSTR)MAKEINTRESOURCE(IDD_FIND),
				hwndEditor,
				reinterpret_cast<DLGPROC>(FindDlg),
				reinterpret_cast<LPARAM>(this));
			if (hWndDlg == NULL)
			{
				MessageBox(NULL, "创建对话框失败.", "", MB_OK);
			}
			::ShowWindow(hWndDlg, SW_SHOW);
		}
	}
}