#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once
#include <textserv.h>
#include <richedit.h>
#include <RichOle.h>

namespace DuiLib {

	class CTxtWinHost : public ITextHost
	{
	public:
		CTxtWinHost();
		virtual ~CTxtWinHost(); 

		virtual BOOL Init(CRichEditUI *re, const CREATESTRUCT *pcs);
		virtual void SetClientRect(RECT *prc);
		virtual void SetWordWrap(BOOL fWordWrap);
		virtual BOOL IsReadOnly();
		virtual void SetReadOnly(BOOL fReadOnly);

		virtual void SetFont(HFONT hFont);
		virtual void SetColor(DWORD dwColor);
		virtual SIZEL* GetExtent();
		virtual void SetExtent(SIZEL *psizelExtent);
		virtual void LimitText(LONG nChars);
		virtual BOOL IsCaptured();
		virtual BOOL IsShowCaret();
		virtual void NeedFreshCaret();
		virtual INT GetCaretWidth();
		virtual INT GetCaretHeight();
		virtual BOOL GetAllowBeep();
		virtual void SetAllowBeep(BOOL fAllowBeep);
		virtual WORD GetDefaultAlign();
		virtual void SetDefaultAlign(WORD wNewAlign);
		virtual BOOL GetRichTextFlag();
		virtual void SetRichTextFlag(BOOL fNew);
		virtual LONG GetDefaultLeftIndent();
		virtual void SetDefaultLeftIndent(LONG lNewIndent);
		virtual BOOL SetSaveSelection(BOOL fSaveSelection);
		virtual HRESULT OnTxInPlaceDeactivate();
		virtual HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
		virtual BOOL DoSetCursor(RECT *prc, POINT *pt);
		virtual void SetTransparent(BOOL fTransparent);
		virtual void GetControlRect(LPRECT prc);
		virtual LONG SetAccelPos(LONG laccelpos);
		virtual WCHAR SetPasswordChar(WCHAR chPasswordChar);
		virtual void SetDisabled(BOOL fOn);
		virtual LONG SetSelBarWidth(LONG lSelBarWidth);
		virtual BOOL GetTimerState();

		virtual void SetCharFormat(CHARFORMAT2W &c);
		virtual void SetParaFormat(PARAFORMAT2 &p);

		// -----------------------------
		//	IUnknown interface
		// -----------------------------
		virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
		virtual ULONG _stdcall AddRef(void);
		virtual ULONG _stdcall Release(void);

