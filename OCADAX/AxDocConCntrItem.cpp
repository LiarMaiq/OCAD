/*******************************************************************************
*	@file	
*		CntrItem.cpp
*	@brief		
*		implementation of the CAxDocConCntrItem class
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#include "stdafx.h"
#include "AxDocConDoc.h"
#include "AxDocConView.h"
#include "AxDocConCntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifndef _AFXDLL
#include <oleimpl2.h>
#else

LPFORMATETC AFXAPI _AfxFillFormatEtc(
	LPFORMATETC lpFormatEtc, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtcFill)
{
	ASSERT(lpFormatEtcFill != NULL);
	if (lpFormatEtc == NULL && cfFormat != 0)
	{
		lpFormatEtc = lpFormatEtcFill;
		lpFormatEtc->cfFormat = cfFormat;
		lpFormatEtc->ptd = NULL;
		lpFormatEtc->dwAspect = DVASPECT_CONTENT;
		lpFormatEtc->lindex = -1;
		lpFormatEtc->tymed = (DWORD) -1;
	}
	return lpFormatEtc;
}
typedef LPVOID* LPLP;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAxDocConCntrItem implementation

IMPLEMENT_SERIAL(CAxDocConCntrItem, COleDocObjectItem, 0)

CAxDocConCntrItem::CAxDocConCntrItem(CAxDocConDoc* pContainer)
	: COleDocObjectItem(pContainer)
{
	// TODO: add one-time construction code here
	
}

CAxDocConCntrItem::~CAxDocConCntrItem()
{
	// TODO: add cleanup code here

}

void CAxDocConCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleDocObjectItem::OnChange(nCode, dwParam);

	// When an item is being edited (either in-place or fully open)
	//  it sends OnChange notifications for changes in the state of the
	//  item or visual appearance of its content.

	// TODO: invalidate the item by calling UpdateAllViews
	//  (with hints appropriate to your application)

	GetDocument()->UpdateAllViews(NULL);
		// for now just update ALL views/no hints
}

BOOL CAxDocConCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// During in-place activation CAxDocConCntrItem::OnChangeItemPosition
	//  is called by the server to change the position of the in-place
	//  window.  Usually, this is a result of the data in the server
	//  document changing such that the extent has changed or as a result
	//  of in-place resizing.
	//
	// The default here is to call the base class, which will call
	//  COleDocObjectItem::SetItemRects to move the item
	//  to the new position.

	if (!COleDocObjectItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: update any cache you may have of the item's rectangle/extent

	return TRUE;
}


void CAxDocConCntrItem::OnActivate()
{
}

void CAxDocConCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleDocObjectItem::OnDeactivateUI(bUndoable);

    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CAxDocConCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleDocObjectItem data.
	// Since this sets up the m_pDocument pointer returned from
	//  CAxDocConCntrItem::GetDocument, it is a good idea to call
	//  the base class Serialize first.
	COleDocObjectItem::Serialize(ar);

	// now store/retrieve data specific to CAxDocConCntrItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAxDocConCntrItem diagnostics

#ifdef _DEBUG
void CAxDocConCntrItem::AssertValid() const
{
	COleDocObjectItem::AssertValid();
}

void CAxDocConCntrItem::Dump(CDumpContext& dc) const
{
	COleDocObjectItem::Dump(dc);
}
#endif

BOOL CAxDocConCntrItem::CreateNewItem(REFCLSID clsid,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the object
	LPOLECLIENTSITE lpClientSite = GetClientSite();
	
	HRESULT hr = ::CoCreateInstance(clsid,NULL,CLSCTX_ALL,IID_IOleObject,(void **)&m_lpObject);
	if(!SUCCEEDED(hr))
		return hr;
	m_lpObject->SetClientSite(lpClientSite);

	BOOL bResult = SUCCEEDED(hr);

	ASSERT_VALID(this);
	return bResult;

}

// Creation from files (in OLE 1.0, the user did this through packager)
BOOL CAxDocConCntrItem::CreateFromFile(LPCTSTR lpszFileName, REFCLSID clsid,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	USES_CONVERSION;

	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the object
	LPOLECLIENTSITE lpClientSite = GetClientSite();
	SCODE sc = ::OleCreateFromFile(clsid, T2COLE(lpszFileName),
		IID_IUnknown, render, lpFormatEtc, lpClientSite, m_lpStorage,
		(LPLP)&m_lpObject);
	BOOL bResult = FinishCreate(sc);

	ASSERT_VALID(this);
	return bResult;
}
