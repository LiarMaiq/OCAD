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

// SrvrItem.h : COCADSrvrItem ��Ľӿ�
//

#pragma once

class COCADSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(COCADSrvrItem)

// ���캯��
public:
	COCADSrvrItem(COCADDoc* pContainerDoc);

// ����
	COCADDoc* GetDocument() const
		{ return reinterpret_cast<COCADDoc*>(COleServerItem::GetDocument()); }

// ��д
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// ʵ��
public:
	~COCADSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
};

