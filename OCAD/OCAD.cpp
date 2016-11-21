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

// OCAD.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include <afxinet.h>
#include "OCAD.h"
#include "MainFrm.h"

#include "IpFrame.h"
#include "OCADDoc.h"
#include "OCADView.h"

#include "ColorMapping.h"
#include "RxVariantValue.h"
#include "../Imports/DwfImport/Include/DwfImport.h"
#include "../Imports/DgnImport/DgnImport.h"
using namespace TD_DGN_IMPORT;
using namespace TD_DWF_IMPORT;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// AE 15.08.2003 - Begin
// AMark 19.05.2008 - Added GetRegistryAVEMAPSFromProfile()
#define SMALL_STRING_SIZE 32
#define LARGE_STRING_SIZE 1024
#define MAX_PATH_LENGTH 1024
OdString GetRegistryAcadLocation();
OdString GetRegistryACADFromProfile();
OdString GetRegistryAVEMAPSFromProfile();
OdString GetRegistryAcadProfilesKey();
bool GetRegistryString(HKEY key, const wchar_t *subkey, const wchar_t *name, wchar_t *value, int size);
bool GetRegistryString(HKEY key, const char *subkey, const char *name, char *value, int size);
HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);


// Find Plot Config file or Style Sheet file
//
static OdString FindConfigPath(const OdString& configType)
{
	TCHAR searchPath[LARGE_STRING_SIZE];
	TCHAR expandedPath[LARGE_STRING_SIZE];

	OdString subkey = GetRegistryAcadProfilesKey();
	if (!subkey.isEmpty())
	{
		subkey += L"\\General";
		// get the value for the ACAD entry in the registry
		if (::GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, (LPCTSTR)configType, searchPath, LARGE_STRING_SIZE))
		{
			ExpandEnvironmentStrings(searchPath, expandedPath, LARGE_STRING_SIZE);
			return OdString(expandedPath);
		}
	}
	return OdString::kEmpty;
}

static OdString FindConfigFile(const OdString& configType, OdString file, OdDbSystemServices* pSs)
{
	OdString searchPath = FindConfigPath(configType);
	if (!searchPath.isEmpty())
	{
		file = searchPath + L"\\" + file;
		if (pSs->accessFile(file, Oda::kFileRead)) return file;
	}
	return OdString::kEmpty;
}

