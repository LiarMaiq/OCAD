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

// OCADDoc.cpp : COCADDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OCAD.h"
#endif

#include "OCADDoc.h"
#include "SrvrItem.h"

#include <propkey.h>



#include "OCADView.h"


//
// ODA
#include "Ed/EdCommandStack.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COCADDoc

IMPLEMENT_DYNCREATE(COCADDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(COCADDoc, COleServerDoc)
	ON_COMMAND(ID_TEST, &COCADDoc::OnTest)
	ON_COMMAND(ID_TEST2, &COCADDoc::OnTest2)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COCADDoc, COleServerDoc)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IOCAD 的支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {A521C422-334D-4899-881F-EBEA50DCB856}
static const IID IID_IOCAD =
{ 0xA521C422, 0x334D, 0x4899, { 0x88, 0x1F, 0xEB, 0xEA, 0x50, 0xDC, 0xB8, 0x56 } };

BEGIN_INTERFACE_MAP(COCADDoc, COleServerDoc)
	INTERFACE_PART(COCADDoc, IID_IOCAD, Dispatch)
END_INTERFACE_MAP()





unsigned short COCADDoc::ClipboardData::m_FormatR15 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r15"));  // r15
unsigned short COCADDoc::ClipboardData::m_FormatR16 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r16"));  // r18
unsigned short COCADDoc::ClipboardData::m_FormatR17 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r17"));  // r21
unsigned short COCADDoc::ClipboardData::m_FormatR18 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r18"));  // r24
unsigned short COCADDoc::ClipboardData::m_FormatR19 = (CLIPFORMAT)::RegisterClipboardFormat(_T("AutoCAD.r19"));  // r27



// COCADDoc 构造/析构

COCADDoc::COCADDoc()
	/*: m_bPartial(false)
	, m_pViewer(0)
	, m_SaveAsType(OdDb::kDwg)
	, m_SaveAsVer(OdDb::kDHL_CURRENT)
	, m_bConsole(false)
	, m_nCmdActive(0)
	, m_bLayoutSwitchable(false)
	, m_bDisableClearSel(false)*/
{
	// 使用 OLE 复合文件
	EnableCompoundFile();

	// TODO: 在此添加一次性构造代码

	EnableAutomation();

	AfxOleLockApp();
}

COCADDoc::~COCADDoc()
{
	AfxOleUnlockApp();
}

BOOL COCADDoc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	try
	{
		m_pDb = theApp.createDatabase(true, OdDb::MeasurementValue::kMetric);
	}
	catch (const OdError& e)
	{
		m_pDb = 0;
		theApp.reportError(_T("Database Creating Error..."), e);
		return FALSE;
	}

	//
	// 初始化命令上下文
	m_pCmdCtx.release();
	m_pCmdCtx = NULL;

	return TRUE;
}


// COCADDoc 服务器实现

COleServerItem* COCADDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem 由框架调用以获取与该文档关联的 COleServerItem。
	//  只在必要时才调用它。

	COCADSrvrItem* pItem = new COCADSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}




// COCADDoc 序列化

void COCADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void COCADDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void COCADDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void COCADDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COCADDoc 诊断

#ifdef _DEBUG
void COCADDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void COCADDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG


// COCADDoc 命令
BOOL COCADDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!COleServerDoc::OnOpenDocument(lpszPathName))
	//	return FALSE;

	// TODO:  在此添加您专用的创建代码
	/**********************************************************************/
	/* Load the specified DWG file into the database, allow code page     */
	/* conversion disallow partial load.                                  */
	/**********************************************************************/
	try
	{
		m_pDb = theApp.openFile(lpszPathName/*, true, false*/);
	}
	catch (OdError& /*e*/)
	{
		return FALSE;
	}

	m_pCmdCtx.release();
	m_pCmdCtx = NULL;

	return TRUE;
}


BOOL COCADDoc::ExecuteCommand(const OdString& strCommand, BOOL bEcho)
{
	if (m_bCommandActive == TRUE)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _T("Cannot execute command while another command is active"), _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	OdDbCommandContextPtr pCmdCtx = cmdCtx();

	try
	{
		OdEdCommandStackPtr pCommands = ::odedRegCmds();
		OdString sCommand = strCommand.spanExcluding(L" \t\r\n");

		if (sCommand.isEmpty()) // User did not enter a command
		{
			if (m_strLastCommand.isEmpty()) // There is no last saved command
			{
				putString(_T("Type a command name at the prompt and press 'Enter'"));
				return FALSE;
			}
			sCommand = m_strLastCommand; // Use last command
		}

		if (bEcho)
			putString(_T("Command: ") + sCommand);

		OdEdCommand* pCommand = pCommands->lookupCmd(sCommand); // Check if command is valid
		if (pCommand == NULL)
			putString(_T("Unknown command '") + sCommand + _T("'"));
		else
		{
			m_bCommandActive = TRUE;
			pCommands->executeCommand(sCommand, pCmdCtx); // Execute the command
			m_strLastCommand = sCommand; // Save as last command
			GetView()->Refresh();
			m_bCommandActive = FALSE;
		}
	}
	catch (const OdError& e)
	{
		m_bCommandActive = FALSE;
		putString(_T("Error: ") + e.description());
		GetView()->Refresh();
		return FALSE;
	}
	return TRUE;
}


OdEdBaseIO* COCADDoc::GetIO()
{
	return this;
}

COCADView* COCADDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	return ((COCADView*)GetNextView(pos));
}

