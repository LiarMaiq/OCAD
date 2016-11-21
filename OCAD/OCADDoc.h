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

// OCADDoc.h : COCADDoc ��Ľӿ�
//


#pragma once


//
// ODA
#include "DbDatabase.h"
#include "../Extensions/ExServices/ExDbCommandContext.h"


class COCADSrvrItem;
class COCADView;


class COdaMfcAppDocStaticRxObjects : public OdDbLayoutManagerReactor
	, public OdEdBaseIO
{
	ODRX_NO_HEAP_OPERATORS();
};

class COCADDoc : public COleServerDoc,
	protected OdStaticRxObject<COdaMfcAppDocStaticRxObjects>
{
protected:
	using COleServerDoc::operator new;
	using COleServerDoc::operator delete;

	//
	// ODA
public:
	BOOL ExecuteCommand(const OdString& strCommand, BOOL bEcho = TRUE);
	OdEdBaseIO* GetIO();
	COCADView* GetView();
	OdDbCommandContextPtr cmdCtx();
	// OdEdBaseIO
	OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
	void putString(const OdString& string);
	OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);
	virtual OdUInt32 getKeyState();
	OdEdBaseIO* cmdIO();
//	void startDrag(const OdGePoint3d& pt);
	OdDbSelectionSetPtr selectionSet() const;

public:
	OdDbDatabasePtr			m_pDb;
	OdString				m_strLastCommand;
	BOOL					m_bCommandActive;
	OdDbCommandContextPtr	m_pCmdCtx;

protected:
	/*class DataSource : COleDataSource
	{
		friend class COCADDoc;
		OdString m_tmpPath;
	public:
		DataSource();
		void Create(COCADDoc* pDoc, const OdGePoint3d& p2 = OdGePoint3d::kOrigin);
		bool DoDragDrop();
		void Empty();
		~DataSource();
	};*/


	template<class TChar>
	struct AcadClipData
	{
		void init()
		{
			memset(this, 0, sizeof(AcadClipData<TChar>));
		}
		void read(CFile* pFile)
		{
			pFile->Read(this, sizeof(AcadClipData<TChar>));
		}

		TChar _tempFileName[0x104];   // name of the temp dwg file, where dragged entities are
		TChar _origFileName[0x104];   // original file name
		TChar _version[4];            // version of the original file, e.g. 'R15'
		int _one1;                    // seem to be always 1
		double _x, _y, _z;            // pick point
		int _zero1;                   // seem to be always zero
		int _one2;                    // seem to be always 1
		int _unk[4];                  //
		int _zero2[4];                // seem to be always zero
	};

	template<class TChar>
	struct AcadClipDataConstr : public AcadClipData<TChar>
	{
		AcadClipDataConstr(const OdString& tempFileName, const OdString& origFileName, const OdGePoint3d& pickPoint)
		{
			init();
			_one1 = 1;
			_one2 = 1;
			_version[0] = 'R';
			_version[1] = '1';
			_version[2] = '5';
			_version[3] = 0;
			_x = pickPoint.x;
			_y = pickPoint.y;
			_z = pickPoint.z;
			memcpy(_tempFileName, (const TChar*)tempFileName, odmin((0x100 * sizeof(TChar)), ((tempFileName.getLength() + 1) * sizeof(TChar))));
			memcpy(_origFileName, (const TChar*)origFileName, odmin((0x100 * sizeof(TChar)), ((origFileName.getLength() + 1) * sizeof(TChar))));
		}
		AcadClipDataConstr()
		{
			init();
		}
	};
	typedef AcadClipDataConstr<char> AcadClipDataR15;
	typedef AcadClipDataConstr<OdChar> AcadClipDataR21;

public:
	class ClipboardData
	{
	public:
		static unsigned short m_FormatR15;
		static unsigned short m_FormatR16;
		static unsigned short m_FormatR17;
		static unsigned short m_FormatR18;
		static unsigned short m_FormatR19;
		static bool isAcadDataAvailable(COleDataObject* pDataObject, bool bAttach = false)
		{
			if (bAttach && !pDataObject->AttachClipboard())
			{
				return false;
			}
			return pDataObject->IsDataAvailable(m_FormatR15)
				|| pDataObject->IsDataAvailable(m_FormatR16)
				|| pDataObject->IsDataAvailable(m_FormatR17)
				|| pDataObject->IsDataAvailable(m_FormatR18)
				|| pDataObject->IsDataAvailable(m_FormatR19);
		}
		static OdSharedPtr<ClipboardData> get(COleDataObject* pDataObject, bool bAttach = false)
		{
			if (bAttach && !pDataObject->AttachClipboard())
				return 0;

			OdSharedPtr<ClipboardData> pData = new ClipboardData();
			if (pData->read(pDataObject))
				return pData;
			return 0;
		}

		ClipboardData()
			: _isR15format(false)
		{
		}

		bool read(COleDataObject* pDataObject)
		{
			OdSharedPtr<CFile> pFile = 0;
			if ((pFile = pDataObject->GetFileData(m_FormatR15)).get()
				|| (pFile = pDataObject->GetFileData(m_FormatR16)).get())
			{
				_isR15format = true;
				_data._r15.read(pFile);
				return true;
			}
			else if ((pFile = pDataObject->GetFileData(m_FormatR17)).get()
				|| (pFile = pDataObject->GetFileData(m_FormatR18)).get()
				|| (pFile = pDataObject->GetFileData(m_FormatR19)).get()
				)
			{
				_isR15format = false;
				_data._r21.read(pFile);
				return true;
			}
			else
			{
				return false;
			}
		}
		OdString tempFileName() { return _isR15format ? OdString(_data._r15._tempFileName) : OdString(_data._r21._tempFileName); }
		OdGePoint3d pickPoint() { return _isR15format ? OdGePoint3d(_data._r15._x, _data._r15._y, _data._r15._z) : OdGePoint3d(_data._r21._x, _data._r21._y, _data._r21._z); }

	private:
		union Data
		{
			AcadClipData<char>   _r15;
			AcadClipData<OdChar> _r21;
			Data() { _r21.init(); }
		} _data;
		bool _isR15format;
	};








protected: // �������л�����
	COCADDoc();
	DECLARE_DYNCREATE(COCADDoc)

// ����
public:
	COCADSrvrItem* GetEmbeddedItem()
		{ return reinterpret_cast<COCADSrvrItem*>(COleServerDoc::GetEmbeddedItem()); }

// ����
public:

// ��д
protected:
	virtual COleServerItem* OnGetEmbeddedItem();
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~COCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnTest();
	afx_msg void OnTest2();
};
