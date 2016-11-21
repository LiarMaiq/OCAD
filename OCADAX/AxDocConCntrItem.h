/*******************************************************************************
*	@file	
*		CntrItem.h
*	@brief		
*		implementation of the CAxDocConCntrItem class
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#if !defined(AFX_CNTRITEM_H__B58CD4B0_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
#define AFX_CNTRITEM_H__B58CD4B0_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxDocConDoc;
class CAxDocConView;

class CAxDocConCntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CAxDocConCntrItem)

// Constructors
public:
	CAxDocConCntrItem(CAxDocConDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CAxDocConDoc* GetDocument()
		{ return (CAxDocConDoc*)COleDocObjectItem::GetDocument(); }
	CAxDocConView* GetActiveView()
		{ return (CAxDocConView*)COleDocObjectItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxDocConCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual void OnInsertMenus(CMenu* pMenuShared,LPOLEMENUGROUPWIDTHS lpMenuWidths)
	{}

	//}}AFX_VIRTUAL

// Implementation
public:
	~CAxDocConCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	BOOL CreateNewItem(REFCLSID clsid, OLERENDER render = OLERENDER_DRAW,
		CLIPFORMAT cfFormat = 0, LPFORMATETC lpFormatEtc = NULL);
	BOOL CreateFromFile(LPCTSTR lpszFileName, REFCLSID clsid = CLSID_NULL,
		OLERENDER render = OLERENDER_DRAW,
		CLIPFORMAT cfFormat = 0, LPFORMATETC lpFormatEtc = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__B58CD4B0_6CAB_11D4_A54D_0050BADB14A3__INCLUDED_)
