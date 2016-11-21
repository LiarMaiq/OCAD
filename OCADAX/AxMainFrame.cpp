/*******************************************************************************
*	@file	
*		MainFrm.cpp
*	@brief		
*		interface of the CMainFrame class
*	@par
*		create on 2014/04/20 by pangyulong
*********************************************************************************/
#include "stdafx.h"
#include "resource.h"
#include "AxMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
/****************************************************//**
*	@brief		构造函数	
*******************************************************/
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}
/****************************************************//**
*	@brief		析构函数
*******************************************************/
CMainFrame::~CMainFrame()
{
}
/***********************************************************************//**
*	@brief	创建框架资源
*	@retval	0	创建成功	
****************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rtClient;
	::GetClientRect(lpCreateStruct->hwndParent,rtClient);
	MoveWindow(rtClient);

	return 0;
}
/***********************************************************************//**
*	@brief	创建窗体
*	@retval	TRUE	创建成功	
****************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.lpszClass == NULL)
	{
		cs.lpszClass = AfxRegisterWndClass(CS_GLOBALCLASS);
	}

	if (cs.style & FWS_ADDTOTITLE)
		cs.style |= FWS_PREFIXTITLE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.hMenu = NULL;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::OnClose()
{
	if (m_lpfnCloseProc != NULL)
		(*m_lpfnCloseProc)(this);

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			if (pView->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
		}
		if (!bOtherFrame)
		{
			pDocument->OnCloseDocument();
			return;
		}

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// then destroy the window
	DestroyWindow();
}

BOOL CMainFrame::LoadFrame(UINT nIDResource,
				           DWORD dwDefaultStyle,
				           CWnd* pParentWnd,
				           CCreateContext* pContext)
{
	if(!CFrameWnd::LoadFrame(nIDResource,dwDefaultStyle,pParentWnd,pContext))
		return FALSE;
	m_pWndOwner = pParentWnd;
	return TRUE;
}
/****************************************************//**
*	@brief		窗体拖拽
*******************************************************/
void CMainFrame::OnOwnerMove()
{
	CRect rt;
	ASSERT(m_pWndOwner);
	m_pWndOwner->GetWindowRect(&rt);
	MoveWindow(&rt);
}