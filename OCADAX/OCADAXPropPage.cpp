// OCADAXPropPage.cpp : COCADAXPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "OCADAX.h"
#include "OCADAXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COCADAXPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(COCADAXPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(COCADAXPropPage, "OCADAX.OCADAXPropPage.1",
	0x7f07f2c, 0x9ee, 0x41d5, 0xb1, 0xe5, 0x58, 0x3, 0xde, 0xfc, 0x3b, 0x13)

// COCADAXPropPage::COCADAXPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� COCADAXPropPage ��ϵͳע�����

BOOL COCADAXPropPage::COCADAXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_OCADAX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// COCADAXPropPage::COCADAXPropPage - ���캯��

COCADAXPropPage::COCADAXPropPage() :
	COlePropertyPage(IDD, IDS_OCADAX_PPG_CAPTION)
{
}

// COCADAXPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void COCADAXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// COCADAXPropPage ��Ϣ�������
