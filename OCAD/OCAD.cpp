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

// OCAD.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// COCADApp ����

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
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("OCAD.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

COCADApp::~COCADApp()
{
	for (INT_PTR i = 0; i < m_tempFilesList.GetCount(); ++i)
	{
		::DeleteFile(m_tempFilesList[i]);
	}
}

// Ψһ��һ�� COCADApp ����

COCADApp theApp;
// ���ɵĴ˱�ʶ����ͳ���϶�������Ӧ�ó�����Ψһ��
// �������Ըѡ��һ���ض��ı�ʶ��������Ը�����

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

// COCADApp ��ʼ��

BOOL COCADApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	//
	// ��ʼ��Teigha
	InitializeTeigha();
	OdRxModulePtr pModCmds = ::odrxDynamicLinker()->loadModule(OdString("Commands_4.01src_14.dll"),false);


	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COCADDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(COCADView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetServerInfo(
		IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);
	// �� COleTemplateServer ���ӵ��ĵ�ģ��
	// COleTemplateServer ͨ��ʹ��
	// �ĵ�ģ����ָ������Ϣ��Ϊ���� OLE ����
	// �������ĵ�
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// ע��: �����������д��� /Embedding �� /Automation ʱ��SDI Ӧ�ó���
		//   ��ע�����������


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Ӧ�ó������� /Embedding �� /Automation ���������ġ�
	//ʹӦ�ó�����Ϊ�Զ������������С�
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// �����е� OLE ����������ע��Ϊ�������С�  �⽫����
		//  OLE ���Դ�����Ӧ�ó����д�������
		COleTemplateServer::RegisterAll();

		// ����ʾ������
		return TRUE;
	}
	// ʹ�� /Unregserver �� /Unregister ��������Ӧ�ó���  ע��
	// ���Ϳ⡣  ����ע�������� ProcessShellCommand() �з�����
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_INPLACE_SERVER, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// Ӧ�ó������Զ�����ʽ������������(�� /Register
	// �� /Regserver)�����ġ�  ����ע�����������Ϳ⡣
	else
	{
		m_server.UpdateRegistry(OAT_INPLACE_SERVER);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int COCADApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	//
	//
	UninitializeTeigha();

	return CWinAppEx::ExitInstance();
}

// COCADApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void COCADApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// COCADApp �Զ������/���淽��

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

// COCADApp ��Ϣ�������



