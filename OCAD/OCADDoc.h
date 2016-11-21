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

// OCADDoc.h : COCADDoc 类的接口
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








protected: // 仅从序列化创建
	COCADDoc();
	DECLARE_DYNCREATE(COCADDoc)

// 特性
public:
	COCADSrvrItem* GetEmbeddedItem()
		{ return reinterpret_cast<COCADSrvrItem*>(COleServerDoc::GetEmbeddedItem()); }

// 操作
public:

// 重写
protected:
	virtual COleServerItem* OnGetEmbeddedItem();
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~COCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	// 生成的 OLE 调度映射函数

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnTest();
	afx_msg void OnTest2();
};
