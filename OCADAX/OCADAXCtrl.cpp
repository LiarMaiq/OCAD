// OCADAXCtrl.cpp : COCADAXCtrl ActiveX 控件类的实现。

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

// 消息映射

BEGIN_MESSAGE_MAP(COCADAXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(COCADAXCtrl, COleControl)
	DISP_FUNCTION_ID(COCADAXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(COCADAXCtrl, "OpenFile", dispidOpenFile, OpenFile, VT_BOOL, VTS_BSTR)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(COCADAXCtrl, COleControl)
END_EVENT_MAP()

// 属性页

// TODO: 根据需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(COCADAXCtrl, 1)
	PROPPAGEID(COCADAXPropPage::guid)
END_PROPPAGEIDS(COCADAXCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(COCADAXCtrl, "OCADAX.OCADAXCtrl.1",
	0xf73a286b, 0x4e03, 0x4dd8, 0xa9, 0x91, 0x4a, 0x13, 0x21, 0xbd, 0x94, 0x8b)

// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(COCADAXCtrl, _tlid, _wVerMajor, _wVerMinor)

// 接口 ID

const IID IID_DOCADAX = { 0xD954DB96, 0xFD53, 0x4725, { 0xB0, 0x12, 0x74, 0x6C, 0x5C, 0x16, 0x3A, 0x6A } };
const IID IID_DOCADAXEvents = { 0xE3968884, 0x1FA8, 0x4C6A, { 0xA9, 0x39, 0xC3, 0x59, 0x89, 0x72, 0xE6, 0xC1 } };

// 控件类型信息

static const DWORD _dwOCADAXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(COCADAXCtrl, IDS_OCADAX, _dwOCADAXOleMisc)

// COCADAXCtrl::COCADAXCtrlFactory::UpdateRegistry -
// 添加或移除 COCADAXCtrl 的系统注册表项

BOOL COCADAXCtrl::COCADAXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO:  验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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


// COCADAXCtrl::COCADAXCtrl - 构造函数

COCADAXCtrl::COCADAXCtrl()
{
	InitializeIIDs(&IID_DOCADAX, &IID_DOCADAXEvents);
	// TODO:  在此初始化控件的实例数据。
	m_pInstanceFactory = 0;
}

// COCADAXCtrl::~COCADAXCtrl - 析构函数

COCADAXCtrl::~COCADAXCtrl()
{
	// TODO:  在此清理控件的实例数据。
}

// COCADAXCtrl::OnDraw - 绘图函数

void COCADAXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO:  用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// COCADAXCtrl::DoPropExchange - 持久性支持

void COCADAXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}


// COCADAXCtrl::OnResetState - 将控件重置为默认状态

void COCADAXCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO:  在此重置任意其他控件状态。
}


// COCADAXCtrl::AboutBox - 向用户显示“关于”框

void COCADAXCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_OCADAX);
	dlgAbout.DoModal();
}


// COCADAXCtrl 消息处理程序


VARIANT_BOOL COCADAXCtrl::OpenFile(LPCTSTR pathName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
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
	// TODO:  在此添加您专用的创建代码
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

	// TODO: 在此处添加消息处理程序代码
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
			//设置控件位置
			ASSERT(IsWindow(pFrameWnd->m_hWnd));
			CRect area;
			GetClientRect(area);
			pFrameWnd->MoveWindow(area.left, area.top, area.right, area.bottom);
		}
	}
}
