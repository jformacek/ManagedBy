# ManagedBy
Implements exception in membership of local Administrators group, based on security principal reference contained in managedBy attribute of computer account in AD.

## Processing logic
Idea is pretty simple:
- AD admin puts reference to user or security group to managedBy attribute of computer account
- this solution works as GPO Client Side Extension and every time GPO is refreshed, it looks into managedBy attribute and adds user or group present there to local Administrators group
  - this addition happens after built-in Windows GPOs run and setup membership to standard defined by GPO's
- so ManagedBy solution effectively implements exception mechanism for Administartors group membership on per-computer basis

## Customization and configuration
Solution can be easily adopted to use of different attribute, even multivalued attribute if there's need to support multiple exceptions per computer. When custom attribute used, make sure it's syntax is DN (attributeSyntax = 2.5.5.1).  
Solution comes with own ADMX template that turns the logic on/off.

## Logging
GPO CSE logs its activity to Application log under own event source - ManagedBy.  
Event reference below
|Severity|Event Id|Description|
|--------|--------|-----------|
|Error|1|Could not get computer object from AD. Error %1.|
|Error|2|Could not add member to local Administrators group.<br/>Member:%1<br/>Error: %2|
|Error|3|Could not translate AD object to security principal.<br/>Object:%1<br/>Error:%2|
|Information|11|List of managers is empty. Nothing to do.|
|Information|12|Object %1 added to local Administrators group.|
|Information|13|Object %1 is already member of local Administrators group. Nothing to do.|
