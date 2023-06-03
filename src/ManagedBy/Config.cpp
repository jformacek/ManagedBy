#include "stdafx.h"
#include "Logger.h"
#include "Config.h"

Config::Config(void)
{
	DWORD data;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH, 0, KEY_QUERY_VALUE, &hGPExtKey);
	
	if (hGPExtKey != 0)
	{
		GetRegistryDWORD(hGPExtKey, LOG_LEVEL_REG_VALUE, &_logLevel);
		if (_logLevel >= LOGLEVEL_INVALID)
			_logLevel = LOGLEVEL_ALL_EVENTS;
	}

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, GPEXT_REG_POLICY_PATH, 0, KEY_QUERY_VALUE, &hPolicyKey);
	if (hPolicyKey != 0)
	{
		data = 0;
		GetRegistryDWORD(hPolicyKey, SOLUTION_ENABLED_REG_VALUE, &data);
		_enabled = (data>0);
	}
}


Config::~Config(void)
{
	if (hPolicyKey!=0)
		RegCloseKey(hPolicyKey);
	if (hGPExtKey!=0)
		RegCloseKey(hGPExtKey);

}

HRESULT Config::GetRegistryDWORD(HKEY hReg, LPCTSTR regValueName, DWORD *retVal) {
	LONG lResult;
	DWORD dwBuffLen = sizeof(*retVal);

	if (!hReg)
		HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	lResult = RegQueryValueEx(hReg, regValueName, NULL, NULL, (LPBYTE) retVal, &dwBuffLen);

	if (lResult == ERROR_MORE_DATA)
	{
		lResult = ERROR_BAD_ARGUMENTS;	//value stored in registry is not REG_DWORD
	}

	return HRESULT_FROM_WIN32(lResult);
}