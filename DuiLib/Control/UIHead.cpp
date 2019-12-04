#include "StdAfx.h"
#include "UIHead.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CHeadUI)

	CHeadUI::CHeadUI()
	{
		m_sDefaultBkImage = _T("logo.png");
		m_bAutoPenColor = false;
		m_dwPenColor = Color(255, 255, 255, 255).GetValue();
		m_nPenWidth = 2;
	}

	LPCTSTR CHeadUI::GetClass() const
	{
		return _T("HeadIconUI");
	}

	LPVOID CHeadUI::GetInterface(LPCTSTR pstrName)
	{
		 
		if (_tcscmp(pstrName, _T("HeadIcon")) == 0) return static_cast<CHeadUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CHeadUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		 
		if (_tcscmp(pstrName, _T("defaultbkimage")) == 0) SetDefaultBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("pencolor")) == 0) {
			while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPenColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("autopencolor")) == 0) SetAutoPenColor(_tcscmp(pstrValue, _T("true")) == 0);
		else if (_tcscmp(pstrName, _T("penwidth")) == 0) SetPenWidth(_ttoi(pstrValue));
		else return CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void CHeadUI::SetBkImage(LPCTSTR pStrImage)
	{
	 
		CDuiString sInstancePath;
		LPCTSTR m_sBkImage;
		if (IsHeadImageExist(pStrImage))
		{
			m_sBkImage = pStrImage;
		}
		else
		{
			/*TCHAR tszModule[MAX_PATH + 1] = { 0 };
			::GetModuleFileName(CPaintManagerUI::GetInstance(), tszModule, MAX_PATH);
			sInstancePath = tszModule;
			int pos = sInstancePath.ReverseFind(_T('\\'));
			if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);*/
			sInstancePath.Append(CPaintManagerUI::GetResourcePath());
			sInstancePath.Append(pStrImage);

			if (IsHeadImageExist(sInstancePath))
			{
				m_sBkImage = sInstancePath;
			}
			else
			{
				m_sBkImage = pStrImage;
			}
		}
		CButtonUI::SetBkImage(m_sBkImage);
		Invalidate();

	}

	void CHeadUI::PaintBkImage(HDC hDC)
	{
		 
		//坐标
		POINT	pt = { m_rcItem.left, m_rcItem.top };
		//大小
		SIZE	sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };
		Graphics    graphics(hDC);
		if (graphics.GetLastStatus() != Ok)
			return;

		//消除锯齿
		graphics.SetSmoothingMode(SmoothingModeHighQuality);

		GraphicsPath graphicspath;
		if (graphicspath.GetLastStatus() != Ok)
			return;

		graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

		//设置裁剪圆
		graphics.SetClip(&graphicspath, CombineModeReplace);

#if _UNICODE
		Image image(GetBkImage());
#else
		//GetBkImage()
		string temp = m_pManager->GetInstancePath()+GetBkImage();
	
		wstring bkImage = Acsi2WideByte(temp);
		Image image(bkImage.c_str());
		
		//Image image(L"E:\\C++\\Duilib Study\\TIM-master\\TIM\\..\\Bin\\Resource\\images/241_100.gif");
