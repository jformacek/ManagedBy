#pragma once
class Computer
{
public:
	Computer();
	~Computer();

	HRESULT Load();
	HRESULT SetManagers(Logger *logger);

	//number of configured managers
	__declspec(property(get = GET_MgrsCount)) ULONG ManagersCount;

	//accessors
	ULONG GET_MgrsCount() {
		return _ulManagersCount;
	};

private:
	TCHAR** _ppManagers;
	ULONG _ulManagersCount;

	LDAP* _conn;
	LPTSTR _dn;

};

