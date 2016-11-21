// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// OCAD.h : OCAD Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

//
// Teigha�İ����ļ�
#include "OdaCommon.h"
#include "OdToolKit.h"
#include "..\Include\teigha\Extensions\ExServices\ExSystemServices.h"
#include "..\Include\teigha\Extensions\ExServices\ExHostAppServices.h"


// COCADApp:
// �йش����ʵ�֣������ OCAD.cpp
//

class COCADApp : public CWinAppEx, public ExSystemServices, public ExHostAppServices
{
//
// Teigha����д
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


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	COleTemplateServer m_server;
		// �����ĵ������ķ���������
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
