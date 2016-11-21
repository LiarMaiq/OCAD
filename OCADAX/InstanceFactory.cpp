/*******************************************************************************
*	@file	
*		InstanceFactory.cpp
*	@brief		
*		调用SVIEW打开文件
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#include "StdAfx.h"
#include "instancefactory.h"
#include "AxDocConCntrItem.h"
#include "axdoccondoc.h"
#include "axdocconview.h"
#include "AxMainFrame.h"
#include <TlHelp32.h>

#define BUFSIZE 4096   


/****************************************************//**
*	@brief	构造函数	
*******************************************************/
CInstanceFactory::CInstanceFactory(UINT nIDResource,
								   CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
								   CRuntimeClass* pViewClass,CWnd *pWnd):
	CSingleDocTemplate(nIDResource,pDocClass,pFrameClass,pViewClass)
{
	m_wndOwner = pWnd;
	m_pFrame = NULL;
	m_Filename.Empty();
	dwAdvise = 0;
	pCP = NULL;
	m_pDSView = NULL;
}
/****************************************************//**
*	@brief	析构函数	
*******************************************************/
CInstanceFactory::~CInstanceFactory(void)
{
	m_wndOwner = NULL;
	if(dwAdvise != 0)
	{
		pCP->Unadvise(this->dwAdvise);
		pCP->Release();
		pCP = NULL;
		dwAdvise = 0;		
	}

	this->CloseAllDocuments(FALSE);
	if (!m_Filename.IsEmpty()) {
		DeleteFile(m_Filename);
	}
}

