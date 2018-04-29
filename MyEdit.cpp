// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MyEdit.h"
#include <inc_path/cor.h>
#include <inc_path/cor_stat.h>
#include <afxcmn.h>
// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

UINT CMyEdit::UWM_CHECK_VALUE = ::RegisterWindowMessage(_T("UWM_CHECK_VALUE-{6FE8A4C0-AE33-11d4-A002-006067718D04}"));
UINT CMyEdit::UWM_VALID_CHANGE = ::RegisterWindowMessage(_T("UWM_VALID_CHANGE-{6FE8A4C1-AE33-11d4-A002-006067718D04}"));


CMyEdit::CMyEdit()
{
   grbrush.CreateSolidBrush(RGB(0,255,0));
   m_greenbrush = (HBRUSH)grbrush;

   redbrush.CreateSolidBrush(RGB(255,0,0));
   m_redbrush = (HBRUSH)redbrush;

   emptybrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
   m_emptybrush = (HBRUSH)emptybrush;
   m_bError = FALSE;

   SyntaxValid = FALSE;
   ValueValid = FALSE;

}

bool CMyEdit::SetParent(CWnd* Parent, CWnd* pCtrl, CToolTipCtrl* pTooltipCtrl)
{
   Parent->EnableToolTips(TRUE);
   m_wndCtrl = pCtrl;
   m_ToolTipCtrl =  pTooltipCtrl;

   if (!pTooltipCtrl->Create(Parent, WS_POPUP | TTS_NOPREFIX | TTS_BALLOON))
   {
      ::AfxMessageBox("Unable to create the ToolTip!");
   }
   else
   {
      //m_ToolTip.AddTool(this, _T("This is Web HMI server name. \n  eg:WEBHMISERVER \n (Or) You can even specify URL \n eg:https://arunwebhmi10/Thingworx/Login"));
      pTooltipCtrl->AddTool(pCtrl, _T("This is the Web HMI server to which mimics will be published. \n You can specify \n MachineName: eg:WEBHMISERVER \n (Or)  URL: eg:https://arunwebhmi10/Thingworx/Login"));
      pTooltipCtrl->SetMaxTipWidth(1000);
      pTooltipCtrl->SetTipBkColor(RGB(255, 255, 255));
      pTooltipCtrl->SetDelayTime(TTDT_AUTOPOP, 60000);
      pTooltipCtrl->Activate(TRUE);

   }

   
   return true;
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg)
{
   //if (m_ToolTipCtrl)
   //m_ToolTipCtrl->RelayEvent(pMsg);
   return CEdit::PreTranslateMessage(pMsg);
}

CMyEdit::~CMyEdit()
{
   emptybrush.DeleteObject();
   
}

void CMyEdit::SetError(bool bError)
{
   m_bError = bError;
   InvalidateRect(NULL);

   GetParent()->SendMessage(UWM_CHECK_VALUE,MAKELONG(GetDlgCtrlID(), EN_CHANGE),(LPARAM)m_hWnd);
   
   //m_ToolTipCtrl->
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
   //ON_WM_CTLCOLOR_REFLECT()
   ON_WM_CHAR()
   ON_WM_NCPAINT()
   ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()



// CMyEdit message handlers

void CMyEdit::OnNcPaint()
{
   // TODO: Add your message handler code here
   // Do not call CEdit::OnNcPaint() for painting messages   
   CDC* pDC = GetWindowDC();

   CRect rect;
   GetWindowRect(&rect);
   rect.OffsetRect(-rect.left, -rect.top);

   if (m_bError && IsWindowEnabled())
   {          
      //Draw a single line around the outside
      CBrush brush(RGB(255, 0, 0));
      pDC->FrameRect(&rect, &brush);            
   }
   else
   {   
      COR_STATUS retstat;
      cor_logstatus(_T("OnNcPaint"), FALSE, &retstat);

      CEdit::OnNcPaint();
   }
      
   ReleaseDC(pDC);   
}



HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
   // TODO:  Change any attributes of the DC here   
   if (m_bError)
   {   
      pDC->SetTextColor(RGB(0, 255, 0));
      pDC->SetBkColor(RGB(255, 0, 0));
      return m_greenbrush;
   }
   else
   {
      return m_emptybrush;
      
   }
   
}

void CMyEdit::PreSubclassWindow()
{
   CEdit::PreSubclassWindow();

   //ModifyStyleEx(0, WS_EX_STATICEDGE, 0);
}


BOOL CMyEdit::OnChange()
{
   Check();
   InvalidateRect(NULL);


   return FALSE;
} // CFloatingEdit::OnChange


bool CMyEdit::Check()
{
   CString s;
   CEdit::GetWindowText(s);

   if (s.IsEmpty())
   {
      SetError(true);
   }
   else
   {
      SetError(false);
   }

   return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
CNumericEdit::CNumericEdit()
{



}

CNumericEdit::~CNumericEdit()
{



}

BEGIN_MESSAGE_MAP(CNumericEdit, CMyEdit)
   //ON_WM_CTLCOLOR_REFLECT()
   ON_WM_CHAR()
   ON_WM_NCPAINT()
END_MESSAGE_MAP()


void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   switch (nChar)
   { /* validate */
   case _T('+'):
   case _T('-'):
   case _T('.'):
   case _T('E'):
   case _T('e'):
   case _T('0'):
   case _T('1'):
   case _T('2'):
   case _T('3'):
   case _T('4'):
   case _T('5'):
   case _T('6'):
   case _T('7'):
   case _T('8'):
   case _T('9'):
   case _T('\b'):
      break;
   default:
      MessageBeep(0);
      return;
   } /* validate */
   CEdit::OnChar(nChar, nRepCnt, nFlags);
}


bool CNumericEdit::Check()
{
   bool bRet = true;
   CString s;
   CEdit::GetWindowText(s);
   int i = atoi(s);
   OutputDebugString(s);

   if (i < 0 || i>65000)
   {
      SetError(true);
      bRet = false;
   }
   else
   {
      SetError(false);
      bRet = true;
   }

   return bRet;

}


/*void CNumericEdit::OnNcPaint()
{
   // TODO: Add your message handler code here
   // Do not call CEdit::OnNcPaint() for painting messages   
   if (!Check())
   {
      SetError(true);
      //CMyEdit::OnNcPaint();
   }
}*/