//////////////////////////////////////////////////////////////////////
OdString COCADApp::findFile(const OdString& file, OdDbBaseDatabase* pDb,
	OdDbBaseHostAppServices::FindFileHint hint)
{
	OdString sFile = ExHostAppServices::findFile(file, pDb, hint);
	if (!sFile.isEmpty())
		return sFile;

	OdDbSystemServices* pSs = odSystemServices();

	OdString sFileName = file;
	OdString ext = sFileName.right(4);
	ext.makeUpper();

	if (ext == L".PC3")
		return FindConfigFile(L"PrinterConfigDir", file, pSs);

	if (ext == L".STB" || ext == L".CTB")
		return FindConfigFile(L"PrinterStyleSheetDir", file, pSs);

	if (ext == L".PMP")
		return FindConfigFile(L"PrinterDescDir", file, pSs);

	switch (hint)
	{
	case kFontFile:
	case kCompiledShapeFile:
	case kTrueTypeFontFile:
	case kPatternFile:
	case kFontMapFile:
	case kTextureMapFile:
		break;
	case kEmbeddedImageFile:
		if (sFileName.left(5).iCompare(L"http:") == 0 || sFileName.left(6).iCompare(L"https:") == 0)
		{
			CInternetSession iss;
			if (CStdioFile* f = iss.OpenURL(sFileName))
			{
				ULONGLONG len = f->GetLength();
				OdString tmpPath = getTempPath();
				TCHAR temp[MAX_PATH_LENGTH + 1] = { _T('\0') };
				::GetTempFileName(tmpPath, _T(""), 0, temp);
				CStdioFile tempFile(temp, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
				char buf[10000];
				do
				{
					UINT N = 10000;
					if (N > len)
						N = (UINT)len;
					f->Read(buf, N);
					tempFile.Write(buf, N);
					len -= N;
				} while (len != 0);
				tempFile.Close();
				f->Close();
				delete f;
				m_tempFilesList.Add(temp);
				sFile = temp;
			}
		}
		// fall through
	default:
		return sFile;
	}

	if (hint != kTextureMapFile && ext != L".SHX" && ext != L".PAT" && ext != L".TTF" && ext != L".TTC" && ext != L".OTF")
	{
		sFileName += L".shx";
	}
	else if (hint == kTextureMapFile)
	{
		sFileName.replace(L'/', L'\\');
		sFileName.deleteChars(0, sFileName.reverseFind(L'\\') + 1);
	}

	sFile = (hint != kTextureMapFile) ? GetRegistryACADFromProfile() : GetRegistryAVEMAPSFromProfile();
	while (!sFile.isEmpty())
	{
		int nFindStr = sFile.find(L";");
		OdString sPath;
		if (-1 == nFindStr)
		{
			sPath = sFile;
			sFile.empty();
		}
		else
		{
			sPath = sFile.left(nFindStr);
			sPath += L"\\";
			sPath += sFileName;
			if (pSs->accessFile(sPath, Oda::kFileRead))
			{
				return sPath;
			}
			sFile = sFile.right(sFile.getLength() - nFindStr - 1);
		}
	}

	if (hint == kTextureMapFile)
	{
		return sFile;
	}

	if (sFile.isEmpty())
	{
		OdString sAcadLocation(GetRegistryAcadLocation());
		if (!sAcadLocation.isEmpty())
		{
			sFile = sAcadLocation;
			sFile += L"\\Fonts\\";
			sFile += sFileName;
			if (!pSs->accessFile(sFile, Oda::kFileRead))
			{
				sFile = sAcadLocation;
				sFile += L"\\Support\\";
				sFile += sFileName;
				if (!pSs->accessFile(sFile, Oda::kFileRead))
				{
					sFile = L"";
				}
			}
		}
	}
	return sFile;
}


//////////////////////////////////////////////////////////////////////
bool GetRegistryString(HKEY key, const char *subkey, const char *name, char *value, int size)
{
	wchar_t *wSubkey = NULL;
	wchar_t *wName = NULL;
	char *tmpChar = NULL;
	bool rv = false;
	AnsiToUnicode(subkey, &wSubkey);
	AnsiToUnicode(name, &wName);
	if (wSubkey && wName)
	{
		wchar_t *wValue = new wchar_t[size];
		rv = GetRegistryString(key, wSubkey, wName, wValue, size);
		if (rv)
		{
			UnicodeToAnsi(wValue, &tmpChar);
			if (tmpChar)
			{
				strcpy(value, tmpChar);
				CoTaskMemFree(tmpChar);
			}
			else
			{
				rv = false;
			}
		}
		delete[] wValue;
		CoTaskMemFree(wName);
		CoTaskMemFree(wSubkey);
	}

	return rv;
}


//////////////////////////////////////////////////////////////////////
bool GetRegistryString(HKEY key, const wchar_t *subkey, const wchar_t *name, wchar_t *value, int size)
{
	bool rv = false;
	HKEY hKey;
	if (RegOpenKeyExW(key, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwSize(MAX_PATH_LENGTH);
		unsigned char data[MAX_PATH_LENGTH];
		memset(&data, 0x00, MAX_PATH_LENGTH);
		if (RegQueryValueExW(hKey, name, 0, 0, &data[0], &dwSize) == ERROR_SUCCESS)
		{
			rv = true;
		}
		else
		{
			if (ERROR_SUCCESS == RegEnumKeyExW(hKey, 0, (LPWSTR)(unsigned short*)&data[0], &dwSize
				, NULL, NULL, NULL, NULL))
			{
				rv = true;
			}
		}

		if (size < MAX_PATH_LENGTH)
		{
			swprintf(value, L"%s\0", data);
		}
		else
		{
			wcsncpy(value, (wchar_t*)data, size - 1);
			value[size - 1] = '\0';
		}

		RegCloseKey(hKey);
	}
	return rv;
}


//////////////////////////////////////////////////////////////////////
OdString GetRegistryAcadLocation()
{
	OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
	TCHAR version[SMALL_STRING_SIZE];
	TCHAR subVersion[SMALL_STRING_SIZE];
	TCHAR searchPaths[LARGE_STRING_SIZE];

	// get the version and concatenate onto subkey
	if (GetRegistryString(HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
		return L"";
	subkey += L"\\";
	subkey += version;



	// get the sub-version and concatenate onto subkey
	if (GetRegistryString(HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
		return L"";
	subkey += L"\\";
	subkey += subVersion;



	// get the value for the AcadLocation entry in the registry
	if (GetRegistryString(HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("AcadLocation"), searchPaths, LARGE_STRING_SIZE) == 0)
		return L"";

	return OdString(searchPaths);
}

//////////////////////////////////////////////////////////////////////
OdString GetRegistryAcadProfilesKey()
{
	OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
	TCHAR version[SMALL_STRING_SIZE];
	TCHAR subVersion[SMALL_STRING_SIZE];
	TCHAR profile[LARGE_STRING_SIZE];
	// char searchPaths[ LARGE_STRING_SIZE];

	if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
		return L"";
	subkey += L"\\";
	subkey += version;


	// get the sub-version and concatenate onto subkey
	if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
		return L"";
	subkey += L"\\";
	subkey += subVersion;
	subkey += L"\\Profiles";


	// get the value for the (Default) entry in the registry
	if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T(""), profile, LARGE_STRING_SIZE) == 0)
		return L"";
	subkey += L"\\";
	subkey += profile;

	return subkey;
}

//////////////////////////////////////////////////////////////////////
OdString GetRegistryACADFromProfile()
{
	OdString subkey; // = "SOFTWARE\\Autodesk\\AutoCAD";
	TCHAR searchPaths[LARGE_STRING_SIZE];
	TCHAR expandedPath[LARGE_STRING_SIZE];

	subkey = GetRegistryAcadProfilesKey();
	if (!subkey.isEmpty())
	{
		subkey += L"\\General";
		// get the value for the ACAD entry in the registry
		if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("ACAD"), searchPaths, LARGE_STRING_SIZE) == 0)
			return L"";

		ExpandEnvironmentStrings(searchPaths, expandedPath, LARGE_STRING_SIZE);
		return OdString(expandedPath);
	}
	else
		return L"";
}

OdString GetRegistryAVEMAPSFromProfile()
{
	OdString subkey; // = "SOFTWARE\\Autodesk\\AutoCAD";
	TCHAR searchPaths[LARGE_STRING_SIZE];
	TCHAR expandedPath[LARGE_STRING_SIZE];

	subkey = GetRegistryAcadProfilesKey();
	if (!subkey.isEmpty())
	{
		subkey += L"\\General";
		// get the value for the ACAD entry in the registry
		if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("AVEMAPS"), searchPaths, LARGE_STRING_SIZE) == 0)
			return L"";

		ExpandEnvironmentStrings(searchPaths, expandedPath, LARGE_STRING_SIZE);
		return OdString(expandedPath);
	}
	else
		return L"";
}


