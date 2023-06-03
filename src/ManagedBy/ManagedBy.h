#ifdef MANAGEDBY_EXPORTS
#define MANAGEDBY_API __declspec(dllexport)
#else
#define MANAGEDBY_API __declspec(dllimport)
#endif

//forward declarations
MANAGEDBY_API DWORD APIENTRY ProcessGroupPolicy(DWORD dwFlags, HANDLE hToken, HKEY hKeyRoot, PGROUP_POLICY_OBJECT pDeletedGPOList, PGROUP_POLICY_OBJECT pChangedGPOList, ASYNCCOMPLETIONHANDLE pHandle, BOOL* pbAbort, PFNSTATUSMESSAGECALLBACK pStatusCallback);

