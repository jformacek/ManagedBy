#include "stdafx.h"
#include "Logger.h"
#include "ManagedByMsg.h"


Logger::Logger(void)
{
	hEventLog=RegisterEventSource(NULL, EVENT_SOURCE_NAME);
}

Logger::~Logger(void)
{
	if(hEventLog!=NULL)
		DeregisterEventSource(hEventLog);
}

void Logger::Log(LPLOGDATA pLogData)
{
	LPINSBUFFER pBuff = nullptr;

	switch(pLogData->dwLogLevel)
	{
	case LOGLEVEL_ERRORS_ONLY:
		if(!FAILED(pLogData->hr)) return;			//success or warning in errors_only level -> do not log
		break;
	case LOGLEVEL_ERRORS_WARNINGS:
		if(pLogData->hr==ERROR_SUCCESS) return;	//success in errors_warnings level -> do not log
		break;
	case LOGLEVEL_ALL_EVENTS:
		break;
	}
	__try
	{
		switch(pLogData->dwID)
		{
			case S_GET_COMPUTER:
				pBuff=InitializeInsertionStringBuffer(1);
				if (pBuff == nullptr || pBuff->pStrings[0] == nullptr)
					__leave;

				if(FAILED(StringCchPrintf(pBuff->pStrings[0], MAX_MSG_LEN, TEXT("0x%x"), pLogData->hr))) __leave;
				if (!ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE,	0, pLogData->dwID, NULL, 1, 0, (LPCTSTR*)pBuff->pStrings, NULL)) __leave;
				break;
			case S_NOT_ADDED:
			case S_NOT_TRANSLATED:
				pBuff=InitializeInsertionStringBuffer(2);
				if (pBuff == nullptr || pBuff->pStrings[0] == nullptr || pBuff->pStrings[1] == nullptr)
					__leave;
				if(FAILED(StringCchPrintf(pBuff->pStrings[0], MAX_MSG_LEN, TEXT("%s"), pLogData->szObject))) __leave;
				if(FAILED(StringCchPrintf(pBuff->pStrings[1], MAX_MSG_LEN, TEXT("0x%x"), pLogData->hr))) __leave;
				if (!ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE,	0, pLogData->dwID, NULL, 2, 0, (LPCTSTR*)pBuff->pStrings, NULL)) __leave;
				break;
			case S_ADDED:
			case S_ALREADY_MEMBER:
				pBuff=InitializeInsertionStringBuffer(1);
				if (pBuff == nullptr || pBuff->pStrings[0] == nullptr)
					__leave;
				if(FAILED(StringCchPrintf(pBuff->pStrings[0], MAX_MSG_LEN, TEXT("%s"), pLogData->szObject))) __leave;
				if (!ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE,	0, pLogData->dwID, NULL, 1, 0, (LPCTSTR*)pBuff->pStrings, NULL)) __leave;
				break;
			case S_NO_ADMIN:
			case S_FINISHED:
				if (!ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE,	0, pLogData->dwID, NULL, 0, 0, NULL, NULL)) __leave;
				break;
		}
	}
	__finally
	{
		
		FreeInsertionStringBuffer(pBuff);
	}
}

LPINSBUFFER Logger::InitializeInsertionStringBuffer(unsigned int NumOfStrings)
{
	LPINSBUFFER retVal = new(std::nothrow) INSBUFFER;
	::ZeroMemory(retVal, sizeof(retVal));

	retVal->pStrings = new(std::nothrow) TCHAR*[NumOfStrings];
	if (retVal->pStrings == nullptr)
		return nullptr;
	retVal->NumOfStrings = NumOfStrings;

	::ZeroMemory(retVal->pStrings, sizeof(TCHAR*)*NumOfStrings);

	for (unsigned int i = 0; i < NumOfStrings; i++)
	{
		retVal->pStrings[i] = new(std::nothrow) TCHAR[MAX_MSG_LEN];
	}

	return retVal;
}

void Logger::FreeInsertionStringBuffer(LPINSBUFFER pb) {
	if (pb == nullptr)
		return;
	for (int i = 0; i < pb->NumOfStrings; i++)
		if (pb->pStrings[i]!=nullptr)
			delete [] pb->pStrings[i];
	if (pb->pStrings!=nullptr)
		delete [] pb->pStrings;
	delete pb;
}
