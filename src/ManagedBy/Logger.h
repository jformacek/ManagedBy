#pragma once

// structure for Event logging
typedef struct _LOGDATA
{
	DWORD dwID;
	HRESULT hr;
	LPCTSTR szObject;
	DWORD dwLogLevel;
} LOGDATA, *LPLOGDATA;

typedef struct _INSBUFFER
{
	int NumOfStrings;
	TCHAR** pStrings;
} INSBUFFER, *LPINSBUFFER;

//Logging levels
enum
{
	LOGLEVEL_ERRORS_ONLY=0,
	LOGLEVEL_ERRORS_WARNINGS,
	LOGLEVEL_ALL_EVENTS,
	LOGLEVEL_INVALID	//always at the end of enum
};

//buffer size for event log messages
#define MAX_MSG_LEN			256
//event source name
#define EVENT_SOURCE_NAME _T("ManagedBy")

class Logger
{
public:
	Logger(void);
	~Logger(void);
	void Log(LPLOGDATA data);
protected:
		HANDLE hEventLog; 
		LPINSBUFFER InitializeInsertionStringBuffer(unsigned int NumOfStrings);
		void FreeInsertionStringBuffer(LPINSBUFFER pb);

};