/*
* AnsiToUnicode converts the ANSI string pszA to a Unicode string
* and returns the Unicode string through ppszW. Space for the
* the converted string is allocated by AnsiToUnicode.
*/
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{
	// If input is null then just return the same.
	if (NULL == pszA)
	{
		*ppszW = NULL;
		return NOERROR;
	}

	// Determine number of wide characters to be allocated for the
	// Unicode string.
	int cCharacters = int(strlen(pszA) + 1);

	// Use of the OLE allocator is required if the resultant Unicode
	// string will be passed to another COM component and if that
	// component will free it. Otherwise you can use your own allocator.
	*ppszW = (LPOLESTR)CoTaskMemAlloc(cCharacters * 2);
	if (NULL == *ppszW)
		return E_OUTOFMEMORY;

	// Covert to Unicode.
	if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
		*ppszW, cCharacters))
	{
		DWORD dwError = GetLastError();
		CoTaskMemFree(*ppszW);
		*ppszW = NULL;
		return HRESULT_FROM_WIN32(dwError);
	}

	return NOERROR;
}
/*
* UnicodeToAnsi converts the Unicode string pszW to an ANSI string
* and returns the ANSI string through ppszA. Space for the
* the converted string is allocated by UnicodeToAnsi.
*/

HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{
	// If input is null then just return the same.
	if (pszW == NULL)
	{
		*ppszA = NULL;
		return NOERROR;
	}

	int cCharacters = int(wcslen(pszW) + 1);
	// Determine number of bytes to be allocated for ANSI string. An
	// ANSI string can have at most 2 bytes per character (for Double
	// Byte Character Strings.)
	int cbAnsi = cCharacters * 2;

	// Use of the OLE allocator is not required because the resultant
	// ANSI  string will never be passed to another COM component. You
	// can use your own allocator.
	*ppszA = (LPSTR)CoTaskMemAlloc(cbAnsi);
	if (NULL == *ppszA)
		return E_OUTOFMEMORY;

	// Convert to ANSI.
	if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
		cbAnsi, NULL, NULL))
	{
		DWORD dwError = GetLastError();
		CoTaskMemFree(*ppszA);
		*ppszA = NULL;
		return HRESULT_FROM_WIN32(dwError);
	}
	return NOERROR;

}

