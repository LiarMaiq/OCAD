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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CStatusBarColor : public CStatusBar
{
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

class CMainFrame : public CFrameWndEx
{
private:
	/*class TempBar : public OdRxObject, public CToolBar
	{
	public:
		ODRX_HEAP_OPERATORS();
		BOOL Create(CWnd* pParentWnd);
	};*/
	LARGE_INTEGER m_pc0;
	LARGE_INTEGER m_pc1;

public:
	//CStatusBarColor  m_wndStatusBar;
	CString m_statusText1;
	CString m_statusText2;

public:
	inline void StartTimer()
	{
		QueryPerformanceCounter(&m_pc0);
	}

	inline void StopTimer(LPCTSTR pOperationName = NULL)
	{
		QueryPerformanceCounter(&m_pc1);
		m_pc1.QuadPart -= m_pc0.QuadPart;
		if (QueryPerformanceFrequency(&m_pc0))
		{
			double loadTime = ((double)m_pc1.QuadPart) / ((double)m_pc0.QuadPart);
			CString msg;
			msg.Format(_T("%s Time: %.6f sec.")
				, pOperationName ? pOperationName : _T("Operation")
				, loadTime);
			setStatusText(_tcscmp(_T("Redraw"), pOperationName) == 0 ? 2 : 1, msg);
		}
	}
	void setStatusText(LPCTSTR msg) { setStatusText(1, msg); }
	void setStatusText(int nColumn, LPCTSTR msg);





	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateIdsStatusPane1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdsStatusPane2(CCmdUI *pCmdUI);
};


