// OCADAXCtrl.cpp : COCADAXCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "OCADAX.h"
#include "OCADAXCtrl.h"
#include "OCADAXPropPage.h"
#include "afxdialogex.h"

#include "InstanceFactory.h"
#include "AXDOCCONDOC.H"
#include "AxMainFrame.h"
#include "AXDOCCONVIEW.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COCADAXCtrl, COleControl)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(COCADAXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// ����ӳ��

BEGIN_DISPATCH_MAP(COCADAXCtrl, COleControl)
	DISP_FUNCTION_ID(COCADAXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(COCADAXCtrl, "OpenFile", dispidOpenFile, OpenFile, VT_BOOL, VTS_BSTR)
END_DISPATCH_MAP()

// �¼�ӳ��

BEGIN_EVENT_MAP(COCADAXCtrl, COleControl)
END_EVENT_MAP()

// ����ҳ

// TODO: ������Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(COCADAXCtrl, 1)
	PROPPAGEID(COCADAXPropPage::guid)
END_PROPPAGEIDS(COCADAXCtrl)

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(COCADAXCtrl, "OCADAX.OCADAXCtrl.1",
	0xf73a286b, 0x4e03, 0x4dd8, 0xa9, 0x91, 0x4a, 0x13, 0x21, 0xbd, 0x94, 0x8b)

// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(COCADAXCtrl, _tlid, _wVerMajor, _wVerMinor)

// �ӿ� ID

const IID IID_DOCADAX = { 0xD954DB96, 0xFD53, 0x4725, { 0xB0, 0x12, 0x74, 0x6C, 0x5C, 0x16, 0x3A, 0x6A } };
const IID IID_DOCADAXEvents = { 0xE3968884, 0x1FA8, 0x4C6A, { 0xA9, 0x39, 0xC3, 0x59, 0x89, 0x72, 0xE6, 0xC1 } };

// �ؼ�������Ϣ

static const DWORD _dwOCADAXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(COCADAXCtrl, IDS_OCADAX, _dwOCADAXOleMisc)

// COCADAXCtrl::COCADAXCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� COCADAXCtrl ��ϵͳע�����

BOOL COCADAXCtrl::COCADAXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO:  ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_OCADAX,
			IDB_OCADAX,
			afxRegApartmentThreading,
			_dwOCADAXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// COCADAXCtrl::COCADAXCtrl - ���캯��

COCADAXCtrl::COCADAXCtrl()
{
	InitializeIIDs(&IID_DOCADAX, &IID_DOCADAXEvents);
	// TODO:  �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_pInstanceFactory = 0;
}

// COCADAXCtrl::~COCADAXCtrl - ��������

COCADAXCtrl::~COCADAXCtrl()
{
	// TODO:  �ڴ�����ؼ���ʵ�����ݡ�
}

// COCADAXCtrl::OnDraw - ��ͼ����

void COCADAXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO:  �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// COCADAXCtrl::DoPropExchange - �־���֧��

void COCADAXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}


// COCADAXCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void COCADAXCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO:  �ڴ��������������ؼ�״̬��
}


// COCADAXCtrl::AboutBox - ���û���ʾ�����ڡ���

void COCADAXCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_OCADAX);
	dlgAbout.DoModal();
}


// COCADAXCtrl ��Ϣ�������


VARIANT_BOOL COCADAXCtrl::OpenFile(LPCTSTR pathName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	CLSID clsid;
	if (FAILED(CLSIDFromProgID(_T("OCAD.Document"), &clsid))) {
		return 0;
	}

	m_pInstanceFactory->OpenFile(clsid,L"d:\\435.dwg",0);

	return VARIANT_TRUE;
}


int COCADAXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������
	if (NULL == m_pInstanceFactory) {
		m_pInstanceFactory = new CInstanceFactory(IDR_MENU_DSVIEW,
			RUNTIME_CLASS(CAxDocConDoc),
			RUNTIME_CLASS(CMainFrame),       // main SDI frame window
			RUNTIME_CLASS(CAxDocConView),
			this);
		
		m_pInstanceFactory->OpenDocumentFile(NULL);
	}

	return 0;
}


void COCADAXCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	RECT rect;
	rect.left = 5;
	rect.top = 5;
	rect.right = cx - 5;
	rect.bottom = cy - 5;
	/*m_pDocObjFrame->OnNotifySizeChange(&rect);*/

	if (m_pInstanceFactory)
	{
		CFrameWnd* pFrameWnd = m_pInstanceFactory->GetMainWnd();
		if (pFrameWnd != NULL)
		{
			//���ÿؼ�λ��
			ASSERT(IsWindow(pFrameWnd->m_hWnd));
			CRect area;
			GetClientRect(area);
			pFrameWnd->MoveWindow(area.left, area.top, area.right, area.bottom);
		}
	}
}
