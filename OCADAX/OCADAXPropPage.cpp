// OCADAXPropPage.cpp : COCADAXPropPage 属性页类的实现。

#include "stdafx.h"
#include "OCADAX.h"
#include "OCADAXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COCADAXPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(COCADAXPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(COCADAXPropPage, "OCADAX.OCADAXPropPage.1",
	0x7f07f2c, 0x9ee, 0x41d5, 0xb1, 0xe5, 0x58, 0x3, 0xde, 0xfc, 0x3b, 0x13)

// COCADAXPropPage::COCADAXPropPageFactory::UpdateRegistry -
// 添加或移除 COCADAXPropPage 的系统注册表项

BOOL COCADAXPropPage::COCADAXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_OCADAX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// COCADAXPropPage::COCADAXPropPage - 构造函数

COCADAXPropPage::COCADAXPropPage() :
	COlePropertyPage(IDD, IDS_OCADAX_PPG_CAPTION)
{
}

// COCADAXPropPage::DoDataExchange - 在页和属性间移动数据

void COCADAXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// COCADAXPropPage 消息处理程序
