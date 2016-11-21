// OCADAX.cpp : COCADAXApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "OCADAX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COCADAXApp theApp;

const GUID CDECL _tlid = { 0x7615D63, 0x5B5B, 0x4750, { 0x99, 0x85, 0xDC, 0xCC, 0xF9, 0x21, 0xAA, 0x54 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// COCADAXApp::InitInstance - DLL ��ʼ��

BOOL COCADAXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO:  �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// COCADAXApp::ExitInstance - DLL ��ֹ

int COCADAXApp::ExitInstance()
{
	// TODO:  �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
