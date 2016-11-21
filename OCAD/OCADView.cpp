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

// OCADView.cpp : COCADView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OCAD.h"
#endif

#include "OCADDoc.h"
#include "OCADView.h"


#include "DbGsManager.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"
#include "SaveState.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COCADView

IMPLEMENT_DYNCREATE(COCADView, CView)

BEGIN_MESSAGE_MAP(COCADView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, &COCADView::OnCancelEditSrvr)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COCADView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
END_MESSAGE_MAP()

// COCADView ����/����

void COCADView::resetDevice(bool zoomeExtents)
{
	/**********************************************************************/
	/* Get the client rectangle                                           */
	/**********************************************************************/
	CRect rc;
	GetClientRect(&rc);
	/**********************************************************************/
	/* Load the WinGDI module                                             */
	/**********************************************************************/
	OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
	/**********************************************************************/
	/* Create a new OdGsDevice object, and associate with the WinGDI      */
	/* GsDevice                                                           */
	/**********************************************************************/
	OdGsDevicePtr pDevice = pGs->createDevice();
	//m_pDevice = pGs->createDevice();
	if (pDevice.isNull())
		return;
	/**********************************************************************/
	/* Return a pointer to the dictionary entity containing the device    */
	/* properties                                                         */
	/**********************************************************************/
	OdRxDictionaryPtr pProperties = pDevice->properties();
	/**********************************************************************/
	/* Set the window handle for this GsDevice                            */
	/**********************************************************************/
	pProperties->putAt("WindowHWND", OdRxVariantValue((OdIntPtr)m_hWnd));



	/**********************************************************************/
	/* Set up the views for the active layout                             */
	/**********************************************************************/
	m_pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, this);

	/**********************************************************************/
	/* Define a device coordinate rectangle equal to the client rectangle.*/
	/**********************************************************************/
	OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);

	/************************************************************************/
	/* Set the device background color and palette from the current app.   */
	/************************************************************************/
	m_pDevice->setBackgroundColor(m_clrBackground);
	m_pDevice->setLogicalPalette(CurrentPalette(), 256);

	if (!database())
		return;

	

	/**********************************************************************/
	/* Return true if and only the current layout is a paper space layout.*/
	/**********************************************************************/
	m_bPsOverall = (GetDocument()->m_pDb->getTILEMODE() == 0);

	/**********************************************************************/
	/* Set the viewport border properties.                                 */
	/**********************************************************************/
	setViewportBorderProperties(m_pDevice, !m_bPsOverall);

#ifdef AUTOMATICALLY_ZOOM_TOEXTENTS_WHEN_OPENINGDRAWING
	if (zoomExtents)
		ViewZoomextents();
#endif

	/**********************************************************************/
	/* Update the client rectangle.                                       */
	/**********************************************************************/
	OnSize(0, rc.Width(), rc.Height());
	/**********************************************************************/
	/* Redraw the window.                                                 */
	/**********************************************************************/
	RedrawWindow();
}

const ODCOLORREF* COCADView::CurrentPalette()
{
	const ODCOLORREF *pColor = odcmAcadPalette(m_clrBackground);
	return pColor;
}

void COCADView::setViewportBorderProperties(OdGsDevice* pDevice, BOOL bModel)
{
	// If current layout is Model, and it has more then one viewport then make their borders visible.
	// If current layout is Paper, then make visible the borders of all but the overall viewport.
	int n = pDevice->numViews();
	if (n > 1)
	{
		for (int i = bModel ? 0 : 1; i < n; ++i)
		{
			// Get the viewport
			OdGsViewPtr pView = pDevice->viewAt(i);

			// Make it visible
			pView->setViewportBorderVisibility(true);

			// Set the color and width
			pView->setViewportBorderProperties(CurrentPalette()[7], 1);
		}
	}
}

void COCADView::Refresh()
{
	// Clean out the tracker
	if (m_pTracker)
		m_pTracker->removeDrawables(GetView());
	m_pTracker = NULL;

	// Refresh the drawing view
	m_pDevice->invalidate();
	PostMessage(WM_PAINT);
}

COCADView::COCADView():
	//: m_hWindowDC(0)
	//, m_bRegenAbort(false)
	m_mode(kQuiescent)
	, m_hCursor(0)
	//, m_bInRegen(false)
	//, m_paintMode(PaintMode_Regen)
	//, m_bPlotGrayscale(false)
	//, m_pagingCounter(0)
{
	// TODO: �ڴ˴���ӹ������
	m_clrBackground = RGB(0, 0, 0);

	/*m_iRenderMode = OdGsView::k2DOptimized;
	m_bLeftButton = FALSE;
	m_bMiddleButton = FALSE;
	m_bRightButton = FALSE;
	m_bZoomWindow = FALSE;*/

	m_pTracker = NULL;
}

