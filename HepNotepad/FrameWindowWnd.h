#pragma once

// ����ʵ������Ϣ��Ӧ����
class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:
	CFrameWindowWnd();
	~CFrameWindowWnd();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const ;
	void OnFinalMessage(HWND /*hWnd*/);

	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_pm;
};