#endif
		
		if (image.GetLastStatus() != Ok)
			return;

		//绘制图像
		graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);

		//绘制一个1像素宽度的圆形，用于消除锯齿
		Pen	myPen(GetPenColor(hDC), GetPenWidth());
		if (myPen.GetLastStatus() != Ok)
			return;

		graphics.DrawEllipse(&myPen, pt.x, pt.y, sz.cx, sz.cy);
	}

	void CHeadUI::PaintStatusImage(HDC hDC)
	{
		if (IsFocused()) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~UISTATE_FOCUSED;
		if (!IsEnabled()) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~UISTATE_DISABLED;

		//坐标
		POINT	pt = { m_rcItem.left, m_rcItem.top };
		//大小
		SIZE	sz = { m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top };
		Graphics    graphics(hDC);
		if (graphics.GetLastStatus() != Ok)
			return;

		//消除锯齿
		graphics.SetSmoothingMode(SmoothingModeHighQuality);

		GraphicsPath graphicspath;
		if (graphicspath.GetLastStatus() != Ok)
			return;

		graphicspath.AddEllipse(pt.x, pt.y, sz.cx, sz.cy);

		//设置裁剪圆
		graphics.SetClip(&graphicspath, CombineModeReplace);


		do
		{
			if ((m_uButtonState & UISTATE_DISABLED) != 0)
			{
				if (m_disabledImage.IsLoadSuccess())
				{
				 
#if _UNICODE
					Image image(GetDisabledImage());
#else
					//GetDisabledImage()
					string temp = GetDisabledImage();
					wstring bkImage = Acsi2WideByte(temp);
					Image image(bkImage.c_str());
#endif
					 
					graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
					//DrawImage(hDC, m_disabledImage);
					break;
				}
			}
			else if ((m_uButtonState & UISTATE_PUSHED) != 0)
			{
				if (m_pushedImage.IsLoadSuccess())
				{
#if _UNICODE
					Image image(GetPushedImage());
#else
					//GetPushedImage()
					string temp = GetPushedImage();
					wstring bkImage = Acsi2WideByte(temp);
					Image image(bkImage.c_str());
#endif 
				 
					graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
					//DrawImage(hDC, m_pushedImage);
					if (m_pushedForeImage.IsLoadSuccess())
					{
#if _UNICODE
						Image image(GetPushedForeImage());
#else
						//GetPushedForeImage()
						string temp = GetPushedForeImage();
						wstring bkImage = Acsi2WideByte(temp);
						Image image(bkImage.c_str());
#endif  
						graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
						//DrawImage(hDC, m_pushedForeImage);
						return;
					}
					else
						break;
				}
			}
			else if ((m_uButtonState & UISTATE_HOT) != 0)
			{
				if (m_hotImage.IsLoadSuccess())
				{
#if _UNICODE
					Image image(GetHotImage());
#else
					//GetHotImage()
					string temp = GetHotImage();
					wstring bkImage = Acsi2WideByte(temp);
					Image image(bkImage.c_str());
#endif  

					 
					graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
					//DrawImage(hDC, m_hotImage);

					if (m_hotForeImage.IsLoadSuccess())
					{
#if _UNICODE
						Image image(GetHotForeImage());
#else
						//GetHotForeImage()
						string temp = GetHotForeImage();
						wstring bkImage = Acsi2WideByte(temp);
						Image image(bkImage.c_str());
#endif   
						graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
						//DrawImage(hDC, m_hotForeImage);
						return;
					}
					else
						break;
				}
				else if (m_dwHotBkColor != 0)
				{
					CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
					return;
				}
			}
			else if ((m_uButtonState & UISTATE_FOCUSED) != 0)
			{
				if (m_focusedImage.IsLoadSuccess())
				{

#if _UNICODE
					Image image(GetForeImage());
#else
					//GetForeImage()
					string temp = GetForeImage();
					wstring bkImage = Acsi2WideByte(temp);
					Image image(bkImage.c_str());
#endif   
					graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
					//DrawImage(hDC, m_focusedImage);
					return;
				}
			}
#if _UNICODE
			Image image(GetNormalImage());
#else 
			string temp = GetNormalImage();
			wstring bkImage = Acsi2WideByte(temp);
			Image image(bkImage.c_str());
#endif    
			graphics.DrawImage(&image, pt.x, pt.y, sz.cx, sz.cy);
			//DrawImage(hDC, m_normalImage);
		} while (0);


		DrawImage(hDC, m_foreImage);
	}

	DWORD CHeadUI::GetPenColor(HDC hDC)
	{
		if (IsAutoPenColor())
		{
			//像素值颜色取点（ pt.x + 1, pt.y + 1）的值
			RECT rc = GetPos();
			COLORREF color = GetPixel(hDC, rc.left + 1, rc.top + 1);

			BYTE r = GetRValue(color);
			BYTE g = GetGValue(color);
			BYTE b = GetBValue(color);

			return Color(255, r, g, b).GetValue();
		}

		return m_dwPenColor;
	}

	bool CHeadUI::IsHeadImageExist(LPCTSTR pStrImage)
	{
		return GetFileAttributes(pStrImage) == -1 ? false : true;
	}

	std::wstring CHeadUI::Acsi2WideByte(std::string& strascii){
		using namespace std;
		int widesize = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
		if (widesize == ERROR_NO_UNICODE_TRANSLATION)
		{
			throw std::exception("Invalid UTF-8 sequence.");
		}
		if (widesize == 0)
		{
			throw std::exception("Error in conversion.");
		}
		std::vector<wchar_t> resultstring(widesize);
		int convresult = MultiByteToWideChar(CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
		if (convresult != widesize)
		{
			throw std::exception("La falla!");
		}
		return std::wstring(&resultstring[0]);
	}
}