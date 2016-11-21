/****************************************************************************//**
*	@file	
*		AxDocConDoc.h
*	@brief		
*		interface of the CAxDocConDoc class
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#if !defined(AFX_AXDOCCONDOC_H__B58CD4AB_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
#define AFX_AXDOCCONDOC_H__B58CD4AB_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAxDocConDoc : public COleDocument
{
protected: // create from serialization only
	CAxDocConDoc();
	DECLARE_DYNCREATE(CAxDocConDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxDocConDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxDocConDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAxDocConDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual BOOL SaveModified();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXDOCCONDOC_H__B58CD4AB_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