		// -----------------------------
		//	ITextHost interface
		// -----------------------------
		virtual HDC TxGetDC();
		virtual INT TxReleaseDC(HDC hdc);
		virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
		virtual BOOL TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags);
		virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
		virtual BOOL TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw);
		virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
		virtual void TxViewChange(BOOL fUpdate);
		virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
		virtual BOOL TxShowCaret(BOOL fShow);
		virtual BOOL TxSetCaretPos(INT x, INT y);
		virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
		virtual void TxKillTimer(UINT idTimer);
		virtual void TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
		virtual void TxSetCapture(BOOL fCapture);
		virtual void TxSetFocus();
		virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
		virtual BOOL TxScreenToClient(LPPOINT lppt);
		virtual BOOL TxClientToScreen(LPPOINT lppt);
		virtual HRESULT TxActivate(LONG * plOldState);
		virtual HRESULT TxDeactivate(LONG lNewState);
		virtual HRESULT TxGetClientRect(LPRECT prc);
		virtual HRESULT TxGetViewInset(LPRECT prc);
		virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF);
		virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
		virtual COLORREF TxGetSysColor(int nIndex);
		virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
		virtual HRESULT TxGetMaxLength(DWORD *plength);
		virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
		virtual HRESULT TxGetPasswordChar(TCHAR *pch);
		virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
		virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
		virtual HRESULT OnTxCharFormatChange(const CHARFORMATW * pcf);
		virtual HRESULT OnTxParaFormatChange(const PARAFORMAT * ppf);
		virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
		virtual HRESULT TxNotify(DWORD iNotify, void *pv);
		virtual HIMC TxImmGetContext(void);
		virtual void TxImmReleaseContext(HIMC himc);
		virtual HRESULT TxGetSelectionBarWidth(LONG *lSelBarWidth);

		ITextServices* GetTextServices(void) { return pserv; }
		RECT* GetClientRect() { return &rcClient; }
		BOOL IsWordWrap(void) { return fWordWrap; }
		BOOL GetActiveState(void) { return fInplaceActive; }
		ITextHost * GetTextHost()
		{
			AddRef();
			return this;
		}

		ITextServices * GetTextServices2()
		{
			if (NULL == pserv)
				return NULL;
			pserv->AddRef();
			return pserv;
		}

		BOOL SetOleCallback(IRichEditOleCallback* pCallback)
		{
			if (NULL == pserv)
				return FALSE;
			HRESULT lRes = 0;
			pserv->TxSendMessage(EM_SETOLECALLBACK, 0, (LPARAM)pCallback, &lRes);
			return (BOOL)lRes;
		}

		BOOL CanPaste(UINT nFormat = 0)
		{
			if (NULL == pserv)
				return FALSE;
			HRESULT lRes = 0;
			pserv->TxSendMessage(EM_CANPASTE, nFormat, 0L, &lRes);
			return (BOOL)lRes;
		}

		void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0)
		{
			if (NULL == pserv)
				return;
			REPASTESPECIAL reps = { dwAspect, (DWORD_PTR)hMF };
			pserv->TxSendMessage(EM_PASTESPECIAL, uClipFormat, (LPARAM)&reps, NULL);
		}
		  
	private:
		CRichEditUI *m_re;
		ULONG	cRefs;					// Reference Count
		ITextServices	*pserv;		    // pointer to Text Services object
		// Properties

		DWORD		dwStyle;				// style bits

		unsigned	fEnableAutoWordSel : 1;	// enable Word style auto word selection?
		unsigned	fWordWrap : 1;	// Whether control should word wrap
		unsigned	fAllowBeep : 1;	// Whether beep is allowed
		unsigned	fRich : 1;	// Whether control is rich text
		unsigned	fSaveSelection : 1;	// Whether to save the selection when inactive
		unsigned	fInplaceActive : 1; // Whether control is inplace active
		unsigned	fTransparent : 1; // Whether control is transparent
		unsigned	fTimer : 1;	// A timer is set
		unsigned    fCaptured : 1;
		unsigned    fShowCaret : 1;
		unsigned    fNeedFreshCaret : 1; // 修正改变大小后点击其他位置原来光标不能消除的问题

		INT         iCaretWidth;
		INT         iCaretHeight;
		INT         iCaretLastWidth;
		INT         iCaretLastHeight;
		LONG		lSelBarWidth;			// Width of the selection bar
		LONG  		cchTextMost;			// maximum text size
		DWORD		dwEventMask;			// DoEvent mask to pass on to parent window
		LONG		icf;
		LONG		ipf;
		RECT		rcClient;				// Client Rect for this control
		SIZEL		sizelExtent;			// Extent array
		CHARFORMAT2W cf;					// Default character format
		PARAFORMAT2	pf;					    // Default paragraph format
		LONG		laccelpos;				// Accelerator position
		WCHAR		chPasswordChar;		    // Password character
	};


	class UILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI
	{
		DECLARE_DUICONTROL(CRichEditUI)
	public:
		CRichEditUI();
		~CRichEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine);
		bool IsWantTab();
		void SetWantTab(bool bWantTab = true);
		bool IsWantReturn();
		void SetWantReturn(bool bWantReturn = true);
		bool IsWantCtrlReturn();
		void SetWantCtrlReturn(bool bWantCtrlReturn = true);
		bool IsTransparent();
		void SetTransparent(bool bTransparent = true);
		bool IsRich();
		void SetRich(bool bRich = true);
		bool IsReadOnly();
		void SetReadOnly(bool bReadOnly = true);
		bool IsWordWrap();
		void SetWordWrap(bool bWordWrap = true);
		int GetFont();
		void SetFont(int index);
		void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		LONG GetWinStyle();
		void SetWinStyle(LONG lStyle);
		DWORD GetTextColor();
		void SetTextColor(DWORD dwTextColor);
		int GetLimitText();
		void SetLimitText(int iChars);
		long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
		CDuiString GetText() const;
		void SetText(LPCTSTR pstrText);
		bool IsModify() const;
		void SetModify(bool bModified = true) const;
		void GetSel(CHARRANGE &cr) const;
		void GetSel(long& nStartChar, long& nEndChar) const;
		int SetSel(CHARRANGE &cr);
		int SetSel(long nStartChar, long nEndChar);
		void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
		void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
		CDuiString GetSelText() const;
		int SetSelAll();
		int SetSelNone();
		WORD GetSelectionType() const;
		bool GetZoom(int& nNum, int& nDen) const;
		bool SetZoom(int nNum, int nDen);
		bool SetZoomOff();
		bool GetAutoURLDetect() const;
		bool SetAutoURLDetect(bool bAutoDetect = true);
		DWORD GetEventMask() const;
		DWORD SetEventMask(DWORD dwEventMask);
		CDuiString GetTextRange(long nStartChar, long nEndChar) const;
		void HideSelection(bool bHide = true, bool bChangeStyle = false);
		void ScrollCaret();
		int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
		int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
		DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
		bool SetDefaultCharFormat(CHARFORMAT2 &cf);
		DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
		bool SetSelectionCharFormat(CHARFORMAT2 &cf);
		bool SetWordCharFormat(CHARFORMAT2 &cf);
		DWORD GetParaFormat(PARAFORMAT2 &pf) const;
		bool SetParaFormat(PARAFORMAT2 &pf);
		bool CanUndo();
		bool CanRedo();
		bool CanPaste();
		bool Redo();
		bool Undo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();
		int GetLineCount() const;
		CDuiString GetLine(int nIndex, int nMaxLength) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		bool LineScroll(int nLines, int nChars = 0);
		CDuiPoint GetCharPos(long lChar) const;
		long LineFromChar(long nIndex) const;
		CDuiPoint PosFromChar(UINT nChar) const;
		int CharFromPos(CDuiPoint pt) const;
		void EmptyUndoBuffer();
		UINT SetUndoLimit(UINT nLimit);
		long StreamIn(int nFormat, EDITSTREAM &es);
		long StreamOut(int nFormat, EDITSTREAM &es);
		void SetAccumulateDBCMode(bool bDBCMode);
		bool IsAccumulateDBCMode();

		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void PaintStatusImage(HDC hDC);

		void SetTipValue(LPCTSTR pStrTipValue);
		LPCTSTR GetTipValue();
		void SetTipValueColor(LPCTSTR pStrColor);
		DWORD GetTipValueColor();
		void SetTipValueAlign(UINT uAlign);
		UINT GetTipValueAlign();

		void DoInit();
		bool SetDropAcceptFile(bool bAccept);
		// 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
		// 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
		virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
		IDropTarget* GetTxDropTarget();
		virtual bool OnTxViewChanged();
		virtual void OnTxNotify(DWORD iNotify, void *pv);

		void SetScrollPos(SIZE szPos, bool bMsg = true);
		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();
		void HomeUp();
		void EndDown();
		void LineLeft();
		void LineRight();
		void PageLeft();
		void PageRight();
		void HomeLeft();
		void EndRight();

		SIZE EstimateSize(SIZE szAvailable);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

		ITextHost * GetTextHost();
		ITextServices * GetTextServices();
		BOOL SetOleCallback(IRichEditOleCallback* pCallback);
		BOOL CanPaste(UINT nFormat = 0);
		void PasteSpecial(UINT uClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0);
		void SetCanPaintChirden(bool paint);
		bool GetCanPaintChirden(bool paint);

		HDC GetTxtHDC();

		virtual void OnScrollChange(SIZE szPos, bool bMsg){};
		 
	protected:
		enum { 
			DEFAULT_TIMERID = 20,
		};

		CTxtWinHost* m_pTwh;
		bool m_bVScrollBarFixing;
		bool m_bWantTab;
		bool m_bWantReturn;
		bool m_bWantCtrlReturn;
		bool m_bTransparent;
		bool m_bRich;
		bool m_bReadOnly;
		bool m_bWordWrap;
		DWORD m_dwTextColor;
		int m_iFont;
		int m_iLimitText;
		LONG m_lTwhStyle;
		bool m_bDrawCaret;
		bool m_bInited;

		bool  m_fAccumulateDBC ; // TRUE - need to cumulate ytes from 2 WM_CHAR msgs
		// we are in this mode when we receive VK_PROCESSKEY
		UINT m_chLeadByte; // use when we are in _fAccumulateDBC mode

		RECT m_rcTextPadding;
		UINT m_uButtonState;
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;
		CDuiString m_sTipValue;
		DWORD m_dwTipValueColor;
		UINT m_uTipValueAlign;
		bool canPaintChirden = true; //>->19.3.30绘制子控件
	};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
