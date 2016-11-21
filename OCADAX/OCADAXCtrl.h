#pragma once

// OCADAXCtrl.h : COCADAXCtrl ActiveX 控件类的声明。


// COCADAXCtrl : 有关实现的信息，请参阅 OCADAXCtrl.cpp。

class CInstanceFactory;

class COCADAXCtrl : public COleControl
{
	DECLARE_DYNCREATE(COCADAXCtrl)




private:
	CInstanceFactory * m_pInstanceFactory;




// 构造函数
public:
	COCADAXCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~COCADAXCtrl();

	DECLARE_OLECREATE_EX(COCADAXCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(COCADAXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(COCADAXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(COCADAXCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidOpenFile = 1L
	};
protected:
	VARIANT_BOOL OpenFile(LPCTSTR pathName);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

