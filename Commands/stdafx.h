// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifndef _TOOLKIT_IN_DLL_
#define _TOOLKIT_IN_DLL_
#endif

#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"
#include "DbCommandContext.h"


#ifdef _DEBUG
#pragma comment(lib,"../lib/win32_debug/TD_Db.lib")
//#pragma comment(lib,"../lib/win32_debug/TD_DbRoot.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Root.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Alloc.lib")
//#pragma comment(lib,"../lib/win32_debug/TD_Db.lib")
//#pragma comment(lib,"../lib/win32_debug/TD_Db.lib")
#else
#pragma comment(lib,"..\\release\\LedCtrlBoard.lib")
#endif