COCADView::~COCADView()
{
}

BOOL COCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COCADView ����

void COCADView::OnDraw(CDC* /*pDC*/)
{
	COCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// COCADView ��ӡ


void COCADView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void COCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void COCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void COCADView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COCADView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// OLE ������֧��

// �������������ṩ�˱�׼����
//  �û�������ȡ���͵ر༭�Ự��  �˴���
//  ������(����������)������ͣ��
void COCADView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}


// COCADView ���

#ifdef _DEBUG
void COCADView::AssertValid() const
{
	CView::AssertValid();
}

void COCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COCADDoc* COCADView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COCADDoc)));
	return (COCADDoc*)m_pDocument;
}
#endif //_DEBUG


// COCADView ��Ϣ�������


void COCADView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	/**********************************************************************/
	/* Set the database to be vectorized                                  */
	/**********************************************************************/
	OdGiContextForDbDatabase::setDatabase(GetDocument()->m_pDb);

	/**********************************************************************/
	/* Set the GS Model flag                                              */
	/**********************************************************************/
	enableGsModel(true);
	resetDevice(true);
	if (m_pDevice.isNull())
	{
		GetParent()->PostMessage(WM_CLOSE);
		return;
	}
	m_editor.initialize(m_pDevice, static_cast<COCADDoc*>(GetDocument())->cmdCtx());

	setCursor(::LoadCursor(0, IDC_ARROW));
}

OdGsViewPtr COCADView::GetView()
{
	return m_pDevice->viewAt(0);
}

void COCADView::Dolly(int x, int y)
{
	// Get the view
	OdGsViewPtr pView = GetView();

	// Set up the dolly vector
	OdGeVector3d Vector(-x, -y, 0.0);
	Vector.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());

	// Perform the dolly
	pView->dolly(Vector);
}

void COCADView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MousePosition = point;

	__super::OnMButtonDown(nFlags, point);
}


void COCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CView::OnMouseMove(nFlags, point);
	::SetCursor(m_hCursor);
	if (point != m_oldPoint)
	{
		m_oldPoint = point;
		switch (m_mode)
		{
		case kQuiescent:
			m_editor.OnMouseMove(nFlags, point.x, point.y);
			break;

		case kGetPoint:
		{
			OdGePoint3d pt = m_editor.toEyeToWorld(point.x, point.y);
			if (!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
				if (!m_editor.toUcsToWorld(pt))
					return;

			if (!GETBIT(m_inpOptions, OdEd::kGptNoOSnap))
				m_editor.snap(pt);
			m_editor.trackPoint(pt);
		}
		break;
		}
		if (!getActiveView()->isValid() || !getActiveTopView()->isValid())
		{
			//getActiveView()->invalidate();
			PostMessage(WM_PAINT);
		}
	}


	//if (GetKeyState(VK_MBUTTON)<0)
	//{
	//	OdGeVector3d Vector(m_MousePosition.x - point.x, m_MousePosition.y - point.y, 0.0);
	//	// Screen to Eye
	//	Vector.transformBy((GetView()->screenMatrix() * GetView()->projectionMatrix()).inverse());
	//	GetView()->dolly(Vector);
	//	m_MousePosition = point;
	//	PostMessage(WM_PAINT);
	//}
	//else if (GetKeyState(VK_RBUTTON)<0)
	//{
	//	GetView()->orbit((m_MousePosition.y - point.y) / 100.0, (m_MousePosition.x - point.x) / 100.0);
	//	m_MousePosition = point;
	//	PostMessage(WM_PAINT);
	//}

	//__super::OnMouseMove(nFlags, point);
}


BOOL COCADView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ScreenToClient(&pt);

	OdGsViewPtr pView = GetView();

	OdGePoint3d Position(pView->position());
	Position.transformBy(pView->worldToDeviceMatrix());
	int x, y;
	x = (int)OdRound(Position.x);
	y = (int)OdRound(Position.y);
	x = pt.x - x;
	y = pt.y - y;

	Dolly(-x, -y);
	pView->zoom(zDelta > 0 ? 1.0 / 0.9 : 0.9);
	Dolly(x, y);

	PostMessage(WM_PAINT);

	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void COCADView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
					   /**********************************************************************/
					   /* Paint the client rectangle with the GS device                      */
					   /**********************************************************************/
	if (!m_pDevice.isNull())
		m_pDevice->update();
}


