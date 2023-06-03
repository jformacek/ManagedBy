# ManagedBy
Implements exception in membership onlocal Administrators group, based on security principal reference contained in managedBy attribute of computer account in AD.

Idea is pretty simple:
- AD admin puts reference to user or security group to managedBy attribute of computer account
- this solution works as GPO Client Side Extension and every time GPO is refreshed, it looks into managedBy attribute and adds user or group present there to local Administrators group
  - this addition happens after built-in Windows GPOs run and setup membership to standard defined by GPO's
- so ManagedBy solution effectively implements exception mechanism for Administartors group membership on per-computer basis

Solution can be easily adopted to use of different attribute, even multivalued attribute if there's need to support multiple exceptions per computer. When custom attribute used, make sure it's syntax is DN (attributeSyntax = 2.5.5.1).  
Solution comes with own ADMX template that turns the logic on/off.

GPO CSE logs its activity to Application log under own event source - ManagedBy.
