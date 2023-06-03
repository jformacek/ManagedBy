SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )

LanguageNames=(English=0x409:MSG00409)

;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;//	1 - 10 ... Error events
;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MessageId=1 Severity=Error SymbolicName=S_GET_COMPUTER
Language=English
Could not get computer object from AD. Error %1.
.

MessageId=2 Severity=Error SymbolicName=S_NOT_ADDED
Language=English
Could not add member to local Administrators group.%n
Member:%t%1%n
Error:%t%2%n
.

MessageId=3 Severity=Error SymbolicName=S_NOT_TRANSLATED
Language=English
Could not translate AD object to security principal.%n
Object:%t%1%n
Error:%t%2%n
.

;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;//	11 - 20 ... Success events
;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MessageId=11 Severity=Success SymbolicName=S_NO_ADMIN
Language=English
List of managers is empty. Nothing to do.
.

MessageId=12 Severity=Success SymbolicName=S_ADDED
Language=English
Object %1 added to local Administrators group. 
.

MessageId=13 Severity=Success SymbolicName=S_ALREADY_MEMBER
Language=English
Object %1 is already member of local Administrators group. Nothing to do.
.

MessageId=14 Severity=Success SymbolicName=S_FINISHED
Language=English
Finished successfully.
.
