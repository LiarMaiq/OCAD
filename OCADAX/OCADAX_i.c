

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Nov 21 20:55:34 2016
 */
/* Compiler settings for OCADAX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_OCADAXLib,0x07615D63,0x5B5B,0x4750,0x99,0x85,0xDC,0xCC,0xF9,0x21,0xAA,0x54);


MIDL_DEFINE_GUID(IID, DIID__DOCADAX,0xD954DB96,0xFD53,0x4725,0xB0,0x12,0x74,0x6C,0x5C,0x16,0x3A,0x6A);


MIDL_DEFINE_GUID(IID, DIID__DOCADAXEvents,0xE3968884,0x1FA8,0x4C6A,0xA9,0x39,0xC3,0x59,0x89,0x72,0xE6,0xC1);


MIDL_DEFINE_GUID(CLSID, CLSID_OCADAX,0xF73A286B,0x4E03,0x4DD8,0xA9,0x91,0x4A,0x13,0x21,0xBD,0x94,0x8B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



