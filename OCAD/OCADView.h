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

// OCADView.h : COCADView ��Ľӿ�
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











protected: // �������л�����
	COCADView();
	DECLARE_DYNCREATE(COCADView)

// ����
public:
	COCADDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~COCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // OCADView.cpp �еĵ��԰汾
inline COCADDoc* COCADView::GetDocument() const
   { return reinterpret_cast<COCADDoc*>(m_pDocument); }
#endif