void COCADView::OnSize(UINT nType, int cx, int cy)
{
	//__super::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();

	if (!m_pDevice.isNull() && cx && cy)
	{
		CRect rc;
		GetClientRect(rc);

		// Update the client rectangle
		OdGsDCRect Rect(OdGsDCPoint(rc.left, rc.bottom), OdGsDCPoint(rc.right, rc.top));
		m_pDevice->onSize(Rect);
	}
}

void COCADView::putString(const OdString& string)
{
	m_sPrompt = (LPCTSTR)string;
	int n = string.reverseFind('\n');
	LPCTSTR txt = (LPCTSTR)string;
	if (n >= 0)
	{
		txt = txt + n + 1;
	}
	theApp.setStatusText(txt);
}

OdUInt32 COCADView::getKeyState()
{
	return GetDocument()->getKeyState();
}

void COCADView::track(OdEdInputTracker* pTracker)
{
	m_editor.setTracker(pTracker);
}

HCURSOR COCADView::cursor() const
{
	return m_hCursor;
}

void COCADView::setCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
	::SetCursor(hCursor);
}

class SaveViewParams
{
protected:
	COCADView*   m_pView;
	HCURSOR       m_hOldCursor;
public:
	SaveViewParams(COCADView* pView, OdEdInputTracker* pTracker, HCURSOR hCursor, bool bSnap)
		: m_pView(pView)
		, m_hOldCursor(pView->cursor())
	{
		pView->track(pTracker);
		pView->setCursor(hCursor);
		if (bSnap)
			pView->editorObject().initSnapping(pView->getActiveTopView());
	}
	~SaveViewParams()
	{
		m_pView->track(0);
		m_pView->setCursor(m_hOldCursor);
		m_pView->editorObject().uninitSnapping(m_pView->getActiveTopView());
	}
};

#define BLINK_CURSOR_TIMER 888
#define BLINK_CURSOR_RATE  GetCaretBlinkTime()

void CALLBACK StringTrackerTimer(HWND hWnd, UINT  nMsg, UINT  nIDTimer, DWORD dwTime)
{
	try
	{
		COCADView* pViewer = (COCADView*)CWnd::FromHandle(hWnd);

		if (!pViewer->UpdateStringTrackerCursor())
			KillTimer(hWnd, nIDTimer);
	}
	catch (...)
	{
		KillTimer(hWnd, nIDTimer);
	}
}

class SaveViewParams2 : public SaveViewParams {
	bool m_bTimerSet;
public:
	SaveViewParams2(COCADView* pView, OdEdStringTracker* pTracker, HCURSOR hCursor)
		: SaveViewParams(pView, pTracker, hCursor, false)
	{
		if (pTracker) {
			pTracker->setCursor(true);
			::SetTimer(m_pView->m_hWnd, BLINK_CURSOR_TIMER, BLINK_CURSOR_RATE, (TIMERPROC)StringTrackerTimer);
			m_bTimerSet = true;
		}
		else
			m_bTimerSet = false;
	}
	~SaveViewParams2() {
		if (m_bTimerSet)
			::KillTimer(m_pView->m_hWnd, BLINK_CURSOR_TIMER);
	}
};



OdString COCADView::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{

	m_sPrompt.empty();
	OdSaveState<OdString> savePrompt(m_sPrompt);
	putString(prompt);

	OdSaveState<Mode>     saved_m_mode(m_mode, kGetString);

	m_response.m_type = Response::kNone;

	if (pTracker) {
		m_inpars.reset(true);
	}

	m_inpOptions = options;

	SaveViewParams2 svp(this, pTracker, ::LoadCursor(0, IDC_IBEAM));

	while (theApp.PumpMessage())
	{
		switch (m_response.m_type)
		{
		case Response::kString:
			return m_response.m_string;

		case Response::kCancel:
			throw OdEdCancel();
			break;
		}
		// Mfc idle loop for toolbars and menus process
		LONG lIdle = 0;
		while (theApp.OnIdle(lIdle++));
	}

	throw OdEdCancel();
}

