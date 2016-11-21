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

// SrvrItem.cpp : COCADSrvrItem ���ʵ��
//

#include "stdafx.h"
#include "OCAD.h"

#include "OCADDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COCADSrvrItem ʵ��

IMPLEMENT_DYNAMIC(COCADSrvrItem, COleServerItem)

COCADSrvrItem::COCADSrvrItem(COCADDoc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO: �ڴ����һ���Թ������
	//  (�罫���Ӽ������ʽ��ӵ��������Դ��)
}

COCADSrvrItem::~COCADSrvrItem()
{
	// TODO: �ڴ˴�����������
}

void COCADSrvrItem::Serialize(CArchive& ar)
{
	// ���������Ƶ��������У����ܽ����� COCADSrvrItem::Serialize��
	//  �����
	//  ͨ�� OLE �ص� OnGetClipboardData ���Զ�ִ�С�
	//  ����Ƕ���Ĭ�����ֱ��ί�и��ĵ��� Serialize ������
	//  ���֧�����ӣ���ֻ�����л�
	//  �ĵ���һ���֡�

	if (!IsLinkedItem())
	{
		COCADDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
			pDoc->Serialize(ar);
	}
}

BOOL COCADSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// �����������Ƶ�Ӧ�ó���ֻ�Ի����������
	//  ������д���  ���ϣ��֧���������棬��
	//  DVASPECT_THUMBNAIL (ͨ����д OnDrawEx)����Ӧ�޸�
	//  OnGetExtent �Ĵ�ʵ���Դ���
	//  �������档

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// ���� COCADSrvrItem::OnGetExtent �Ա��ȡ�������
	//  HIMETRIC ��Ԫ��ʾ�ķ�Χ��  �˴���Ĭ��ʵ��
	//  ֻ���ص�Ԫ��Ӳ�������֡�

	// TODO: �滻�������С

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC ��Ԫ

	return TRUE;
}

BOOL COCADSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// ���ʹ�� rSize���򽫴��Ƴ�
	UNREFERENCED_PARAMETER(rSize);

	// TODO: ����ӳ��ģʽ�ͷ�Χ
	//  (�˷�Χͨ����� OnGetExtent ���صĴ�С��ͬ)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO:  �ڴ˴���ӻ��ƴ��롣  Ҳ������� HIMETRIC ��Χ��
	//  ���л��ƶ�����ͼԪ�ļ��豸������(pDC)�н��С�

	return TRUE;
}


// COCADSrvrItem ���

#ifdef _DEBUG
void COCADSrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void COCADSrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

