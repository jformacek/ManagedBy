// stdafx.cpp : source file that includes just the standard includes
// test2.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "Config.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

HINSTANCE hDll=(HINSTANCE)INVALID_HANDLE_VALUE;

//DllMain, Installation, Uninstallation
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason, LPVOID lpReserved)
{
	if(ul_reason==DLL_PROCESS_ATTACH)
	{
		hDll=(HINSTANCE)hModule;
	}
    return TRUE;
}

 STDAPI DllUnregisterServer()
 {
	HRESULT hr=SHDeleteKey(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH);
	if(SUCCEEDED(hr)) hr=SHDeleteKey(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH_EVT);

	return hr;
}

STDAPI DllRegisterServer()
{
    HKEY hKey=0;
    LONG lResult;
	size_t size;
	DWORD dwData;
	LPTSTR szPathBuff=NULL;


	__try
	{
		szPathBuff=new(std::nothrow) TCHAR[MAX_PATH];
		if(szPathBuff==NULL)
			__leave;
		lResult=::GetModuleFileName((HINSTANCE)hDll, szPathBuff, MAX_PATH);
		// Register extension.
		if( lResult == 0 ) __leave;

		//this is to handle the scenario when replacing existing dll 
		//and registering the new version using code running within module with temporary name
		//installer might replaced extension ".new", so we're replacing here back to ".dll"
		PathRenameExtension(szPathBuff,FINAL_FILE_EXTENSION);

		// Register extension.
		lResult = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
								GPEXT_REG_PATH,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL);

		if( lResult != ERROR_SUCCESS ) __leave;
		
		//Name of entry point
		size=sizeof(ENTRY_POINT);
		lResult = RegSetValueEx( hKey,
					L"ProcessGroupPolicy",
					0,
					REG_SZ,
					(BYTE*)ENTRY_POINT,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		//Name of extension
		size=sizeof(EXTENSION_NAME);
		lResult = RegSetValueEx( hKey,
					NULL,
					0,
					REG_SZ,
					(BYTE*)EXTENSION_NAME,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		// Path to DLL
		HRESULT hr = StringCbLength(szPathBuff, MAX_PATH, &size);
		if (FAILED(hr))
			__leave;
		size++;	//to include terminating null
		lResult = RegSetValueEx( hKey,
					L"DllName",
					0,
					REG_EXPAND_SZ,
					(BYTE*)szPathBuff,
					size );
		if( lResult != ERROR_SUCCESS ) __leave;

		//we don't want be called when refreshing user policy
		dwData=1;
		lResult = RegSetValueEx( hKey,
					L"NoUserPolicy",
					0,
					REG_DWORD,
					(BYTE*)&dwData,
					sizeof(DWORD) );
		if( lResult != ERROR_SUCCESS ) __leave;

		RegCloseKey(hKey);

		//Register event log message file
		lResult = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
								GPEXT_REG_PATH_EVT,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL);

		if( lResult != ERROR_SUCCESS ) __leave;

		//Path to DLL
		lResult = RegSetValueEx( hKey,
					L"EventMessageFile",
					0,
					REG_EXPAND_SZ,
					(BYTE*)szPathBuff,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		//we provide event log with errors and successes
		dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_INFORMATION_TYPE;
		lResult = RegSetValueEx( hKey,
					L"TypesSupported",
					0,
					REG_DWORD,
					(BYTE*)&dwData,
					sizeof(DWORD) );
		if( lResult != ERROR_SUCCESS ) __leave;

	}
	__finally
	{
		//Close the registry key if necessary
		if(hKey) RegCloseKey( hKey );
		if(szPathBuff!=NULL)
			delete [] szPathBuff;
	}

    return lResult;
}