OdString COCADDoc::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
	/*if (theApp.AllowInteraction() == FALSE)
	return OdString(_T(""));

	GetView()->Track(pTracker);

	CString strText;
	GetChildFrame()->m_wndCommandPrompt.GetString((LPCTSTR)prompt, strText);
	return OdString(strText);*/

	return OdString::kEmpty;
}

void COCADDoc::putString(const OdString& string)
{
	//GetChildFrame()->m_wndCommandPrompt.PutString((LPCTSTR)string);
}

OdGePoint3d COCADDoc::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
	//if (m_pMacro.get() && !m_pMacro->isEof())
	//{
	//	console()->putString(prompt);
	//	return m_pMacro->getPoint(prompt, options, pTracker);
	//}

	//if (m_bConsole)
	//	return m_pConsole->getPoint(prompt, options, pTracker);

	if (GetView())
	{
		//console()->putString(prompt);
		return GetView()->getPoint(prompt, options, pTracker);
	}

	//return console()->getPoint(prompt, options, pTracker);
	return OdGePoint3d();
}
OdUInt32 COCADDoc::getKeyState()
{
	OdUInt32 res = 0;
	if (::GetKeyState(VK_CONTROL) != 0)
		res |= MK_CONTROL;
	if (::GetKeyState(VK_SHIFT) != 0)
		res |= MK_SHIFT;
	return res;
}

OdDbCommandContextPtr COCADDoc::cmdCtx()
{
	if (m_pCmdCtx.isNull())
	{
		m_pCmdCtx = ExDbCommandContext::createObject(cmdIO(), m_pDb);
	}
	return m_pCmdCtx;
}

OdEdBaseIO* COCADDoc::cmdIO()
{
	return this;
}

//void COCADDoc::startDrag(const OdGePoint3d& pt)
//{
//	DataSource ds;
//	ds.Create(this, pt);
//	if (ds.DoDragDrop())
//		UpdateAllViews(0);
//}

OdDbSelectionSetPtr COCADDoc::selectionSet() const
{
	OdDbCommandContext* pCtx = const_cast<COCADDoc*>(this)->cmdCtx();
	OdDbSelectionSetPtr pRes = pCtx->arbitraryData(L"OdaMfcApp Working Selection Set");
	if (pRes.isNull())
	{
		pRes = OdDbSelectionSet::createObject(m_pDb);
		pCtx->setArbitraryData(L"OdaMfcApp Working Selection Set", pRes);
	}
	ODA_TRACE1("Working Selection set contains %d items\n", pRes->numEntities());
	return pRes;
}

//void COCADDoc::DataSource::Create(COCADDoc* pDoc, const OdGePoint3d& p2)
//{
//	Empty();
//
//	OdDbObjectIdArray objs = pDoc->selectionSet()->objectIdArray();
//
//	OdDbDatabasePtr pDb = pDoc->m_pDb->wblock(objs, OdGePoint3d::kOrigin);
//	OdString tmpPath = pDoc->m_pDb->appServices()->getTempPath();
//	TCHAR tempname[MAX_PATH];
//	::GetTempFileName(tmpPath/*.c_str()*/, _T(""), 0, tempname);
//	m_tmpPath = tempname;
//	m_tmpPath.makeLower();
//	m_tmpPath.replace(L".tmp", L".dwg");
//	OdStreamBufPtr sbuf = theApp.createFile(m_tmpPath, Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateNew);
//
//	//pDb->writeFile(sbuf,OdDb::kDwg,OdDb::vAC21);
//	//HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED, sizeof(AcadClipDataR15));
//	//new (hGlobal) AcadClipDataR15( m_tmpPath, OdString(pDoc->GetPathName()), p2 );
//	//CacheGlobalData(ClipboardData::m_FormatR16, hGlobal);
//
//	pDb->writeFile(sbuf, OdDb::kDwg, OdDb::vAC21);
//	HGLOBAL hGlobalR21 = GlobalAlloc(GMEM_FIXED, sizeof(AcadClipDataR21));
//	new (hGlobalR21) AcadClipDataR21(m_tmpPath, OdString(pDoc->GetPathName()), p2);
//	CacheGlobalData(ClipboardData::m_FormatR17, hGlobalR21);
//}
//
//COCADDoc::DataSource::DataSource()
//{
//}
//
//bool COCADDoc::DataSource::DoDragDrop()
//{
//	return (COleDataSource::DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE) != DROPEFFECT_NONE);
//}
//
//void COCADDoc::DataSource::Empty()
//{
//	COleDataSource::Empty();
//	if (!m_tmpPath.isEmpty())
//		DeleteFile(m_tmpPath);
//}
//
//COCADDoc::DataSource::~DataSource()
//{
//	Empty();
//}

void COCADDoc::OnTest()
{
	// TODO: 在此添加命令处理程序代码
	//PrintModules(GetCurrentProcessId());
	//cmdCtx()->dbUserIO()->getPoint(L"pt");
	//OdDbUserIOPtr pUserIO = cmdCtx()->userIO();
	//pUserIO->getPoint(L"pt");
	//MessageBox(GetView()->GetSafeHwnd(), L"Test Complate", NULL, NULL);
	ExecuteCommand("line");
}




void COCADDoc::OnTest2()
{
	// TODO: 在此添加命令处理程序代码
	OdDbUserIOPtr pUserIO = cmdCtx()->userIO();
	pUserIO->getPoint(L"pt");
}
