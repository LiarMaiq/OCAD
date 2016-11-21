/*******************************************************************************
*	@file	
*		AxDocConView.h
*	@brief		
*		interface of the CAxDocConView class
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#if !defined(AFX_AXDOCCONVIEW_H__B58CD4AD_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
#define AFX_AXDOCCONVIEW_H__B58CD4AD_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxDocConCntrItem;

class CAxDocConView : public CView
{
protected: // create from serialization only
	CAxDocConView();
	DECLARE_DYNCREATE(CAxDocConView)

// Attributes
public:
	CAxDocConDoc* GetDocument();
	// m_pSelection holds the selection to the current CAxDocConCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CAxDocConCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	
// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxDocConView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxDocConView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAxDocConView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in AxDocConView.cpp
inline CAxDocConDoc* CAxDocConView::GetDocument()
   { return (CAxDocConDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXDOCCONVIEW_H__B58CD4AD_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
