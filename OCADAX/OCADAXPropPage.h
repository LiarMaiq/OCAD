#pragma once

// OCADAXPropPage.h : COCADAXPropPage 属性页类的声明。


// COCADAXPropPage : 有关实现的信息，请参阅 OCADAXPropPage.cpp。

class COCADAXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(COCADAXPropPage)
	DECLARE_OLECREATE_EX(COCADAXPropPage)

// 构造函数
public:
	COCADAXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_OCADAX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

