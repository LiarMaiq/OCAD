// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// OCAD.h : OCAD 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

//
// Teigha的包含文件
#include "OdaCommon.h"
#include "OdToolKit.h"
#include "..\Include\teigha\Extensions\ExServices\ExSystemServices.h"
#include "..\Include\teigha\Extensions\ExServices\ExHostAppServices.h"


// COCADApp:
// 有关此类的实现，请参阅 OCAD.cpp
//

class COCADApp : public CWinAppEx, public ExSystemServices, public ExHostAppServices
{
//
// Teigha的重写
public:
	using CWinApp::operator new;
	using CWinApp::operator delete;
	void addRef() {}
	void release() {}
public:
	BOOL InitializeTeigha();
	void UninitializeTeigha();
	OdDbDatabasePtr openFile(LPCTSTR lpszPathName);
	// ODA_MT_DBIO_BEGIN
	void setMTLoadingOption(bool val) { m_bUseMTLoading = val; }
	// ODA_MT_DBIO_END
	OdString findFile(const OdString& pcFilename,
		OdDbBaseDatabase* pDb = NULL,
		OdDbBaseHostAppServices::FindFileHint hint = kDefault);
	static int messageBox(HWND hParent, LPCTSTR lpCaption, LPCTSTR lpText, UINT uType)
	{
		return ::MessageBox(hParent, lpText, lpCaption, uType);
	}
	int messageBox(LPCTSTR lpCaption, LPCTSTR lpText, UINT uType)
	{
		CWnd* pMainWnd = GetMainWnd();
		HWND hWnd = 0;
		if (pMainWnd)
			hWnd = pMainWnd->m_hWnd;
		return messageBox(hWnd, lpCaption, lpText, uType);
	}
	void reportError(HWND hParent, LPCTSTR szContextMsg, const OdError& e)
	{
		messageBox(hParent, szContextMsg, (LPCTSTR)e.description(), MB_OK | MB_ICONERROR);
	}
	void reportError(LPCTSTR szContextMsg, const OdError& e)
	{
		messageBox(szContextMsg, (LPCTSTR)e.description(), MB_OK | MB_ICONERROR);
	}
	void reportError(LPCTSTR szContextMsg, unsigned int eCode)
	{
		messageBox(szContextMsg, (LPCTSTR)getErrorDescription(eCode), MB_OK | MB_ICONERROR);
	}
	const ODCOLORREF  activeBackground() const { return m_background; }
	void              setActiveBackground(const ODCOLORREF &color)
	{
		m_background = color & 0xffffff;
	}
	const ODCOLORREF* curPalette() const;
	void setPartialOption(bool val) { m_bPartial = val; }
	void setRecoverOption(bool val) { m_bRecover = val; }
	void setStatusText(LPCTSTR msg);
	void setStatusText(int nCol, LPCTSTR msg);

private:
	// ODA_MT_DBIO_BEGIN
	bool                  m_bUseMTLoading;
	// ODA_MT_DBIO_END
	CStringArray m_tempFilesList;
	ODCOLORREF            m_background;
	bool                  m_bRecover;
	bool                  m_bLoading;
	bool                  m_bPartial;




public:
	COCADApp();
	~COCADApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	COleTemplateServer m_server;
		// 用于文档创建的服务器对象
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COCADApp theApp;

class UserBreak
{
};
