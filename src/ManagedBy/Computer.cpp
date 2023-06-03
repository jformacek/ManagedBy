#include "stdafx.h"
#include "Logger.h"
#include "Computer.h"
#include "resource.h"
#include "ManagedByMsg.h"

Computer::Computer()
{
	ULONG retVal=LDAP_SUCCESS;
	ULONG buffLen;
	
	//initialize members
	_conn=nullptr;
	_dn = nullptr;
	_ppManagers=nullptr;
	_ulManagersCount=0;

	//Find computer account in AD
	if (!GetComputerObjectName(NameFullyQualifiedDN, _dn, &buffLen))
		throw HRESULT_FROM_WIN32(GetLastError());
	_dn = new(std::nothrow) TCHAR[buffLen];
	if (!GetComputerObjectName(NameFullyQualifiedDN, _dn, &buffLen))
		throw HRESULT_FROM_WIN32(GetLastError());

	//initialize connection
	_conn = ldap_init(nullptr, LDAP_PORT);
	if (_conn==nullptr)
		throw HRESULT_FROM_WIN32(LdapGetLastError());
	//require encryption
	retVal = ldap_set_option(_conn, LDAP_OPT_ENCRYPT, LDAP_OPT_ON);
	if(retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
	//require signing
	retVal = ldap_set_option(_conn, LDAP_OPT_SIGN, LDAP_OPT_ON);
	if (retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
	//bind
	retVal = ldap_bind_s(_conn, nullptr, nullptr, LDAP_AUTH_NEGOTIATE);
	if (retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
}

Computer::~Computer()
{
	if (_dn != nullptr)
		delete [] _dn;
	if(_ppManagers!=nullptr)
		ldap_value_free(_ppManagers);

	if (_conn != nullptr)
		ldap_unbind_s(_conn);
}


HRESULT Computer::Load()
{
	HRESULT hr = S_OK;
	BerElement *pBer = nullptr;
	LDAPMessage* rslt = nullptr;
	TCHAR* pAttribute = nullptr;
	TCHAR** ppValues = nullptr;

	TCHAR attrName[MAX_ATTR_NAME];
	ZeroMemory(attrName, MAX_ATTR_NAME*sizeof(TCHAR));

	int nChars = 0;

	extern HINSTANCE hDll;

	//retrieve attribute name storing list of admins
	nChars = ::LoadString(hDll, IDS_ATTR_MANAGED_BY, attrName, MAX_ATTR_NAME);
	if (nChars == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	
	//load value of attribute
	TCHAR *attrs[2];
	attrs[0] = attrName;
	attrs[1] = nullptr;

	//load the computer object
	if (LDAP_SUCCESS != ldap_search_s(_conn, _dn, LDAP_SCOPE_BASE, _T("(objectClass=*)"), attrs, 0, &rslt))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	//should be always one entry
	ULONG numEntries = ldap_count_entries(_conn, rslt);
	if (numEntries < 1)
	{
		if (_conn->ld_errno!=0)
			hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		else
			hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
		goto Cleanup;
	}

	//get the entry. Does not to be explicitly freed as it's freed with rslt
	LDAPMessage *entry = ldap_first_entry(_conn, rslt);

	//get attribute name
	pAttribute = ldap_first_attribute(_conn, entry, &pBer);
	if (pAttribute == NULL)
	{
		if (_conn->ld_errno != 0)
			//error in processing
			hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		else
			//processing OK but attribute not found
			hr = S_OK;
		goto Cleanup;
	}

	_ppManagers = ldap_get_values(_conn, entry, pAttribute);
	if (_conn->ld_errno != 0)
	{
		hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		goto Cleanup;
	}
	_ulManagersCount = ldap_count_values(_ppManagers);

Cleanup:
	if (rslt!=nullptr)
		ldap_msgfree(rslt);
	if (pBer != nullptr)
		ber_free(pBer,0);
	if (pAttribute != nullptr)
		ldap_memfree(pAttribute);

	return hr;
}

HRESULT Computer::SetManagers(Logger *pLogger)
{
	HRESULT retVal=S_OK;
	LOGDATA LogData;

	//put each manager separately to Administrators group
	for(ULONG i=0;i<_ulManagersCount;i++)
	{
		ULONG ulNameLen=0;
		TCHAR* pName=nullptr;
		__try
		{
			//logging user as DN
			LogData.szObject=*(_ppManagers+i);
			if(!TranslateName(*(_ppManagers+i),EXTENDED_NAME_FORMAT::NameFullyQualifiedDN,EXTENDED_NAME_FORMAT::NameSamCompatible,NULL,&ulNameLen))
			{
				LogData.dwID=S_NOT_TRANSLATED;
				LogData.hr=HRESULT_FROM_WIN32(GetLastError());
				pLogger->Log(&LogData);
				__leave;
			}
			pName=new(std::nothrow) TCHAR[ulNameLen];
			if(pName==nullptr)
			{
				LogData.dwID=S_NOT_ADDED;
				LogData.hr=ERROR_OUTOFMEMORY;
				pLogger->Log(&LogData);
				__leave;
			}
			if(!TranslateName(*(_ppManagers+i),EXTENDED_NAME_FORMAT::NameFullyQualifiedDN,EXTENDED_NAME_FORMAT::NameSamCompatible,pName,&ulNameLen))
			{
				LogData.dwID=S_NOT_TRANSLATED;
				LogData.hr=HRESULT_FROM_WIN32(GetLastError());
				pLogger->Log(&LogData);
				__leave;
			}

			LOCALGROUP_MEMBERS_INFO_3 lgmi3;
			lgmi3.lgrmi3_domainandname=pName;
			
			//for the rest, we're logging user in domain\account format
			LogData.szObject=pName;

			NET_API_STATUS nrslt=NetLocalGroupAddMembers(nullptr,_T("Administrators"),3,(LPBYTE)&lgmi3,1);
			switch(nrslt)
			{
				case ERROR_MEMBER_IN_ALIAS:
					LogData.dwID=S_ALREADY_MEMBER;
					pLogger->Log(&LogData);
					break;

				case NERR_Success:
					LogData.dwID=S_ADDED;
					pLogger->Log(&LogData);
					break;
				default:
					LogData.dwID=S_NOT_ADDED;
					LogData.hr=HRESULT_FROM_WIN32(nrslt);
					pLogger->Log(&LogData);
					break;
			}
		}
		__finally
		{
			if(pName!=nullptr)
				delete [] pName;
		}
	}

	return retVal;
}