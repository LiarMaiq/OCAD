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

// MainFrm.h : CMainFrame 类的接口
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





	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// 生成的消息映射函数
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


