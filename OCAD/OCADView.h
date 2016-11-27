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

// OCADView.h : COCADView 类的接口
//

#pragma once

#include "GiContextForDbDatabase.h"
#include "../Examples/Editor/EditorObject.h"





class COCADView : public CView, 
	public OdGiContextForDbDatabase
	, OdEdBaseIO
	, OdExEditorObject::OleDragCallback
{
protected:
	using CView::operator new;
	using CView::operator delete;
	void addRef() {}
	void release() {}

public:
	void resetDevice(bool zoomeExtents = false);
	const ODCOLORREF* CurrentPalette();
	void setViewportBorderProperties(OdGsDevice* pDevice, BOOL bModel);
	void Refresh();
	OdGsViewPtr GetView();
	void Dolly(int x, int y);
	OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);
	OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
	void putString(const OdString& string);
	OdUInt32 getKeyState();
	void track(OdEdInputTracker* pTracker);
	void setCursor(HCURSOR hCursor);
	HCURSOR cursor() const;
	OdExEditorObject &editorObject();
	const OdExEditorObject &editorObject() const;
	OdGsView* getActiveView();
	const OdGsView* getActiveView() const;
	OdGsView* getActiveTopView();
	const OdGsView* getActiveTopView() const;
	bool UpdateStringTrackerCursor(void);

public:
	OdGsLayoutHelperPtr m_pDevice;		// Vectorizer device
	ODCOLORREF m_clrBackground;			// Drawing background colo
	OdEdInputTracker* m_pTracker;		// Input tracker
	CPoint m_MousePosition;				// Position of mouse pointer

private:
	BOOL m_bPsOverall;
	OdString m_sPrompt;
	ExEdInputParser m_inpars;
	int m_inpOptions;
	HCURSOR m_hCursor;
	OdExEditorObject m_editor;
	CPoint m_oldPoint;

private:
	enum Mode
	{
		kQuiescent,
		kGetPoint,
		kGetString,
		kDragDrop
	}
	m_mode;
	struct Response
	{
	public:
		enum Type
		{
			kNone,
			kPoint,
			kString,
			kCancel
		};
		Type            m_type;
		OdGePoint3d     m_point;
		OdString        m_string;
	}
	m_response;

private:
	bool beginDragCallback(const OdGePoint3d& pt);
	void destroyDevice();











protected: // 仅从序列化创建
	COCADView();
	DECLARE_DYNCREATE(COCADView)

// 特性
public:
	COCADDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // OCADView.cpp 中的调试版本
inline COCADDoc* COCADView::GetDocument() const
   { return reinterpret_cast<COCADDoc*>(m_pDocument); }
#endif

