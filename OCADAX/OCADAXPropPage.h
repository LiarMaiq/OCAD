#pragma once

// OCADAXPropPage.h : COCADAXPropPage ����ҳ���������


// COCADAXPropPage : �й�ʵ�ֵ���Ϣ������� OCADAXPropPage.cpp��

class COCADAXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(COCADAXPropPage)
	DECLARE_OLECREATE_EX(COCADAXPropPage)

// ���캯��
public:
	COCADAXPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_OCADAX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

