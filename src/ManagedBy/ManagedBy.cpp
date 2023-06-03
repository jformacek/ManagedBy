// ManagedBy.cpp


#include "stdafx.h"
#include "resource.h"
#include "ManagedBy.h"
#include "Logger.h"
#include "Computer.h"
#include "Config.h"
#include "ManagedByMsg.h"

#pragma warning( disable : 4267 )
#pragma warning( disable : 4995 )

MANAGEDBY_API DWORD APIENTRY ProcessGroupPolicy(DWORD dwFlags,
									   HANDLE hToken,
									   HKEY hKeyRoot,
									   PGROUP_POLICY_OBJECT pDeletedGPOList,
									   PGROUP_POLICY_OBJECT pChangedGPOList,
									   ASYNCCOMPLETIONHANDLE pHandle,
									   BOOL* pAbort,
									   PFNSTATUSMESSAGECALLBACK pStatusCallback)
{
   
	// We only work if refreshing machine policy

	if(*pAbort)
		return ERROR_SUCCESS;

	if(!(dwFlags & GPO_INFO_FLAG_MACHINE))
		return (ERROR_SUCCESS);

	Config config;
	//solution not allowed to run
	if(!config.Enabled)
		return ERROR_SUCCESS;

	Logger logger;
	LOGDATA LogData;
	ZeroMemory(&LogData, sizeof(LogData));
	LogData.dwLogLevel=config.LogLevel;
        
	LogData.dwID=S_GET_COMPUTER;
	Computer comp;
	LogData.hr=comp.Load();
	if(FAILED(LogData.hr))
	{
		logger.Log(&LogData);
		return ERROR_SUCCESS;
	}

	if(comp.ManagersCount==0) 	//ManagedBy attribute is Empty
	{
		LogData.dwID=S_NO_ADMIN;
		logger.Log(&LogData);
		return ERROR_SUCCESS;
	}

	comp.SetManagers(&logger);
	//everything succeeded
	LogData.dwID=S_FINISHED;
	logger.Log(&LogData);
 
	return ERROR_SUCCESS;
}

