// ConsolePane.h : ConsolePane DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CConsolePaneApp
// �йش���ʵ�ֵ���Ϣ������� ConsolePane.cpp
//

class CConsolePaneApp : public CWinApp
{
public:
	CConsolePaneApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
