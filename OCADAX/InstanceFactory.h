/*******************************************************************************
*	@file	
*		InstanceFactory.h
*	@brief		
*		调用SVIEW打开文件
*	@par
*		from EmbOleObjCtrl
*********************************************************************************/
#pragma once
#include "afxwin.h"
#include <list>
//#include "DSView.h"

class OCAD;
class CInstanceFactory : public CSingleDocTemplate
{
public:
	CInstanceFactory(UINT nIDResource, CRuntimeClass* pDocClass,
					 CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,CWnd *pWnd);

	virtual ~CInstanceFactory(void);
	virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);

    BOOL		OpenFile(REFCLSID rclsid,const CString &strPath, int iCount,...);//打开SVL文件
	CFrameWnd	*GetMainWnd();//得到主句柄
	IOCAD*	GetDSView() { return m_pDSView; };
private:
	CWnd		*m_wndOwner;//句柄指针
	CFrameWnd	*m_pFrame;//框架指针
	CString		 m_Filename;//文件名
	IOCAD*	 m_pDSView;

	DWORD dwAdvise;
	LPCONNECTIONPOINT pCP;
public:
	void TerminateItemServer();
};
