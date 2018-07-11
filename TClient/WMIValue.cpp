#include <iostream> 
#include <comdef.h> 
#include <Wbemidl.h> 
#include <string.h> 
#include <windows.h> 
#include <tchar.h> 
#include <objbase.h> 
#include <atlbase.h> 

#pragma comment(lib, "wbemuuid.lib") 

string CTWMIValue::GetWMIValue(char* type,wchar_t *value)
{ 
	string ReturnValue = "(null)"; 
    
    HRESULT hResult; 
  
    hResult =  CoInitializeEx(0, COINIT_MULTITHREADED); 
    if (FAILED(hResult)) { 
        ReturnValue = "Error at CoInitializeEx"; 
        return ReturnValue; 
    } 
  
    hResult =  CoInitializeSecurity(NULL, -1,NULL,NULL,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL); 
    if(FAILED(hResult)) { 
        ReturnValue = "Error at CoInitializeSecurity"; 
        CoUninitialize(); 
        return ReturnValue; 
    } 
    
    IWbemLocator *pLoc = NULL; 
    hResult = CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator,(LPVOID *)&pLoc); 
  
    if(FAILED(hResult)) { 
        CoUninitialize(); 
        ReturnValue = "Error at CoCreateInstance"; 
        return ReturnValue; 
    } 
  
    IWbemServices *pSvc = NULL; 
    hResult = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL,NULL,NULL,NULL,NULL,NULL,&pSvc); 
    
    if(FAILED(hResult)) { 
        ReturnValue = "Error at Connecting Server"; 
        pLoc->Release();     
        CoUninitialize(); 
        return ReturnValue; 
    } 
  
    hResult = CoSetProxyBlanket(pSvc,RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE); 
    if (FAILED(hResult)) { 
        ReturnValue = "Error at CoSetProxyBlanket"; 
        pSvc->Release(); 
        pLoc->Release();     
        CoUninitialize(); 
        return ReturnValue; 
    } 
  
    IEnumWbemClassObject* pEnumerator = NULL; 
    hResult = pSvc->ExecQuery(bstr_t("WQL"),bstr_t(type),WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumerator); 
    if(FAILED(hResult)) { 
       ReturnValue = "Error at ExecQuery"; 
        pSvc->Release(); 
        pLoc->Release(); 
        CoUninitialize(); 
        return ReturnValue; 
    } 
  
    IWbemClassObject *pclsObj;
    ULONG uReturn = 0; 
    if(pEnumerator) { 
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn); 
        if(uReturn) { 
            _variant_t var_val; 
            hr = pclsObj->Get(value, 0, &var_val, 0, 0); 
            _bstr_t str = var_val; 
            ReturnValue = (char*)str; 
            VariantClear(&var_val); 
        } 
        else { 
            ReturnValue = "Error at pEnumerator->Next"; 
        } 
    } 
    
    pSvc->Release(); 
    pLoc->Release(); 
    pEnumerator->Release(); 
    pclsObj->Release();
    CoUninitialize(); 
  
    return ReturnValue; 
}