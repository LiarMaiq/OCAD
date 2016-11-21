

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Mon Nov 21 20:55:34 2016
 */
/* Compiler settings for OCAD.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __OCAD_h_h__
#define __OCAD_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOCAD_FWD_DEFINED__
#define __IOCAD_FWD_DEFINED__
typedef interface IOCAD IOCAD;

#endif 	/* __IOCAD_FWD_DEFINED__ */


#ifndef __COCADDoc_FWD_DEFINED__
#define __COCADDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class COCADDoc COCADDoc;
#else
typedef struct COCADDoc COCADDoc;
#endif /* __cplusplus */

#endif 	/* __COCADDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __OCAD_LIBRARY_DEFINED__
#define __OCAD_LIBRARY_DEFINED__

/* library OCAD */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_OCAD;

#ifndef __IOCAD_DISPINTERFACE_DEFINED__
#define __IOCAD_DISPINTERFACE_DEFINED__

/* dispinterface IOCAD */
/* [uuid] */ 


EXTERN_C const IID DIID_IOCAD;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A521C422-334D-4899-881F-EBEA50DCB856")
    IOCAD : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IOCADVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOCAD * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOCAD * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOCAD * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOCAD * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOCAD * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOCAD * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOCAD * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IOCADVtbl;

    interface IOCAD
    {
        CONST_VTBL struct IOCADVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOCAD_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOCAD_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOCAD_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOCAD_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOCAD_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOCAD_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOCAD_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IOCAD_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_COCADDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("4B3DB332-8607-4A3E-97B9-81850B06D49F")
COCADDoc;
#endif
#endif /* __OCAD_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


