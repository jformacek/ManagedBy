#pragma once

// extension registration place
#define GPEXT_REG_PATH L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\GPExtensions\\{B62916D5-8E4A-4e49-A6DD-BCA2AD18A815}"
// extension policy store
#define GPEXT_REG_POLICY_PATH L"Software\\Policies\\Microsoft Services\\ManagedBy"
//extension Eventlog message file registration place
#define GPEXT_REG_PATH_EVT L"System\\CurrentControlSet\\Services\\EventLog\\Application\\ManagedBy"
//name of entry point
#define ENTRY_POINT L"ProcessGroupPolicy"
//name of extension
#define EXTENSION_NAME L"ManagedBy"
//installation support
#define FINAL_FILE_EXTENSION _T(".dll")

//name of registry value storing the logging level
#define LOG_LEVEL_REG_VALUE L"ExtensionDebugLevel"
//whether or not enabled
#define SOLUTION_ENABLED_REG_VALUE L"Enabled"
class Config
{
public:
	Config(void);
	~Config(void);
	HRESULT GetRegistryDWORD(HKEY hReg, LPCTSTR regValueName, DWORD *retVal);

	//whether or not solution is enabled
	__declspec(property(get = GET_Enabled)) bool Enabled;
	//log level
	__declspec(property(get = GET_LogLevel)) DWORD LogLevel;

	//accessors
	bool GET_Enabled() {
		return _enabled;
	};

	DWORD GET_LogLevel() {
		return _logLevel;
	};

protected:
	bool _enabled=false;
	DWORD _logLevel=0;

	HKEY hPolicyKey = 0;
	HKEY hGPExtKey = 0;
};