/***********************************************************************//**
*	@brief		创建新的框架
*	@param[in]	pDoc
*	@param[in]	pOther
*	@retval		CFrameWnd	框架句柄
****************************************************************************/
CFrameWnd *CInstanceFactory::CreateNewFrame(CDocument *pDoc,CFrameWnd *pOther)
{
	if (pDoc != NULL)
		ASSERT_VALID(pDoc);
	// create a frame wired to the specified document

	ASSERT(m_nIDResource != 0); // must have a resource ID to load from
	CCreateContext context;
	context.m_pCurrentFrame = pOther;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	if (m_pFrameClass == NULL)
	{
		TRACE(traceAppMsg, 0, "Error: you must override CDocTemplate::CreateNewFrame.\n");
		ASSERT(FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE(traceAppMsg, 0, "Warning: Dynamic create of frame %hs failed.\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
		TRACE(traceAppMsg, 0, "Warning: creating frame with no default view.\n");

	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
		WS_CLIPCHILDREN|WS_CHILD | FWS_ADDTOTITLE ,m_wndOwner, &context))
	{
		TRACE(traceAppMsg, 0, "Warning: CDocTemplate couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	if(!m_pFrame)
		m_pFrame = pFrame;

	//初始化DSView进程队列
	//updateDSViewPids();
	// it worked !
	return pFrame;
}

/*************************************************************************
*	@brief		打开SVL文件
*	@param[in]	rclsid	程序id
*	@param[in]	strPath	文件路径
*	@retval		TRUE	打开成功
*	@retval		FALSE	打开失败
****************************************************************************/
BOOL CInstanceFactory::OpenFile(REFCLSID rclsid,const CString &strPath, int iCount,...)
{
	va_list arg_prt;
	va_start(arg_prt,iCount);
    short loadMode;
	loadMode=va_arg(arg_prt,short);
	long modelViewID;
	modelViewID=va_arg(arg_prt,long);
	CFileFind finder;
	if(!finder.FindFile(strPath)){
		return FALSE;
	}
	CDocument* m_pOnlyDoc = this->GetMainWnd()->GetActiveDocument();
	if (!m_pOnlyDoc) {
		return FALSE;
	}
	ASSERT(m_pOnlyDoc->IsKindOf(RUNTIME_CLASS(CAxDocConDoc)));
	CAxDocConDoc *pDoc = (CAxDocConDoc *)m_pOnlyDoc;
	CView *pView = NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos) {
		pView = m_pOnlyDoc->GetNextView(pos);
	}

	if (pView == NULL) {
		return FALSE;
	}

	//clear items
	POSITION itemPos = pDoc->GetStartPosition();
	COleClientItem *pItemItem;
	while ((pItemItem = (COleClientItem *)pDoc->GetNextItem(itemPos)) != NULL)
	{
		HWND hServer; 
		DWORD dwProcessId;
		HANDLE Hwnd;
		hServer = pItemItem->m_hWndServer;
		GetWindowThreadProcessId(hServer, &dwProcessId);
		Hwnd = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, 0, dwProcessId);
		if (Hwnd)
		{
			TerminateProcess(Hwnd, 0);
		}

		pItemItem->Delete();
	}
	


	BeginWaitCursor();

	CAxDocConCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		pItem = new CAxDocConCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// this is the pdf server's application id
		pItem->CreateNewItem(rclsid,OLERENDER_NONE);
		VERIFY(pItem->DoVerb(OLEIVERB_SHOW, pView));

		//updateDSViewPids();

		LPUNKNOWN lpUnk = pItem->m_lpObject;
		LPOLELINK lpOleLink = NULL;
		if (pItem->m_lpObject->QueryInterface(IID_IOleLink,(LPVOID FAR*)&lpOleLink) == NOERROR)
		{
			ASSERT(lpOleLink != NULL);
			lpUnk = NULL;
			//Retrieve the IUnknown interface to the linked application.
			if(lpOleLink->GetBoundSource(&lpUnk) != NOERROR)
			{
				TRACE(traceAppMsg, 0,"Warning: Link is not connected!/n");
				lpOleLink->Release();
				return NULL;
			}
			ASSERT(lpUnk != NULL);
		}
		//QI for the IDispatch interface of the linked application.
		LPDISPATCH lpDispatch = NULL;
		if(lpUnk->QueryInterface(IID_IDispatch, (LPVOID FAR*)&lpDispatch)!=NOERROR)
		{
			TRACE(traceAppMsg, 0,"Warning: does not support IDispatch!/n");
			return NULL;
		}

		//创建sview对象
		//m_pDSView = new CDSView(lpDispatch);
		m_pDSView = (IOCAD*)lpDispatch;

		IConnectionPointContainer   * pCPC;
		HRESULT hr = pItem->m_lpObject->QueryInterface(IID_IConnectionPointContainer,(void **)&pCPC);
		if(dwAdvise != 0){
			pCP->Unadvise(this->dwAdvise);
			dwAdvise = 0;
		}

		//if(SUCCEEDED(hr))
		//{
		//	hr = pCPC->FindConnectionPoint(DIID_IOCADEvents,&pCP);
		//	pCPC->Release();			

		//	if(SUCCEEDED(hr))
		//	{
		//		LPUNKNOWN pSinkUnk;
		//		/*CSViewSink *pSink = new CSViewSink((CSViewOCX3Ctrl*)m_wndOwner);
		//		pSink->AddRef();
		//		hr = pSink->QueryInterface (IID_IUnknown,(void **)&pSinkUnk);
		//		hr = pCP->Advise(pSinkUnk,&dwAdvise); 
		//		pSink->Release();
		//		pSinkUnk->Release();*/
		//	}
		//}

		IPersistFile *pPersistFile = NULL;
		hr = pItem->m_lpObject->QueryInterface(IID_IPersistFile,(void **)&pPersistFile);
		USES_CONVERSION;

		CString hr_str;
		hr_str.Format(L"%d", hr);

		if(SUCCEEDED(hr))
		{
			CString strPath2;
			strPath2 = strPath;
			hr = pPersistFile->Load(T2COLE(strPath2.GetBuffer()),STGM_READ/*|STGM_SHARE_DENY_READ|STGM_TRANSACTEDSTGM_READ | STGM_SHARE_DENY_NONE*/);
			
			//CString hr_str;
			hr_str.Format(L"%d", hr);
			
			pPersistFile->Release();
			if(!SUCCEEDED(hr))
			{
				//strPath2 临时文件
				TCHAR lpPathBuffer[BUFSIZE];

				// 获取临时文件路径   
				GetTempPath(BUFSIZE, lpPathBuffer); 

				//获得年月日时间字符串
				//获取系统时间
				CTime locatm = GetCurrentTime();   
				CString strtime=locatm.Format("%Y%m%d%H%M%S.svl");
				CString tempPath = lpPathBuffer;
				tempPath += strtime;
				int ref = CopyFile(strPath,tempPath,FALSE);
				if (ref > 0)
				{
					hr = pPersistFile->Load(T2COLE(tempPath.GetBuffer()),STGM_READ|STGM_SHARE_DENY_READ|STGM_TRANSACTED/*STGM_READ | STGM_SHARE_DENY_NONE*/);
					m_Filename = tempPath;
				}                
			}
			ASSERT_VALID(pItem);

			// As an arbitrary user interface design, this sets the selection
			//  to the last item inserted.
			pDoc->UpdateAllViews(NULL);
		}
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
		}
		EndWaitCursor();
	}
	END_CATCH
	//绘制窗体
	CRect rtCln;
	m_wndOwner->GetClientRect(rtCln);
	rtCln.DeflateRect(0,0,0,1);
	m_pFrame->MoveWindow(rtCln,FALSE);
	EndWaitCursor();
	return TRUE;
}

CFrameWnd * CInstanceFactory::GetMainWnd(){
	return m_pFrame;
}

void CInstanceFactory::TerminateItemServer()
{
	CDocument* m_pOnlyDoc = this->GetMainWnd()->GetActiveDocument();
	if (!m_pOnlyDoc) {
		return;
	}
	ASSERT(m_pOnlyDoc->IsKindOf(RUNTIME_CLASS(CAxDocConDoc)));
	CAxDocConDoc *pDoc = (CAxDocConDoc *)m_pOnlyDoc;
	POSITION itemPos = pDoc->GetStartPosition();
	COleClientItem *pItemItem;
	while ((pItemItem = (COleClientItem *)pDoc->GetNextItem(itemPos)) != NULL)
	{
		HWND hServer;
		DWORD dwProcessId;
		HANDLE Hwnd;
		hServer = pItemItem->m_hWndServer;
		GetWindowThreadProcessId(hServer, &dwProcessId);
		Hwnd = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, 0, dwProcessId);
		if (Hwnd)
		{
			TerminateProcess(Hwnd, 0);
		}
		//pItemItem->Delete();
	}

}