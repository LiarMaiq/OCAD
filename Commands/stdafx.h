// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>


// TODO:  在此处引用程序需要的其他头文件
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
