#pragma once
#include <afxcmn.h>

// CMyEdit

class CMyEdit : public CEdit
{
   DECLARE_DYNAMIC(CMyEdit)

 public:
   CMyEdit();
   virtual ~CMyEdit();
   void SetError(bool bError);

   CBrush redbrush;
   CBrush grbrush;
   CBrush emptybrush;
   HBRUSH m_redbrush;
   HBRUSH m_greenbrush;
   HBRUSH m_emptybrush;
   bool m_bError;
   BOOL SyntaxValid;
   BOOL ValueValid;
   CToolTipCtrl* m_ToolTipCtrl;
   CWnd* m_wndCtrl;
   
   static UINT UWM_CHECK_VALUE;   
   static UINT UWM_VALID_CHANGE;

 protected:
   DECLARE_MESSAGE_MAP()
 public:
   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   BOOL PreTranslateMessage(MSG* pMsg);
   virtual void PreSubclassWindow();
   afx_msg void OnNcPaint();
   BOOL IsValid() { return SyntaxValid && ValueValid; };
   BOOL IsSyntaxValid() { return SyntaxValid; }
   BOOL IsValueValid() { return ValueValid; }
   afx_msg BOOL OnChange();
   virtual bool Check();
   bool SetParent(CWnd* Parent, CWnd* pCtrl, CToolTipCtrl* pTooltipCtrl);

};

class CNumericEdit : public CMyEdit
{
public:
   CNumericEdit();
   ~CNumericEdit();
   bool Check();
   //void OnNcPaint();
protected:
   DECLARE_MESSAGE_MAP()
   afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

};
