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

// SrvrItem.h : COCADSrvrItem 类的接口
//

#pragma once

class COCADSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(COCADSrvrItem)

// 构造函数
public:
	COCADSrvrItem(COCADDoc* pContainerDoc);

// 特性
	COCADDoc* GetDocument() const
		{ return reinterpret_cast<COCADDoc*>(COleServerItem::GetDocument()); }

// 重写
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// 实现
public:
	~COCADSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
};

