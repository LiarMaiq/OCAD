#pragma once

// OCADAXCtrl.h : COCADAXCtrl ActiveX �ؼ����������


// COCADAXCtrl : �й�ʵ�ֵ���Ϣ������� OCADAXCtrl.cpp��

class CInstanceFactory;

class COCADAXCtrl : public COleControl
{
	DECLARE_DYNCREATE(COCADAXCtrl)




private:
	CInstanceFactory * m_pInstanceFactory;




// ���캯��
public:
	COCADAXCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~COCADAXCtrl();

	DECLARE_OLECREATE_EX(COCADAXCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(COCADAXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(COCADAXCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(COCADAXCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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

