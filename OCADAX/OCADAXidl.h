

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat Oct 29 13:51:05 2016
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __OCADAXidl_h__
#define __OCADAXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DOCADAX_FWD_DEFINED__
#define ___DOCADAX_FWD_DEFINED__
typedef interface _DOCADAX _DOCADAX;

#endif 	/* ___DOCADAX_FWD_DEFINED__ */


#ifndef ___DOCADAXEvents_FWD_DEFINED__
#define ___DOCADAXEvents_FWD_DEFINED__
typedef interface _DOCADAXEvents _DOCADAXEvents;

#endif 	/* ___DOCADAXEvents_FWD_DEFINED__ */


#ifndef __OCADAX_FWD_DEFINED__
#define __OCADAX_FWD_DEFINED__

#ifdef __cplusplus
typedef class OCADAX OCADAX;
#else
typedef struct OCADAX OCADAX;
#endif /* __cplusplus */

#endif 	/* __OCADAX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_OCADAX_0000_0000 */
/* [local] */ 

#pragma once
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_OCADAX_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_OCADAX_0000_0000_v0_0_s_ifspec;


#ifndef __OCADAXLib_LIBRARY_DEFINED__
#define __OCADAXLib_LIBRARY_DEFINED__

/* library OCADAXLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_OCADAXLib;

#ifndef ___DOCADAX_DISPINTERFACE_DEFINED__
#define ___DOCADAX_DISPINTERFACE_DEFINED__

/* dispinterface _DOCADAX */
/* [uuid] */ 


EXTERN_C const IID DIID__DOCADAX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D954DB96-FD53-4725-B012-746C5C163A6A")
    _DOCADAX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOCADAXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOCADAX * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOCADAX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOCADAX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOCADAX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOCADAX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOCADAX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOCADAX * This,
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
    } _DOCADAXVtbl;

    interface _DOCADAX
    {
        CONST_VTBL struct _DOCADAXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOCADAX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOCADAX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOCADAX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOCADAX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOCADAX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOCADAX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOCADAX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOCADAX_DISPINTERFACE_DEFINED__ */


#ifndef ___DOCADAXEvents_DISPINTERFACE_DEFINED__
#define ___DOCADAXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DOCADAXEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DOCADAXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E3968884-1FA8-4C6A-A939-C3598972E6C1")
    _DOCADAXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOCADAXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOCADAXEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOCADAXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOCADAXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOCADAXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOCADAXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOCADAXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOCADAXEvents * This,
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
    } _DOCADAXEventsVtbl;

    interface _DOCADAXEvents
    {
        CONST_VTBL struct _DOCADAXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOCADAXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOCADAXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOCADAXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOCADAXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOCADAXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOCADAXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOCADAXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOCADAXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OCADAX;

#ifdef __cplusplus

class DECLSPEC_UUID("F73A286B-4E03-4DD8-A991-4A1321BD948B")
OCADAX;
#endif
#endif /* __OCADAXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


