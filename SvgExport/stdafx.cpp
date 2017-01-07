// stdafx.cpp : 只包括标准包含文件的源文件
// SvgExport.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
#ifdef _DEBUG
#pragma comment(lib,"../lib/win32_debug/TD_Key.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Gs.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Db.lib")
#pragma comment(lib,"../lib/win32_debug/TD_DbRoot.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Gi.lib")
#pragma comment(lib,"../lib/win32_debug/TD_SpatialIndex.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Ge.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Root.lib")
#pragma comment(lib,"../lib/win32_debug/TD_Alloc.lib")

#else


#endif // _DEBUG