OdGePoint3d COCADView::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
	m_sPrompt.empty();
	OdSaveState<OdString> savePrompt(m_sPrompt);
	putString(prompt);

	OdSaveState<Mode>     saved_m_mode(m_mode, kGetPoint);

	m_response.m_type = Response::kNone;

	m_inpOptions = options;

	SaveViewParams svp(this, pTracker, ::LoadCursor(0, IDC_CROSS), !GETBIT(options, OdEd::kGptNoOSnap));

	while (theApp.PumpMessage())
	{
		switch (m_response.m_type)
		{
		case Response::kPoint:
			if (GETBIT(m_inpOptions, OdEd::kGptBeginDrag))
				SetCapture();
			return m_response.m_point;

		case Response::kString:
			throw OdEdOtherInput(m_response.m_string);

		case Response::kCancel:
			throw OdEdCancel();
		}
		// Mfc idle loop for toolbars and menus process
		LONG lIdle = 0;
		while (theApp.OnIdle(lIdle++));
	}

	throw OdEdCancel();
}

OdExEditorObject &COCADView::editorObject()
{
	return m_editor;
}

const OdExEditorObject &COCADView::editorObject() const
{
	return m_editor;
}

OdGsView* COCADView::getActiveView()
{
	return m_pDevice->activeView();
}

const OdGsView* COCADView::getActiveView() const
{
	return m_pDevice->activeView();
}

OdGsView* COCADView::getActiveTopView()
{
	return m_editor.activeTopView();
}

const OdGsView* COCADView::getActiveTopView() const
{
	return m_editor.activeTopView();
}


// Blink cursor timer

bool COCADView::UpdateStringTrackerCursor(void)
{
	if (m_mode == kGetString &&
		m_response.m_type != Response::kString)
	{

		if (m_editor.trackString(m_inpars.result()))
		{
			getActiveTopView()->invalidate();
			PostMessage(WM_PAINT);
			return true;
		}
	}
	return false;
}


bool COCADView::beginDragCallback(const OdGePoint3d& pt)
{
	OdSaveState<Mode> saved_m_mode(m_mode, kDragDrop);
	//GetDocument()->startDrag(pt);
	return true;
}



void COCADView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CView::OnChar(nChar, nRepCnt, nFlags);
	m_response.m_string = m_inpars.result();
	switch (nChar)
	{
	case VK_BACK:
		while (nRepCnt--)
			m_inpars.eraseChar();
		break;

	case VK_ESCAPE:
		m_response.m_type = Response::kCancel;
		m_inpars.reset(false);

		switch (m_mode)
		{
		case kQuiescent:
			if (m_editor.unselect())
				PostMessage(WM_PAINT);
			break;

		case kGetPoint:
			// no op
			break;
		}
		break;

	default:
		while (nRepCnt--)
		{
			if (!m_inpars.addChar((OdChar)nChar))
			{
				m_inpars.reset(false);
				switch (m_mode)
				{
				case kQuiescent:
					/*if (m_response.m_string.isEmpty())
						GetDocument()->ExecuteCommand(GetDocument()->recentCmdName());
					else
						GetDocument()->ExecuteCommand(m_response.m_string);*/
					break;

				case kGetPoint:
				case kGetString:
					m_response.m_type = Response::kString;
					break;
				}
			}
		}
		break;
	}

	if (m_mode == kGetString &&
		m_response.m_type != Response::kString &&
		m_inpars.result() != m_response.m_string)
	{
		if (m_editor.trackString(m_inpars.result()))
		{
			getActiveTopView()->invalidate();
			PostMessage(WM_PAINT);
		}
	}

	if (m_sPrompt.isEmpty())
		m_sPrompt = _T("command: ");
	else if (m_inpars.result().isEmpty())
		theApp.setStatusText(m_sPrompt);
	else
		theApp.setStatusText(m_inpars.result());

	//__super::OnChar(nChar, nRepCnt, nFlags);
}


void COCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CView::OnLButtonDown(nFlags, point);

	switch (m_mode)
	{

	case kQuiescent:
		if (m_editor.OnMouseLeftButtonClick(nFlags, point.x, point.y, this))
		{
			//getActiveView()->invalidate();
			PostMessage(WM_PAINT);
		}
		break;

	case kGetPoint:
		m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
		if (!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
		{
			if (!m_editor.toUcsToWorld(m_response.m_point))
				break;
		}
		m_editor.snap(m_response.m_point);
		m_response.m_type = Response::kPoint;
		break;

	}

	//__super::OnLButtonDown(nFlags, point);
}


void COCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CView::OnLButtonUp(nFlags, point);
	if (m_mode == kGetPoint && GetCapture() == this)
	{
		m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
		if (!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
		{
			if (!m_editor.toUcsToWorld(m_response.m_point))
				return;
		}
		m_response.m_type = Response::kPoint;
		ReleaseCapture();
	}

	m_editor.Set_Entity_centers();
	//__super::OnLButtonUp(nFlags, point);
}


void COCADView::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO: �ڴ˴������Ϣ����������
}