// AE - End













// COCADApp

BEGIN_MESSAGE_MAP(COCADApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &COCADApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// COCADApp 构造

const ODCOLORREF * COCADApp::curPalette() const
{
	return odcmAcadPalette(m_background);
}

void COCADApp::setStatusText(LPCTSTR msg)
{
	((CMainFrame*)GetMainWnd())->setStatusText(msg);
}

void COCADApp::setStatusText(int nCol, LPCTSTR msg)
{
	((CMainFrame*)GetMainWnd())->setStatusText(nCol, msg);
}

COCADApp::COCADApp():
	// ODA_MT_DBIO_BEGIN
	m_bUseMTLoading(false)
	// ODA_MT_DBIO_END
	, m_bLoading(false)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OCAD.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

COCADApp::~COCADApp()
{
	for (INT_PTR i = 0; i < m_tempFilesList.GetCount(); ++i)
	{
		::DeleteFile(m_tempFilesList[i]);
	}
}

// 唯一的一个 COCADApp 对象

COCADApp theApp;
// 生成的此标识符在统计上对于您的应用程序是唯一的
// 如果您更愿选择一个特定的标识符，则可以更改它

// {4B3DB332-8607-4A3E-97B9-81850B06D49F}
static const CLSID clsid =
{ 0x4B3DB332, 0x8607, 0x4A3E, { 0x97, 0xB9, 0x81, 0x85, 0xB, 0x6, 0xD4, 0x9F } };

const GUID CDECL _tlid = { 0xE6F6A255, 0xDAFC, 0x4263, { 0xA6, 0x53, 0x8C, 0x7C, 0xAC, 0xD4, 0x3C, 0xB4 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

BOOL COCADApp::InitializeTeigha()
{
	try
	{
		::odInitialize(this);
	}
	catch (OdError& /*err*/)
	{
		AfxMessageBox(_T("Teigha Initialization error"));
		return FALSE;
	}
	catch (...)
	{
		AfxMessageBox(_T("Teigha Initialization error"));
		return FALSE;
	}
	return TRUE;
}

void COCADApp::UninitializeTeigha()
{
	::odUninitialize();
}

OdDbDatabasePtr COCADApp::openFile(LPCTSTR lpszPathName)
{
	CMainFrame* pMFrame = (CMainFrame*)GetMainWnd();
	OdDbDatabasePtr pDb;

	// ODA_MT_DBIO_BEGIN
	OdInt16 nMode = getMtMode();
	SETBIT(nMode, 1, m_bUseMTLoading);
	setMtMode(nMode);
	// ODA_MT_DBIO_END

	// open an existing document
	pMFrame->StartTimer();
	try
	{
		CWaitCursor   wait;
		OdString path(lpszPathName);
		OdString shxPath;

		OdString sFile = GetRegistryACADFromProfile();

		while (!sFile.isEmpty())
		{
			int nFindStr = sFile.find(L";");
			OdString sPath;

			if (-1 == nFindStr)
			{
				sPath = sFile;
				sFile.empty();
			}
			else
			{
				sPath = sFile.left(nFindStr);
				sPath += L"\\";

				OdString sPathU = sPath;
				sPathU.makeUpper();

				if (sPathU.find(L"FONTS") != -1)
				{
					shxPath = sPath;
					break;
				}

				sFile = sFile.right(sFile.getLength() - nFindStr - 1);
			}
		}

		if (path.right(4).iCompare(L".dgn") == 0)
		{
			//       OdStringArray arrRsrc;
			//       arrRsrc.push_back( L"D:/font.rsc" );
			OdDgnImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnImportModuleName, false);
			OdDgnImportPtr importer = pModule->create();
			importer->properties()->putAt(L"Services", static_cast<ExHostAppServices*>(this));
			importer->properties()->putAt(L"DgnPath", OdRxVariantValue(path));
			//      importer->properties()->putAt( L"MS_SYMBRSRC", OdRxVariantValue(arrRsrc) );
			importer->properties()->putAt(L"XRefImportMode", OdRxVariantValue(OdUInt8(2)));
			importer->properties()->putAt(L"ImportActiveModelToModelSpace", OdRxVariantValue(true));
			importer->properties()->putAt(L"ImportPaperSpaceModels", OdRxVariantValue(true));
			importer->properties()->putAt(L"RecomputeDimensionsAfterImport", OdRxVariantValue(false));
			importer->properties()->putAt(L"ImportViewIndex", OdRxVariantValue(OdUInt8(0)));
			importer->properties()->putAt(L"3dShapeImportMode", OdRxVariantValue(OdUInt8(1)));
			importer->properties()->putAt(L"shxFontsPath", OdRxVariantValue(shxPath));

			OdDgnImport::ImportResult res = importer->import();

			if (res == OdDgnImport::success)
				pDb = importer->properties()->getAt(L"Database");
			else
			{
				switch (res)
				{
				case OdDgnImport::bad_database:
					messageBox(_T("DGN import"), _T("Bad database"), MB_OK | MB_ICONERROR);
					break;
				case OdDgnImport::bad_file:
					messageBox(_T("DGN import"), _T("Bad file"), MB_OK | MB_ICONERROR);
					break;
				case OdDgnImport::encrypted_file:
				case OdDgnImport::bad_password:
					messageBox(_T("DGN import"), _T("The file is encrypted"), MB_OK | MB_ICONERROR);
					break;
				case OdDgnImport::fail:
					messageBox(_T("DGN import"), _T("Unknown import error"), MB_OK | MB_ICONERROR);
					break;
				}
			}
			pMFrame->StopTimer(_T("Loading"));
		}
		else if (path.right(4).iCompare(L".dwf") == 0 || path.right(5).iCompare(L".dwfx") == 0)
		{
			OdDwfImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ImportModuleName);
			OdDwfImportPtr importer = pModule->create();
			OdRxDictionaryPtr pProps = importer->properties();
			pDb = createDatabase();
			pProps->putAt(OD_T("Database"), pDb);
			pProps->putAt(OD_T("DwfPath"), OdRxVariantValue(path));
			pProps->putAt(OD_T("PreserveColorIndices"), OdRxVariantValue(false));
			pProps->putAt(OD_T("DwgPalette"), OdRxVariantValue(OdIntPtr(curPalette())));
			switch (importer->import())
			{
			case OdDwfImport::success:
				break;
			case OdDwfImport::bad_password:
				messageBox(_T("DWF import"), _T("The file is encrypted"), MB_OK | MB_ICONERROR);
				break;
			default:
				messageBox(_T("DWF import"), _T("Import error"), MB_OK | MB_ICONERROR);
				break;
			}
			OdRxVariantValue backGround = (OdRxObject*)pProps->getAt(OD_T("Background"));
			setActiveBackground((ODCOLORREF)backGround->getInt32());
			pMFrame->StopTimer(_T("Loading"));
		}
		//else if (m_bRecover)
		//{
		//	ODA_ASSERT(!m_pAuditDlg);
		//	m_pAuditDlg = new CAuditDlg();
		//	ODA_ASSERT(m_pAuditDlg);
		//	ODA_VERIFY(m_pAuditDlg->Create(IDD_AUDITINFO));

		//	CAppAuditInfo aiInfo;
		//	aiInfo.setHostAppServices(&theApp);

		//	pDb = recoverFile(createFile(OdString(lpszPathName)), &aiInfo);
		//	pMFrame->StopTimer(_T("Recovering"));

		//	// Show recover result
		//	CString strTitle = _T("Recover info ");
		//	strTitle += lpszPathName;
		//	m_pAuditDlg->SetWindowText(strTitle);
		//	m_pAuditDlg->ShowWindow(SW_SHOW);

		//	// Dialog will be destroyed on exit
		//	m_pAuditDlg = NULL;
		//}
		else
		{
			m_bLoading = true;
			pDb = readFile(OdString(lpszPathName), false, m_bPartial);
			pMFrame->StopTimer(_T("Loading"));
			m_bLoading = false;
		}
	}
	catch (const OdError& e)
	{
		pDb = 0;
		pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
		reportError(_T("Loading Error..."), e);
	}
	catch (const UserBreak&)
	{
		pDb = 0;
		pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
		setStatusText(_T("Operation was canceled by user."));
	}
	catch (std::bad_alloc&)
	{
		pDb = 0;
		pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
		setStatusText(_T("Memory Allocation Error..."));
	}

	// Destroy audit dialog if recover failed
	/*if (m_pAuditDlg)
	{
		delete m_pAuditDlg;
		m_pAuditDlg = 0;
	}*/

	return pDb;
}

// COCADApp 初始化

BOOL COCADApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	//
	// 初始化Teigha
	InitializeTeigha();
	OdRxModulePtr pModCmds = ::odrxDynamicLinker()->loadModule(OdString("Commands_4.01src_14.dll"),false);


	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COCADDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(COCADView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetServerInfo(
		IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);
	// 将 COleTemplateServer 连接到文档模板
	// COleTemplateServer 通过使用
	// 文档模板中指定的信息来为请求 OLE 容器
	// 创建新文档
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// 注意: 仅当命令行中存在 /Embedding 或 /Automation 时，SDI 应用程序
		//   才注册服务器对象


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 应用程序是用 /Embedding 或 /Automation 开关启动的。
	//使应用程序作为自动化服务器运行。
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 将所有的 OLE 服务器工厂注册为正在运行。  这将启用
		//  OLE 库以从其他应用程序中创建对象
		COleTemplateServer::RegisterAll();

		// 不显示主窗口
		return TRUE;
	}
	// 使用 /Unregserver 或 /Unregister 开关启动应用程序。  注销
	// 类型库。  其他注销操作在 ProcessShellCommand() 中发生。
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_INPLACE_SERVER, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// 应用程序是以独立方式或用其他开关(如 /Register
	// 或 /Regserver)启动的。  更新注册表项，包括类型库。
	else
	{
		m_server.UpdateRegistry(OAT_INPLACE_SERVER);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int COCADApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	//
	//
	UninitializeTeigha();

	return CWinAppEx::ExitInstance();
}

// COCADApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void COCADApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COCADApp 自定义加载/保存方法

void COCADApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void COCADApp::LoadCustomState()
{
}

void COCADApp::SaveCustomState()
{
}

// COCADApp 消息处理程序



